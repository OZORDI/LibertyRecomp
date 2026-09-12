#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <array>
#include <deque>
#include <memory>
#include <random>
#include <span>
#include <unordered_set>
#include <vector>
#include "graphics/gta4_native/native_immutable_bindings.h"
#include "graphics/gta4_native/native_prepared_bindings.h"
#include "graphics/gta4_native/native_profile_shader_category.h"
#include "graphics/gta4_native/native_texture_protection.h"
using namespace rex::graphics::gta4_native;

namespace {
struct Allocation { uint64_t offset=0, size=0; };
using Binding=NativeImmutableBindings<Allocation>;
using Result=Binding::Result;
auto materialize=[](const auto& v){return AuthoritativeConstantState::MaterializeView(v);};
std::shared_ptr<const ConstantStateVersion> version(uint8_t value,uint64_t hash=7) {
  auto v=std::make_shared<ConstantStateVersion>();v->content_hash=hash;v->byte_size=64;
  std::vector<uint8_t> data(64,value);
  REQUIRE(CaptureCompleteConstantSnapshot(data,v->delta));return v;
}
struct Allocator {
  uint64_t calls=0;
  std::vector<std::vector<uint8_t>> data;
  bool operator()(FrameConstantKind,uint64_t,const std::vector<uint8_t>& bytes,Allocation& out) {
    out={++calls,bytes.size()};data.push_back(bytes);return true;
  }
};
}
TEST_CASE("Immutable constant identity avoids materialization and uploads on repeated draws", "[hotpath]") {
  Binding bindings;Allocator alloc;auto v=version(0x53);Allocation a,b;
  const std::vector<uint8_t>* bytes=nullptr;uint64_t materializations=0;
  auto mat=[&](const auto& value){++materializations;return materialize(value);};
  REQUIRE(bindings.Bind(FrameConstantKind::kVertex,v,a,bytes,mat,alloc)==Result::kUploaded);
  for(size_t i=0;i<10000;++i) {
    REQUIRE(bindings.Bind(FrameConstantKind::kVertex,v,b,bytes,mat,alloc)==Result::kVersionHit);
    REQUIRE(a.offset==b.offset);REQUIRE(*bytes==alloc.data[0]);
  }
  REQUIRE(materializations==1);REQUIRE(alloc.calls==1);REQUIRE(bindings.owner_count()==1);
}
TEST_CASE("Constant content collisions never alias different bytes or shader stages", "[hotpath]") {
  Binding bindings;Allocator alloc;auto a=version(1),same=version(1),collision=version(2);
  Allocation first,copy,different,pixel;const std::vector<uint8_t>* bytes=nullptr;
  REQUIRE(bindings.Bind(FrameConstantKind::kVertex,a,first,bytes,materialize,alloc)==Result::kUploaded);
  REQUIRE(bindings.Bind(FrameConstantKind::kVertex,same,copy,bytes,materialize,alloc)==Result::kContentHit);
  REQUIRE(copy.offset==first.offset);
  REQUIRE(bindings.Bind(FrameConstantKind::kVertex,collision,different,bytes,materialize,alloc)==Result::kUploaded);
  REQUIRE(different.offset!=first.offset);REQUIRE((*bytes)[0]==2);
  REQUIRE(bindings.Bind(FrameConstantKind::kPixel,a,pixel,bytes,materialize,alloc)==Result::kUploaded);
  REQUIRE(pixel.offset!=first.offset);REQUIRE(bindings.owner_count()==4);
}
TEST_CASE("Failed materialization and uploads publish no reusable constant identity", "[hotpath]") {
  Binding bindings;Allocation a;const std::vector<uint8_t>* bytes=nullptr;auto v=version(9);
  auto fail=[](auto&&...){return false;};
  REQUIRE(bindings.Bind(FrameConstantKind::kVertex,v,a,bytes,materialize,fail)==Result::kFailure);
  REQUIRE(bindings.entry_count()==0);REQUIRE(bindings.owner_count()==0);
  auto bad=[](const auto&)->const std::vector<uint8_t>*{return nullptr;};Allocator alloc;
  REQUIRE(bindings.Bind(FrameConstantKind::kVertex,v,a,bytes,bad,alloc)==Result::kFailure);
  REQUIRE(alloc.calls==0);
  REQUIRE(bindings.Bind(FrameConstantKind::kVertex,v,a,bytes,materialize,alloc)==Result::kUploaded);
}
TEST_CASE("Constant owners are fence-bounded and all obsolete generations expire", "[hotpath][memory]") {
  struct Slot {Binding cache;FrameConstantArenaIndex index;std::vector<std::weak_ptr<const ConstantStateVersion>> weak;};
  std::array<Slot,2> slots;
  for(uint64_t frame=1;frame<=1200;++frame) {
    auto& slot=slots[frame%slots.size()];
    if(slot.index.in_flight_submission()) {
      REQUIRE_FALSE(slot.index.ResetAfterCompletion(slot.index.in_flight_submission()-1));
      for(const auto& weak:slot.weak)REQUIRE_FALSE(weak.expired());
    }
    REQUIRE(slot.index.ResetAfterCompletion(frame));REQUIRE(slot.cache.Reset());
    for(const auto& weak:slot.weak)REQUIRE(weak.expired());slot.weak.clear();
    Allocator allocator;Allocation allocation;const std::vector<uint8_t>* bytes=nullptr;
    for(size_t i=0;i<96;++i) {
      auto v=version(uint8_t(i),i);slot.weak.push_back(v);
      REQUIRE(slot.cache.Bind(FrameConstantKind::kVertex,v,allocation,bytes,materialize,allocator)==Result::kUploaded);
      REQUIRE(slot.cache.Bind(FrameConstantKind::kVertex,v,allocation,bytes,materialize,allocator)==Result::kVersionHit);
    }
    REQUIRE(slot.cache.owner_count()==96);REQUIRE(slot.cache.entry_count()==96);
    REQUIRE(slot.index.MarkSubmitted(frame));
  }
  for(auto& slot:slots){REQUIRE(slot.index.ResetAfterCompletion(UINT64_MAX));REQUIRE(slot.cache.Reset());
    for(const auto& weak:slot.weak)REQUIRE(weak.expired());REQUIRE(slot.cache.owner_count()==0);}
}
TEST_CASE("Ordered and out-of-order constant reconstruction match complete snapshots", "[hotpath][constants]") {
  std::mt19937 random(7923);AuthoritativeConstantState state(512);
  std::vector<uint8_t> reference(512);ConstantPayloadDelta bootstrap;
  REQUIRE(CaptureCompleteConstantSnapshot(reference,bootstrap));
  auto hash=[](std::span<const uint8_t> bytes){uint64_t h=0;for(auto v:bytes)h=h*31+v;return h;};
  REQUIRE(state.Apply(bootstrap,hash));
  std::vector<std::pair<std::shared_ptr<const ConstantStateVersion>,std::vector<uint8_t>>> snapshots;
  for(unsigned i=0;i<4096;++i) {
    const uint32_t offset=(random()%128)*4;
    ConstantPayloadDelta delta;delta.ranges.push_back({offset,0,4});
    for(size_t j=0;j<4;++j){auto v=uint8_t(random());reference[offset+j]=v;delta.payload.push_back(v);}
    auto result=state.Apply(delta,hash);REQUIRE(result);snapshots.emplace_back(result.version,reference);
    if(i%3==0)REQUIRE(*materialize(result.version)==reference);
  }
  std::shuffle(snapshots.begin(),snapshots.end(),random);
  for(const auto& [v,expected]:snapshots){REQUIRE(*materialize(v)==expected);REQUIRE_FALSE(v->parent);}
}

