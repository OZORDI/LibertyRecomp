#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <array>
#include <cmath>
#include <limits>
#include "../../../gta4-recomp/src/gta4_aspect_policy.h"
#include "../../../gta4-recomp/src/gta4_aspect_resolution.h"
namespace a = gta4::aspect;
using Catch::Approx;

TEST_CASE("Aspect presets accept legacy configuration and fit independently of resolution", "[gta4-aspect]") {
  REQUIRE(a::CanonicalPreset("original") == "16:9");
  REQUIRE(a::kPresets.size() == 10);
  REQUIRE_FALSE(a::ParseRatio("bogus"));
  REQUIRE(a::SelectExtent({2560,1600}, "original", {}) == a::Extent{2560,1440});
  REQUIRE(a::SelectExtent({1920,1080}, "auto", {2560,1600}) == a::Extent{1728,1080});
  REQUIRE(a::SelectExtent({3440,1440}, "43:18", {}) == a::Extent{3440,1440});
  REQUIRE(a::SelectExtent({3440,1440}, "21:9", {}) == a::Extent{3360,1440});
  REQUIRE(a::SelectExtent({2560,1600}, "16:10", {}) == a::Extent{2560,1600});
  REQUIRE(a::SelectExtent({1920,1080}, "auto", {}) == a::Extent{1920,1080});
  REQUIRE(a::SelectExtent({1920,1080}, "bogus", {}) == a::Extent{1920,1080});
  for (a::Extent bounds : {a::Extent{640,480}, {1280,720}, {2560,1600}, {3456,2160},
                         {3440,1440}, {7680,2160}, {2160,7680}, {4096,4096}}) {
    const auto limited = a::resolution::Limit(bounds,4095);
    REQUIRE(limited.width <= 4095); REQUIRE(limited.height <= 4095);
    REQUIRE(std::abs(limited.width-limited.height*bounds.aspect()) <= std::max(1.0,bounds.aspect()));
    for (const auto& preset : a::kPresets) {
      auto e = a::resolution::Select(limited,preset.value,bounds);
      REQUIRE(e.valid()); REQUIRE(e.width <= limited.width); REQUIRE(e.height <= limited.height);
      const double ratio = preset.ratio.x ? double(preset.ratio.x)/preset.ratio.y : bounds.aspect();
      REQUIRE(std::abs(e.width-e.height*ratio) <= std::max(1.0,ratio));
    }
  }
  REQUIRE_FALSE(a::LimitExtent({},4095).valid());
  REQUIRE_FALSE(a::LimitExtent({1280,720},0).valid());
}
TEST_CASE("Camera framing expands rather than stretching or cropping", "[gta4-aspect]") {
  constexpr double radians = 3.14159265358979323846/180.0;
  for (double ratio : {1.25,4.0/3.0,1.5,1.6,16.0/9.0,21.0/9.0,43.0/18.0,32.0/9.0}) {
    for (double fov : {5.0,12.0,30.0,52.5,75.0,100.0}) {
      const double resolved = a::ExpandVerticalFov(fov,ratio);
      const double t = std::tan(resolved*radians*0.5), original = std::tan(fov*radians*0.5);
      REQUIRE(t >= original-1e-12); REQUIRE(t*ratio >= original*a::kReferenceAspect-1e-12);
      REQUIRE((1/t)/(ratio/(ratio*t)) == Approx(1));
      if (ratio < a::kReferenceAspect) REQUIRE(t*ratio == Approx(original*a::kReferenceAspect));
      else REQUIRE(resolved == fov);
      for (int repeat=0;repeat<32;++repeat) REQUIRE(a::ExpandVerticalFov(fov,ratio) == resolved);
    }
  }
  REQUIRE(a::ExpandVerticalFov(0,1.6) == 0);
  REQUIRE(a::ExpandVerticalFov(52.5,0) == 52.5);
  REQUIRE(a::ExpandVerticalFov(52.5,std::numeric_limits<double>::quiet_NaN()) == 52.5);
}
TEST_CASE("Only identified screen owners receive display camera projection", "[gta4-aspect]") {
  for (uint32_t owner : {0x820B9284u,0x820212F4u,0x820BCB40u}) REQUIRE(a::ScreenCameraOwner(owner));
  for (uint32_t owner : {0u,0x8200E0BCu,0x820212B4u,0x820B9248u,0x820B95F0u,0x820BE368u})
    REQUIRE_FALSE(a::ScreenCameraOwner(owner));
  REQUIRE(a::PhoneCameraOwner(0x820B95F0));
  REQUIRE(a::PrimaryUiOwner(0x820B92C4));
}
TEST_CASE("Uniform UI scaling preserves edge anchors and pointer round trips", "[gta4-aspect]") {
  for (a::Extent out : {a::Extent{1280,720},{2560,1600},{1024,768},{3440,1440},{3840,1080},{1513,947}}) {
    for (a::Point anchor : {a::Point{0,0},{0,1},{1,0},{1,1},{0.5,0.5}}) {
      const auto t = a::Layout(out,anchor);
      REQUIRE(out.width*t.sx/1280 == Approx(out.height*t.sy/720));
      const auto fixed = t.Map(anchor);
      REQUIRE(fixed.x == Approx(anchor.x)); REQUIRE(fixed.y == Approx(anchor.y));
      for (int x=0;x<=20;++x) for (int y=0;y<=20;++y) {
        const a::Point p{double(x)/20,double(y)/20};
        const auto restored = t.Unmap(t.Map(p));
        REQUIRE(restored.x == Approx(p.x).margin(1e-12));
        REQUIRE(restored.y == Approx(p.y).margin(1e-12));
      }
      // Pixel-space menu highlights and normalized text must meet at the same point.
      const auto normalized = t.Map(a::Point{0.2,0.8});
      const auto pixels = t.Pixels(out).Map(a::Point{0.2*out.width,0.8*out.height});
      REQUIRE(pixels.x == Approx(normalized.x*out.width));
      REQUIRE(pixels.y == Approx(normalized.y*out.height));
    }
  }
  REQUIRE(a::Layout({1280,720}).identity());
  REQUIRE(a::Layout({}).identity());
}
TEST_CASE("Fixed artwork fits and solid covering backgrounds stay full screen", "[gta4-aspect]") {
  const auto t = a::Layout({2560,1600});
  REQUIRE(t.sx == 1); REQUIRE(t.sy == Approx(0.9)); REQUIRE(t.oy*1600 == Approx(80));
  REQUIRE(a::CoveringBackground(t,{0,0,1,1}).identity());
  const auto box = a::CoveringBackground(t,{0.1,0.1,0.4,0.4});
  REQUIRE(box.sy == t.sy); REQUIRE(box.oy == t.oy);
  REQUIRE(a::IsLoadingArt(0x821440C0));
  REQUIRE_FALSE(a::IsLoadingArt(0x82144100));
  REQUIRE_FALSE(a::IsLoadingArt(0x82144178));
  REQUIRE_FALSE(a::IsLoadingArt(0x821445BC));
  for (a::Extent e : {a::Extent{2560,1600},{3440,1440},{1024,768}}) {
    auto r=a::Layout(e).Map(a::Rect{0,0,1,1});
    REQUIRE(r.left>=0); REQUIRE(r.top>=0); REQUIRE(r.right<=1); REQUIRE(r.bottom<=1);
    REQUIRE((r.right-r.left)*e.width/((r.bottom-r.top)*e.height) == Approx(a::kReferenceAspect));
  }
}
TEST_CASE("Phone projection includes homogeneous translation and leaves depth unchanged", "[gta4-aspect]") {
  const std::array<float,16> original={2,0.1f,0,0.2f, 0.2f,3,0,0.3f, 0.1f,0.2f,4,-1, 0.3f,0.4f,0.5f,1};
  const std::array<double,4> position={0.2,-0.1,0.3,1};
  for (a::Extent extent : {a::Extent{1280,720},{2560,1600},{3440,1440}}) {
    auto matrix=original; auto t=a::Layout(extent,{1,1}); a::TransformProjection(matrix,t);
    std::array<double,4> before{},after{};
    for (unsigned col=0;col<4;++col) for(unsigned row=0;row<4;++row) {
      before[col]+=position[row]*original[row*4+col]; after[col]+=position[row]*matrix[row*4+col];
    }
    auto expected=t.Map(a::Point{(before[0]/before[3]+1)*0.5,(1-before[1]/before[3])*0.5});
    REQUIRE((after[0]/after[3]+1)*0.5 == Approx(expected.x));
    REQUIRE((1-after[1]/after[3])*0.5 == Approx(expected.y));
    REQUIRE(after[2] == before[2]); REQUIRE(after[3] == before[3]);
  }
}
TEST_CASE("Font advances match glyph geometry and queue identity rejects address reuse", "[gta4-aspect]") {
  auto t=a::Layout({3440,1440});
  REQUIRE(a::FontScale(4,t) == t.sx); REQUIRE(a::FontScale(8,t) == t.sy);
  REQUIRE(a::FontScale(12,t) == t.sx); REQUIRE(a::FontScale(64,t) == t.sx);
  const a::DcIdentity old{0x1000,11,0x8200146C};
  REQUIRE(old == a::DcIdentity{0x1000,11,0x8200146C});
  REQUIRE_FALSE(old == a::DcIdentity{0x1000,12,0x8200146C});
  REQUIRE_FALSE(old == a::DcIdentity{0x1000,11,0x820014C0});
  REQUIRE_FALSE(a::HasUiDrawExecutor(0x82001370));
  REQUIRE_FALSE(a::HasUiDrawExecutor(0x8200138C));
  REQUIRE(a::HasUiDrawExecutor(0x8200146C));
  // World-positioned labels may resize without moving their projection point.
  const a::Point world{0.3,0.7}; auto anchored=a::Layout({3440,1440},world).Map(world);
  REQUIRE(anchored.x == Approx(world.x)); REQUIRE(anchored.y == Approx(world.y));
}


