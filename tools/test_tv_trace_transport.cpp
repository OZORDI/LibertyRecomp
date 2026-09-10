#define REX_TV_TRACE_TEST
#include <rex/graphics/gta4_native/tv_trace.h>
#include <cassert>
#include <iostream>

using namespace rex::graphics::gta4_native;
int main() {
  const std::array<uint32_t,4> payload{16,42,0xDEADBEEF,0x12345678};
  TvTraceContext context{};context.run=100;context.event=5;
  context.script_surface=0x40003100;context.script_texture=0x40003140;
  context.plane_handles={10,20,30,0};context.guest_frame=123;
  auto encoded=PackTvTraceEnvelope(payload.data(),sizeof(payload),31,context);
  auto check=[&](const std::vector<uint8_t>& bytes, bool expected) {
    const void* data=bytes.data();size_t size=bytes.size();uint32_t abi=kTvTraceEnvelopeAbi;
    TvTraceContext decoded{};
    const bool ok=UnpackTvTraceEnvelope(data,size,abi,decoded);assert(ok==expected);
    if(ok) {
      assert(abi==31 && size==sizeof(payload));
      assert(std::memcmp(data,payload.data(),size)==0);
      assert(decoded.run==context.run && decoded.event==context.event);
      assert(decoded.plane_handles==context.plane_handles && decoded.script_texture==context.script_texture);
    } else {
      assert(data==bytes.data() && size==bytes.size() && abi==kTvTraceEnvelopeAbi);
    }
  };
  check(encoded,true);
  for(size_t size=0;size<encoded.size();++size) {
    auto truncated=encoded;truncated.resize(size);check(truncated,false);
  }
  for(int corruption=0;corruption<6;++corruption) {
    auto bad=encoded;TvTraceEnvelope header{};std::memcpy(&header,bad.data(),sizeof(header));
    switch(corruption) {
      case 0:header.version=2;break;
      case 1:header.reserved=1;break;
      case 2:header.context.run=0;break;
      case 3:header.context.event=0;break;
      case 4:header.command_size=0;break;
      case 5:header.command_abi=kTvTraceEnvelopeAbi;break;
    }
    std::memcpy(bad.data(),&header,sizeof(header));check(bad,false);
  }
  bool rejected=false;
  try { PackTvTraceEnvelope(nullptr,sizeof(payload),31,context); }
  catch(const std::invalid_argument&) {rejected=true;}
  assert(rejected);
  assert(IsTvBinkShader(0xA6C9E2B8B2A59D7Aull));
  assert(IsTvBinkShader(0x9E76B68B60127349ull));
  assert(!IsTvBinkShader(0));
  setenv("REX_TV_TEST_INTEGER","-1",1);assert(TvTraceUnsigned("REX_TV_TEST_INTEGER",7,10)==7);
  setenv("REX_TV_TEST_INTEGER","abc",1);assert(TvTraceUnsigned("REX_TV_TEST_INTEGER",7,10)==7);
  setenv("REX_TV_TEST_INTEGER","11",1);assert(TvTraceUnsigned("REX_TV_TEST_INTEGER",7,10)==7);
  setenv("REX_TV_TEST_INTEGER","3",1);assert(TvTraceUnsigned("REX_TV_TEST_INTEGER",7,10)==3);
  std::cout<<"TV transport: round-trip, unchanged payload, all truncated envelopes, malformed metadata, and bounds passed\n";
}
