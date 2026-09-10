// Regression coverage for the exact metadata/pixel selection policy in production.
#include <rex/ui/frame_pixel_probe.h>
#include <array>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
using namespace rex::ui;
static uint32_t checks = 0;
static void require(bool result, const char* name) {
  ++checks;
  if (!result) { std::cerr << "FAIL " << name << '\n'; std::exit(1); }
}
static FramePixelProbe valid_probe() {
  FramePixelProbe p;
  p.run = 7; p.source_sequence = 53; p.native_submission = 14; p.guest_image = 101;
  p.guest_version = 3; p.frame = 12; p.fixture = 4; p.width = 1280; p.height = 720;
  p.normalized_region = {0.25f,0.25f,0.75f,0.75f}; return p;
}
int main() {
  const auto p = valid_probe();
  require(p.valid(), "valid request");
  require(!FramePixelProbe{}.valid(), "disabled default");
  require(p.matches(12,101,1280,720,3), "exact image and version");
  require(!p.matches(13,101,1280,720,3), "reject later title frame");
  require(!p.matches(12,102,1280,720,3), "reject other image");
  require(!p.matches(12,101,1280,720,4), "reject reused image version");
  require(!p.matches(12,101,1279,720,3), "reject width mismatch");
  require(!p.matches(12,101,1280,719,3), "reject height mismatch");
  require(PublishFramePixelProbe(p,true,12,1280,720).valid(), "publish successful callback");
  require(!PublishFramePixelProbe(p,false,12,1280,720).valid(), "failed callback cannot publish selection");
  require(!PublishFramePixelProbe(p,true,13,1280,720).valid(), "stale producer selection");
  require(!PublishFramePixelProbe(p,true,12,0,720).valid(), "inactive output");
  for (int field=0;field<7;++field) {
    auto invalid=p;
    switch(field) {
      case 0: invalid.run=0; break; case 1: invalid.source_sequence=0;break;
      case 2: invalid.native_submission=0;break;case 3:invalid.guest_image=0;break;
      case 4:invalid.frame=0;break;case 5:invalid.width=0;break;case 6:invalid.height=0;break;
    }
    require(!invalid.valid(),"missing required identity");
  }
  for (float value : {std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::infinity(),-1.0f}) {
    auto invalid=p;invalid.normalized_region[0]=value;require(!invalid.valid(),"invalid coordinate");
  }
  auto reversed=p;reversed.normalized_region={0.7f,0.2f,0.3f,0.9f};require(!reversed.valid(),"reversed rectangle");
  auto beyond=p;beyond.normalized_region[2]=2;require(!beyond.valid(),"outside normalized image");
  auto region=MapFramePixelProbe(p,0,0,1280,720,1280,720);require(region.valid(),"visible region");
  require(!MapFramePixelProbe(p,10000,10000,1280,720,1280,720).valid(),"offscreen cannot copy pixels");
  require(!MapFramePixelProbe(p,0,0,0,720,1280,720).valid(),"zero output extent");
  require(!MapFramePixelProbe(p,0,0,1280,720,0,720).valid(),"zero swapchain extent");
  for(uint32_t y=0;y<16;++y)for(uint32_t x=0;x<16;++x) {
    auto sample=region.sample(x,y);
    require(sample[0]>=region.rectangle[0]&&sample[0]<region.rectangle[0]+region.rectangle[2],"sample X bounds");
    require(sample[1]>=region.rectangle[1]&&sample[1]<region.rectangle[1]+region.rectangle[3],"sample Y bounds");
  }
  FramePixelProbeBudget budget;
  FramePixelProbeKey key{7,53,12,1};
  require(budget.can_record(key),"first capture allowed");
  require(budget.commit(key),"accepted submission commits");
  require(!budget.can_record(key),"duplicate repaint does not re-capture");
  require(!budget.commit(key),"duplicate commit rejected");
  require(budget.commit({7,53,12,2}),"new swapchain epoch distinct");
  require(!budget.can_record({}),"invalid identity rejected");
  FramePixelProbeBudget bounded;
  for(size_t i=0;i<FramePixelProbeBudget::kLimit;++i)
    require(bounded.commit({7,uint64_t(i)+1,uint32_t(i)+1,1}),"bounded capture slot");
  require(!bounded.can_record({8,100,100,1}),"capacity exhaustion");
  for(bool pending:{false,true})for(bool mapped:{false,true})for(bool visible:{false,true})
    for(uint64_t submitted:{0ull,12ull,14ull})for(uint64_t completed:{0ull,12ull,15ull})
      require(FramePixelProbeReadable(pending,submitted,completed,mapped,visible)==
          (pending&&submitted!=0&&submitted<=completed&&mapped&&visible),"fence and noncoherent visibility gating");
  std::cout << "CHECKS " << checks << '\n';
  std::cout << "CHECKSUM " << FramePixelProbeChecksum(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>("hello"),5)) << '\n';
  // Python supplies adversarial transforms and independently verifies every row.
  uint32_t width,height,sw,sh;int32_t x,y;float l,t,r,b;size_t index=0;
  for(;std::cin>>x>>y>>width>>height>>sw>>sh>>l>>t>>r>>b;++index) {
    auto input=p;input.normalized_region={l,t,r,b};const auto mapped=MapFramePixelProbe(input,x,y,width,height,sw,sh);
    std::cout << "REGION " << index << ' ' << mapped.valid();
    for(auto v:mapped.rectangle)std::cout << ' ' << v;
    if(mapped.valid())for(auto v:mapped.sample(15,15))std::cout << ' ' << v;
    std::cout << '\n';
  }
  return 0;
}
