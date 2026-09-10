#pragma once
// Diagnostic identities and arithmetic only; never used to choose game rendering.
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace rex::graphics::gta4_native {
struct BulbFixtureIdentity {
  uint64_t vertex_hash;
  uint32_t vertex_bytes, indices;
  std::string_view role, model, preset;
};
// Matched byte-for-byte to the installed brownstones.img drawables, then XXH3.
// These are selection labels, not light IDs and not brightness overrides.
inline constexpr std::array<BulbFixtureIdentity, 6> kBulbFixtures{{
    {0x86996067DBF012D5ull,3792,576,"table-shade-base","romslamp.xdr","gta_default.sps"},
    {0x750BB075C25B1F74ull,624,72,"table-shade","romslamp.xdr","gta_emissivenight_alpha.sps"},
    {0x5B47288B40EF2966ull,1392,174,"kitchen-bulb","dodgyraylight.xdr","gta_emissive.sps"},
    {0x62B75CBD5AE38C0Dull,4968,492,"ceiling-base","romsbabepost1.xdr","gta_default.sps"},
    {0xBB3A1BB34AB96383ull,984,150,"ceiling-bulb","romsbabepost1.xdr","gta_emissive_alpha.sps"},
    {0x4320D33832EFB3BCull,5472,1080,"ceiling-shade","romsbabepost1.xdr","gta_emissive_alpha.sps"},
}};
inline int FindBulbFixture(uint64_t hash, size_t bytes, uint32_t indices) {
  for (size_t i=0;i<kBulbFixtures.size();++i)
    if(kBulbFixtures[i].vertex_hash==hash && kBulbFixtures[i].vertex_bytes==bytes &&
       kBulbFixtures[i].indices==indices) return int(i);
  return -1;
}
inline uint32_t BulbGuestWord(std::span<const uint8_t> bytes,size_t at) {
  if(at>bytes.size() || bytes.size()-at<4) return 0x7FC00000u;
  return (uint32_t(bytes[at])<<24)|(uint32_t(bytes[at+1])<<16)|
         (uint32_t(bytes[at+2])<<8)|uint32_t(bytes[at+3]);
}
inline float BulbConstant(std::span<const uint8_t> bytes,size_t reg,size_t component=0) {
  return std::bit_cast<float>(BulbGuestWord(bytes,reg*16+component*4));
}
struct BulbEmissionState {
  bool known=false;
  float multiplier=0,night=0,gate=0,scalar=0,exposure=0;
  std::array<float,4> tint{};
  std::string_view state="unknown-shader-contract";
};
inline BulbEmissionState InspectBulbEmission(uint64_t pixel_shader,std::span<const uint8_t> constants) {
  BulbEmissionState s;
  // Only the recovered forward PS1 contracts. Other families/register layouts
  // remain unknown; the absence of a draw never means a lamp is switched off.
  if(pixel_shader!=0xF634FCEBA607E8A5ull && pixel_shader!=0x07675BF8EF5E48DCull) return s;
  if(constants.size()<209*16) {s.state="missing-constants";return s;}
  s.multiplier=BulbConstant(constants,208);
  s.night=pixel_shader==0x07675BF8EF5E48DCull?BulbConstant(constants,45,3):1.0f;
  s.scalar=BulbConstant(constants,39);s.exposure=BulbConstant(constants,46);
  for(size_t i=0;i<4;++i)s.tint[i]=BulbConstant(constants,51,i);
  s.gate=s.multiplier*s.night;
  if(!std::isfinite(s.multiplier)||!std::isfinite(s.night)||!std::isfinite(s.gate)||
     !std::isfinite(s.scalar)||!std::isfinite(s.exposure)||
     !std::all_of(s.tint.begin(),s.tint.end(),[](float v){return std::isfinite(v);})) {
    s.state="invalid-emission-input";return s;
  }
  s.known=true;
  if(s.multiplier<=0)s.state="off-material-multiplier";
  else if(s.night<=0)s.state="off-night-gate";
  else if(std::max({s.tint[0],s.tint[1],s.tint[2]})<=0)s.state="off-color-tint";
  else if(s.scalar<=0 || s.tint[3]<=0 || s.exposure<=0)s.state="emission-on-zero-alpha-input";
  else s.state="emission-on";
  return s;
}
struct BulbSourceSnapshot {
  uint32_t fixture=0,caller=0,command_list=0,origin=0;
  uint64_t vertex_shader=0,pixel_shader=0;
  std::vector<uint8_t> vertex_constants,pixel_constants;
  std::vector<uint8_t> display_gamma;
  uint32_t display_format=0,display_dirty=0;
  std::array<uint32_t,4> game_clock{};
  bool clock_captured=false;
};
struct BulbScreenRegion {
  bool known=false,visible=false;
  std::array<float,4> normalized{}; // left, top, right, bottom after shader Y inversion
};
inline BulbScreenRegion InspectBulbScreenRegion(std::span<const uint8_t> vertices,
    size_t offset,size_t stride,std::span<const uint8_t> constants) {
  BulbScreenRegion r;
  if(stride<12 || offset>vertices.size() || vertices.size()-offset<stride ||
     (vertices.size()-offset)%stride || constants.size()<45*16)return r;
  const float w=BulbConstant(constants,44),h=BulbConstant(constants,44,1);
  if(!std::isfinite(w)||!std::isfinite(h)||w<=0||h<=0 ||
     !std::isfinite(BulbConstant(constants,44,2)) ||
     !std::isfinite(BulbConstant(constants,44,3)))return r;
  std::array<float,3> lo,hi;lo.fill(std::numeric_limits<float>::infinity());hi.fill(-std::numeric_limits<float>::infinity());
  for(size_t at=offset;at<vertices.size();at+=stride) {
    std::array<float,4> p{std::bit_cast<float>(BulbGuestWord(vertices,at)),
      std::bit_cast<float>(BulbGuestWord(vertices,at+4)),std::bit_cast<float>(BulbGuestWord(vertices,at+8)),1};
    std::array<float,4> clip{};
    for(size_t j=0;j<4;++j)for(size_t i=0;i<4;++i)clip[j]+=p[i]*BulbConstant(constants,8+i,j);
    if(!std::all_of(clip.begin(),clip.end(),[](float v){return std::isfinite(v);}) || clip[3]<=0)return r;
    // Recovered VS2 c255=(1e-5,0,0.5,0), shared half-pixel=(1/w,-1/h).
    clip[0]+=(0.5f*BulbConstant(constants,44,2)+1.0f/w)*clip[3];
    clip[1]+=(0.5f*BulbConstant(constants,44,3)-1.0f/h)*clip[3];
    clip[2]+=std::bit_cast<float>(uint32_t{0x3727C5AC});
    clip[1]=-clip[1];
    if(!std::all_of(clip.begin(),clip.end(),[](float v){return std::isfinite(v);}))return {};
    for(size_t j=0;j<3;++j) {
      const float v=clip[j]/clip[3];
      if(!std::isfinite(v))return {};
      lo[j]=std::min(lo[j],v);hi[j]=std::max(hi[j],v);
    }
  }
  r.known=true;
  r.visible=hi[0]>-1 && lo[0]<1 && hi[1]>-1 && lo[1]<1 && hi[2]>=0 && lo[2]<=1;
  if(!r.visible)return r;
  r.normalized={std::clamp((lo[0]+1)*0.5f-2/w,0.0f,1.0f),std::clamp((lo[1]+1)*0.5f-2/h,0.0f,1.0f),
                std::clamp((hi[0]+1)*0.5f+2/w,0.0f,1.0f),std::clamp((hi[1]+1)*0.5f+2/h,0.0f,1.0f)};
  return r;
}
struct BulbProbeIdentity {
  uint64_t run=0,source_sequence=0,sequence=0,submission=0,image=0,lifetime=0,generation=0,writer=0;
  uint32_t fixture=0,frame=0,command_index=0,bytes_per_texel=0;
  std::array<uint32_t,4> rectangle{}; // pixel left, top, width, height
  std::string role;
};
} // namespace rex::graphics::gta4_native
