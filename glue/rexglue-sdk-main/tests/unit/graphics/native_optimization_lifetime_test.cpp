#include <catch2/catch_test_macros.hpp>
#include <atomic>
#include <bit>
#include <deque>
#include <functional>
#include <map>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_set>
#include <xxhash.h>
#include "graphics/gta4_native/native_immutable_bindings.h"
#include "graphics/gta4_native/native_texture_protection.h"
#include "graphics/gta4_native/native_gpu_attribution.h"

using namespace rex::graphics::gta4_native;
namespace {
struct Allocation { uint64_t id=0; uint32_t kind=0; };
using Bindings=NativeImmutableBindings<Allocation>;
std::shared_ptr<const ConstantStateVersion> Version(std::vector<uint8_t> bytes, uint64_t hash, uint64_t revision=1) {
  auto v=std::make_shared<ConstantStateVersion>();v->byte_size=bytes.size();v->content_hash=hash;
  v->token={1,revision};v->materialized=std::make_shared<const std::vector<uint8_t>>(std::move(bytes));return v;
}
struct Backend {
  size_t materializations=0,uploads=0;bool fail=false;
  std::map<uint64_t,std::vector<uint8_t>> gpu;
  const std::vector<uint8_t>* Materialize(const auto& v){++materializations;return AuthoritativeConstantState::MaterializeView(v);}
  bool Upload(FrameConstantKind kind,uint64_t,const std::vector<uint8_t>& bytes,Allocation& out){
    if(fail)return false;out={++uploads,uint32_t(kind)};gpu[out.id]=bytes;return true;
  }
  auto Bind(Bindings& cache,FrameConstantKind kind,const auto& version,Allocation& allocation,const std::vector<uint8_t>*& bytes){
    return cache.Bind(kind,version,allocation,bytes,[&](const auto& v){return Materialize(v);},
        [&](auto k,auto identity,const auto& data,auto& out){return Upload(k,identity,data,out);});
  }
};
}

