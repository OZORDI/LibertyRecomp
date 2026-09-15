#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include "../../gta4-recomp/src/gta4_aspect_policy.h"
#ifdef GTA4_ASPECT_STANDALONE
#include <iostream>
static uint64_t checks = 0;
#define ASPECT_REQUIRE(...) do { ++checks; if (!(__VA_ARGS__)) throw std::runtime_error(#__VA_ARGS__); } while (false)
#else
#include <catch2/catch_test_macros.hpp>
#define ASPECT_REQUIRE(...) REQUIRE(__VA_ARGS__)
#endif
namespace {
namespace a = gta4::aspect;
bool Near(double a, double b, double tolerance = 1e-9) { return std::abs(a - b) <= tolerance; }
void CheckAspectPolicy() {
  ASPECT_REQUIRE(a::CanonicalPreset("original") == "16:9");
  ASPECT_REQUIRE(a::kPresets.size() == 10);
  ASPECT_REQUIRE(!a::ParseRatio("invalid"));
  ASPECT_REQUIRE(a::SelectExtent({2560,1600}, "original", {}) == a::Extent{2560,1440});
  ASPECT_REQUIRE(a::SelectExtent({1920,1080}, "auto", {2560,1600}) == a::Extent{1728,1080});
  ASPECT_REQUIRE(a::SelectExtent({2560,1600}, "16:10", {}) == a::Extent{2560,1600});
  ASPECT_REQUIRE(a::SelectExtent({2560,1600}, "4:3", {}) == a::Extent{2133,1600});
  ASPECT_REQUIRE(a::SelectExtent({3440,1440}, "auto", {3440,1440}) == a::Extent{3440,1440});
  ASPECT_REQUIRE(a::SelectExtent({3440,1440}, "21:9", {}) == a::Extent{3360,1440});
  ASPECT_REQUIRE(a::SelectExtent({3440,1440}, "43:18", {}) == a::Extent{3440,1440});
  ASPECT_REQUIRE(a::SelectExtent({1280,720}, "unknown", {}) == a::Extent{1280,720});
  ASPECT_REQUIRE(!a::LimitExtent({},4095).valid());
  ASPECT_REQUIRE(!a::LimitExtent({1920,1080},0).valid());
  const std::array outputs = {a::Extent{1280,720}, a::Extent{1920,1080}, a::Extent{2560,1600},
      a::Extent{3456,2160}, a::Extent{1024,768}, a::Extent{1280,1024}, a::Extent{3000,2000},
      a::Extent{3440,1440}, a::Extent{3840,1080}, a::Extent{7680,2160}, a::Extent{1513,947},
      a::Extent{2160,4096}};
  const std::array anchors = {a::Point{0,0}, a::Point{0,1}, a::Point{1,0},
      a::Point{1,1}, a::Point{0.5,0.5}, a::Point{0.5,0}, a::Point{0.5,1}};
  constexpr double rad = 3.14159265358979323846 / 180.0;
  for (auto output : outputs) {
    const auto limited = a::LimitExtent(output,4095);
    ASPECT_REQUIRE(limited.width <= 4095 && limited.height <= 4095);
    ASPECT_REQUIRE(std::abs(double(limited.width) - limited.height * output.aspect()) <=
                   std::max(1.0,output.aspect()));
    for (auto preset : a::kPresets) {
      auto fit = a::SelectExtent(limited,preset.value,output);
      ASPECT_REQUIRE(fit.valid() && fit.width <= limited.width && fit.height <= limited.height);
      const double expected = preset.ratio.x ? double(preset.ratio.x)/preset.ratio.y : output.aspect();
      ASPECT_REQUIRE(std::abs(fit.width - fit.height * expected) <= std::max(1.0, expected));
    }
    for (double fov : {5.0,12.0,30.0,52.5,75.0,100.0}) {
      const auto adapted = a::ExpandVerticalFov(fov,output.aspect());
      const double original_tan = std::tan(fov*rad*0.5);
      const double ty = std::tan(adapted*rad*0.5), tx = ty*output.aspect();
      ASPECT_REQUIRE(ty >= original_tan - 1e-12);
      ASPECT_REQUIRE(tx >= original_tan * a::kReferenceAspect - 1e-12);
      ASPECT_REQUIRE(Near((output.width/tx)/(output.height/ty),1.0));
      if (output.aspect() < a::kReferenceAspect)
        ASPECT_REQUIRE(Near(tx, original_tan*a::kReferenceAspect));
      else ASPECT_REQUIRE(adapted == fov);
      for (unsigned repeat=0;repeat<32;++repeat)
        ASPECT_REQUIRE(a::ExpandVerticalFov(fov, output.aspect()) == adapted);
    }
    for (auto anchor : anchors) {
      const auto t = a::Layout(output,anchor);
      ASPECT_REQUIRE(Near(t.Map(anchor).x,anchor.x) && Near(t.Map(anchor).y,anchor.y));
      ASPECT_REQUIRE(Near(t.sx*output.width/(t.sy*output.height),a::kReferenceAspect));
      for (int x=0;x<=12;++x) for (int y=0;y<=8;++y) {
        const a::Point point{double(x)/12,double(y)/8};
        const auto mapped=t.Map(point), roundtrip=t.Unmap(mapped);
        ASPECT_REQUIRE(Near(roundtrip.x,point.x) && Near(roundtrip.y,point.y));
        const auto pixel=t.Pixels(output).Map(a::Point{point.x*output.width,point.y*output.height});
        ASPECT_REQUIRE(Near(pixel.x,mapped.x*output.width,1e-8));
        ASPECT_REQUIRE(Near(pixel.y,mapped.y*output.height,1e-8));
      }
      const auto fade=a::CoveringBackground(t,{-0.001,-0.001,1.001,1.001});
      ASPECT_REQUIRE(fade.identity());
      const auto panel=a::CoveringBackground(t,{0,0.1,1,0.3});
      ASPECT_REQUIRE(panel.sx == 1 && panel.ox == 0);
      ASPECT_REQUIRE(panel.sy == t.sy && panel.oy == t.oy);
      // The frontend publishes the very same transformed rectangle to hit testing.
      const a::Rect row{0.05,0.2,0.9,0.25};
      const auto hit=t.Map(row);
      ASPECT_REQUIRE(Near(hit.left,t.Map(a::Point{row.left,row.top}).x));
      ASPECT_REQUIRE(Near(hit.bottom,t.Map(a::Point{row.right,row.bottom}).y));
      ASPECT_REQUIRE(Near((hit.bottom-hit.top), (row.bottom-row.top)*t.sy));
      for (auto offset:a::kFontScaledOffsets)
        ASPECT_REQUIRE(a::FontScale(offset,t) == (offset==8 ? t.sy:t.sx));
    }
    const auto art=a::Layout(output);
    const auto fitted=art.Map(a::Rect{0,0,1,1});
    ASPECT_REQUIRE(fitted.left>=0 && fitted.top>=0 && fitted.right<=1 && fitted.bottom<=1);
    ASPECT_REQUIRE(Near((fitted.right-fitted.left)*output.width /
                        ((fitted.bottom-fitted.top)*output.height),a::kReferenceAspect));
    ASPECT_REQUIRE(Near((fitted.left+fitted.right)*0.5,0.5));
    ASPECT_REQUIRE(Near((fitted.top+fitted.bottom)*0.5,0.5));
    // Phone projection keeps Z/W and clipping distances, transforming only X/Y.
    std::array<float,16> matrix{1.25f,0,0,0, 0,2.0f,0,0, 0,0,-1,-1, 0,0,-0.25f,0};
    const auto before=matrix;
    const auto phone=a::Layout(output,{1,1});
    a::TransformProjection(matrix,phone);
    for (unsigned r=0;r<4;++r) {
      ASPECT_REQUIRE(matrix[r*4+2] == before[r*4+2]);
      ASPECT_REQUIRE(matrix[r*4+3] == before[r*4+3]);
    }
    for (a::Point point : {a::Point{-0.5,-0.2},a::Point{0,0},a::Point{0.3,0.4}}) {
      const std::array<double,4> vertex{point.x,point.y,-2,1};
      auto clip=[&](const std::array<float,16>& p,unsigned col) {
        double sum=0; for(unsigned r=0;r<4;++r)sum+=vertex[r]*p[r*4+col];return sum;
      };
      const double w=clip(before,3);
      a::Point screen{(clip(before,0)/w+1)*0.5,(1-clip(before,1)/w)*0.5};
      const auto expected=phone.Map(screen);
      ASPECT_REQUIRE(Near((clip(matrix,0)/w+1)*0.5,expected.x,1e-7));
      ASPECT_REQUIRE(Near((1-clip(matrix,1)/w)*0.5,expected.y,1e-7));
    }
  }
  // Standalone route sections and the enclosing radar widget must never pick
  // different anchors on non-16:9 outputs.
  for (a::Extent output : {a::Extent{2560,1600}, a::Extent{3456,2234},
                           a::Extent{1024,768}, a::Extent{3440,1440}}) {
    const auto radar = a::RadarLayout(output);
    const auto expected = a::Layout(output,{0,1});
    ASPECT_REQUIRE(Near(radar.sx,expected.sx) && Near(radar.sy,expected.sy));
    ASPECT_REQUIRE(Near(radar.ox,expected.ox) && Near(radar.oy,expected.oy));
    for (a::Point point : {a::Point{0.1,0.9}, a::Point{0.25,0.8}, a::Point{0.5,0.75}}) {
      const auto map_layer=radar.Map(point);
      const auto route_layer=a::RadarLayout(output).Map(point);
      ASPECT_REQUIRE(Near(map_layer.x,route_layer.x) && Near(map_layer.y,route_layer.y));
    }
  }
  // Expected placements calculated with Python from an authored HUD rectangle.
  struct RadarCase { a::Extent output; double inset, top, bottom; };
  const std::array radar_cases = {
      RadarCase{{1280,720}, 0.0, 0.040000000000000036, 0.26},
      RadarCase{{1280,720}, 0.05, 0.09000000000000004, 0.31},
      RadarCase{{2560,1600}, 0.0, 0.03600000000000003, 0.2340000000000001},
      RadarCase{{2560,1600}, 0.05, 0.08600000000000003, 0.2840000000000001},
      RadarCase{{1024,768}, 0.0, 0.030000000000000027, 0.19500000000000006},
      RadarCase{{1024,768}, 0.05, 0.08000000000000003, 0.24500000000000005},
      RadarCase{{3440,1440}, 0.0, 0.040000000000000036, 0.26},
      RadarCase{{3440,1440}, 0.05, 0.09000000000000004, 0.31},
      RadarCase{{1080,1920}, 0.0, 0.012656250000000036, 0.08226562500000001},
      RadarCase{{1080,1920}, 0.05, 0.06265625000000004, 0.132265625},
  };
  const a::Rect radar_box{0.04,0.74,0.24,0.96};
  for (const auto& item : radar_cases) {
    const auto normal = a::RadarLayout(item.output);
    const auto moved = a::TopLeftRadarLayout(item.output, radar_box, item.inset);
    const auto bounds = moved.Map(radar_box);
    ASPECT_REQUIRE(Near(bounds.top, item.top));
    ASPECT_REQUIRE(Near(bounds.bottom, item.bottom));
    ASPECT_REQUIRE(Near(bounds.left, normal.Map(radar_box).left));
    ASPECT_REQUIRE(Near(bounds.right, normal.Map(radar_box).right));
    ASPECT_REQUIRE(Near(moved.sx, normal.sx) && Near(moved.sy, normal.sy));
    // Every layer and its hit point move by the identical translation.
    for (a::Point point : {a::Point{0.14,0.85}, {0.08,0.78}, {0.22,0.94}}) {
      const auto roundtrip = moved.Unmap(moved.Map(point));
      ASPECT_REQUIRE(Near(roundtrip.x, point.x) && Near(roundtrip.y, point.y));
    }
  }
  ASPECT_REQUIRE(a::TopLeftRadarLayout({}, radar_box).identity());
  for (auto invalid : {a::Rect{}, a::Rect{0,0,1,2},
                       a::Rect{0,0,1,std::numeric_limits<double>::quiet_NaN()}}) {
    const auto untouched = a::TopLeftRadarLayout({1280,720}, invalid);
    ASPECT_REQUIRE(untouched.identity());
  }
  const auto mac=a::Layout({2560,1600});
  ASPECT_REQUIRE(mac.sx==1 && Near(mac.sy,0.9));
  ASPECT_REQUIRE(Near(mac.oy*1600,80));
  ASPECT_REQUIRE(a::Layout({1280,720}).identity());
  ASPECT_REQUIRE(a::Layout({}).identity());
  ASPECT_REQUIRE(a::ExpandVerticalFov(0,1.6)==0);
  ASPECT_REQUIRE(a::ExpandVerticalFov(52.5,0)==52.5);
  ASPECT_REQUIRE(a::ExpandVerticalFov(52.5,std::numeric_limits<double>::quiet_NaN())==52.5);
  for(uint32_t owner:{0x820B9284u,0x820212F4u,0x820BCB40u})
    ASPECT_REQUIRE(a::ScreenCameraOwner(owner));
  for(uint32_t owner:{0u,0x8200E0BCu,0x820212B4u,0x820B9248u,0x820B95F0u,0x820BE368u})
    ASPECT_REQUIRE(!a::ScreenCameraOwner(owner));
  ASPECT_REQUIRE(a::PhoneCameraOwner(0x820B95F0));
  ASPECT_REQUIRE(a::PrimaryUiOwner(0x820B92C4));
  // The frontend mutates the command-size field after construction. Layout
  // identity survives that publication but rejects a different instance.
  for (uint32_t instance=0;instance<128;++instance) {
    const uint32_t constructed=(instance<<18)|0x21u;
    for (uint32_t size=0;size<2048;size+=17) {
      const uint32_t published=constructed|(size<<7);
      ASPECT_REQUIRE(a::StableDcToken(constructed)==a::StableDcToken(published));
      ASPECT_REQUIRE(a::StableDcToken(published)!=a::StableDcToken(published^(1u<<18)));
    }
  }

}
}
#ifdef GTA4_ASPECT_STANDALONE
int main() {
  try { CheckAspectPolicy(); std::cout << "PASS " << checks << " aspect policy assertions\n"; }
  catch (const std::exception& error) { std::cerr << error.what() << '\n';return 1; }
}
#else
TEST_CASE("Content-aware aspect geometry and camera contracts", "[gta4-aspect]") {
  CheckAspectPolicy();
}
#endif
