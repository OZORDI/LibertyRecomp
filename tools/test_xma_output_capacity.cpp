// Direct regression against XmaContext::Consume in the built Rex runtime.
// Compile this test with -fno-access-control; the application remains unchanged.
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <rex/audio/xma/context.h>
#include <rex/logging.h>
using namespace rex::audio;
static void Check(bool value,const char* message) {if(!value)throw std::runtime_error(message);}
int main() {try {
  rex::InitLogging();
  uint64_t cases=0;
  for(uint32_t channels:{1u,2u})for(uint32_t end=1;end<=4;++end)
  for(uint32_t first=0;first<4*channels;++first)
  for(uint32_t quantum:{1u,2u,4u,8u})for(uint32_t padding:{0u,1u,3u})
  for(uint32_t budget=0;budget<=10;++budget) {
    XmaContext context;std::array<uint8_t,64> state{};XMA_CONTEXT_DATA data(state.data());
    data.is_stereo=channels==2;data.subframe_decode_count=quantum;data.output_buffer_padding=padding;
    const uint32_t total=4*channels,limit=end*channels;
    const uint32_t remaining=total-first,available=first<limit?limit-first:0;
    const uint32_t emitted=std::min(available,quantum);
    const bool finishes=first+emitted>=limit;
    const uint32_t required=emitted+(finishes?padding:0);
    context.current_frame_remaining_subframes_=uint8_t(remaining);
    context.loop_frame_output_limit_=uint8_t(limit);
    context.remaining_subframe_blocks_in_output_buffer_=int32_t(budget);
    for(size_t i=0;i<context.raw_frame_.size();++i)context.raw_frame_[i]=uint8_t((i*31+7)%251);
    std::array<uint8_t,8192> storage{};rex::memory::RingBuffer ring(storage.data(),storage.size());
    // The transport's accounting budget can be smaller than physical capacity
    // after reserving the guest's frame padding in an earlier consume call.
    context.Consume(&ring,&data);
    if(budget<required) {
      Check(ring.read_count()==0,"insufficient output budget still wrote PCM");
      Check(context.current_frame_remaining_subframes_==remaining,"insufficient space discarded pending samples");
      Check(context.loop_frame_output_limit_==limit,"insufficient space discarded loop tail state");
      Check(context.remaining_subframe_blocks_in_output_buffer_==int32_t(budget),"output budget underflow");
      // Retry after the game drains the output; the identical samples must survive.
      context.remaining_subframe_blocks_in_output_buffer_=31;
      context.Consume(&ring,&data);
    }
    Check(ring.read_count()==emitted*256,"wrong pending-tail byte count");
    std::vector<uint8_t> actual(emitted*256);ring.Read(actual.data(),actual.size());
    Check(std::equal(actual.begin(),actual.end(),context.raw_frame_.begin()+first*256),"retry changed pending PCM");
    Check(context.current_frame_remaining_subframes_==(finishes?0:remaining-emitted),"wrong pending-tail cursor");
    Check(context.remaining_subframe_blocks_in_output_buffer_>=0,"negative output budget");
    ++cases;
  }
  // Physical space is an independent bound. Deliberately let the accounting
  // report more free space than the output ring and verify no unread overwrite.
  XmaContext context;std::array<uint8_t,64> state{};XMA_CONTEXT_DATA data(state.data());
  data.is_stereo=1;data.subframe_decode_count=8;data.output_buffer_padding=1;
  context.current_frame_remaining_subframes_=8;context.remaining_subframe_blocks_in_output_buffer_=31;
  std::array<uint8_t,1024> storage;storage.fill(0xA5);
  rex::memory::RingBuffer ring(storage.data(),storage.size());ring.set_write_offset(768);
  context.Consume(&ring,&data);
  Check(context.current_frame_remaining_subframes_==8 && ring.write_offset()==768,"short physical ring consumed pending audio");
  Check(std::all_of(storage.begin(),storage.end(),[](uint8_t byte){return byte==0xA5;}),"short ring overwritten");
  ++cases;
  // End-of-stream draining reaches Consume through Work's consume-only path,
  // bypassing the ordinary decode admission check. Exercise that path with
  // actual guest memory, an almost-full ring and a later consumer advance.
  rex::memory::Memory memory;Check(memory.Initialize(),"memory initialization");
  for(uint32_t channels:{1u,2u}) {
    const auto state_address=memory.SystemHeapAlloc(4096,4096,rex::memory::kSystemHeapVirtual);
    const auto output_address=memory.SystemHeapAlloc(8192,4096,rex::memory::kSystemHeapPhysical);
    Check(state_address && output_address,"guest allocation");
    const auto physical=memory.GetPhysicalAddress(output_address);
    Check(physical!=UINT32_MAX,"guest physical address");
    std::memset(memory.TranslateVirtual(state_address),0,64);
    std::memset(memory.TranslatePhysical(physical),0xA5,8192);
    XmaContext tail;Check(tail.Setup(0,&memory,state_address)==0,"tail context setup");
    tail.set_is_allocated(true);tail.Clear();
    for(size_t i=0;i<tail.raw_frame_.size();++i)tail.raw_frame_[i]=uint8_t((i*13+23)%251);
    const std::vector<uint8_t> expected(tail.raw_frame_.begin(),tail.raw_frame_.begin()+1024*channels);
    tail.current_frame_remaining_subframes_=uint8_t(4*channels);
    XMA_CONTEXT_DATA d(memory.TranslateVirtual(state_address));
    d.output_buffer_ptr=physical;d.output_buffer_block_count=31;d.output_buffer_valid=1;
    d.output_buffer_write_offset=29;d.output_buffer_read_offset=0;
    d.is_stereo=channels==2;d.subframe_decode_count=8;d.output_buffer_padding=1;
    d.Store(memory.TranslateVirtual(state_address));
    tail.Enable();Check(tail.Work(),"consume-only work declined");
    d=XMA_CONTEXT_DATA(memory.TranslateVirtual(state_address));
    Check(d.output_buffer_write_offset==29 && tail.current_frame_remaining_subframes_==4*channels,"consume-only short ring lost decoded tail");
    for(size_t i=0;i<8192;++i)Check(memory.TranslatePhysical(physical)[i]==0xA5,"consume-only overwrote queued samples");
    d.output_buffer_read_offset=d.output_buffer_write_offset;d.Store(memory.TranslateVirtual(state_address));
    tail.Enable();Check(tail.Work(),"consume-only retry declined");
    d=XMA_CONTEXT_DATA(memory.TranslateVirtual(state_address));
    rex::memory::RingBuffer result(memory.TranslatePhysical(physical),31*256);
    result.set_read_offset(d.output_buffer_read_offset*256);result.set_write_offset(d.output_buffer_write_offset*256);
    Check(result.read_count()==expected.size() && !tail.current_frame_remaining_subframes_,"consume-only retry length");
    std::vector<uint8_t> actual(expected.size());result.Read(actual.data(),actual.size());
    Check(actual==expected,"consume-only retry changed decoded tail");
    Check(d.error_status==0,"ordinary output backpressure is not a decoder error");
    tail.Release();memory.SystemHeapFree(output_address);memory.SystemHeapFree(state_address);
    ++cases;
  }
  std::cout<<"PASS pending-output-capacity cases="<<cases<<" short-budget=deferred physical-capacity=bounded pending-PCM=preserved retry=exact consume-only-Work=passed\n";
  return 0;
}catch(const std::exception& error){std::cerr<<"FAIL "<<error.what()<<'\n';return 1;}}
