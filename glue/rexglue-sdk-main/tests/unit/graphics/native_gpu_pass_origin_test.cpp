#include <catch2/catch_test_macros.hpp>
#include <array>
#include <atomic>
#include <map>
#include <random>
#include <thread>
#include <vector>
#include <rex/graphics/gta4_native/gpu_pass_origin.h>
#include "graphics/gta4_native/native_gpu_attribution.h"
#include "graphics/gta4_native/native_performance_samples.h"

using namespace rex::graphics::gta4_native;
namespace {
struct Fixture {
  std::map<uint32_t,uint32_t> memory;
  static constexpr uint32_t arena=0x10000,page=0x20000,r13=0x18000,tls=0x19000,task=0x1A000,offset=0x400;
  Fixture(){memory[arena+16]=0;memory[arena]=page;memory[page+offset]=kRetailNewDrawListVtable;
    memory[page+offset+8]=12;memory[page+offset+12]=31;}
  auto Read(uint32_t address)const->std::optional<uint32_t>{const auto i=memory.find(address);return i==memory.end()?std::nullopt:std::optional(i->second);}
  GpuPassOrigin Decode()const{return DecodeExecutedGpuDrawList(arena,offset,73,12,r13,[&](auto a){return Read(a);});}
  void Task(){memory[r13]=tls;memory[tls+16]=task;memory[task]=offset;memory[task+4]=73;memory[task+8]=31;memory[task+12]=3;memory[task+16]=12;}
};
}
TEST_CASE("GPU origin comes from the executing list header not the builder phase", "[graphics][gpu-origin]") {
  Fixture f;const auto before=f.memory;auto origin=f.Decode();
  REQUIRE(origin.attributed());REQUIRE(origin.source==GpuPassOriginSource::kListHeader);
  REQUIRE(origin.retail_phase==31);REQUIRE(origin.list_id==12);REQUIRE(origin.ordinal==12);
  REQUIRE(origin.arena==Fixture::arena);REQUIRE(f.memory==before);
  f.memory[Fixture::arena+16]=1;f.memory[Fixture::arena+4]=Fixture::page;
  REQUIRE(f.Decode().retail_phase==31);
  f.memory[Fixture::arena+16]=2;REQUIRE_FALSE(f.Decode().attributed());
}
TEST_CASE("GPU origin validates the task record and supports resumed lists", "[graphics][gpu-origin]") {
  Fixture f;f.Task();auto both=f.Decode();REQUIRE(both.source==GpuPassOriginSource::kVerifiedBoth);
  f.memory[Fixture::page+Fixture::offset]=0x82001070; // CDrawEntityDC after a resume.
  REQUIRE(f.Decode().source==GpuPassOriginSource::kExecutionRecord);
  for(auto field:{0u,4u,12u,16u}){const auto value=f.memory[Fixture::task+field];++f.memory[Fixture::task+field];
    REQUIRE_FALSE(f.Decode().attributed());f.memory[Fixture::task+field]=value;}
  f.memory[Fixture::page+Fixture::offset]=kRetailNewDrawListVtable;
  f.memory[Fixture::task+8]=23;auto conflict=f.Decode();
  REQUIRE_FALSE(conflict.attributed());REQUIRE(conflict.source==GpuPassOriginSource::kMismatch);
  REQUIRE(conflict.retail_phase==UINT32_MAX);
}
TEST_CASE("GPU origin treats invalid reads and address overflow as unknown", "[graphics][gpu-origin]") {
  Fixture f;f.memory.clear();REQUIRE_FALSE(f.Decode().attributed());
  size_t reads=0;
  auto decode=[&](uint32_t arena,uint32_t offset){return DecodeExecutedGpuDrawList(arena,offset,0,0,0,[&](uint32_t a)->std::optional<uint32_t>{++reads;if(a==0x10010)return 0;if(a==0x10000)return 0xFFFFFFF0;return {};});};
  REQUIRE_FALSE(decode(0,0).attributed());REQUIRE(reads==0);
  REQUIRE_FALSE(decode(UINT32_MAX-8,0).attributed());
  REQUIRE_FALSE(decode(0x10000,kRetailCommandArenaCapacity).attributed());
  REQUIRE_FALSE(decode(0x10000,32).attributed());
}
TEST_CASE("Retail GPU IDs stay distinct from host semantic phase enums", "[graphics][gpu-origin]") {
  Fixture f;f.memory[Fixture::page+Fixture::offset+12]=23;
  REQUIRE(std::string_view(RetailGpuPassName(f.Decode().retail_phase))=="frontend-or-phone-model");
  f.memory[Fixture::page+Fixture::offset+12]=0;
  REQUIRE(f.Decode().attributed());REQUIRE(f.Decode().retail_phase==0);
  f.memory[Fixture::page+Fixture::offset+12]=999;
  REQUIRE(f.Decode().retail_phase==999);REQUIRE(std::string_view(RetailGpuPassName(999))=="unknown-retail-phase");
  REQUIRE(performance::PerformanceRangeForRetailPhase(999)==performance::GpuRange::kUnattributed);
  REQUIRE(performance::PerformanceRangeForRetailPhase(31)==performance::GpuRange::kRetailSceneToGBuffer);
}
TEST_CASE("GPU envelope copies immutable origin and preserves every inner command byte", "[graphics][gpu-origin]") {
  Fixture f;auto origin=f.Decode();origin.scope=123;
  std::mt19937 random(9137);
  for(size_t size=1;size<1900;size+=13){
    std::vector<uint8_t> command(size);for(auto& b:command)b=uint8_t(random());
    const auto original=command;std::array<uint8_t,4096> bytes{};
    const auto packed=PackGpuPassEnvelope(bytes,command.data(),command.size(),16,origin);REQUIRE(packed==size+sizeof(GpuPassEnvelopeHeader));
    std::fill(command.begin(),command.end(),0);
    const void* payload=bytes.data();size_t length=packed;uint32_t abi=0;GpuPassOrigin decoded{};
    REQUIRE(UnpackGpuPassEnvelope(payload,length,abi,decoded));REQUIRE(length==size);REQUIRE(abi==16);REQUIRE(decoded==origin);
    REQUIRE(std::memcmp(payload,original.data(),size)==0);
  }
  for(uint32_t inner:{16u,0x50484F4Eu,0x1234u}){std::array<uint8_t,128> storage{};uint32_t cmd=0xAABBCCDD;
    size_t size=PackGpuPassEnvelope(storage,&cmd,sizeof(cmd),inner,origin);const void* data=storage.data();uint32_t abi=0;GpuPassOrigin out;
    REQUIRE(UnpackGpuPassEnvelope(data,size,abi,out));REQUIRE(abi==inner);REQUIRE(out==origin);}
}
TEST_CASE("Malformed GPU envelopes fail without partial output mutation", "[graphics][gpu-origin]") {
  std::array<uint8_t,256> storage{};uint64_t cmd=17;GpuPassOrigin in;in.scope=8;
  const auto size=PackGpuPassEnvelope(storage,&cmd,sizeof(cmd),16,in);REQUIRE(size>0);
  auto reject=[&](std::array<uint8_t,256> changed,size_t length){const void* data=changed.data();const void* before=data;uint32_t abi=99;GpuPassOrigin out;out.scope=999;
    REQUIRE_FALSE(UnpackGpuPassEnvelope(data,length,abi,out));REQUIRE(data==before);REQUIRE(abi==99);REQUIRE(out.scope==999);};
  reject(storage,size-1);reject(storage,size+1);reject(storage,0);
  for(size_t byte:{size_t(0),size_t(4),size_t(12)}){auto bad=storage;bad[byte]^=0x80;reject(bad,size);}
  auto bad=storage;std::memcpy(bad.data()+8,&kGpuPassEnvelopeAbi,4);reject(bad,size);
  REQUIRE(PackGpuPassEnvelope(std::span(storage).first(8),&cmd,sizeof(cmd),16,in)==0);
  REQUIRE(PackGpuPassEnvelope(storage,nullptr,8,16,in)==0);
  REQUIRE(PackGpuPassEnvelope(storage,&cmd,sizeof(cmd),kGpuPassEnvelopeAbi,in)==0);
}
TEST_CASE("The submitted list identity survives CPU queuing and region coalescing", "[graphics][gpu-origin]") {
  using namespace attribution;Fixture f;auto origin=f.Decode();origin.scope=1;
  NativePassKeyInput input;input.render_phase=0;input.origin=origin;input.pixel_shader_hash=11;
  auto a=BuildNativePassKey(input);REQUIRE(a.render_phase==0);REQUIRE(a.origin.retail_phase==31);
  input.origin.scope=2;auto b=BuildNativePassKey(input);REQUIRE(a!=b);REQUIRE(NativePassKeyLess(a,b));
  std::array<NativePassObservation,2> obs{};obs[0].command_index=1;obs[0].coarse_range=3;obs[0].key=a;obs[0].draw_count=1;
  obs[1]=obs[0];obs[1].command_index=2;obs[1].key=b;
  auto separated=BuildNativeAttributionPlan(obs);REQUIRE(separated.regions.size()==2);
  NativeAttributionPlanBudget budget;budget.total_detail_boundaries=0;
  auto merged=BuildNativeAttributionPlan(obs,budget);REQUIRE(merged.regions.size()==1);
  REQUIRE(merged.regions[0].key.origin.source==GpuPassOriginSource::kMixed);
  REQUIRE_FALSE(merged.regions[0].key.origin.attributed());
  REQUIRE(CheckNativeAttributionCoverage(obs,merged).complete);
}
TEST_CASE("Draw-list origin transport has bounded storage independent of history", "[graphics][gpu-origin][memory]") {
  Fixture f;std::array<uint8_t,128> storage{};
  for(uint32_t frame=0;frame<30000;++frame){
    f.memory[Fixture::page+Fixture::offset+12]=frame%2?31:23;
    auto origin=f.Decode();origin.scope=uint64_t(frame)+1;
    const auto n=PackGpuPassEnvelope(storage,&frame,sizeof(frame),16,origin);
    const void* p=storage.data();auto size=n;uint32_t abi=0;GpuPassOrigin out;
    REQUIRE(UnpackGpuPassEnvelope(p,size,abi,out));REQUIRE(out==origin);REQUIRE(f.memory.size()==5);
  }
}

TEST_CASE("Next-frame GPU drilldown follows the phase not the old scope address", "[graphics][gpu-origin]") {
  using namespace attribution;NativePassObservation observation;observation.command_index=2;
  observation.coarse_range=3;observation.draw_count=1;observation.key.pixel_shader_hash=123;
  observation.key.origin={2,31,GpuPassOriginSource::kVerifiedBoth,0x2000,48,4,4};
  NativeDrilldownTarget target;target.target_id=7;target.key=observation.key;
  target.key.origin.scope=1;target.key.origin.offset=32;target.key.origin.ordinal=2;
  const auto plan=BuildNativeAttributionPlan(std::span(&observation,1),{},target);
  REQUIRE(plan.target_matched);REQUIRE(plan.regions.size()==1);
  REQUIRE(plan.regions[0].key.origin.scope==2);
  REQUIRE(plan.regions[0].detail==NativePassRegionDetail::kDrilldown);
}
