#!/usr/bin/env python3
"""Exercise production native Touch Controls activation with the real CVar registry.

The hook, descriptor checks, choice mutation, value resolver and save routine are
extracted verbatim from the current source. Guest allocation/renderer submission
are bounded test doubles; no alternative setting implementation is supplied.
"""
from pathlib import Path
import argparse
import re
import shlex
import subprocess
import tempfile

SDK = Path(__file__).resolve().parents[2]
ROOT = SDK.parent.parent
SOURCE = SDK / "gta4-recomp/src/gta4_frontend_hooks.cpp"


def function(text, signature):
    start = re.search(re.escape(signature) + r"[^;{]*\{", text).start()
    end = text.index("\n}", start) + len("\n}")
    return text[start:end]


PREFIX = r"""
#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>
#include <sys/mman.h>
#include <rex/cvar.h>
#include <rex/graphics/gta4_native/anti_aliasing_policy.h>
#include <rex/graphics/gta4_native/hdr_policy.h>
#include "gta4_frontend_menu_policy.h"
#include "gta4_draw_distance_policy.h"
#define REXLOG_INFO(...) ((void)0)
#define REXLOG_ERROR(...) ((void)0)
union Register { uint64_t u64; int64_t s64; uint32_t u32; int32_t s32; uint8_t u8; };
struct Cr { bool eq=false,lt=false,gt=false; template<class T> void compare(T a,T b,int) { eq=a==b;lt=a<b;gt=a>b; } };
struct PPCContext { REGISTER_MEMBERS uint64_t lr=0; Cr cr6; int xer=0; };
using PPCFunc = void (*)(PPCContext&, uint8_t*);
uint16_t Read16(const uint8_t* p) { uint16_t v; std::memcpy(&v,p,sizeof(v)); return std::byteswap(v); }
uint32_t Read32(const uint8_t* p) { uint32_t v; std::memcpy(&v,p,sizeof(v)); return std::byteswap(v); }
void Write16(uint8_t* p,uint16_t v) { v=std::byteswap(v); std::memcpy(p,&v,sizeof(v)); }
void Write32(uint8_t* p,uint32_t v) { v=std::byteswap(v); std::memcpy(p,&v,sizeof(v)); }
#define REX_LOAD_U8(a) (*(base + uint32_t(a)))
#define REX_LOAD_U16(a) Read16(base + uint32_t(a))
#define REX_LOAD_U32(a) Read32(base + uint32_t(a))
#define REX_STORE_U8(a,v) (*(base + uint32_t(a))=uint8_t(v))
#define REX_STORE_U16(a,v) Write16(base + uint32_t(a),uint16_t(v))
#define REX_STORE_U32(a,v) Write32(base + uint32_t(a),uint32_t(v))
#define REX_FUNC_PROLOGUE()
#define DEFINE_REX_FUNC(name) void __imp__##name(PPCContext& ctx,uint8_t* base)
void __savegprlr_29(PPCContext&,uint8_t*) {}
void __restgprlr_29(PPCContext&,uint8_t*) {}
std::vector<uint32_t> released_widgets;
void sub_8229C2E0(PPCContext& ctx,uint8_t*) { released_widgets.push_back(ctx.r3.u32); }
void sub_8229C478(PPCContext&,uint8_t*) { std::abort(); }
void sub_82256D08(PPCContext&,uint8_t*) { std::abort(); }
unsigned editor_requests=0;
bool editor_captures=false;
namespace gta4::input {
bool ContextTouchEditorCapturesInput(uint64_t = 0) noexcept { return editor_captures; }
void RequestContextTouchEditor() noexcept { ++editor_requests; editor_captures=true; }
}
void ResetOwnedScrollTrace() {}
bool FrontendDiagnosticsEnabled() { return false; }
bool IsBodyTraceScreen(uint32_t) { return false; }
bool IsGuestSpanValid(uint32_t address, size_t size) {
  return address >= 0x83000000u && uint64_t(address) + size <= 0x83010000ull;
}
// These unrelated graphics APIs must not be reached by the touch setting.
namespace rex::graphics::gta4_native {
AntiAliasingMode GetConfiguredAntiAliasingMode() { std::abort(); }
std::string_view GetConfiguredAntiAliasingModeName() { std::abort(); }
AntiAliasingApplyResult SetConfiguredAntiAliasingMode(std::string_view) { std::abort(); }
std::string_view GetConfiguredHdrModeName() { std::abort(); }
bool SetConfiguredHdrMode(std::string_view) { std::abort(); }
}
namespace rex::input::sony {
bool SetFeaturesEnabled(bool) { std::abort(); }
}
extern "C" void sub_82255D00(PPCContext&,uint8_t*);
extern "C" void sub_82252A98(PPCContext&,uint8_t*);
void __imp__sub_82252A98(PPCContext&,uint8_t*) { std::abort(); }
void __imp__sub_82253370(PPCContext&,uint8_t*) { std::abort(); }
unsigned original_input_polls=0;
void __imp__sub_82258FB0(PPCContext&,uint8_t*) { ++original_input_polls; }
unsigned stock_calls=0, rebuilds=0;
std::string rendered_value;
void __imp__sub_82258388(PPCContext& ctx,uint8_t*) { ++stock_calls; ctx.r3.u64=123; }
void GTA4_RunWithPrimaryPlayerInfoAlias(PPCContext& ctx,uint8_t* base,PPCFunc original) { original(ctx,base); }
void SwitchDisplayPage(PPCContext&,uint8_t*,gta4::frontend_menu::policy::Page,uint32_t,size_t=0) { std::abort(); }
"""

