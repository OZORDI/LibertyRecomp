#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "xma_decoder_lifecycle.h"

extern "C" {
void xma_reset_test_seed_overlap(AVCodecContext*);
int xma_reset_test_silent_packet(AVCodecContext*, uint8_t*, int);
}
using rex::audio::PrepareXmaDecoderForStream;
static void Check(bool condition, const char* message) {
  if (!condition) throw std::runtime_error(message);
}
struct Decoder {
  AVCodec* codec = avcodec_find_decoder(AV_CODEC_ID_XMAFRAMES);
  AVCodecContext* context = nullptr;
  AVFrame* frame = av_frame_alloc();
  bool pending = true;
  ~Decoder() { av_frame_free(&frame); avcodec_free_context(&context); }
  int Prepare(int rate, int channels) {
    return PrepareXmaDecoderForStream(codec, context, frame, pending, rate, channels);
  }
  std::vector<float> Silent() {
    AVPacket* packet = av_packet_alloc();
    Check(packet != nullptr, "packet allocation failed");
    Check(av_new_packet(packet, 128) == 0, "packet storage failed");
    packet->size = xma_reset_test_silent_packet(context, packet->data, 128);
    Check(packet->size > 0, "synthetic packet invalid");
    int result = avcodec_send_packet(context, packet);
    av_packet_free(&packet);
    Check(result == 0, "codec rejected synthetic silent frame");
    result = avcodec_receive_frame(context, frame);
    Check(result == 0 && frame->nb_samples == 512, "silent frame decode failed");
    std::vector<float> samples;
    for (int c = 0; c < context->channels; ++c) {
      const auto* data = reinterpret_cast<const float*>(frame->extended_data[c]);
      samples.insert(samples.end(), data, data + frame->nb_samples);
    }
    return samples;
  }
};
static float Peak(const std::vector<float>& data) {
  float peak = 0;
  for (float sample : data) {
    Check(std::isfinite(sample), "non-finite output");
    peak = std::max(peak, std::abs(sample));
  }
  return peak;
}
int main() {
  try {
    for (int rate : {24000, 32000, 44100, 48000}) for (int channels : {1, 2}) {
      Decoder fresh, reset, old;
      Check(fresh.Prepare(rate, channels) == 1, "fresh prepare failed");
      Check(reset.Prepare(rate, channels) == 1, "reset prepare failed");
      Check(old.Prepare(rate, channels) == 1, "control prepare failed");
      const auto reference = fresh.Silent();
      Check(Peak(reference) == 0, "fresh silent source not silent");
      xma_reset_test_seed_overlap(old.context);
      const auto old_pointer = old.context;
      Check(old.Prepare(rate, channels) == 0 && old.context == old_pointer,
            "continuation unexpectedly discarded history");
      auto dirty = old.Silent();
      Check(Peak(dirty) > 0.01f, "old-lifecycle control did not reproduce residue");
      xma_reset_test_seed_overlap(old.context);
      avcodec_flush_buffers(old.context);
      auto flushed = old.Silent();
      Check(Peak(flushed) > 0.01f, "bundled flush behavior changed; revisit reset strategy");
      for (int iteration = 0; iteration < 64; ++iteration) {
        xma_reset_test_seed_overlap(reset.context);
        reset.pending = true;
        Check(reset.Prepare(rate, channels) == 1 && !reset.pending,
              "same-format reset did not recreate codec");
        Check(reset.context->frame_number == 0, "codec frame history survived reset");
        Check(reset.frame->nb_samples == 0, "old decoded frame survived reset");
        const auto result = reset.Silent();
        Check(result.size() == reference.size() &&
              !std::memcmp(result.data(), reference.data(), result.size() * sizeof(float)),
              "reset output differs from a fresh decoder");
      }
      auto* current = reset.context;
      Check(reset.Prepare(rate, channels) == 0 && reset.context == current,
            "ordinary packet continuation reopens codec");
      Check(PrepareXmaDecoderForStream(nullptr, reset.context, reset.frame,
                                      reset.pending, rate, channels) < 0 && reset.pending,
            "invalid codec did not leave reset pending");
      Check(reset.context == current, "failed replacement corrupted owner");
      Check(reset.Prepare(rate, channels) == 1, "retry after rejected setup failed");
      Check(reset.Prepare(rate, 3) < 0 && reset.pending, "invalid channels accepted");
      Check(reset.Prepare(rate, channels) == 1, "retry after invalid channels failed");
      const int alternate_rate = rate == 32000 ? 48000 : 32000;
      Check(reset.Prepare(alternate_rate, channels) == 1, "rate change not reopened");
      Check(reset.Prepare(alternate_rate, channels == 1 ? 2 : 1) == 1,
            "channel change not reopened");
      std::cout << "PASS rate=" << rate << " channels=" << channels
                << " old_peak=" << Peak(dirty) << " flush_only_peak=" << Peak(flushed)
                << " fresh_peak=" << Peak(reference)
                << " reset_peak=0 repeated_resets=64 continuation=preserved errors=checked\n";
    }
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "FAIL " << error.what() << '\n'; return 1;
  }
}
