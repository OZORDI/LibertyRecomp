// Standalone integration test, linked to the actual built Rex runtime.
// Compile with -fno-access-control only for this test translation unit.
// No generated PPC code or production decoder implementation is copied here.
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <rex/audio/xma/context.h>
#include <rex/logging.h>
extern "C" {
#include <libavcodec/avcodec.h>
}
using namespace rex::audio;
static void Check(bool ok, const std::string& message) { if (!ok) throw std::runtime_error(message); }
static std::vector<uint8_t> Read(const std::filesystem::path& p) {
  std::ifstream f(p,std::ios::binary|std::ios::ate);Check(bool(f),"open: "+p.string());
  auto size=f.tellg();Check(size>=0 && size<16*1024*1024,"fixture size");
  std::vector<uint8_t> b(static_cast<size_t>(size)); f.seekg(0); f.read(reinterpret_cast<char*>(b.data()),size);
  Check(bool(f),"read fixture");return b;
}
struct Frame {uint32_t bit, length;std::vector<uint8_t> packet;};
struct Wave {std::string name;uint32_t rate,channels,start,end,skip,end_subframe,samples;std::vector<uint8_t> data;std::vector<Frame> frames;};
struct Allocation {
 rex::memory::Memory& memory;uint32_t guest,physical;
 Allocation(rex::memory::Memory& m,uint32_t size,bool phys):memory(m) {
  guest=m.SystemHeapAlloc(size,4096,phys?rex::memory::kSystemHeapPhysical:rex::memory::kSystemHeapVirtual);
  Check(guest!=0,"test heap allocation");physical=m.GetPhysicalAddress(guest);
  if(phys)Check(physical!=UINT32_MAX,"physical mapping");
 }
 ~Allocation(){memory.SystemHeapFree(guest);}
};
struct Reference {
 XmaContext owner;
 AVCodecContext* codec=nullptr;
 Reference(rex::memory::Memory& memory,uint32_t guest,int rate,int channels){
  Check(owner.Setup(1,&memory,guest)==0,"reference setup");
  auto r=std::find(std::begin(kIdToSampleRate),std::end(kIdToSampleRate),rate);
  Check(r!=std::end(kIdToSampleRate),"reference rate");
  Check(owner.PrepareDecoder(int(r-std::begin(kIdToSampleRate)),channels==2)>=0,"reference open");
  codec=owner.av_context_;
 }
 std::vector<uint8_t> Decode(const Frame& source,bool stereo){
  std::vector<uint8_t> padded=source.packet;padded.resize(padded.size()+AV_INPUT_BUFFER_PADDING_SIZE);
  owner.av_packet_->data=padded.data();owner.av_packet_->size=int(source.packet.size());
  Check(owner.DecodePacket(owner.av_context_,owner.av_packet_,owner.av_frame_),"reference decode");
  owner.av_packet_->data=nullptr;owner.av_packet_->size=0;
  Check(owner.av_frame_->nb_samples==512,"reference sample count");
  std::vector<uint8_t> pcm(1024*(stereo?2:1));
  XmaContext::ConvertFrame(const_cast<const uint8_t**>(owner.av_frame_->data),stereo,pcm.data());return pcm;
 }
};
struct Outcome {uint64_t emitted=0,frames=0,wraps=0;bool nonfinite=false;};
static Outcome Run(rex::memory::Memory& memory,const Wave& wave,uint32_t loops,uint32_t wraps_to_test,uint32_t quantum,uint32_t padding,bool compare) {
 const bool stereo=wave.channels==2;const uint32_t channels=wave.channels;
 Allocation input(memory,uint32_t(wave.data.size()),true),guest_state(memory,4096,false);
 std::memcpy(memory.TranslatePhysical(input.physical),wave.data.data(),wave.data.size());
 std::memset(memory.TranslateVirtual(guest_state.guest),0,64);
 XmaContext context;Check(context.Setup(0,&memory,guest_state.guest)==0,"context setup");
 context.set_is_allocated(true);context.Clear();
 XMA_CONTEXT_DATA data(memory.TranslateVirtual(guest_state.guest));
 data.input_buffer_0_ptr=input.physical;data.input_buffer_0_packet_count=uint32_t(wave.data.size()/2048);
 data.input_buffer_0_valid=1;data.current_buffer=0;data.input_buffer_read_offset=wave.frames.front().bit;
 data.output_buffer_block_count=31;data.output_buffer_valid=1;data.subframe_decode_count=quantum;
 data.output_buffer_padding=padding;data.is_stereo=stereo;
 auto rate=std::find(std::begin(kIdToSampleRate),std::end(kIdToSampleRate),int(wave.rate));Check(rate!=std::end(kIdToSampleRate),"sample rate");
 data.sample_rate=uint32_t(rate-std::begin(kIdToSampleRate));data.loop_count=loops;
 data.loop_start=wave.start;data.loop_end=wave.end;data.loop_subframe_skip=wave.skip;data.loop_subframe_end=wave.end_subframe;
 Reference reference(memory,guest_state.guest,wave.rate,channels);Outcome result;uint32_t expected_count=loops;
 size_t index=0;bool start_skip=false;const auto start=std::find_if(wave.frames.begin(),wave.frames.end(),[&](auto& f){return f.bit==wave.start;});
 Check(start!=wave.frames.end(),"start frame missing");const size_t start_index=size_t(start-wave.frames.begin());
 const uint64_t bound=wave.frames.size()*uint64_t(wraps_to_test+4)+32;
 for(uint64_t iteration=0;iteration<bound && index<wave.frames.size();++iteration){
  const auto& f=wave.frames[index];const bool end=(expected_count!=0 && f.bit==wave.end);
  const auto all=reference.Decode(f,stereo);
  const uint32_t first=start_skip?wave.skip:0,last=end?wave.end_subframe+1:4;
  Check(first<=last,"invalid fixture interval");
  std::vector<uint8_t> expected(all.begin()+first*128*channels*2,all.begin()+last*128*channels*2);
  Check(data.input_buffer_read_offset==f.bit,"input frame offset mismatch before decode "+wave.name+" at frame "+std::to_string(result.frames)+" expected="+std::to_string(f.bit)+" actual="+std::to_string(data.input_buffer_read_offset)+" loops="+std::to_string(loops));
  context.Decode(&data);Check(data.error_status!=4,"production decoder error "+wave.name);
  ++result.frames;
  std::vector<uint8_t> actual;
  for(unsigned call=0;context.current_frame_remaining_subframes_ && call<16;++call){
   std::array<uint8_t,8192> storage{};rex::memory::RingBuffer ring(storage.data(),storage.size());
   context.remaining_subframe_blocks_in_output_buffer_=31;
   auto previous=context.current_frame_remaining_subframes_;
   context.Consume(&ring,&data);
   const auto written=ring.write_offset();actual.insert(actual.end(),storage.begin(),storage.begin()+written);
   Check(context.current_frame_remaining_subframes_<previous,"consume made no progress");
   Check(context.remaining_subframe_blocks_in_output_buffer_>=0,"output budget underflow");
  }
  if(compare && actual!=expected){
    for(size_t j=0,n=0;j<std::min(actual.size(),expected.size()) && n<12;++j)if(actual[j]!=expected[j]){std::cerr<<"MISMATCH byte="<<j<<" actual="<<int(actual[j])<<" expected="<<int(expected[j])<<"\n";++n;}
    std::cerr<<"DIAG reference_rate="<<reference.codec->sample_rate<<" production_rate="<<context.av_context_->sample_rate<<" reference_length="<<f.packet.size()<<" production_length="<<context.av_packet_->size<<"\n";
    for(size_t j=0;j<std::min(size_t{32},actual.size());++j)std::cerr<<int(actual[j])<<',';std::cerr<<" actual\n";
    for(size_t j=0;j<std::min(size_t{32},expected.size());++j)std::cerr<<int(expected[j])<<',';std::cerr<<" expected\n";
    for(size_t j=0;j<std::min(size_t{16},f.packet.size());++j)std::cerr<<int(f.packet[j])<<',';std::cerr<<" reference-packet\n";
    for(size_t j=0;j<std::min(size_t{16},size_t(context.av_packet_->size));++j)std::cerr<<int(context.av_packet_->data[j])<<',';std::cerr<<" production-packet\n";
  }
  if(compare)Check(actual==expected,"PCM mismatch "+wave.name+" frame="+std::to_string(result.frames-1)+" bit="+std::to_string(f.bit)+" expected_bytes="+std::to_string(expected.size())+" actual_bytes="+std::to_string(actual.size()));
  result.emitted+=actual.size()/(channels*2);
  Check(context.av_context_ && context.av_context_->frame_number==int(result.frames),"codec reopened across ordinary loop");
  start_skip=false;
  if(end){++result.wraps;index=start_index;start_skip=true;if(expected_count<255)--expected_count;
    Check(data.loop_count==expected_count,"loop repetition count");
    if(loops==255 && result.wraps>=wraps_to_test)break;
  }else ++index;
 }
 Check(result.frames<bound,"decode did not terminate");
 if(loops!=255)Check(result.wraps==loops,"finite repeats incomplete");
 context.Release();return result;
}