#define SHARED_SCALARS(X) \
 X(boolean_version.epoch) X(boolean_version.revision) X(image_descriptor_epoch) X(sampler_descriptor_epoch) \
 X(cached_descriptor_epoch) X(environmental_data_hash) X(environmental_sequence) X(device) \
 X(descriptor_copy) X(descriptor_page) X(width) X(height) X(logical_width) X(logical_height) \
 X(sample_count) X(alpha_reference_bits) X(alpha_to_mask) X(color_output_mask) X(clip_plane_enable_mask) \
 X(vertex_booleans) X(pixel_booleans) X(descriptor_backend) X(environment_present)
TEST_CASE("Single-pass shared key includes every semantic field without struct padding", "[hotpath]") {
  using Key=SharedConstantSemanticKey<16>;Key original{};const auto words=NativeSharedKeyWords(original);
#define X(field) {auto changed=original;++changed.field;REQUIRE(NativeSharedKeyWords(changed)!=words);}
  SHARED_SCALARS(X)
#undef X
  for(size_t i=0;i<16;++i){auto c=original;++c.texture_descriptor_indices[i];REQUIRE(NativeSharedKeyWords(c)!=words);
    c=original;++c.sampler_descriptor_indices[i];REQUIRE(NativeSharedKeyWords(c)!=words);
    c=original;c.sampler_lod_bias_bits[i]=0x80000000u;REQUIRE(NativeSharedKeyWords(c)!=words);}
  for(size_t i=0;i<4;++i){auto c=original;++c.color_output_info[i];REQUIRE(NativeSharedKeyWords(c)!=words);
    c=original;c.clip_plane_bits[i]=0x7FC00001u;REQUIRE(NativeSharedKeyWords(c)!=words);}
  Key a,b;std::memset(&a,0xA5,sizeof(a));std::memset(&b,0x5A,sizeof(b));
#define X(field) a.field=original.field;b.field=original.field;
  SHARED_SCALARS(X)
#undef X
  a.texture_descriptor_indices=b.texture_descriptor_indices=original.texture_descriptor_indices;
  a.sampler_descriptor_indices=b.sampler_descriptor_indices=original.sampler_descriptor_indices;
  a.sampler_lod_bias_bits=b.sampler_lod_bias_bits=original.sampler_lod_bias_bits;
  a.color_output_info=b.color_output_info=original.color_output_info;
  a.clip_plane_bits=b.clip_plane_bits=original.clip_plane_bits;
  REQUIRE(a==b);REQUIRE(NativeSharedKeyWords(a)==NativeSharedKeyWords(b));
}
#undef SHARED_SCALARS