SUFFIX = r"""
extern "C" void sub_82255D00(PPCContext& parent,uint8_t* base) {
  ++rebuilds;
  PPCContext value=parent;
  value.r4.u64=kDisplayScreen;
  value.r6.u64=0;
  sub_82252A98(value,base);
  rendered_value=ReadGuestString(base,value.r3.u32,kMenuTraceStringCapacity);
}

int main(int argc,char** argv) {
  assert(argc==2);
  constexpr size_t map_bytes=0x100000000ull;
  auto* base=static_cast<uint8_t*>(mmap(nullptr,map_bytes,PROT_READ|PROT_WRITE,
                                       MAP_PRIVATE|MAP_ANON,-1,0));
  assert(base!=MAP_FAILED);
  constexpr uint32_t rows=0x83000000;
  const auto touch=std::find_if(kSettings.begin(),kSettings.end(),[](const auto& s) { return s.cvar=="touch_controls"; });
  const auto motion=std::find_if(kSettings.begin(),kSettings.end(),[](const auto& s) { return s.cvar=="gta4_motion_enabled"; });
  const auto editor=std::find_if(kSettings.begin(),kSettings.end(),[](const auto& s) { return s.binding==SettingBinding::kTouchLayoutEditor; });
  assert(touch!=kSettings.end() && motion!=kSettings.end() && editor!=kSettings.end());
  assert(rex::cvar::GetFlagByName("touch_controls")=="auto");
  g_display_menu.page=gta4::frontend_menu::policy::Page::kAdvanced;
  g_display_menu.native_pages.push_back({.rows=rows,.count=5});
  REX_STORE_U32(kCurrentScreenAddress,kDisplayScreen);
  REX_STORE_U32(kDisplayOptionsPointer,rows);
  REX_STORE_U16(kDisplayOptionsCount,5);
  assert(WriteSettingRow(base,rows,*touch));
  assert(WriteSettingRow(base,0x83000016,*motion));
  assert(WriteJumpRow(base,0x8300002C,kSaveKey));
  assert(WriteSettingRow(base,0x83000042,*editor));
  WriteSentinelRow(base,0x83000058);
  WriteStringPool(base,g_display_menu,0x83001000);
  g_config_path=std::filesystem::path(argv[1])/"native-touch.toml";
  auto activate=[&](int32_t row) {
    PPCContext ctx; ctx.r1.u64=0x83008000; ctx.r3.u64=7; ctx.r4.s32=row;
    sub_82258388(ctx,base);
    assert(ctx.r4.s32==row);
    return ctx.r3.u32;
  };
  assert(rex::cvar::SetFlagByName("touch_controls","off"));
  for (const auto& expected : {std::pair{"on","On"},std::pair{"auto","Auto"},std::pair{"off","Off"}}) {
    const unsigned previous=rebuilds;
    assert(activate(0)==0);
    assert(rex::cvar::GetFlagByName("touch_controls")==expected.first);
    assert(rendered_value==expected.second && rebuilds>previous);
    assert(!g_display_menu.rebuild_pending && stock_calls==0);
  }
  std::cout<<"PASS native Accept cycles Off/On/Auto and publishes the real value label\n";
  const unsigned previous=rebuilds;
  assert(activate(1)==0);
  assert(rebuilds==previous && rex::cvar::GetFlagByName("touch_controls")=="off");
  g_adjustment={.active=true,.frontend_channel=7,.delta=-1};
  PPCContext arrow; arrow.r3.u64=0;
  sub_82253370(arrow,base);
  assert(rex::cvar::GetFlagByName("touch_controls")=="auto");
  assert(g_display_menu.rebuild_pending);
  PPCContext poll; poll.r3.u64=7;
  sub_82258FB0(poll,base);
  assert(!g_display_menu.rebuild_pending && rendered_value=="Auto");
  std::cout<<"PASS arrows retain reverse cycling and other settings retain their activation behavior\n";
  activate(0); activate(0);
  assert(rex::cvar::GetFlagByName("touch_controls")=="on");
  assert(activate(2)==1 && std::filesystem::is_regular_file(g_config_path));
  assert(rex::cvar::SetFlagByName("touch_controls","off"));
  rex::cvar::LoadConfig(g_config_path);
  assert(rex::cvar::GetFlagByName("touch_controls")=="on");
  activate(0);
  assert(rex::cvar::GetFlagByName("touch_controls")=="auto");
  assert(activate(2)==1);
  rex::cvar::ResetToDefault("touch_controls");
  rex::cvar::LoadConfig(g_config_path);
  assert(rex::cvar::GetFlagByName("touch_controls")=="auto");
  std::cout<<"PASS native Save persists On and restored default Auto through the real TOML registry\n";
  const unsigned before_editor_arrows=rebuilds;
  REX_STORE_U8(kPauseMenuActiveAddress,1);
  for (const int32_t delta : {-1,1}) {
    g_adjustment={.active=true,.frontend_channel=7,.delta=delta};
    PPCContext editor_arrow; editor_arrow.r3.u64=3;
    sub_82253370(editor_arrow,base);
    assert(editor_arrow.r3.u32==0);
    // Also protect the mutation helper if a future dispatch resolves an action
    // row as a Setting: this row deliberately has no choices to dereference.
    ChangeSetting(*editor,delta);
    assert(editor_requests==0 && released_widgets.empty());
    assert(!g_display_menu.rebuild_pending && rebuilds==before_editor_arrows);
    assert(rex::cvar::GetFlagByName("touch_controls")=="auto");
  }
  std::cout<<"PASS both native arrow directions leave Edit Touch Layout inactive and unchanged\n";
  g_display_menu.primary_rows=0x83000080;
  g_display_menu.primary_count=2;
  g_display_menu.last_frontend_channel=7;
  constexpr std::array widgets={101u,102u,103u,104u,105u};
  for(size_t i=0;i<widgets.size();++i) REX_STORE_U32(0x82BFA10C+i*sizeof(uint32_t),widgets[i]);
  REX_STORE_U32(0x82BFA13C,0x13572468);
  REX_STORE_U32(0x82CC7BEC,0x83004000);
  REX_STORE_U32(0x83004C7C,3);
  for (const auto [pause_active, expected_requests] : {std::pair{0,1u},std::pair{1,2u}}) {
    editor_captures=false;
    REX_STORE_U8(kPauseMenuActiveAddress,pause_active);
    const auto descriptor=PublishedDisplayRows(base);
    const auto row_count=REX_LOAD_U16(kDisplayOptionsCount);
    const auto page_index=g_display_menu.current_native_page;
    const unsigned prior_rebuilds=rebuilds;
    assert(activate(3)==1 && editor_requests==expected_requests);
    assert(activate(3)==0 && editor_requests==expected_requests);
    assert(editor_captures && released_widgets.empty());
    assert(IsOwnedAdvancedPage(base) && PublishedDisplayRows(base)==descriptor);
    assert(REX_LOAD_U16(kDisplayOptionsCount)==row_count);
    assert(g_display_menu.current_native_page==page_index);
    assert(g_display_menu.last_frontend_channel==7);
    assert(REX_LOAD_U32(0x82CC7BEC)==0x83004000 && REX_LOAD_U32(0x83004C7C)==3);
    assert(REX_LOAD_U32(0x82BFA13C)==0x13572468);
    for(size_t i=0;i<widgets.size();++i) assert(REX_LOAD_U32(0x82BFA10C+i*sizeof(uint32_t))==widgets[i]);
    assert(rebuilds==prior_rebuilds && rex::cvar::GetFlagByName("touch_controls")=="auto");
    // Captured preview/fade input must not poll the native menu or rebuild a
    // queued value. The deferred refresh resumes when capture is released.
    const unsigned prior_polls=original_input_polls;
    g_display_menu.rebuild_pending=true;
    PPCContext captured_poll; captured_poll.r3.u64=7;
    sub_82258FB0(captured_poll,base);
    assert(captured_poll.r3.u32==0 && original_input_polls==prior_polls);
    assert(rebuilds==prior_rebuilds && g_display_menu.rebuild_pending);
    editor_captures=false;
    PPCContext resumed_poll; resumed_poll.r3.u64=7;
    sub_82258FB0(resumed_poll,base);
    assert(original_input_polls>prior_polls && rebuilds>prior_rebuilds);
    assert(!g_display_menu.rebuild_pending && rendered_value=="Auto");
    assert(REX_LOAD_U32(0x83004C7C)==3 && IsOwnedAdvancedPage(base));
  }
  std::cout<<"PASS Edit Touch Layout requests one editor over main and pause menus without changing native state\n";
  std::cout<<"PASS editor capture blocks native input polling and row refresh until capture ends\n";
  REX_STORE_U32(kDisplayOptionsPointer,0x830000A0);
  assert(activate(0)==123 && stock_calls==1);
  assert(rex::cvar::GetFlagByName("touch_controls")=="auto");
  munmap(base,map_bytes);
  std::cout<<"PASS unowned rows fall through to the original native activation\n";
}
"""


