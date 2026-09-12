#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <deque>
#include <memory>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <xxhash.h>
#include "graphics/gta4_native/native_immutable_bindings.h"
#include "graphics/gta4_native/native_texture_protection.h"
#include "graphics/gta4_native/native_prepared_bindings.h"
#include "graphics/gta4_native/native_profile_shader_category.h"
#include "graphics/gta4_native/native_gpu_attribution.h"

namespace hot = rex::graphics::gta4_native;
namespace {
struct Allocation { uint64_t address=0; uint32_t size=0; bool operator==(const Allocation&) const = default; };
using Bindings=hot::NativeImmutableBindings<Allocation>;
using Result=Bindings::Result;
using Kind=hot::FrameConstantKind;
using Version=hot::ConstantStateVersion;
std::shared_ptr<const Version> MakeVersion(uint64_t tag,uint64_t hash,uint8_t fill,size_t size=256) {
 auto v=std::make_shared<Version>();v->byte_size=size;v->content_hash=hash;v->token.revision=tag;
 hot::CaptureCompleteConstantSnapshot(std::vector<uint8_t>(size,fill),v->delta);
 return v;
}
struct Backend {
 size_t materializations=0,uploads=0;bool fail=false;
 std::vector<std::vector<uint8_t>> data;
 const std::vector<uint8_t>* Materialize(const std::shared_ptr<const Version>& v){++materializations;return hot::AuthoritativeConstantState::MaterializeView(v);}
 bool Upload(Kind,uint64_t,const std::vector<uint8_t>& bytes,Allocation& out){
  if(fail)return false;
  ++uploads;data.push_back(bytes);out={uint64_t(data.size()),uint32_t(bytes.size())};return true;
 }
 Result Bind(Bindings& c,Kind kind,const std::shared_ptr<const Version>& v,Allocation& out,const std::vector<uint8_t>*& view){
  return c.Bind(kind,v,out,view,[&](const auto& x){return Materialize(x);},[&](auto k,uint64_t id,const auto& x,auto& a){return Upload(k,id,x,a);});
 }
};
}
TEST_CASE("hotpath bindings avoid repeated materialization and preserve stage identity","[hotpath]") {
 Bindings c;Backend backend;Allocation a{},b{};const std::vector<uint8_t>* bytes=nullptr;
 auto v=MakeVersion(1,123,0xAB);
 REQUIRE(backend.Bind(c,Kind::kVertex,v,a,bytes)==Result::kUploaded);
 for(size_t i=0;i<50000;++i){REQUIRE(backend.Bind(c,Kind::kVertex,v,b,bytes)==Result::kVersionHit);REQUIRE(b==a);REQUIRE(bytes->at(i%bytes->size())==0xAB);}
 REQUIRE(backend.materializations==1);REQUIRE(backend.uploads==1);REQUIRE(c.owner_count()==1);
 REQUIRE(backend.Bind(c,Kind::kPixel,v,b,bytes)==Result::kUploaded);REQUIRE(b.address!=a.address);REQUIRE(c.owner_count()==2);
 REQUIRE(c.Reset());REQUIRE(c.owner_count()==0);REQUIRE(c.entry_count()==0);
 REQUIRE(backend.Bind(c,Kind::kVertex,v,b,bytes)==Result::kUploaded);
}
TEST_CASE("constant hash collisions cannot select different shader bytes","[hotpath]") {
 Bindings c;Backend backend;Allocation original{},equal{},collision{};const std::vector<uint8_t>* view=nullptr;
 auto a=MakeVersion(1,0,0xA1),b=MakeVersion(2,0,0xA1),d=MakeVersion(3,0,0xB2);
 REQUIRE(backend.Bind(c,Kind::kVertex,a,original,view)==Result::kUploaded);
 REQUIRE(backend.Bind(c,Kind::kVertex,b,equal,view)==Result::kContentHit);REQUIRE(original==equal);
 REQUIRE(backend.Bind(c,Kind::kVertex,d,collision,view)==Result::kUploaded);REQUIRE(collision.address!=original.address);
 REQUIRE(backend.data.at(collision.address-1)==*view);REQUIRE(view->at(0)==0xB2);
 REQUIRE(backend.Bind(c,Kind::kVertex,d,collision,view)==Result::kVersionHit);REQUIRE(view->at(0)==0xB2);
 REQUIRE(c.owner_count()==3);REQUIRE(c.Reset());
}
TEST_CASE("temporary diagnostic versions stay alive exactly until slot cache reset","[hotpath][memory]") {
 Bindings c;Backend backend;Allocation a;const std::vector<uint8_t>* view=nullptr;
 std::weak_ptr<const Version> weak;
 {auto v=MakeVersion(1,12,14);weak=v;REQUIRE(backend.Bind(c,Kind::kPixel,v,a,view)==Result::kUploaded);}
 REQUIRE_FALSE(weak.expired());REQUIRE(view->at(0)==14);REQUIRE(c.Reset());REQUIRE(weak.expired());
 backend.fail=true;auto rejected=MakeVersion(2,42,51);weak=rejected;
 REQUIRE(backend.Bind(c,Kind::kPixel,rejected,a,view)==Result::kFailure);REQUIRE(c.owner_count()==0);rejected.reset();REQUIRE(weak.expired());
 REQUIRE(backend.Bind(c,Kind::kShared,{},a,view)==Result::kFailure);
}
TEST_CASE("two arena lifetimes release their own constant owners after exact completion","[hotpath][memory]") {
 Bindings cache[2];hot::FrameConstantArenaIndex fences[2];std::weak_ptr<const Version> weak[2];
 Backend backend;Allocation a;const std::vector<uint8_t>* view=nullptr;
 for(size_t slot=0;slot<2;++slot){auto v=MakeVersion(slot+1,slot+99,uint8_t(slot));weak[slot]=v;REQUIRE(backend.Bind(cache[slot],Kind::kVertex,v,a,view)==Result::kUploaded);REQUIRE(fences[slot].MarkSubmitted(10+slot));}
 REQUIRE_FALSE(fences[0].CanResetAfterCompletion(9));REQUIRE_FALSE(weak[0].expired());
 REQUIRE(fences[0].ResetAfterCompletion(10));REQUIRE(cache[0].Reset());REQUIRE(weak[0].expired());REQUIRE_FALSE(weak[1].expired());
 REQUIRE_FALSE(fences[1].ResetUnsubmitted());REQUIRE(fences[1].ResetAfterCompletion(11));REQUIRE(cache[1].Reset());REQUIRE(weak[1].expired());
}
TEST_CASE("thousands of frame resets do not retain constant histories","[hotpath][memory]") {
 Bindings cache[2];Backend backend;Allocation a;const std::vector<uint8_t>* view=nullptr;
 for(size_t frame=0;frame<4000;++frame){auto& c=cache[frame%2];REQUIRE(c.Reset());
  std::vector<std::weak_ptr<const Version>> owners;
  for(size_t i=0;i<32;++i){auto v=MakeVersion(frame*32+i+1,i%3,uint8_t(i));owners.push_back(v);REQUIRE(backend.Bind(c,Kind::kVertex,v,a,view)!=Result::kFailure);REQUIRE(backend.data.at(a.address-1)==*view);}
  REQUIRE(c.owner_count()==32);REQUIRE(c.Reset());REQUIRE(c.owner_count()==0);
  for(const auto& owner:owners)REQUIRE(owner.expired());backend.data.clear();
 }
}
TEST_CASE("unconsumed constant updates keep ancestry bounded without losing old snapshots","[hotpath][memory]") {
 hot::AuthoritativeConstantState state(1024);hot::ConstantPayloadDelta delta;
 std::vector<uint8_t> expected(1024);hot::CaptureCompleteConstantSnapshot(expected,delta);
 auto hash=[](std::span<const uint8_t> b){return XXH3_64bits(b.data(),b.size());};REQUIRE(state.Apply(delta,hash));
 std::vector<std::pair<std::shared_ptr<const Version>,std::vector<uint8_t>>> retained;
 for(uint32_t i=1;i<=50000;++i){size_t offset=(i*4)%expected.size();uint32_t value=i;
  delta={};delta.ranges={{uint32_t(offset),0,4}};delta.payload.resize(4);std::memcpy(delta.payload.data(),&value,4);std::memcpy(expected.data()+offset,&value,4);
  auto applied=state.Apply(delta,hash);REQUIRE(applied);REQUIRE(applied.changed);
  if(i%37==0){size_t count=0;auto cursor=applied.version;for(;cursor;cursor=cursor->parent)++count;REQUIRE(count<=65);}
  if(i%1000==0)retained.emplace_back(applied.version,expected);
 }
 REQUIRE(*hot::AuthoritativeConstantState::Materialize(state.current_version())==expected);
 for(const auto& [version,bytes]:retained)REQUIRE(*hot::AuthoritativeConstantState::Materialize(version)==bytes);
}
TEST_CASE("full constant replacements release unused ancestry immediately","[hotpath][memory]") {
 hot::AuthoritativeConstantState state(256);hot::ConstantPayloadDelta d;std::vector<uint8_t> bytes(256,1);
 auto hash=[](auto x){return XXH3_64bits(x.data(),x.size());};hot::CaptureCompleteConstantSnapshot(bytes,d);
 auto first=state.Apply(d,hash);std::weak_ptr<const Version> old=first.version;first.version.reset();
 std::fill(bytes.begin(),bytes.end(),2);hot::CaptureCompleteConstantSnapshot(bytes,d);auto second=state.Apply(d,hash);
 REQUIRE(second);REQUIRE_FALSE(second.version->parent);REQUIRE(old.expired());REQUIRE(*hot::AuthoritativeConstantState::Materialize(second.version)==bytes);
}
TEST_CASE("fast direct-parent materialization matches randomized guest-endian replay","[hotpath]") {
 std::mt19937 rng(1957);hot::AuthoritativeConstantState state(512);std::vector<uint8_t> expected(512,0);hot::ConstantPayloadDelta d;
 auto hash=[](auto x){return XXH3_64bits(x.data(),x.size());};hot::CaptureCompleteConstantSnapshot(expected,d);REQUIRE(state.Apply(d,hash));
 for(size_t i=0;i<20000;++i){const auto old=state.current_version();const auto saved=hot::AuthoritativeConstantState::Materialize(old);const auto old_bytes=*saved;
  size_t offset=(rng()%128)*4;uint32_t word=rng();d={};d.ranges={{uint32_t(offset),0,4}};d.payload.resize(4);std::memcpy(d.payload.data(),&word,4);std::memcpy(expected.data()+offset,&word,4);
  auto result=state.Apply(d,hash);REQUIRE(result);const auto* view=hot::AuthoritativeConstantState::MaterializeView(result.version);REQUIRE(view);REQUIRE(*view==expected);REQUIRE(*saved==old_bytes);
 }
}
TEST_CASE("queue generation pins match brute-force scanning through interleaved streaming","[hotpath][memory]") {
 hot::NativeTextureProtectionIndex pins;std::deque<std::vector<uint64_t>> queue;std::mt19937 rng(1729);
 for(size_t step=0;step<30000;++step){
  if(queue.empty()|| (queue.size()<80 && rng()%3)){auto& refs=queue.emplace_back();for(size_t i=0,n=1+rng()%9;i<n;++i){uint64_t id=1+rng()%500;refs.push_back(id);REQUIRE(pins.Retain(id));}}
  else{for(auto id:queue.front())REQUIRE(pins.Release(id));queue.pop_front();}
  if(step%17==0){std::unordered_set<uint64_t> expected,actual;for(const auto& refs:queue)for(auto id:refs)expected.insert(id);pins.AppendTo(actual);REQUIRE(actual==expected);REQUIRE(pins.size()==expected.size());REQUIRE(pins.valid());}
 }
 for(const auto& refs:queue)for(auto id:refs)REQUIRE(pins.Release(id));REQUIRE(pins.size()==0);
 REQUIRE(pins.Retain(0));REQUIRE(pins.Release(0));REQUIRE(pins.size()==0);REQUIRE_FALSE(pins.Release(123));REQUIRE_FALSE(pins.valid());pins.Reset();REQUIRE(pins.valid());
}
TEST_CASE("shared semantic key serialization covers every array and scalar without padding","[hotpath]") {
 using Key=hot::SharedConstantSemanticKey<3>;Key base{};const auto original=hot::NativeSharedKeyWords(base);size_t mutations=0;
 const auto check=[&](auto mutation){Key value=base;mutation(value);REQUIRE(value!=base);REQUIRE(hot::NativeSharedKeyWords(value)!=original);++mutations;};
 for(size_t i=0;i<3;++i){check([&](auto& k){k.texture_descriptor_indices[i]=1;});check([&](auto& k){k.sampler_descriptor_indices[i]=1;});check([&](auto& k){k.sampler_lod_bias_bits[i]=0x80000000u;});}
#define FIELD(name) check([](auto& k){k.name=1;})
 FIELD(boolean_version.epoch);FIELD(boolean_version.revision);FIELD(image_descriptor_epoch);FIELD(sampler_descriptor_epoch);FIELD(cached_descriptor_epoch);
 FIELD(environmental_data_hash);FIELD(environmental_sequence);FIELD(device);FIELD(descriptor_copy);FIELD(descriptor_page);FIELD(width);FIELD(height);FIELD(logical_width);FIELD(logical_height);FIELD(sample_count);FIELD(alpha_reference_bits);FIELD(alpha_to_mask);FIELD(color_output_mask);FIELD(clip_plane_enable_mask);FIELD(vertex_booleans);FIELD(pixel_booleans);FIELD(descriptor_backend);FIELD(environment_present);
#undef FIELD
 for(size_t i=0;i<4;++i){check([&](auto& k){k.color_output_info[i]=1;});check([&](auto& k){k.clip_plane_bits[i]=0x7FC00001u;});}
 REQUIRE(mutations==original.size());
}
namespace {
struct Texture {uint64_t generation=1;};
struct Pipeline {std::array<uint32_t,4> textures{};};
struct Fetch {std::array<uint32_t,6> words{};};
struct Prepared {
 std::shared_ptr<Pipeline> pipeline_state=std::make_shared<Pipeline>();
 std::array<std::shared_ptr<Texture>,4> textures{};std::array<Fetch,4> texture_fetches{};
 uint32_t used_texture_mask=0,failed_texture_mask=0;bool bindings_prepared=false;
 uint32_t descriptor_page=0,descriptor_copy=0;uint64_t image_descriptor_epoch=0,sampler_descriptor_epoch=0,cached_descriptor_epoch=0;
 std::array<uint32_t,4> texture_descriptor_indices{},sampler_descriptor_indices{},draw_descriptor_sets{},binding_realization{};
 uint32_t realized_image_mask=0,realized_sampler_mask=0,guest_null_texture_mask=0;
 std::shared_ptr<int> room_light_input_bindings;
};
}
TEST_CASE("prepared textures require exact used resource fetch and guest-null semantics","[hotpath]") {
 Prepared a;a.bindings_prepared=true;a.used_texture_mask=3;a.textures[0]=std::make_shared<Texture>();a.pipeline_state->textures[0]=42;
 Prepared b=a;REQUIRE(hot::NativePreparedTextureInputsEqual(a,b));
 for(size_t word=0;word<6;++word){b=a;b.texture_fetches[0].words[word]=1;REQUIRE_FALSE(hot::NativePreparedTextureInputsEqual(a,b));}
 b=a;b.textures[0]=std::make_shared<Texture>();REQUIRE_FALSE(hot::NativePreparedTextureInputsEqual(a,b));
 b=a;b.pipeline_state=std::make_shared<Pipeline>(*a.pipeline_state);b.pipeline_state->textures[1]=71;REQUIRE_FALSE(hot::NativePreparedTextureInputsEqual(a,b));
 b=a;b.used_texture_mask=1;REQUIRE_FALSE(hot::NativePreparedTextureInputsEqual(a,b));
 b=a;a.failed_texture_mask=1;REQUIRE_FALSE(hot::NativePreparedTextureInputsEqual(a,b));a.failed_texture_mask=0;
 a.bindings_prepared=false;REQUIRE_FALSE(hot::NativePreparedTextureInputsEqual(a,b));a.bindings_prepared=true;
 b=a;b.texture_fetches[3].words[0]=99;REQUIRE(hot::NativePreparedTextureInputsEqual(a,b));
 a.descriptor_page=7;a.image_descriptor_epoch=19;a.texture_descriptor_indices[0]=33;a.room_light_input_bindings=std::make_shared<int>(1);
 hot::CopyNativePreparedTextureBindings(a,b);REQUIRE(b.bindings_prepared);REQUIRE(b.descriptor_page==7);REQUIRE(b.image_descriptor_epoch==19);REQUIRE(b.texture_descriptor_indices[0]==33);REQUIRE_FALSE(b.room_light_input_bindings);REQUIRE(b.texture_fetches[3].words[0]==99);
}
TEST_CASE("generation caches retain capacity but no valid values across reset","[hotpath][memory]") {
 hot::FrameGenerationMap<uint64_t,uint64_t> map;size_t capacity=0;
 for(uint64_t frame=0;frame<1000;++frame){REQUIRE(map.ResetGeneration());REQUIRE(map.size()==0);
  for(uint64_t i=1;i<=600;++i){uint64_t key=frame*1000+i;REQUIRE(map.Insert(key,key+1));REQUIRE(*map.Find(key)==key+1);if(frame)REQUIRE(map.Find(key-1000)==nullptr);}
  if(frame==0)capacity=map.bucket_count();REQUIRE(map.bucket_count()==capacity);
 }
}
TEST_CASE("profiling shader families do not reclassify unknown code as a known pass","[hotpath][profiler]") {
 using R=hot::performance::GpuRange;auto classify=hot::performance::ProfileShaderCategory;
 REQUIRE(classify("shader/rage_shaders/shadowzdir/shadowzdir_vs0.bin","")==R::kDirectionalShadowShaders);
 REQUIRE(classify("shader/rage_shaders/shadowz/shadowz_vs0.bin","")==R::kLocalShadowShaders);
 REQUIRE(classify("shader/rage_shaders/gta_default/gta_default_vs0.bin","")==R::kMaterialShaders);
 REQUIRE(classify("","shader/rage_shaders/rage_postfx_e2.fxc")==R::kComposite);
 REQUIRE(classify("shader/runtime/unknown.vert","shader/runtime/unknown.frag")==R::kUnattributed);
 REQUIRE(classify("","")==R::kUnattributed);
}
TEST_CASE("configured GPU detail budget exceeds legacy default without query overrun","[hotpath][profiler]") {
 namespace a=hot::attribution;std::vector<a::NativePassObservation> observations(700);
 for(size_t i=0;i<observations.size();++i){auto& o=observations[i];o.command_index=uint32_t(i);o.coarse_range=1;o.key.vertex_shader_hash=i+1;o.key.command_class=a::NativePassCommandClass::kDraw;o.draw_count=1;o.primitive_count=3;}
 a::NativeAttributionQueryBudgetInput in;in.query_capacity=512;in.current_coarse_range=1;in.maximum_detail_boundaries=256;
 auto b=a::CalculateNativeAttributionQueryBudget(observations,in);REQUIRE(b.mandatory_boundaries_fit);
 auto plan=a::BuildNativeAttributionPlan(observations,b.plan_budget);REQUIRE(plan.regular_detail_boundaries>48);REQUIRE(plan.regular_detail_boundaries+plan.drilldown_boundaries<=b.available_detail_boundaries);REQUIRE(b.maximum_planned_query_count<=512);
 REQUIRE(a::CheckNativeAttributionCoverage(observations,plan).complete);
 auto legacy=a::BuildNativeAttributionPlan(observations);REQUIRE(legacy.regular_detail_boundaries<=48);
}
