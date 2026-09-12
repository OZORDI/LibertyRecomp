// Generates valid synthetic mono/stereo raw XMA frames for transport tests.
// Uses the test-only fixture implementation, never linked into the application.
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
extern "C" {
#include <libavcodec/avcodec.h>
int xma_reset_test_silent_packet(AVCodecContext*, uint8_t*, int);
}
int main(int argc,char**argv) {
  if(argc!=2) return 2;
  const std::filesystem::path output(argv[1]);
  std::filesystem::create_directories(output);
  for(int channels:{1,2}) {
    auto* codec=avcodec_find_decoder(AV_CODEC_ID_XMAFRAMES);
    auto* context=avcodec_alloc_context3(codec);
    if(!context) return 3;
    context->sample_rate=48000;context->channels=channels;
    if(avcodec_open2(context,codec,nullptr)<0) {avcodec_free_context(&context);return 4;}
    uint8_t bytes[128]{};
    const int size=xma_reset_test_silent_packet(context,bytes,sizeof(bytes));
    if(size<=0){avcodec_free_context(&context);return 5;}
    std::ofstream file(output/("silent-"+std::to_string(channels)+".raw"),std::ios::binary);
    file.write(reinterpret_cast<const char*>(bytes),size);file.close();
    avcodec_free_context(&context);
    if(!file)return 6;
    std::cout<<"generated channels="<<channels<<" bytes="<<size<<'\n';
  }
  return 0;
}