def main():
    parser=argparse.ArgumentParser()
    parser.add_argument("--build-directory",type=Path,default=ROOT/"out/build/macos-release")
    args=parser.parse_args()
    source=SOURCE.read_text()
    definitions=source[source.index("constexpr uint32_t kDisplayScreen"):source.index("bool FrontendDiagnosticsEnabled()")]
    declarations=source[source.index("struct AdjustmentContext"):source.index("PPCContext InvokeGuest")]
    names=["PPCContext InvokeGuest", "std::string ReadGuestString", "bool GuestStringEquals",
           "const Setting* FindSettingByKey", "bool IsDisplayScreen", "uint32_t PublishedDisplayRows",
           "const NativePageState* CurrentNativePage", "bool OwnsPublishedDisplayDescriptor",
           "bool IsOwnedPrimaryPage", "bool IsOwnedAdvancedPage", "uint32_t OwnedDisplayRowAddress",
           "const Setting* FindSettingByRow", "bool IsOwnedJumpRow", "uint32_t TextAddress",
           "std::string CurrentSettingValue", "const Choice& CurrentChoice", "uint8_t CurrentChoiceIndex",
           "double CurrentDrawDistanceScale", "bool WriteInlineKey", "bool WriteJumpRow", "bool WriteSettingRow",
           "void WriteSentinelRow", "void WriteStringPool", "void PublishDisplayDescriptor",
           "void TraceDescriptorPublish", "void PublishPrimaryPage", "void ChangeSetting",
           "void SaveGraphicsSettings"]
    pieces=[PREFIX.replace("REGISTER_MEMBERS", ";".join(f"Register r{i}{{}}" for i in range(32)) + ";"),definitions,declarations]
    cvar=(SDK/"src/input/absolute_pointer.cpp").read_text()
    pieces.append(re.search(r'REXCVAR_DEFINE_STRING\(touch_controls,.*?;',cvar,re.S).group(0))
    pieces.extend(function(source,name) for name in names)
    start=source.index("class ScopedAdjustment final")
    pieces.append(source[start:source.index("\n};",start)+len("\n};")])
    generated=(SDK/"gta4-recomp/generated/gta4_recomp.9.cpp").read_text()
    pieces.append(function(generated,"DEFINE_REX_FUNC(sub_82257450)"))
    for name in ["sub_82252A98","sub_82258FB0","sub_82253370","sub_82258388"]:
        pieces.append(function(source,'extern "C" void '+name+'('))
    pieces.append(SUFFIX)
    ninja=(args.build_directory/"build.ninja").read_text()
    stanza=re.search(r'^build [^\n]*gta4_frontend_hooks\.cpp\.o[^\n]*\n(?P<rest>(?:  [^\n]*\n)+)',ninja,re.M)
    values=dict(re.findall(r'^  (\w+) = (.*)$',stanza.group("rest"),re.M))
    includes=shlex.split(values.get("INCLUDES",""))
    cache=(args.build_directory/"CMakeCache.txt").read_text()
    compiler=re.search(r'^CMAKE_CXX_COMPILER:[^=]*=(.*)$',cache,re.M).group(1)
    library=SDK/"out/mac-arm64"
    with tempfile.TemporaryDirectory(prefix="liberty-touch-frontend-") as directory:
        directory=Path(directory)
        cpp=directory/"test.cpp"
        cpp.write_text("\n\n".join(pieces))
        binary=directory/"test"
        subprocess.run([compiler,"-std=c++23","-O1","-UNDEBUG",*includes,
                        "-I"+str(SDK/"gta4-recomp/src"), "-I"+str(SDK/"thirdparty/cli11/include"),str(cpp), str(SDK/"src/core/cvar.cpp"),
                        "-L"+str(library),"-lrexruntime","-Wl,-rpath,"+str(library),
                        "-o",str(binary)],check=True)
        subprocess.run([str(binary),str(directory)],check=True)


if __name__=="__main__":
    main()
