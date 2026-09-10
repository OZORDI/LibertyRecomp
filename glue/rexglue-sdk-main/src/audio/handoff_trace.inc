// Included once by audio_system.cpp. Standalone queue tests use the public header.
#include <rex/audio/handoff_trace.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>

namespace rex::audio::handoff {
std::atomic<bool> enabled{false};
namespace {
struct Event {
  const char* point=nullptr;
  uint64_t tick=0,sequence=0,epoch=0,thread=0,id=0;
  std::array<uint64_t,16> values{};
  std::array<char,80> text{};
  Signal signal{};
};
struct Pcm {
  uint64_t tick=0,epoch=0,sequence=0,id=0,serial=0,position=0,auxiliary=0;
  uint32_t frames=0,channels=0,rate=0;
  std::array<float,256*6> samples{};
};
struct State {
  Queue<Event,32768> events;
  std::array<Queue<Pcm,512>,5> pcm;
  std::atomic<uint64_t> sequence{1},epoch{0},pcm_attempts{0},pcm_invalid{0};
  std::atomic<bool> stop{false};
  std::thread writer;
  std::filesystem::path directory;
  uint64_t start=0,deadline=0,pcm_budget=0;
};
// Storage remains alive for process lifetime so a callback already in flight
// cannot touch freed diagnostic queues when tracing is disabled or finalized.
State* state=nullptr;
std::mutex lifetime_mutex;
const char* StageName(size_t i) {
  constexpr const char* names[]{"guest","converted","mix","output","decoded"};
  return i<5?names[i]:"invalid";
}
uint64_t Setting(const char* name,uint64_t fallback,uint64_t minimum,uint64_t maximum) {
  const char* p=std::getenv(name);if(!p||!*p||*p=='-')return fallback;
  char* end=nullptr;const auto n=std::strtoull(p,&end,10);
  return end&&!*end&&n>=minimum&&n<=maximum?n:fallback;
}
uint64_t ThreadId() {
  static thread_local const uint64_t id=std::hash<std::thread::id>{}(std::this_thread::get_id());
  return id;
}
std::string SafeText(const char* text) {
  std::string result;
  if(text) for(size_t i=0;i<80&&text[i];++i) {
    const unsigned char c=text[i];result.push_back(c>=32&&c<127&&c!=','&&c!='"'?char(c):'_');
  }
  return result;
}
void Writer(State* s) {
  std::ofstream events(s->directory/"events.csv"),pcm_index(s->directory/"pcm.csv"),visible(s->directory/"audio-handoff.log");
  std::array<std::ofstream,5> samples;
  for(size_t i=0;i<5;++i)samples[i].open(s->directory/(std::string(StageName(i))+".f32"),std::ios::binary);
  if(!events||!pcm_index||!visible||std::any_of(samples.begin(),samples.end(),[](auto& f){return !f;})) {
    enabled.store(false,std::memory_order_release);
    REXAPU_ERROR("AudioHandoff: failed to create trace output directory={}",s->directory.string());return;
  }
  events<<"sequence,time_ns,epoch,thread,point,id,text";
  for(size_t i=0;i<16;++i)events<<",v"<<i;
  events<<",peak,rms,max_step,first,last,clipped,nonfinite\n";
  pcm_index<<"sequence,time_ns,epoch,stage,id,serial,position,auxiliary,frames,channels,rate,byte_offset,byte_size,hash,peak,rms,max_step,boundary_step,clipped,nonfinite,first,last\n";
  events<<std::setprecision(9);pcm_index<<std::setprecision(9);
  auto log=[&](const std::string& message) {visible<<message<<'\n';};
  log("AudioHandoff: armed observation-only=true timestamps=steady-nanoseconds PCM=float32-native-endian callbacks=bounded-queue writes=writer-thread");
  REXAPU_INFO("AudioHandoff: armed output={} decoder,mixer,CoreAudio,loading,cutscene=true",s->directory.string());
  uint64_t event_count=0,pcm_count=0,stored_bytes=0,budget_drops=0,next_flush=s->start;
  uint64_t next_signal_log=0,next_slow_log=0,signal_count=0,deadline_noted=0;
  std::array<uint64_t,5> offsets{},last_serial{};
  std::array<std::array<float,6>,5> tails{};
  std::array<bool,5> have_tail{};
  Event e;Pcm p;unsigned idle=0;
  for(;;) {
    bool work=false;
    for(unsigned i=0;i<8192 && s->events.Pop(e);++i) {
      work=true;++event_count;
      events<<e.sequence<<','<<(e.tick-s->start)<<','<<e.epoch<<','<<e.thread<<','<<e.point<<','<<e.id<<','<<SafeText(e.text.data());
      for(auto v:e.values)events<<','<<v;
      const auto& q=e.signal;
      events<<','<<q.peak<<','<<q.rms<<','<<q.max_step<<','<<q.first<<','<<q.last<<','<<q.clipped<<','<<q.nonfinite<<'\n';
      const std::string_view point=e.point,tag=e.text.data();
      const bool boundary=point=="loading"||point=="cutscene"||point=="world"||point=="device"||point=="underrun"||point=="rebuffer"||point=="preroll-start"||point=="xma-error";
      const bool lifecycle=point=="xma-allocate"||point=="xma-clear"||point=="xma-release"||point=="xma-reopen"||point=="voice-lock";
      const bool slow=point=="span-end" && e.values[1]>=5000000;
      const bool signal=point=="xma-frame"&&(q.nonfinite||q.clipped||q.max_step>1.5f);
      if(boundary||lifecycle||(slow&&e.tick>=next_slow_log)||(signal&&e.tick>=next_signal_log)) {
        std::ostringstream line;line<<"AudioHandoff: point="<<point<<" time_ns="<<(e.tick-s->start)<<" epoch="<<e.epoch<<" id="<<e.id<<" seq="<<e.sequence<<" thread="<<e.thread<<" text="<<SafeText(e.text.data());
        for(size_t i=0;i<16;++i)if(e.values[i])line<<" v"<<i<<'='<<e.values[i];
        if(signal)line<<" peak="<<q.peak<<" rms="<<q.rms<<" clipped="<<q.clipped<<" nonfinite="<<q.nonfinite;
        log(line.str());
        if(boundary || (slow&&e.tick>=next_slow_log) || signal) REXAPU_INFO("{}",line.str());
        if(slow)next_slow_log=e.tick+250000000;
        if(signal){++signal_count;next_signal_log=e.tick+250000000;}
      }
    }
    for(size_t stage=0;stage<s->pcm.size();++stage) {
      for(unsigned i=0;i<512&&s->pcm[stage].Pop(p);++i) {
        work=true;++pcm_count;
        const size_t count=size_t(p.frames)*p.channels,bytes=count*sizeof(float);
        const auto q=Inspect(nullptr,p.samples.data(),p.frames,p.channels);
        uint64_t hash=1469598103934665603ull;
        for(size_t i=0;i<count;++i){hash^=std::bit_cast<uint32_t>(p.samples[i]);hash*=1099511628211ull;}
        float boundary_step=0;
        if(stage!=size_t(Stage::Decoded)&&have_tail[stage])for(uint32_t c=0;c<p.channels;++c)
          if(std::isfinite(p.samples[c])&&std::isfinite(tails[stage][c]))boundary_step=std::max(boundary_step,std::abs(p.samples[c]-tails[stage][c]));
        for(uint32_t c=0;c<p.channels;++c)tails[stage][c]=p.samples[count-p.channels+c];
        have_tail[stage]=true;last_serial[stage]=p.serial;
        const bool store=stored_bytes+bytes<=s->pcm_budget;
        const uint64_t offset=store?offsets[stage]:UINT64_MAX;
        if(store){samples[stage].write(reinterpret_cast<const char*>(p.samples.data()),bytes);offsets[stage]+=bytes;stored_bytes+=bytes;}
        else if(++budget_drops==1)log("AudioHandoff: point=pcm-budget-limit PCM data unavailable beyond this point; metrics continue");
        pcm_index<<p.sequence<<','<<(p.tick-s->start)<<','<<p.epoch<<','<<StageName(stage)<<','<<p.id<<','<<p.serial<<','<<p.position<<','<<p.auxiliary<<','<<p.frames<<','<<p.channels<<','<<p.rate<<','<<offset<<','<<(store?bytes:0)<<','<<hash<<','<<q.peak<<','<<q.rms<<','<<q.max_step<<','<<boundary_step<<','<<q.clipped<<','<<q.nonfinite<<','<<q.first<<','<<q.last<<'\n';
        if((q.nonfinite||q.clipped||q.max_step>1.5f||boundary_step>1.5f)&&p.tick>=next_signal_log) {
          std::ostringstream line;line<<"AudioHandoff: point=signal stage="<<StageName(stage)<<" time_ns="<<(p.tick-s->start)<<" seq="<<p.sequence<<" epoch="<<p.epoch<<" id="<<p.id<<" serial="<<p.serial<<" peak="<<q.peak<<" rms="<<q.rms<<" max_step="<<q.max_step<<" boundary_step="<<boundary_step<<" clipped="<<q.clipped<<" nonfinite="<<q.nonfinite<<" byte_offset="<<offset;
          log(line.str());REXAPU_INFO("{}",line.str());++signal_count;next_signal_log=p.tick+250000000;
        }
      }
    }
    const uint64_t now=Clock();
    if(now>=next_flush) {
      uint64_t pcm_dropped=0;for(auto& q:s->pcm)pcm_dropped+=q.dropped.load();
      std::ostringstream status;status<<"AudioHandoff: point=heartbeat elapsed_ns="<<(now-s->start)<<" epoch="<<s->epoch.load()<<" events="<<event_count<<" pcm_blocks="<<pcm_count<<" pcm_bytes="<<stored_bytes<<" event_drops="<<s->events.dropped.load()<<" pcm_drops="<<pcm_dropped<<" pcm_invalid="<<s->pcm_invalid.load()<<" budget_drops="<<budget_drops;
      log(status.str());
      events.flush();pcm_index.flush();visible.flush();for(auto& f:samples)f.flush();
      std::ofstream receipt(s->directory/"status.json");
      receipt<<"{\"elapsed_ns\":"<<(now-s->start)<<",\"epoch\":"<<s->epoch.load()<<",\"events\":"<<event_count<<",\"pcm_blocks\":"<<pcm_count<<",\"pcm_bytes\":"<<stored_bytes<<",\"event_drops\":"<<s->events.dropped.load()<<",\"pcm_drops\":"<<pcm_dropped<<",\"pcm_invalid\":"<<s->pcm_invalid.load()<<",\"budget_drops\":"<<budget_drops<<",\"enabled\":"<<(Enabled()?"true":"false")<<"}\n";
      next_flush=now+1000000000;
      std::error_code ec;auto space=std::filesystem::space(s->directory,ec);
      if(!events||!pcm_index||!visible||std::any_of(samples.begin(),samples.end(),[](auto& f){return !f;})||ec||space.available<256ull*1024*1024) {
        log("AudioHandoff: point=incomplete reason=io-or-disk-headroom");enabled.store(false);s->stop.store(true);
      }
    }
    if(now>=s->deadline&&!deadline_noted++) {log("AudioHandoff: point=trace-duration-complete game-remains-running=true");enabled.store(false);s->stop.store(true);}
    if(s->stop.load()&&!work) {if(++idle>=20)break;}else idle=0;
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
  log("AudioHandoff: point=writer-stopped");
}
} // namespace
Signal Inspect(const float* const* planar,const float* interleaved,uint32_t frames,uint32_t channels) noexcept {
  Signal r;if(!frames||!channels||channels>6||(!planar&&!interleaved))return r;
  double sum=0;std::array<float,6> previous{};
  for(uint32_t f=0;f<frames;++f)for(uint32_t c=0;c<channels;++c) {
    const float v=planar?planar[c][f]:interleaved[size_t(f)*channels+c];
    if(!f&&!c)r.first=v;r.last=v;
    if(!std::isfinite(v)){++r.nonfinite;previous[c]=v;continue;}
    const float a=std::abs(v);r.peak=std::max(r.peak,a);r.clipped+=a>1.0f;sum+=double(v)*v;
    if(f&&std::isfinite(previous[c]))r.max_step=std::max(r.max_step,std::abs(v-previous[c]));
    previous[c]=v;
  }
  r.rms=float(std::sqrt(sum/(uint64_t(frames)*channels)));return r;
}
uint64_t Record(const char* point,uint64_t id,std::initializer_list<uint64_t> values,const char* text,const Signal* signal) noexcept {
  if(!Enabled())return 0;
  auto* s=state;Event e;e.point=point;e.id=id;e.tick=Clock();e.epoch=s->epoch.load(std::memory_order_relaxed);e.thread=ThreadId();e.sequence=s->sequence.fetch_add(1,std::memory_order_relaxed);
  std::copy_n(values.begin(),std::min(values.size(),e.values.size()),e.values.begin());
  if(text)for(size_t i=0;i+1<e.text.size()&&text[i];++i)e.text[i]=text[i];
  if(signal)e.signal=*signal;
  s->events.Push(e);return e.sequence;
}
void Loading(uint32_t pc,uint32_t lr,bool active,uint32_t index,uint64_t flags) noexcept {
  if(!Enabled())return;
  static std::atomic<uint64_t> previous{UINT64_MAX};
  const uint64_t value=uint64_t(active)|(uint64_t(index)<<1)|(flags<<33);
  const auto before=previous.exchange(value,std::memory_order_relaxed);
  if(value==before)return;
  if(active&&(before==UINT64_MAX||!(before&1)))state->epoch.fetch_add(1,std::memory_order_relaxed);
  Record("loading",pc,{lr,active,index,flags,before});
}
void Capture(Stage stage,const float* interleaved,uint32_t frames,uint32_t channels,uint32_t rate,
             uint64_t id,uint64_t serial,uint64_t position,uint64_t auxiliary,bool planar_big_endian,const float* const* planar) noexcept {
  if(!Enabled())return;
  auto* s=state;
  if(size_t(stage)>=5||!frames||channels<1||channels>6||(!interleaved&&!planar)) {s->pcm_invalid.fetch_add(1);return;}
  const auto tick=Clock();
  for(uint32_t offset=0;offset<frames;offset+=256) {
    Pcm p;p.tick=tick;p.epoch=s->epoch.load(std::memory_order_relaxed);p.sequence=s->sequence.fetch_add(1,std::memory_order_relaxed);p.id=id;p.serial=serial;p.position=position+offset;p.auxiliary=auxiliary;p.frames=std::min(256u,frames-offset);p.channels=channels;p.rate=rate;
    if(planar)for(uint32_t f=0;f<p.frames;++f)for(uint32_t c=0;c<channels;++c)p.samples[size_t(f)*channels+c]=planar[c][offset+f];
    else if(planar_big_endian)for(uint32_t f=0;f<p.frames;++f)for(uint32_t c=0;c<channels;++c) {
      uint32_t bits;std::memcpy(&bits,interleaved+size_t(c)*frames+offset+f,sizeof(bits));
      if constexpr(std::endian::native==std::endian::little)bits=__builtin_bswap32(bits);
      p.samples[size_t(f)*channels+c]=std::bit_cast<float>(bits);
    } else std::memcpy(p.samples.data(),interleaved+size_t(offset)*channels,size_t(p.frames)*channels*sizeof(float));
    s->pcm[size_t(stage)].Push(p);s->pcm_attempts.fetch_add(1,std::memory_order_relaxed);
  }
}
void Initialize() {
  std::lock_guard lock(lifetime_mutex);
  if(state)return;
  const auto* path=std::getenv("REX_AUDIO_HANDOFF_DIR");if(!path||!*path)return;
  try {
    auto owner=std::make_unique<State>();auto* s=owner.get();s->directory=path;
    std::filesystem::create_directories(s->directory);
    s->start=Clock();s->deadline=s->start+Setting("REX_AUDIO_HANDOFF_SECONDS",600,60,1800)*1000000000ull;
    s->pcm_budget=Setting("REX_AUDIO_HANDOFF_PCM_MIB",1536,64,4096)*1024*1024;
    std::ofstream info(s->directory/"clock.json");info<<"{\"clock\":\"steady_clock_nanoseconds\",\"start_tick\":"<<s->start<<",\"deadline_tick\":"<<s->deadline<<",\"pcm_budget\":"<<s->pcm_budget<<"}\n";info.close();
    state=owner.release();enabled.store(true,std::memory_order_release);s->writer=std::thread(Writer,s);
  } catch(const std::exception& e) {enabled.store(false);REXAPU_ERROR("AudioHandoff: initialization failed: {}",e.what());}
}
void Shutdown() {
  std::lock_guard lock(lifetime_mutex);enabled.store(false,std::memory_order_release);
  if(state){state->stop.store(true);if(state->writer.joinable())state->writer.join();}
}
} // namespace rex::audio::handoff