namespace {
struct FakeState {std::array<uint32_t,16> textures{};};
struct FakeCommand {
  std::shared_ptr<FakeState> pipeline_state=std::make_shared<FakeState>();
  std::array<std::shared_ptr<int>,16> textures{};
  std::array<std::array<uint32_t,6>,16> texture_fetches{};
  uint32_t used_texture_mask=1,failed_texture_mask=0,descriptor_page=0,descriptor_copy=0;
  uint64_t image_descriptor_epoch=0,sampler_descriptor_epoch=0,cached_descriptor_epoch=0;
  std::array<uint32_t,16> texture_descriptor_indices{},sampler_descriptor_indices{},binding_realization{};
  std::array<uint64_t,5> draw_descriptor_sets{};
  uint32_t realized_image_mask=0,realized_sampler_mask=0,guest_null_texture_mask=0;
  bool bindings_prepared=true;
  std::shared_ptr<int> room_light_input_bindings;
};
}
TEST_CASE("Prepared texture bindings preserve null meaning fetch data and descriptor epochs", "[hotpath]") {
  FakeCommand a;a.textures[0]=std::make_shared<int>(42);a.pipeline_state->textures[0]=100;
  a.descriptor_page=4;a.descriptor_copy=0;a.image_descriptor_epoch=17;a.sampler_descriptor_epoch=19;
  a.texture_descriptor_indices[0]=12;a.sampler_descriptor_indices[0]=7;
  auto b=a;b.bindings_prepared=false;
  REQUIRE(NativePreparedTextureInputsEqual(a,b));CopyNativePreparedTextureBindings(a,b);
  REQUIRE(b.bindings_prepared);REQUIRE(b.texture_descriptor_indices==a.texture_descriptor_indices);
  REQUIRE(b.descriptor_page==4);REQUIRE(b.image_descriptor_epoch==17);REQUIRE(b.sampler_descriptor_epoch==19);
  for(size_t i=0;i<6;++i){auto c=b;++c.texture_fetches[0][i];REQUIRE_FALSE(NativePreparedTextureInputsEqual(a,c));}
  auto c=b;c.textures[0]=std::make_shared<int>(42);REQUIRE_FALSE(NativePreparedTextureInputsEqual(a,c));
  c=b;c.pipeline_state=std::make_shared<FakeState>(*b.pipeline_state);c.pipeline_state->textures[0]=0;
  REQUIRE_FALSE(NativePreparedTextureInputsEqual(a,c));
  c=b;c.used_texture_mask=3;REQUIRE_FALSE(NativePreparedTextureInputsEqual(a,c));
  a.failed_texture_mask=1;REQUIRE_FALSE(NativePreparedTextureInputsEqual(a,b));
  a.failed_texture_mask=0;a.room_light_input_bindings=std::make_shared<int>(1);
  b.room_light_input_bindings=std::make_shared<int>(2);CopyNativePreparedTextureBindings(a,b);
  REQUIRE_FALSE(b.room_light_input_bindings);REQUIRE(a.room_light_input_bindings);
}
TEST_CASE("Incremental queue protection equals brute-force scans across streaming and flushes", "[hotpath][memory]") {
  NativeTextureProtectionIndex queued;std::deque<std::vector<uint64_t>> queue;
  std::vector<std::vector<uint64_t>> frame;std::unordered_set<uint64_t> frame_set;
  std::mt19937 rng(1674);
  for(unsigned step=0;step<20000;++step) {
    const bool enqueue=queue.empty() || (rng()%3!=0 && queue.size()<128);
    if(enqueue){std::vector<uint64_t> refs;for(unsigned i=0;i<5;++i){auto g=uint64_t(rng()%800+1);refs.push_back(g);REQUIRE(queued.Retain(g));}queue.push_back(refs);}
    else {auto refs=queue.front();queue.pop_front();for(auto g:refs){REQUIRE(queued.Release(g));frame_set.insert(g);}frame.push_back(std::move(refs));}
    if(step%67==0){frame.clear();frame_set.clear();}
    if(step%7==0){
      std::unordered_set<uint64_t> fast=frame_set;queued.AppendTo(fast);std::unordered_set<uint64_t> exact;
      for(const auto& command:queue)exact.insert(command.begin(),command.end());
      for(const auto& command:frame)exact.insert(command.begin(),command.end());
      REQUIRE(fast==exact);REQUIRE(queued.valid());REQUIRE(queued.size()<=800);
    }
  }
  for(const auto& command:queue)for(auto g:command)REQUIRE(queued.Release(g));
  REQUIRE(queued.size()==0);REQUIRE_FALSE(queued.Release(7));REQUIRE_FALSE(queued.valid());
  queued.Reset();REQUIRE(queued.valid());REQUIRE(queued.size()==0);
}
TEST_CASE("POD frame maps discard old allocation identities after every reset", "[hotpath][memory]") {
  FrameGenerationMap<uint64_t,Allocation> map;
  for(uint64_t frame=1;frame<600;++frame){REQUIRE(map.ResetGeneration());
    REQUIRE(map.size()==0);REQUIRE_FALSE(map.Find(frame-1));
    for(uint64_t key=1;key<257;++key)REQUIRE(map.Insert(frame*1000+key,{key,64}));
    REQUIRE(map.size()==256);REQUIRE(map.bucket_count()<=1024);
  }
}
TEST_CASE("Shader diagnostic fallback classifies shadows without mutating runtime phases", "[hotpath]") {
  using namespace rex::graphics::gta4_native::performance;
  REQUIRE(ProfileShaderCategory("shader/rage_shaders/shadowzdir/shadowzdir_vs0.bin","")==GpuRange::kDirectionalShadowShaders);
  REQUIRE(ProfileShaderCategory("shader/rage_shaders/gta_default/gta_default_vs0.bin","")==GpuRange::kMaterialShaders);
  REQUIRE(ProfileShaderCategory("something-new", "something-new")==GpuRange::kUnattributed);
  REQUIRE(ProfileShaderCategory("", "shader/rage_shaders/rage_postfx_e2.fxc")==GpuRange::kComposite);
}