TEST_CASE("Immutable draw bindings reuse exact versions before materializing", "[graphics][optimization]") {
  Bindings cache;Backend backend;Allocation out;const std::vector<uint8_t>* bytes=nullptr;
  const auto v=Version({0x7f,0xc0,0,1,0x80,0,0,0,0xff,0xff,0xff,0xff},42);
  REQUIRE(backend.Bind(cache,FrameConstantKind::kVertex,v,out,bytes)==Bindings::Result::kUploaded);
  const auto first=out;
  for(size_t i=0;i<10000;++i){REQUIRE(backend.Bind(cache,FrameConstantKind::kVertex,v,out,bytes)==Bindings::Result::kVersionHit);REQUIRE(out.id==first.id);}
  REQUIRE(backend.materializations==1);REQUIRE(backend.uploads==1);REQUIRE(cache.owner_count()==1);
  REQUIRE(*bytes==*v->materialized);REQUIRE(backend.gpu[out.id]==*v->materialized);
  // Identical hash/data in a different stage still has independent binding identity.
  REQUIRE(backend.Bind(cache,FrameConstantKind::kPixel,v,out,bytes)==Bindings::Result::kUploaded);
  REQUIRE(out.id!=first.id);REQUIRE(out.kind==uint32_t(FrameConstantKind::kPixel));
}
TEST_CASE("Hash collisions never alias unequal constants and errors publish nothing", "[graphics][optimization]") {
  Bindings cache;Backend backend;Allocation a,b,c;const std::vector<uint8_t>* bytes=nullptr;
  auto v1=Version({1,2,3,4},0),v2=Version({5,6,7,8},0),v3=Version({1,2,3,4},0);
  REQUIRE(backend.Bind(cache,FrameConstantKind::kVertex,v1,a,bytes)==Bindings::Result::kUploaded);
  REQUIRE(backend.Bind(cache,FrameConstantKind::kVertex,v2,b,bytes)==Bindings::Result::kUploaded);
  REQUIRE(a.id!=b.id);REQUIRE(backend.gpu.at(b.id)==*v2->materialized);
  REQUIRE(backend.Bind(cache,FrameConstantKind::kVertex,v3,c,bytes)==Bindings::Result::kContentHit);
  REQUIRE(c.id==a.id);REQUIRE(*bytes==*v3->materialized);
  REQUIRE(cache.owner_count()==3);REQUIRE(cache.content_count()==1);
  auto v4=Version({9,10,11,12},7);backend.fail=true;
  REQUIRE(backend.Bind(cache,FrameConstantKind::kVertex,v4,c,bytes)==Bindings::Result::kFailure);
  REQUIRE(cache.entry_count()==3);REQUIRE(cache.owner_count()==3);
  backend.fail=false;REQUIRE(backend.Bind(cache,FrameConstantKind::kVertex,v4,c,bytes)==Bindings::Result::kUploaded);
  REQUIRE(backend.gpu.at(c.id)==*v4->materialized);
  REQUIRE(backend.Bind(cache,FrameConstantKind::kShared,v4,c,bytes)==Bindings::Result::kFailure);
}
TEST_CASE("Temporary diagnostic versions stay alive only until the slot resets", "[graphics][optimization][memory]") {
  Bindings slot0,slot1;Backend backend;Allocation out;const std::vector<uint8_t>* bytes=nullptr;
  std::weak_ptr<const ConstantStateVersion> a,b;std::weak_ptr<const std::vector<uint8_t>> data;
  {
    auto v=Version({9,8,7,6},99);a=v;data=v->materialized;
    REQUIRE(backend.Bind(slot0,FrameConstantKind::kPixel,v,out,bytes)==Bindings::Result::kUploaded);
  }
  REQUIRE_FALSE(a.expired());REQUIRE_FALSE(data.expired());REQUIRE((*bytes)[0]==9);
  {
    auto v=Version({9,8,7,6},99);b=v;
    REQUIRE(backend.Bind(slot1,FrameConstantKind::kPixel,v,out,bytes)==Bindings::Result::kUploaded);
  }
  REQUIRE(slot0.Reset());REQUIRE(a.expired());REQUIRE(data.expired());REQUIRE_FALSE(b.expired());
  REQUIRE(slot1.Reset());REQUIRE(b.expired());REQUIRE(slot0.owner_count()==0);REQUIRE(slot1.owner_count()==0);
}
TEST_CASE("Streaming constant generations match a byte oracle and release every owner", "[graphics][optimization][memory]") {
  Bindings cache;Backend backend;std::mt19937 random(0x83211);size_t peak=0;
  for(uint32_t frame=0;frame<600;++frame){
    std::vector<std::weak_ptr<const ConstantStateVersion>> expired;
    for(uint32_t draw=0;draw<80;++draw){
      std::vector<uint8_t> expected(128);for(auto& byte:expected)byte=uint8_t(random());
      // Deliberately frequent hash collisions exercise full-byte checks.
      auto v=Version(expected,draw%4,draw+1);expired.emplace_back(v);
      Allocation first,out;const std::vector<uint8_t>* bytes=nullptr;
      auto kind=draw%2?FrameConstantKind::kVertex:FrameConstantKind::kPixel;
      REQUIRE(backend.Bind(cache,kind,v,first,bytes)!=Bindings::Result::kFailure);
      REQUIRE(backend.gpu.at(first.id)==expected);
      REQUIRE(backend.Bind(cache,kind,v,out,bytes)==Bindings::Result::kVersionHit);
      REQUIRE(out.id==first.id);REQUIRE(*bytes==expected);
    }
    peak=std::max(peak,cache.retained_capacity_bytes());
    REQUIRE(cache.owner_count()==80);REQUIRE(cache.Reset());REQUIRE(cache.entry_count()==0);
    for(const auto& v:expired)REQUIRE(v.expired());
    backend.gpu.clear();
  }
  REQUIRE(peak<=16384); // Bounded capacity, not one retained owner per historical frame.
}
TEST_CASE("Shared semantic key encoding includes every field without padding", "[graphics][optimization]") {
  using Key=SharedConstantSemanticKey<16>;const Key baseline{};const auto encoded=NativeSharedKeyWords(baseline);
  const auto differs=[&](const auto& mutate){Key changed{};mutate(changed);REQUIRE(changed!=baseline);REQUIRE(NativeSharedKeyWords(changed)!=encoded);};
  for(size_t i=0;i<16;++i){
    differs([&](auto& k){k.texture_descriptor_indices[i]=1;});differs([&](auto& k){k.sampler_descriptor_indices[i]=1;});
    differs([&](auto& k){k.sampler_lod_bias_bits[i]=0x80000000;});
  }
#define FIELD(f) differs([](auto& k){k.f=1;})
  FIELD(boolean_version.epoch);FIELD(boolean_version.revision);FIELD(image_descriptor_epoch);FIELD(sampler_descriptor_epoch);
  FIELD(cached_descriptor_epoch);FIELD(environmental_data_hash);FIELD(environmental_sequence);FIELD(device);
  FIELD(descriptor_copy);FIELD(descriptor_page);FIELD(width);FIELD(height);FIELD(logical_width);FIELD(logical_height);
  FIELD(sample_count);FIELD(alpha_reference_bits);FIELD(alpha_to_mask);FIELD(color_output_mask);FIELD(clip_plane_enable_mask);
  FIELD(vertex_booleans);FIELD(pixel_booleans);FIELD(descriptor_backend);FIELD(environment_present);
#undef FIELD
  for(size_t i=0;i<4;++i){differs([&](auto& k){k.color_output_info[i]=1;});differs([&](auto& k){k.clip_plane_bits[i]=0x7fc00001;});}
  REQUIRE(NativeSharedKeyWords(baseline)==encoded);
}
TEST_CASE("Queued texture generation references preserve multiplicity and fail closed", "[graphics][optimization][memory]") {
  NativeTextureProtectionIndex index;
  REQUIRE(index.Retain(0));REQUIRE(index.Release(0));REQUIRE(index.size()==0);
  REQUIRE(index.Retain(12));REQUIRE(index.Retain(12));REQUIRE(index.Retain(24));
  REQUIRE(index.Release(12));REQUIRE(index.Contains(12));REQUIRE(index.Release(12));REQUIRE_FALSE(index.Contains(12));
  std::unordered_set<uint64_t> pins{100};index.AppendTo(pins);REQUIRE(pins==std::unordered_set<uint64_t>{24,100});
  REQUIRE_FALSE(index.Release(12));REQUIRE_FALSE(index.valid());
  index.Reset();REQUIRE(index.valid());REQUIRE(index.size()==0);
}
TEST_CASE("Queued generation index agrees with an independent queue scan", "[graphics][optimization][memory]") {
  NativeTextureProtectionIndex index;std::deque<std::array<uint64_t,8>> queue;std::mt19937 random(1079);
  for(size_t step=0;step<30000;++step){
    if(queue.empty() || (random()%3 && queue.size()<64)){
      std::array<uint64_t,8> refs{};for(auto& id:refs){id=random()%200;REQUIRE(index.Retain(id));}queue.push_back(refs);
    }else{for(auto id:queue.front())REQUIRE(index.Release(id));queue.pop_front();}
    if(step%50==0){std::unordered_set<uint64_t> oracle,actual;for(const auto& cmd:queue)for(auto id:cmd)if(id)oracle.insert(id);
      index.AppendTo(actual);REQUIRE(actual==oracle);REQUIRE(index.size()==oracle.size());REQUIRE(index.valid());}
  }
  for(const auto& cmd:queue)for(auto id:cmd)REQUIRE(index.Release(id));REQUIRE(index.size()==0);REQUIRE(index.valid());
}
TEST_CASE("Generation reuse cache never returns data from a previous recording", "[graphics][optimization][memory]") {
  FrameGenerationMap<uint64_t,Allocation> map;size_t peak=0;
  for(uint32_t frame=0;frame<1000;++frame){
    for(uint64_t i=0;i<256;++i){REQUIRE_FALSE(map.Find(i));REQUIRE(map.Insert(i,{uint64_t(frame)+1,uint32_t(i%2)}));}
    size_t count=0;map.ForEach([&](auto,const auto& value){REQUIRE(value.id==frame+1);++count;});REQUIRE(count==256);
    peak=std::max(peak,map.bucket_count());REQUIRE(map.ResetGeneration());REQUIRE(map.size()==0);
  }
  REQUIRE(peak==512);
}

