#pragma once
#include <array>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <optional>
#include <span>
#include <type_traits>

namespace rex::graphics::gta4_native {
// Observational provenance, independent of RenderPhase (which also controls
// actual rendering). Retail phase IDs come from CNewDrawListDC +12, populated
// by CRenderPhase virtual slot 9. They are NOT the host RenderPhase enum.
enum class GpuPassOriginSource : uint32_t { kNone, kListHeader, kExecutionRecord, kVerifiedBoth, kUnresolved, kMismatch, kMixed };
struct GpuPassOrigin {
  uint64_t scope = 0;
  uint32_t retail_phase = UINT32_MAX;
  GpuPassOriginSource source = GpuPassOriginSource::kNone;
  uint32_t arena = 0, offset = 0, ordinal = 0, list_id = 0;
  auto operator<=>(const GpuPassOrigin&) const = default;
  bool attributed() const {
    return source==GpuPassOriginSource::kListHeader || source==GpuPassOriginSource::kExecutionRecord ||
           source==GpuPassOriginSource::kVerifiedBoth;
  }
};
static_assert(std::is_trivially_copyable_v<GpuPassOrigin> && sizeof(GpuPassOrigin)==32);
inline constexpr uint32_t kGpuPassEnvelopeAbi = 0x47505031;
inline constexpr uint32_t kGpuPassEnvelopeVersion = 1;
inline constexpr uint32_t kRetailNewDrawListVtable = 0x82001038;
inline constexpr uint32_t kRetailCommandArenaCapacity = 0xFA000;
inline const char* GpuPassOriginSourceName(GpuPassOriginSource source) {
  switch(source){
    case GpuPassOriginSource::kNone:return "not-in-draw-list";
    case GpuPassOriginSource::kListHeader:return "executed-list-header";
    case GpuPassOriginSource::kExecutionRecord:return "executed-task-record";
    case GpuPassOriginSource::kVerifiedBoth:return "header-and-task-record";
    case GpuPassOriginSource::kUnresolved:return "unresolved-list";
    case GpuPassOriginSource::kMismatch:return "conflicting-list-record";
    case GpuPassOriginSource::kMixed:return "mixed-list-origins";
  }return "invalid";
}
// Verified against vtables_with_addrs.txt slot 9 and compiled generated getters.
// ID 23 is genuinely shared by FrontEnd and PhoneModel; do not invent a split.
inline const char* RetailGpuPassName(uint32_t phase) {
  switch(phase){
    case 0:return "base-render-phase";
    case 1:return "lights-to-screen";
    case 2:return "radar";
    case 3:return "blit";
    case 6:return "default-render-state";
    case 9:return "pre-render-viewport";
    case 10:return "post-render-viewport";
    case 13:return "script-2d";
    case 14:return "tree-imposters";
    case 15:return "draw-scene";
    case 16:return "water-surface";
    case 17:return "water-reflection";
    case 19:return "environment-reflection";
    case 20:return "interior-reflection";
    case 21:return "warp-shadow";
    case 23:return "frontend-or-phone-model";
    case 24:return "html";
    case 31:return "scene-to-gbuffer";
    case 32:return "cloud-generation";
    case 34:return "player-settings";
    case 35:return "rain-update";
    case 36:return "mirror-reflection";
    default:return "unknown-retail-phase";
  }
}

// Decode from the command list that actually executes, not its earlier builder.
// Read returns optional guest-endian-decoded uint32_t, validating host access.
// No guest writes, pointer ownership, address->origin map, or historical cache.
template <typename Read>
GpuPassOrigin DecodeExecutedGpuDrawList(uint32_t arena, uint32_t offset, uint32_t end_token,
                                       uint32_t ordinal, uint32_t guest_r13, Read&& read) {
  GpuPassOrigin result{};result.source=GpuPassOriginSource::kUnresolved;
  result.arena=arena;result.offset=offset;result.ordinal=ordinal;
  const auto at=[&](uint32_t base,uint32_t displacement)->std::optional<uint32_t>{
    if(!base || uint64_t(base)+displacement>UINT32_MAX)return {};
    return read(base+displacement);
  };
  std::optional<uint32_t> header_phase,task_phase;
  const auto selector=at(arena,16);
  if(selector && *selector<2 && offset<=kRetailCommandArenaCapacity-16){
    const auto page=at(arena,*selector*4);
    if(page && *page && uint64_t(*page)+offset+16<=uint64_t(UINT32_MAX)+1){
      const uint32_t start=*page+offset;
      const auto vtable=at(start,0);
      if(vtable && *vtable==kRetailNewDrawListVtable){
        header_phase=at(start,12);
        const auto id=at(start,8);if(id)result.list_id=*id;else header_phase.reset();
      }
    }
  }
  // sub_821BA928 publishes its executing 28-byte task record at guest TLS +16.
  // The recorded start/end/ordinal must all match this invocation. This also
  // supports a resumed list whose first DC is no longer CNewDrawListDC.
  const auto tls=at(guest_r13,0);
  const auto record=tls?at(*tls,16):std::optional<uint32_t>{};
  if(record && *record){
    const auto start=at(*record,0),end=at(*record,4),phase=at(*record,8),state=at(*record,12),serial=at(*record,16);
    if(start&&end&&phase&&state&&serial && *start==offset && *end==end_token && *serial==ordinal && *state==3)
      task_phase=phase;
  }
  if(header_phase&&task_phase&&*header_phase!=*task_phase){result.source=GpuPassOriginSource::kMismatch;return result;}
  if(header_phase||task_phase){
    result.retail_phase=header_phase?*header_phase:*task_phase;
    result.source=header_phase?(task_phase?GpuPassOriginSource::kVerifiedBoth:GpuPassOriginSource::kListHeader):GpuPassOriginSource::kExecutionRecord;
  }
  return result;
}

struct GpuPassEnvelopeHeader {
  uint32_t magic=kGpuPassEnvelopeAbi, version=kGpuPassEnvelopeVersion, inner_abi=0, payload_size=0;
  GpuPassOrigin origin{};
};
static_assert(sizeof(GpuPassEnvelopeHeader)==48);
inline size_t PackGpuPassEnvelope(std::span<uint8_t> storage,const void* command,size_t size,
                                  uint32_t abi,const GpuPassOrigin& origin) {
  if(!command||!size||size>UINT32_MAX||abi==kGpuPassEnvelopeAbi ||
     storage.size()<sizeof(GpuPassEnvelopeHeader)||size>storage.size()-sizeof(GpuPassEnvelopeHeader))return 0;
  const GpuPassEnvelopeHeader header{kGpuPassEnvelopeAbi,kGpuPassEnvelopeVersion,abi,uint32_t(size),origin};
  std::memcpy(storage.data(),&header,sizeof(header));
  std::memcpy(storage.data()+sizeof(header),command,size);
  return sizeof(header)+size;
}
inline bool UnpackGpuPassEnvelope(const void*& command,size_t& size,uint32_t& abi,GpuPassOrigin& origin) {
  if(!command||size<sizeof(GpuPassEnvelopeHeader))return false;
  GpuPassEnvelopeHeader h;std::memcpy(&h,command,sizeof(h));
  if(h.magic!=kGpuPassEnvelopeAbi||h.version!=kGpuPassEnvelopeVersion||!h.payload_size||
     h.payload_size!=size-sizeof(h)||h.inner_abi==kGpuPassEnvelopeAbi||
     uint32_t(h.origin.source)>uint32_t(GpuPassOriginSource::kMismatch))return false;
  abi=h.inner_abi;origin=h.origin;command=static_cast<const uint8_t*>(command)+sizeof(h);size=h.payload_size;return true;
}
} // namespace rex::graphics::gta4_native