static uint64_t TestConsumeBounds() {
  uint64_t cases=0;
  std::array<uint8_t,64> guest{};
  for(uint32_t stereo:{0u,1u})for(uint32_t end=1;end<=4;++end)
  for(uint32_t first=0;first<4*(stereo+1);++first)
  for(uint32_t quantum=0;quantum<=15;++quantum)for(uint32_t padding=0;padding<=7;++padding){
    XmaContext c;XMA_CONTEXT_DATA data(guest.data());
    data.is_stereo=stereo;data.subframe_decode_count=quantum;data.output_buffer_padding=padding;
    const uint32_t total=4*(stereo+1),limit=end*(stereo+1);
    for(size_t i=0;i<c.raw_frame_.size();++i)c.raw_frame_[i]=uint8_t((i*37+19)%251);
    c.current_frame_remaining_subframes_=uint8_t(total-first);c.loop_frame_output_limit_=uint8_t(limit);
    c.remaining_subframe_blocks_in_output_buffer_=31;
    std::array<uint8_t,8192> storage;storage.fill(0xA5);
    rex::memory::RingBuffer ring(storage.data(),storage.size());
    ring.set_read_offset(storage.size()-256);ring.set_write_offset(storage.size()-256);
    for(unsigned call=0;c.current_frame_remaining_subframes_ && call<16;++call)c.Consume(&ring,&data);
    Check(c.current_frame_remaining_subframes_==0,"consume termination");
    const auto count=first<limit?(limit-first)*256:0;
    Check(ring.read_count()==count,"consume emitted incorrect byte count");
    std::vector<uint8_t> actual(count);ring.Read(actual.data(),count);
    std::vector<uint8_t> expected;
    if(count)expected.assign(c.raw_frame_.begin()+first*256,c.raw_frame_.begin()+limit*256);
    Check(actual==expected,"consume selected wrong subframe or wrap bytes");
    Check(c.remaining_subframe_blocks_in_output_buffer_==31-int(count/256+padding),"padding charged incorrectly");
    auto capacity=c.remaining_subframe_blocks_in_output_buffer_;c.Consume(&ring,&data);
    Check(c.remaining_subframe_blocks_in_output_buffer_==capacity,"padding charged twice");
    ++cases;
  }
  std::cout<<"PASS bounded-consume cases="<<cases<<" wrapped-output=true stereo-block-order=exact padding=once\n";
  return cases;
}
static void TestWork(rex::memory::Memory& memory,const Wave& w,uint32_t capacity,uint32_t quantum,bool split=false){
 Allocation in(memory,uint32_t(w.data.size()),true),out(memory,8192,true),state(memory,4096,false);
 std::memcpy(memory.TranslatePhysical(in.physical),w.data.data(),w.data.size());
 std::memset(memory.TranslatePhysical(out.physical),0xA5,8192);
 std::memset(memory.TranslateVirtual(state.guest),0,64);
 XmaContext c;Check(c.Setup(0,&memory,state.guest)==0,"work setup");c.set_is_allocated(true);c.Clear();
 XMA_CONTEXT_DATA d(memory.TranslateVirtual(state.guest));
 d.input_buffer_0_ptr=in.physical;d.input_buffer_0_packet_count=uint32_t(w.data.size()/2048);d.input_buffer_0_valid=1;
 if(split) {
   Check(w.data.size()>2048,"split fixture needs two packets");
   d.input_buffer_0_packet_count=1;
   d.input_buffer_1_ptr=in.physical+2048;
   d.input_buffer_1_packet_count=uint32_t(w.data.size()/2048-1);
   d.input_buffer_1_valid=1;
 }
 d.input_buffer_read_offset=w.frames.front().bit;
 d.output_buffer_ptr=out.physical;d.output_buffer_block_count=capacity;d.output_buffer_read_offset=capacity-1;d.output_buffer_write_offset=capacity-1;
 d.output_buffer_padding=1;d.output_buffer_valid=1;d.subframe_decode_count=quantum;d.is_stereo=w.channels==2;
 const auto r=std::find(std::begin(kIdToSampleRate),std::end(kIdToSampleRate),int(w.rate));d.sample_rate=uint32_t(r-std::begin(kIdToSampleRate));
 d.loop_start=w.start;d.loop_end=w.end;d.loop_subframe_skip=w.skip;d.loop_subframe_end=w.end_subframe;d.loop_count=split?0:2;
 d.Store(memory.TranslateVirtual(state.guest));
 Reference reference(memory,state.guest,w.rate,w.channels);
 std::vector<uint8_t> expected,actual;uint32_t remaining=split?0:2;bool skip=false;
 const auto begin=std::find_if(w.frames.begin(),w.frames.end(),[&](auto& f){return f.bit==w.start;});
 size_t index=0;uint64_t decoded=0;
 for(uint64_t i=0;i<w.frames.size()*4+8 && index<w.frames.size();++i){
   const auto& f=w.frames[index];auto pcm=reference.Decode(f,w.channels==2);++decoded;
   bool end=remaining && f.bit==w.end;auto first=skip?w.skip:0,last=end?w.end_subframe+1:4;
   expected.insert(expected.end(),pcm.begin()+first*128*w.channels*2,pcm.begin()+last*128*w.channels*2);skip=false;
   if(end){index=size_t(begin-w.frames.begin());--remaining;skip=true;}else ++index;
 }
 const uint64_t bound=w.frames.size()*32+64;
 for(uint64_t work=0;work<bound;++work){
   c.Enable();Check(c.Work(),"work declined");d=XMA_CONTEXT_DATA(memory.TranslateVirtual(state.guest));
   Check(d.error_status!=4,"work decoder error: "+w.name);
   rex::memory::RingBuffer ring(memory.TranslatePhysical(out.physical),capacity*256);
   ring.set_read_offset(d.output_buffer_read_offset*256);ring.set_write_offset(d.output_buffer_write_offset*256);
   auto count=ring.read_count();auto old=actual.size();actual.resize(old+count);ring.Read(actual.data()+old,count);
   d.output_buffer_read_offset=d.output_buffer_write_offset;d.output_buffer_valid=1;d.Store(memory.TranslateVirtual(state.guest));
   c.Disable();Check(!c.Work(),"disabled context decoded audio");
   if(!d.IsAnyInputBufferValid() && c.current_frame_remaining_subframes_==0)break;
   Check(count || c.current_frame_remaining_subframes_ || work==0,"work playback stalled");
 }
 Check(actual==expected,"Work output differs from reference: "+w.name+" capacity="+std::to_string(capacity)+" quantum="+std::to_string(quantum)+" expected="+std::to_string(expected.size())+" actual="+std::to_string(actual.size()));
 Check(c.av_context_->frame_number==int(decoded),"work loop codec history changed");
 for(size_t i=capacity*256;i<8192;++i)Check(memory.TranslatePhysical(out.physical)[i]==0xA5,"output write exceeded guest ring");
 c.Release();
}
static void TestInvalidContexts(rex::memory::Memory& memory,const Wave& w){
 Allocation in(memory,uint32_t(w.data.size()),true),out(memory,8192,true),state(memory,4096,false);
 std::memcpy(memory.TranslatePhysical(in.physical),w.data.data(),w.data.size());
 std::memset(memory.TranslateVirtual(state.guest),0,64);
 XmaContext c;Check(c.Setup(0,&memory,state.guest)==0,"invalid setup");c.set_is_allocated(true);c.Clear();
 XMA_CONTEXT_DATA d(memory.TranslateVirtual(state.guest));d.output_buffer_valid=1;d.output_buffer_block_count=0;
 d.Store(memory.TranslateVirtual(state.guest));c.Enable();Check(c.Work(),"zero-ring work refused");
 Check(XMA_CONTEXT_DATA(memory.TranslateVirtual(state.guest)).error_status==4,"zero ring did not fail safely");
 d.output_buffer_block_count=31;d.output_buffer_ptr=out.physical;d.input_buffer_0_ptr=in.physical;
 d.input_buffer_0_packet_count=uint32_t(w.data.size()/2048);d.input_buffer_0_valid=1;d.error_status=0;
 d.input_buffer_read_offset=uint32_t(w.data.size()*8+32);c.Decode(&d);Check(d.error_status==4,"invalid read offset accepted");
 d.input_buffer_read_offset=w.end;d.error_status=0;d.loop_count=255;d.loop_start=w.end;d.loop_end=w.end;d.loop_subframe_skip=4;d.loop_subframe_end=3;
 c.Decode(&d);Check(d.error_status==4,"zero-length infinite loop accepted");
 c.Release();
 std::cout<<"PASS invalid-output-capacity invalid-input-offset empty-loop=bounded-error\n";
}