#include "graphics/gta4_native/native_profile_shader_category.h"
TEST_CASE("Profiler fallback distinguishes observed shader families", "[graphics][optimization][profile]") {
  using namespace performance;
  REQUIRE(ProfileShaderCategory("shader/rage_shaders/shadowzdir/shadowzdir_vs0.bin",{})==GpuRange::kDirectionalShadowShaders);
  REQUIRE(ProfileShaderCategory("shader/rage_shaders/shadowz/shadowz_vs0.bin",{})==GpuRange::kLocalShadowShaders);
  REQUIRE(ProfileShaderCategory("shader/rage_shaders/gta_default/gta_default_vs0.bin",{})==GpuRange::kMaterialShaders);
  REQUIRE(ProfileShaderCategory("shader/rage_shaders/gta_default_typo/test.bin",{})==GpuRange::kUnattributed);
  REQUIRE(ProfileShaderCategory("shader/rage_shaders/gta_im/gta_im_vs2.bin",{})==GpuRange::kImmediateShaders);
  REQUIRE(ProfileShaderCategory({},{})==GpuRange::kUnattributed);
}
TEST_CASE("Larger validated GPU budgets actually expand pass detail and retain endpoints", "[graphics][optimization][profile]") {
  using namespace attribution;std::vector<NativePassObservation> observations;
  for(uint32_t i=0;i<400;++i){NativePassObservation o;o.command_index=i;o.coarse_range=0;o.key.pixel_shader_hash=i+1;o.draw_count=1;observations.push_back(o);}
  NativeAttributionQueryBudgetInput input;input.query_capacity=512;input.maximum_detail_boundaries=256;
  const auto budget=CalculateNativeAttributionQueryBudget(observations,input);
  REQUIRE(budget.mandatory_boundaries_fit);
  const auto plan=BuildNativeAttributionPlan(observations,budget.plan_budget);
  REQUIRE(plan.regular_detail_boundaries>kRegularDetailBoundaryBudget);
  REQUIRE(plan.regular_detail_boundaries+plan.drilldown_boundaries<=budget.maximum_detail_boundaries);
  REQUIRE(budget.required_query_count+plan.regular_detail_boundaries+plan.drilldown_boundaries<=input.query_capacity);
  REQUIRE(CheckNativeAttributionCoverage(observations,plan).complete);
  for(uint32_t limit=16;limit<1024;limit+=17){
    input.query_capacity=limit;const auto b=CalculateNativeAttributionQueryBudget(observations,input);
    const auto p=BuildNativeAttributionPlan(observations,b.plan_budget);
    REQUIRE(b.required_query_count+p.regular_detail_boundaries+p.drilldown_boundaries<=limit);
    REQUIRE(CheckNativeAttributionCoverage(observations,p).complete);
  }
}