TEST_CASE("Menu rows retain their authored gap below the tab divider", "[gta4-aspect][menu-padding]") {
  for (const auto extent : {a::Extent{1280,720}, a::Extent{2560,1600}, a::Extent{3456,2234},
                            a::Extent{1024,768}, a::Extent{3440,1440}, a::Extent{3840,1080}}) {
    for (double divider : {0.15,0.192,0.21}) {
      const auto body = a::MenuBodyLayout(extent,divider);
      REQUIRE(body.Map(a::Point{0.5,divider}).y == Approx(divider));
      // HD authoring: list starts at 0.175 plus one 0.043 row advance.
      const double first_row = 0.175 + 0.043;
      const double mapped_first = body.Map(a::Point{0.07,first_row}).y;
      REQUIRE(mapped_first > divider);
      REQUIRE(mapped_first-divider == Approx((first_row-divider)*body.sy));
      for (unsigned row=0;row<12;++row) {
        const double y = first_row + row*0.043;
        const auto mapped = body.Map(a::Point{0.28,y});
        const auto recovered = body.Unmap(mapped);
        REQUIRE(recovered.y == Approx(y));
        REQUIRE(mapped.y < 0.808); // authored footer divider
        REQUIRE(mapped.y-mapped_first == Approx(row*0.043*body.sy).margin(1e-12));
      }
    }
  }
  REQUIRE(a::MenuBodyLayout({1280,720}).identity());
  const auto native = a::MenuBodyLayout({3456,2234});
  REQUIRE(native.oy*2234 == Approx(55.68));
}

TEST_CASE("Menu divider layout handles unavailable or invalid frontend data", "[gta4-aspect][menu-padding]") {
  const auto expected = a::MenuBodyLayout({2560,1600});
  for (double boundary : {0.0,-0.5,1.0,1.2,std::numeric_limits<double>::quiet_NaN(),
                          std::numeric_limits<double>::infinity()}) {
    const auto actual = a::MenuBodyLayout({2560,1600},boundary);
    REQUIRE(actual.sx == expected.sx);
    REQUIRE(actual.sy == expected.sy);
    REQUIRE(actual.ox == expected.ox);
    REQUIRE(actual.oy == expected.oy);
  }
  REQUIRE(a::MenuBodyLayout({}).identity());
}
