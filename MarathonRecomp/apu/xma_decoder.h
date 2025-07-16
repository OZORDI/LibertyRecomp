#pragma once

#ifndef _WIN32

#include <utils/bit_stream.h>
#include <utils/ring_buffer.h>
#include <kernel/function.h>
#include <kernel/heap.h>
#include <condition_variable>

extern "C" {
    #include <libavcodec/avcodec.h>
}

struct XMAPLAYBACKINIT {
  be<uint32_t> sampleRate;
  be<uint32_t> outputBufferSize;
  uint8_t channelCount;
  uint8_t subframes;
};

constexpr uint32_t kBytesPerPacket = 2048;
constexpr uint32_t kBytesPerPacketHeader = 4;
constexpr uint32_t kBytesPerPacketData = kBytesPerPacket - kBytesPerPacketHeader;
constexpr uint32_t kBytesPerSample = 2;
constexpr uint32_t kSamplesPerFrame = 512;
constexpr uint32_t kBytesPerFrameChannel = kSamplesPerFrame * kBytesPerSample;

struct XmaPlayback {
  uint32_t sampleRate;
  uint32_t outputBufferSize;
  uint32_t channelCount;
  uint32_t subframes;
  uint32_t outputBuffer;
  uint32_t currentDataSize;

  uint32_t partialBytesRead = 0;
  uint32_t streamPosition = 0;

  bool bAllowedToDecode = false;

  // ffmpeg
  AVCodecContext *codec_ctx = nullptr;
  const AVCodec *codec = nullptr;
  AVPacket *av_packet_ = nullptr;
  AVFrame *av_frame_ = nullptr;

  // xenia
  std::array<uint8_t, kBytesPerPacketData * 2> input_buffer_;
  std::array<uint8_t, 1 + 4096> xma_frame_;
  std::array<uint8_t, kBytesPerFrameChannel * 2> raw_frame_;
  uint32_t output_buffer_block_count = 0;
  uint32_t output_buffer_read_offset = 0;
  uint32_t output_buffer_write_offset = 0;
  int32_t remaining_subframe_blocks_in_output_buffer_ = 0;
  uint8_t current_frame_remaining_subframes_ = 0;
  uint32_t input_buffer_read_offset = 32;

  uint32_t num_subframes_to_skip = 0;

  RingBuffer output_rb;
  uint32_t inputBuffer1 = 0;
  uint32_t inputBuffer2 = 0;
  size_t inputBuffer1Size = 0;
  size_t inputBuffer2Size = 0;
  uint32_t valid_input_buffer = 0;
  uint32_t input_buffer_1_valid = 0;
  uint32_t input_buffer_2_valid = 0;
  uint32_t currentBuffer = 0;

  uint32_t output_buffer_valid = 1;

  uint8_t num_loops = 0;
  uint8_t loop_subframe_end = 0;
  uint8_t loop_subframe_skip = 0;
  uint32_t loop_start_offset = 0;
  uint32_t loop_end_offset = 0;

  std::thread decoderThread;
  std::mutex mutex;
  std::condition_variable cv;
  std::atomic<bool> isLocked{false};
  std::atomic<bool> isRunning{true};

  XmaPlayback(uint32_t sampleRate, uint32_t outputBufferSize,
              uint32_t channelCount, uint32_t subframes)
      : sampleRate(sampleRate), outputBufferSize(outputBufferSize),
        channelCount(channelCount), subframes(subframes),
        output_rb(nullptr, 0) {
    output_buffer_block_count =
        (((channelCount * outputBufferSize) << 15) & 0x7C00000) >> 22;
    outputBuffer =
        g_memory.MapVirtual(g_userHeap.AllocPhysical((size_t)0x2000, 0));

    codec = avcodec_find_decoder(AV_CODEC_ID_XMAFRAMES);
    if (!codec) {
      throw std::runtime_error("Decoder not found");
    }

    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
      throw std::runtime_error("Failed to allocate codec context");
    }

    codec_ctx->sample_rate = sampleRate;
    codec_ctx->ch_layout.nb_channels = channelCount;

    av_frame_ = av_frame_alloc();
    if (!av_frame_) {
      throw std::runtime_error("Coulnd't allocate frame");
    }

    if (int err = avcodec_open2(codec_ctx, codec, nullptr); err < 0) {
      throw std::runtime_error("Failed to open codec");
    }
    av_packet_ = av_packet_alloc();
  }

  const uint32_t GetInputBufferAddress(uint8_t buffer_index) const {
    return buffer_index == 0 ? inputBuffer1 : inputBuffer2;
  }

  const uint32_t GetCurrentInputBufferAddress() const {
    return GetInputBufferAddress(currentBuffer);
  }

  const uint32_t GetInputBufferPacketCount(uint8_t buffer_index) const {
    return buffer_index == 0 ? inputBuffer1Size : inputBuffer2Size;
  }
  const uint32_t GetCurrentInputBufferPacketCount() const {
    return GetInputBufferPacketCount(currentBuffer);
  }

  uint8_t *GetCurrentInputBuffer() {
    return (uint8_t *)g_memory.Translate(GetCurrentInputBufferAddress());
  }

  bool IsInputBufferValid(uint8_t buffer_index) const {
    return buffer_index == 0 ? input_buffer_1_valid : input_buffer_2_valid;
  }

  bool IsCurrentInputBufferValid() const {
    return IsInputBufferValid(currentBuffer);
  }

  bool IsAnyInputBufferValid() const {
    return input_buffer_1_valid || input_buffer_2_valid;
  }

  ~XmaPlayback() {
    {
      std::lock_guard<std::mutex> lock(mutex);
      isRunning = false;
      cv.notify_one();
    }

    if (decoderThread.joinable()) {
      decoderThread.join();
    }
  }
};

#endif
