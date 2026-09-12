// Exercises the actual runtime's CoreAudio callback with deterministic buffers.
// No audio device, routing or user settings are changed. -fno-access-control is
// used only by this test, not the application.
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <rex/audio/coreaudio/coreaudio_output.h>
#include <rex/logging.h>
using namespace rex::audio::coreaudio;
static void Check(bool value,const char* message){if(!value)throw std::runtime_error(message);}
static float Sample(uint32_t frame,uint32_t channel){return float(int(frame%17)-8)/128.0f+float(channel+1)/64.0f;}
static std::vector<float> Source(uint32_t frames,uint32_t channels){
 std::vector<float> data(size_t(frames)*channels);
 for(uint32_t f=0;f<frames;++f)for(uint32_t c=0;c<channels;++c)data[size_t(f)*channels+c]=Sample(f,c);
 return data;
}
static std::vector<float> Render(CoreAudioOutput& output,uint32_t frames,uint32_t channels,uint32_t bytes=UINT32_MAX){
 const size_t count=size_t(frames)*channels;
 std::vector<float> guarded(count+16,-1234.5f);
 AudioBufferList list{};list.mNumberBuffers=1;list.mBuffers[0].mNumberChannels=channels;
 list.mBuffers[0].mDataByteSize=bytes==UINT32_MAX?uint32_t(count*sizeof(float)):bytes;
 list.mBuffers[0].mData=guarded.data()+8;
 Check(output.RenderFrames(frames,&list)==noErr,"callback returned error");
 for(size_t i=0;i<8;++i)Check(guarded[i]==-1234.5f && guarded[count+8+i]==-1234.5f,"callback wrote outside provided buffer");
 return {guarded.begin()+8,guarded.begin()+8+count};
}
int main(){try{
 rex::InitLogging();uint64_t cases=0,checked=0;
 for(uint32_t channels:{2u,6u})for(uint32_t frames:{1u,64u,128u,256u,512u,4096u})
 for(uint32_t clients:{1u,2u,8u})for(bool wrap:{false,true}){
  CoreAudioOutput output;output.channel_count_=channels;
  std::vector<std::unique_ptr<CoreAudioClientState>> owners;
  auto source=Source(frames,channels);
  for(uint32_t i=0;i<clients;++i){
   auto client=std::make_unique<CoreAudioClientState>(nullptr);client->channels=channels;
   if(wrap){auto padding=Source(kCoreAudioRingFrames-32,channels);Check(client->ring.Write(padding.data(),kCoreAudioRingFrames-32,channels),"wrap setup write");Check(client->ring.Read(padding.data(),kCoreAudioRingFrames-32,channels,false)==kCoreAudioRingFrames-32,"wrap setup read");}
   Check(client->ring.Write(source.data(),frames,channels),"source ring write");
   output.clients_[i]=client.get();owners.push_back(std::move(client));
  }
  auto actual=Render(output,frames,channels);
  for(size_t i=0;i<actual.size();++i){float expected=0;for(uint32_t j=0;j<clients;++j)expected+=source[i];expected=std::clamp(expected,-1.0f,1.0f);Check(actual[i]==expected,"callback mixed different PCM");++checked;}
  for(auto& c:owners)Check(c->ring.available_frames()==0 && c->underrun_frames==0,"unexpected output starvation");
  Check(!output.rebuffer_requested_,"false rebuffer request");
  for(auto& slot:output.clients_)slot=nullptr;
  ++cases;
 }
 for(uint32_t channels:{2u,6u}){
  {
   CoreAudioOutput output;output.channel_count_=channels;CoreAudioClientState client(nullptr);client.channels=channels;output.clients_[0]=&client;
   const auto input=Source(13,channels);Check(client.ring.Write(input.data(),13,channels),"partial write");
   auto actual=Render(output,256,channels);Check(std::equal(input.begin(),input.end(),actual.begin()),"partial input changed");
   for(size_t i=input.size();i<actual.size();++i)Check(actual[i]==0,"underrun not zero-filled");
   Check(client.underrun_frames==243 && output.rebuffer_requested_,"underrun accounting");output.clients_[0]=nullptr;++cases;
  }
  {
   CoreAudioOutput output;output.channel_count_=channels;CoreAudioClientState client(nullptr);client.channels=channels;output.clients_[0]=&client;
   std::vector<float> input(size_t(256)*channels);const std::array<float,8> values{0.25f,-0.75f,1.5f,-3.0f,std::numeric_limits<float>::quiet_NaN(),std::numeric_limits<float>::infinity(),-std::numeric_limits<float>::infinity(),0};
   for(size_t i=0;i<input.size();++i)input[i]=values[i%values.size()];
   Check(client.ring.Write(input.data(),256,channels),"clamp input");auto actual=Render(output,256,channels);
   for(size_t i=0;i<actual.size();++i)Check(actual[i]==(std::isfinite(input[i])?std::clamp(input[i],-1.0f,1.0f):0),"output numerical conversion");
   output.clients_[0]=nullptr;++cases;
  }
  {
   CoreAudioOutput output;output.channel_count_=channels;CoreAudioClientState client(nullptr);client.channels=channels;output.clients_[0]=&client;
   const auto input=Source(256,channels);Check(client.ring.Write(input.data(),256,channels),"pause input");
   client.paused=true;auto silent=Render(output,256,channels);
   Check(std::all_of(silent.begin(),silent.end(),[](float s){return s==0;}),"paused voice not silent");Check(client.ring.available_frames()==256,"paused voice consumed");
   client.paused=false;Check(Render(output,256,channels)==input,"resume PCM changed");
   Check(client.ring.Write(input.data(),256,channels),"mute input");output.muted_=true;silent=Render(output,256,channels);
   Check(std::all_of(silent.begin(),silent.end(),[](float s){return s==0;}),"mute output nonzero");Check(client.ring.available_frames()==0,"mute did not advance timeline");
   output.clients_[0]=nullptr;++cases;
  }
  {
   CoreAudioOutput output;output.channel_count_=channels;CoreAudioClientState client(nullptr);client.channels=channels==2?6:2;output.clients_[0]=&client;
   const auto input=Source(256,client.channels);Check(client.ring.Write(input.data(),256,client.channels),"layout mismatch input");
   auto silent=Render(output,256,channels);Check(std::all_of(silent.begin(),silent.end(),[](float s){return s==0;}),"wrong channel-layout data consumed");
   Check(client.ring.available_frames()==256,"wrong channel-layout source retired");output.clients_[0]=nullptr;++cases;
  }
  {
   CoreAudioOutput output;output.channel_count_=channels;
   Render(output,256,channels,128);Check(output.callback_buffer_errors_==1,"small output buffer accepted");++cases;
  }
 }
 // Mix unequal source lengths in both client orders, including an empty
 // first client and circular splits. Verify sample values, silence tails and
 // block retirement with the exact callback, not a rewritten mixing model.
 for(uint32_t channels:{2u,6u})for(uint32_t frames:{1u,64u,256u,512u})
 for(uint32_t a:{0u,1u,13u,255u,256u,512u})
 for(uint32_t b:{0u,1u,13u,255u,256u,512u})for(bool wrap:{false,true}) {
   CoreAudioOutput output;output.channel_count_=channels;
   CoreAudioClientState first(nullptr),second(nullptr);
   first.channels=channels;second.channels=channels;
   auto x=Source(a,channels),y=Source(b,channels);
   for(auto& value:y)value *= -0.5f;
   const auto position=wrap?kCoreAudioRingFrames-19:0u;
   for(auto* client:{&first,&second})if(position) {
     auto unused=Source(position,channels);
     Check(client->ring.Write(unused.data(),position,channels),"unequal-source position write");
     Check(client->ring.Read(unused.data(),position,channels,false)==position,"unequal-source position read");
   }
   if(a)Check(first.ring.Write(x.data(),a,channels),"first unequal source");
   if(b)Check(second.ring.Write(y.data(),b,channels),"second unequal source");
   output.clients_[0]=&first;output.clients_[1]=&second;
   auto actual=Render(output,frames,channels);
   for(uint32_t frame=0;frame<frames;++frame)for(uint32_t channel=0;channel<channels;++channel) {
     const auto index=size_t(frame)*channels+channel;
     const auto expected=std::clamp((frame<a?x[index]:0.0f)+(frame<b?y[index]:0.0f),-1.0f,1.0f);
     Check(actual[index]==expected,"unequal-source callback PCM mismatch");++checked;
   }
   for(const auto& pair: {std::pair{&first,a},std::pair{&second,b}}) {
     const auto used=std::min(frames,pair.second);
     Check(pair.first->ring.available_frames()==pair.second-used,"unequal-source cursor");
     Check(pair.first->retired_blocks_total==uint64_t(position+used)/256-uint64_t(position)/256,
           "unequal-source credit retirement");
     Check(pair.first->underrun_frames==frames-used,"unequal-source underrun count");
   }
   output.clients_[0]=nullptr;output.clients_[1]=nullptr;++cases;
 }
 std::cout<<"PASS actual-CoreAudio-callback cases="<<cases<<" compared_samples="<<checked<<" channels=2,6 frames=1,64,128,256,512,4096 clients=1,2,8 wrap/partial/unequal-lengths/credit-retirement/mute/pause/clamp/nonfinite/invalid-buffer=passed device-opened=false\n";
 return 0;
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