int main(int argc,char**argv){try{
 Check(argc==2,"usage: test_xma_loop_playback manifest.tsv");rex::InitLogging();
 rex::memory::Memory memory;Check(memory.Initialize(),"memory setup");
 std::ifstream manifest(argv[1]);Check(bool(manifest),"manifest open");
 const auto directory=std::filesystem::path(argv[1]).parent_path();
 const auto consume_cases=TestConsumeBounds();
 uint64_t cases=0,total_frames=0,total_samples=0,work_cases=0;std::string filename;
 for(Wave w;manifest>>w.name>>w.rate>>w.channels>>w.start>>w.end>>w.skip>>w.end_subframe>>w.samples>>filename;){
  w.data=Read(directory/filename);w.frames.clear();
  std::ifstream frame_list(directory/(filename+".frames"));std::string packet;
  for(Frame f;frame_list>>f.bit>>f.length>>packet;){f.packet=Read(directory/packet);w.frames.push_back(std::move(f));}
  Check(!w.frames.empty(),"no frames");
  for(uint32_t loops:{0u,1u,3u,255u})for(uint32_t quantum:{1u,2u,4u,8u})for(uint32_t padding:{0u,1u}){
   auto o=Run(memory,w,loops,6,quantum,padding,true);++cases;total_frames+=o.frames;total_samples+=o.emitted;
  }
  if(w.name.starts_with("FAST_2_") || w.name=="SYNTHETIC_c2_r48000_s4_e2") {
    for(uint32_t capacity:{3u,5u,31u})for(uint32_t quantum:{1u,2u}) {
      TestWork(memory,w,capacity,quantum);++work_cases;
      if(w.data.size()>2048) {TestWork(memory,w,capacity,quantum,true);++work_cases;}
    }
    TestInvalidContexts(memory,w);
  }
  if(w.name.starts_with("INTERLEAVED_")) {
    // Stream selection inside an interleaved buffer is independent of channel
    // sample interleaving. The reference follows only this stream's packets.
    for(uint32_t capacity:{5u,31u}) {
      TestWork(memory,w,capacity,1,false);++work_cases;
    }
  }
  std::cout<<"PASS wave="<<w.name<<" frames="<<w.frames.size()<<" mono/stereo="<<w.channels<<" loop-modes=4 quantum-modes=4 padding-modes=2 PCM=exact codec-history=preserved\n";
 }
 std::cout<<"RESULT cases="<<cases<<" decoded_frames="<<total_frames<<" compared_samples="<<total_samples<<" consume_cases="<<consume_cases<<" work_cases="<<work_cases<<"\n";return 0;
}catch(const std::exception&e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