TEST_CASE("Unconsumed constant updates bound ancestry without losing old snapshots", "[graphics][optimization][memory]") {
  AuthoritativeConstantState state(128);
  std::vector<uint8_t> oracle(128);
  const auto hash=[](std::span<const uint8_t> b){return XXH3_64bits(b.data(),b.size());};
  ConstantPayloadDelta initial;REQUIRE(CaptureCompleteConstantSnapshot(oracle,initial));
  REQUIRE(state.Apply(initial,hash).status==ConstantApplyStatus::kApplied);
  std::vector<std::pair<std::shared_ptr<const ConstantStateVersion>,std::vector<uint8_t>>> saved;
  // The bootstrap is deliberately owned by the first saved snapshot. Track
  // a separate unconsumed update to verify that obsolete ancestry is released.
  std::weak_ptr<const ConstantStateVersion> bootstrap=state.current_version();
  std::weak_ptr<const ConstantStateVersion> early;
  for(uint32_t update=0;update<12000;++update){
    const uint32_t offset=(update%32)*4;
    const uint32_t value=update+1;std::memcpy(oracle.data()+offset,&value,sizeof(value));
    ConstantPayloadDelta delta;delta.ranges.push_back({offset,0,4});delta.payload.resize(4);
    std::memcpy(delta.payload.data(),&value,4);
    const auto result=state.Apply(delta,hash);REQUIRE(result.status==ConstantApplyStatus::kApplied);
    if(update==1)early=result.version;
    REQUIRE(result.version->content_hash==hash(oracle));
    size_t ancestors=0;auto parent=result.version->parent;
    for(;parent;parent=parent->parent)++ancestors;
    REQUIRE(ancestors<=AuthoritativeConstantState::kMaximumDeferredAncestors);
    if(update%773==0)saved.emplace_back(result.version,oracle);
  }
  REQUIRE_FALSE(bootstrap.expired());
  REQUIRE(early.expired());
  REQUIRE(*AuthoritativeConstantState::Materialize(state.current_version())==oracle);
  for(auto it=saved.rbegin();it!=saved.rend();++it)
    REQUIRE(*AuthoritativeConstantState::Materialize(it->first)==it->second);
  std::vector<std::weak_ptr<const ConstantStateVersion>> owners;
  for(const auto& item:saved)owners.push_back(item.first);
  saved.clear();for(const auto& owner:owners)REQUIRE(owner.expired());
  REQUIRE(bootstrap.expired());
}
