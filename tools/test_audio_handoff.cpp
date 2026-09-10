// Standalone tests: no audio device or game process is used.
#define REXAPU_INFO(...) ((void)0)
#define REXAPU_ERROR(...) ((void)0)
#include "handoff_trace.inc"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>
using namespace rex::audio::handoff;
static void Check(bool value,const char* message) {if(!value)throw std::runtime_error(message);}
int main(int argc,char** argv) {try {
  Check(argc==2,"provide test output directory");
  struct Item {uint64_t value;};
  Queue<Item,64> q;
  for(uint64_t i=0;i<64;++i)Check(q.Push({i}),"fill");
  Check(!q.Push({65}),"full queue must drop");
  for(uint64_t i=0;i<64;++i){Item x;Check(q.Pop(x)&&x.value==i,"FIFO");}
  Item empty;Check(!q.Pop(empty),"empty read");
  constexpr uint64_t producers=4,attempts=4000;
  Queue<Item,256> concurrent;
  std::atomic<uint64_t> accepted{0},done{0};std::vector<std::thread> threads;
  for(uint64_t t=0;t<producers;++t)threads.emplace_back([&,t]{for(uint64_t n=0;n<attempts;++n)if(concurrent.Push({t*attempts+n}))++accepted;++done;});
  std::set<uint64_t> consumed;
  for(;;){Item x;if(concurrent.Pop(x))Check(consumed.insert(x.value).second,"duplicate publication");else if(done==producers)break;else std::this_thread::yield();}
  for(auto& t:threads)t.join();
  Check(consumed.size()==accepted,"lost accepted queue records");
  Check(accepted+concurrent.dropped==producers*attempts,"drop accounting");
  const std::array<float,8> data{0,0,0.25f,-0.25f,1.5f,std::numeric_limits<float>::infinity(),-0.5f,std::numeric_limits<float>::quiet_NaN()};
  auto signal=Inspect(nullptr,data.data(),4,2);
  Check(signal.nonfinite==2&&signal.clipped==1&&signal.peak==1.5f,"signal classification");
  Check(signal.max_step==2.0f,"per-channel sample step");
  Check(!Enabled(),"trace enabled without initialization");
  Record("disabled");
  setenv("REX_AUDIO_HANDOFF_DIR",argv[1],1);
  Initialize();Check(Enabled(),"trace enable");
  const std::array<float,12> original{0,1,2,3,4,5,6,7,8,9,10,11};
  std::array<float,12> guest{};
  for(size_t i=0;i<guest.size();++i){auto bits=std::bit_cast<uint32_t>(original[i]);if constexpr(std::endian::native==std::endian::little)bits=__builtin_bswap32(bits);guest[i]=std::bit_cast<float>(bits);}
  const auto preserved=guest;
  Capture(Stage::Guest,guest.data(),2,6,48000,7,1,0,0,true);
  Record("test",7,{1,2,3},"queue-and-capture");
  Shutdown();Check(!Enabled(),"shutdown did not disable");
  Check(!std::memcmp(preserved.data(),guest.data(),sizeof(guest)),"capture modified source samples");
  std::ifstream f(std::filesystem::path(argv[1])/"guest.f32",std::ios::binary);std::array<float,12> decoded{};f.read(reinterpret_cast<char*>(decoded.data()),sizeof(decoded));
  Check(bool(f),"PCM not drained to disk");
  for(size_t frame=0;frame<2;++frame)for(size_t c=0;c<6;++c)Check(decoded[frame*6+c]==original[c*2+frame],"planar big-endian capture mismatch");
  std::cout<<"PASS queue FIFO/full/empty, concurrent publication, explicit drops, signal metrics, disabled state, PCM layout, unchanged input and shutdown drain\n";
  return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
