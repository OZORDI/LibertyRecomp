// Almsot all decoding code is from Xenia Canary, so leave the copyright here
/**
******************************************************************************
* Xenia : Xbox 360 Emulator Research Project                                 *
******************************************************************************
* Copyright 2024 Xenia Canary. All rights reserved.                          *
* Released under the BSD license - see LICENSE in the root for more details. *
******************************************************************************
*/

#include "xma_decoder.h"

// #define ENABLE_DEBUG_XMA_DECODER

#ifdef ENABLE_DEBUG_XMA_DECODER
    #define debug_printf(...) printf(__VA_ARGS__)
#else
    #define debug_printf(...)
#endif

uint32_t XMAPlaybackGetFrameOffsetFromPacketHeader(uint32_t header) {

  uint32_t result = 0;
  if (header != 0x7FFF)
    return ((header >> 11) & 0x7FFF) + 32;
  return result;
}

constexpr uint32_t kOutputBytesPerBlock = 256;
constexpr uint32_t kBitsPerPacketHeader = 32;
constexpr uint32_t kBitsPerPacket = kBytesPerPacket * 8;
constexpr uint32_t kMaxFrameLength = 0x7FFF;
constexpr uint32_t kBitsPerFrameHeader = 15;
constexpr uint32_t kMaxFrameSizeinBits = 0x4000 - kBitsPerPacketHeader;

uint32_t XMAGetOutputBufferBlockCount(XmaPlayback *playback) {
  return (((playback->channelCount * playback->outputBufferSize) << 15) &
          0x7C00000) >>
         22;
}

int16_t GetPacketNumber(size_t size, size_t bit_offset) {
  if (bit_offset < kBitsPerPacketHeader) {
    return -1;
  }

  if (bit_offset >= (size << 3)) {
    return -1;
  }

  size_t byte_offset = bit_offset >> 3;
  size_t packet_number = byte_offset / kBytesPerPacket;

  return (int16_t)packet_number;
}

static uint32_t GetPacketFrameOffset(const uint8_t *packet) {
  uint32_t val = (uint16_t)(((packet[0] & 0x3) << 13) | (packet[1] << 5) |
                            (packet[2] >> 3));
  return val + 32;
}

struct kPacketInfo {
  uint8_t frame_count_;
  uint8_t current_frame_;
  uint32_t current_frame_size_;

  const bool isLastFrameInPacket() const {
    return current_frame_ == frame_count_ - 1;
  }
};

kPacketInfo GetPacketInfo(uint8_t *packet, uint32_t frame_offset) {
  kPacketInfo packet_info = {};

  const uint32_t first_frame_offset = GetPacketFrameOffset(packet);
  BitStream stream(packet, kBitsPerPacket);
  stream.SetOffset(first_frame_offset);

  // Handling of splitted frame
  if (frame_offset < first_frame_offset) {
    packet_info.current_frame_ = 0;
    packet_info.current_frame_size_ = first_frame_offset - frame_offset;
  }

  while (true) {
    if (stream.BitsRemaining() < kBitsPerFrameHeader) {
      break;
    }

    const uint64_t frame_size = stream.Peek(kBitsPerFrameHeader);
    if (frame_size == kMaxFrameLength) {
      break;
    }

    if (stream.offset_bits() == frame_offset) {
      packet_info.current_frame_ = packet_info.frame_count_;
      packet_info.current_frame_size_ = (uint32_t)frame_size;
    }

    packet_info.frame_count_++;

    if (frame_size > stream.BitsRemaining()) {
      // Last frame.
      break;
    }

    stream.Advance(frame_size - 1);

    // Read the trailing bit to see if frames follow
    if (stream.Read(1) == 0) {
      break;
    }
  }

  return packet_info;
}

uint8_t *GetNextPacket(XmaPlayback *playback, uint32_t next_packet_index,
                       uint32_t current_input_packet_count) {
  if (next_packet_index < current_input_packet_count) {
    uint8_t *current_input_buffer =
        (uint8_t *)g_memory.Translate(playback->GetCurrentInputBufferAddress());
    return current_input_buffer + next_packet_index * kBytesPerPacket;
  }

  const uint8_t next_buffer_index = playback->currentBuffer ^ 1;

  if (!playback->IsInputBufferValid(next_buffer_index)) {
    return nullptr;
  }

  const uint32_t next_buffer_address =
      playback->GetInputBufferAddress(next_buffer_index);

  if (!next_buffer_address) {
    // This should never occur but there is always a chance
    debug_printf("XmaContext: Buffer is marked as valid, but doesn't have valid "
           "pointer!\n");
    return nullptr;
  }

  return (uint8_t *)g_memory.Translate(next_buffer_address);
}

uint8_t GetPacketSkipCount(const uint8_t *packet) { return packet[3]; }

uint32_t GetAmountOfBitsToRead(const uint32_t remaining_stream_bits,
                               const uint32_t frame_size) {
  return std::min(remaining_stream_bits, frame_size);
}

template <typename T> T clamp_float(T value, T min_value, T max_value) {
  float clamped_to_min = std::isgreater(value, min_value) ? value : min_value;
  return std::isless(clamped_to_min, max_value) ? clamped_to_min : max_value;
}

const uint32_t GetNextPacketReadOffset(uint8_t *buffer,
                                       uint32_t next_packet_index,
                                       uint32_t current_input_packet_count) {
  if (next_packet_index >= current_input_packet_count) {
    return kBitsPerPacketHeader;
  }

  uint8_t *next_packet = buffer + (next_packet_index * kBytesPerPacket);
  const uint32_t packet_frame_offset = GetPacketFrameOffset(next_packet);

  if (packet_frame_offset > kMaxFrameSizeinBits) {
    const uint32_t offset = GetNextPacketReadOffset(
        buffer, next_packet_index + 1, current_input_packet_count);
    return offset;
  }

  const uint32_t new_input_buffer_offset =
      (next_packet_index * kBitsPerPacket) + packet_frame_offset;

  debug_printf("XmaContext: new offset: %d packet_offset: %d packet: %d/%d\n",
         new_input_buffer_offset, packet_frame_offset, next_packet_index,
         current_input_packet_count);
  return new_input_buffer_offset;
}

void SwapInputBuffer(XmaPlayback *playback) {
  debug_printf("swap buffer\n");
  // No more frames.
  if (playback->currentBuffer == 0) {
    playback->input_buffer_1_valid = 0;
  } else {
    playback->input_buffer_2_valid = 0;
  }
  playback->currentBuffer ^= 1;
  playback->input_buffer_read_offset = kBitsPerPacketHeader;
}

void UpdateLoopStatus(XmaPlayback *playback) {
  if (playback->num_loops == 0) {
    return;
  }

  const uint32_t loop_start =
      std::max(kBitsPerPacketHeader, playback->loop_start_offset);
  const uint32_t loop_end =
      std::max(kBitsPerPacketHeader, playback->loop_end_offset);

  debug_printf("XmaContext: Looped Data: %d < %d (Start: %d) Remaining: %d\n",
         playback->input_buffer_read_offset, playback->loop_end_offset,
         playback->loop_start_offset, playback->num_loops);

  if (playback->input_buffer_read_offset != loop_end) {
    return;
  }

  playback->input_buffer_read_offset = loop_start;

  if (playback->num_loops != 255) {
    playback->num_loops--;
  }
}

void Decode(XmaPlayback *playback) {

  if (!playback->IsAnyInputBufferValid()) {
    return;
  }

  if (playback->current_frame_remaining_subframes_ > 0) {
    return;
  }

  uint8_t *current_input_buffer = playback->GetCurrentInputBuffer();

  playback->input_buffer_.fill(0);

  UpdateLoopStatus(playback); // TODO

  debug_printf("Write Count: %d, Capacity: %d - Subframes: %d\n",
         playback->output_rb.write_count(),
         playback->remaining_subframe_blocks_in_output_buffer_,
         playback->subframes);

  const uint32_t current_input_size =
      playback->GetCurrentInputBufferPacketCount() * kBytesPerPacket;
  debug_printf("input size 1: %d\n", playback->inputBuffer1Size);
  debug_printf("current_input_size: %d\n", current_input_size);
  uint32_t current_input_packet_count = current_input_size / kBytesPerPacket;
  debug_printf("current_input_packet_count: %d\n", current_input_packet_count);
  debug_printf("playback->input_buffer_read_offset: %d\n",
         playback->input_buffer_read_offset);

  int16_t packet_index =
      GetPacketNumber(current_input_size, playback->input_buffer_read_offset);

  uint8_t *packet = current_input_buffer + (packet_index * kBytesPerPacket);

  if (packet_index == -1) {
    debug_printf("Invalid packet index. Input read offset: %d\n",
           playback->input_buffer_read_offset);
    return;
  }

  const uint32_t frame_offset = GetPacketFrameOffset(packet);
  if (playback->input_buffer_read_offset < frame_offset) {
    playback->input_buffer_read_offset = frame_offset;
  }

  uint32_t relative_offset =
      playback->input_buffer_read_offset % kBitsPerPacket;
  const kPacketInfo packet_info = GetPacketInfo(packet, relative_offset);
  const uint32_t packet_to_skip = GetPacketSkipCount(packet) + 1;
  const uint32_t next_packet_index = packet_index + packet_to_skip;

  BitStream stream =
      BitStream(current_input_buffer, (packet_index + 1) * kBitsPerPacket);
  stream.SetOffset(playback->input_buffer_read_offset);

  const uint64_t bits_to_copy = GetAmountOfBitsToRead(
      (uint32_t)stream.BitsRemaining(), packet_info.current_frame_size_);

  if (bits_to_copy == 0) {
    debug_printf("There is no bits to copy!\n");
    SwapInputBuffer(playback);
    return;
  }

  if (packet_info.isLastFrameInPacket()) {
    debug_printf("packet_info.isLastFrameInPacket\n");
    // Frame is a splitted frame
    if (stream.BitsRemaining() < packet_info.current_frame_size_) {
      const uint8_t *next_packet = GetNextPacket(playback, next_packet_index,
                                                 current_input_packet_count);

      if (!next_packet) {
        // Error path
        // Decoder probably should return error here
        // Not sure what error code should be returned
        // data->error_status = 4;
        __builtin_debugtrap();
        return;
      }
      // Copy next packet to buffer
      std::memcpy(playback->input_buffer_.data() + kBytesPerPacketData,
                  next_packet + kBytesPerPacketHeader, kBytesPerPacketData);
    }
  }

  std::memcpy(playback->input_buffer_.data(), packet + kBytesPerPacketHeader,
              kBytesPerPacketData);

  stream = BitStream(playback->input_buffer_.data(),
                     (kBitsPerPacket - kBitsPerPacketHeader) * 2);

  stream.SetOffset(relative_offset - kBitsPerPacketHeader);

  playback->xma_frame_.fill(0);

  debug_printf("Reading Frame %d/%d (size: %d) From Packet %d/%d\n",
         (int32_t)packet_info.current_frame_, packet_info.frame_count_,
         packet_info.current_frame_size_, packet_index,
         current_input_packet_count);

  const uint32_t padding_start = static_cast<uint8_t>(stream.Copy(
      playback->xma_frame_.data() + 1, packet_info.current_frame_size_));

  playback->raw_frame_.fill(0);

  playback->av_packet_->data = playback->xma_frame_.data();
  playback->av_packet_->size = static_cast<int>(
      1 + ((padding_start + packet_info.current_frame_size_) / 8) +
      (((padding_start + packet_info.current_frame_size_) % 8) ? 1 : 0));

  auto padding_end = playback->av_packet_->size * 8 -
                     (8 + padding_start + packet_info.current_frame_size_);
  playback->xma_frame_[0] =
      ((padding_start & 7) << 5) | ((padding_end & 7) << 2);
  auto ret = avcodec_send_packet(playback->codec_ctx, playback->av_packet_);
  if (ret < 0) {
    debug_printf("Error sending packet for decoding: %s\n", av_err2str(ret));
  }
  ret = avcodec_receive_frame(playback->codec_ctx, playback->av_frame_);
  if (ret < 0) {
    debug_printf("Error receiving frame from decoder: %s\n", av_err2str(ret));
  }
  constexpr float scale = (1 << 15) - 1;
  auto out = reinterpret_cast<int16_t *>(playback->raw_frame_.data());
  auto samples = reinterpret_cast<const uint8_t **>(&playback->av_frame_->data);
  debug_printf("decoded samples: %d\n", playback->av_frame_->nb_samples);
  uint32_t o = 0;
  if (playback->av_frame_->nb_samples != 0) {
      for (uint32_t i = 0; i < kSamplesPerFrame; i++) {
          for (uint32_t j = 0; j <= uint32_t(true); j++) {
              // Select the appropriate array based on the current channel.
              auto in = reinterpret_cast<const float *>(samples[j]);

              // Raw samples sometimes aren't within [-1, 1]
              float scaled_sample = clamp_float(in[i], -1.0f, 1.0f) * scale;

              // Convert the sample and output it in big endian.
              auto sample = static_cast<int16_t>(scaled_sample);
              out[o++] = ByteSwap(sample);
          }
      }
  }
  playback->current_frame_remaining_subframes_ = 4 << 1;

  if (!packet_info.isLastFrameInPacket()) {
    const uint32_t next_frame_offset =
        (playback->input_buffer_read_offset + bits_to_copy) % kBitsPerPacket;

    debug_printf("Index: %d/%d - Next frame offset: %d\n",
           (int32_t)packet_info.current_frame_, packet_info.frame_count_,
           next_frame_offset);

    playback->input_buffer_read_offset =
        (packet_index * kBitsPerPacket) + next_frame_offset;
    return;
  }
  const uint8_t *next_packet =
      GetNextPacket(playback, next_packet_index, current_input_packet_count);

  uint32_t next_input_offset = GetNextPacketReadOffset(
      current_input_buffer, next_packet_index, current_input_packet_count);

  if (next_input_offset == kBitsPerPacketHeader) {
    SwapInputBuffer(playback);
    // We're at start of next buffer
    // If it have any frame in this packet decoder should go to first frame in
    // packet If it doesn't have any frame then it should immediatelly go to
    // next packet
    if (playback->IsAnyInputBufferValid()) {
      next_input_offset = GetPacketFrameOffset((uint8_t *)g_memory.Translate(
          playback->GetCurrentInputBufferAddress()));

      if (next_input_offset > kMaxFrameSizeinBits) {
        debug_printf("XmaContext: Next buffer contains no frames in packet! Frame "
               "offset: %d\n",
               next_input_offset);
        SwapInputBuffer(playback);
        return;
      }
      debug_printf("XmaContext: Next buffer first frame starts at: %d\n",
             next_input_offset);
    } else {
      // HACK
      SwapInputBuffer(playback);
    }
  }
  playback->input_buffer_read_offset = next_input_offset;
  return;
}

// FILE *outfile = fopen("output.pcm", "ab");

void Consume(XmaPlayback *playback) {
  debug_printf("playback->current_frame_remaining_subframes_: %d\n",
         playback->current_frame_remaining_subframes_);
  if (!playback->current_frame_remaining_subframes_) {
    return;
  }

  const int8_t subframes_to_write =
      std::min((int8_t)playback->current_frame_remaining_subframes_,
               (int8_t)playback->subframes);

  const int8_t raw_frame_read_offset =
      ((kBytesPerFrameChannel / kOutputBytesPerBlock) << 1) - // is stereo
      playback->current_frame_remaining_subframes_;
  // + data->subframe_skip_count;

  playback->output_rb.Write(playback->raw_frame_.data() +
                                (kOutputBytesPerBlock * raw_frame_read_offset),
                            subframes_to_write * kOutputBytesPerBlock);
  debug_printf("kOutputBytesPerBlock * raw_frame_read_offset %d\n",
         kOutputBytesPerBlock * raw_frame_read_offset);
  debug_printf("subframes_to_write * kOutputBytesPerBlock %d\n",
         subframes_to_write * kOutputBytesPerBlock);
  debug_printf("write_count %d\n", playback->output_rb.write_count());
  playback->remaining_subframe_blocks_in_output_buffer_ -= subframes_to_write;
  playback->current_frame_remaining_subframes_ -= subframes_to_write;

  debug_printf("Consume: %d - %d - %d - %d - %d\n",
         playback->remaining_subframe_blocks_in_output_buffer_,
         playback->output_buffer_write_offset,
         playback->output_buffer_read_offset,
         playback->output_rb.write_offset(),
         playback->current_frame_remaining_subframes_);
}

void DecoderThreadFunc(XmaPlayback *playback) {
  while (playback->isRunning) {
    std::unique_lock<std::mutex> lock(playback->mutex);

    playback->cv.wait(lock, [&] {
      return (!playback->isRunning || (playback->output_buffer_valid == 1 &&
                                       playback->IsAnyInputBufferValid())) &&
             !playback->isLocked.load();
    });

    if (!playback->output_buffer_valid)
      continue;

    // debug_printf("allowed to decode: %d\n", playback->bAllowedToDecode);

    if (!playback->isRunning)
      break;

    lock.unlock();

    uint8_t *current_input_buffer = playback->GetCurrentInputBuffer();

    const int32_t minimum_subframe_decode_count = (playback->subframes * 2) - 1;

    size_t output_capacity =
        playback->output_buffer_block_count * kOutputBytesPerBlock;

    const uint32_t output_read_offset =
        playback->output_buffer_read_offset * kOutputBytesPerBlock;
    const uint32_t output_write_offset =
        playback->output_buffer_write_offset * kOutputBytesPerBlock;

    playback->output_rb = RingBuffer(
        (uint8_t *)g_memory.Translate(playback->outputBuffer), output_capacity);
    playback->output_rb.set_read_offset(output_read_offset);
    playback->output_rb.set_write_offset(output_write_offset);
    playback->remaining_subframe_blocks_in_output_buffer_ =
        (int32_t)playback->output_rb.write_count() / kOutputBytesPerBlock;
    if (minimum_subframe_decode_count >
        playback->remaining_subframe_blocks_in_output_buffer_) {
      debug_printf("No space for subframe decoding %d/%d!\n",
              minimum_subframe_decode_count,
              playback->remaining_subframe_blocks_in_output_buffer_);
      playback->bAllowedToDecode = false;
      lock.lock();
      continue;
    }

    while (playback->remaining_subframe_blocks_in_output_buffer_ >=
           minimum_subframe_decode_count) {
      debug_printf("while: %d >= %d\n",
             playback->remaining_subframe_blocks_in_output_buffer_,
             minimum_subframe_decode_count);
      Decode(playback);
      Consume(playback);

      if (!playback->IsAnyInputBufferValid()) {
        break;
      }
    }

    playback->output_buffer_write_offset =
        playback->output_rb.write_offset() / kOutputBytesPerBlock;
    debug_printf("playback->output_buffer_write_offset: %d\n",
           playback->output_buffer_write_offset);
    playback->bAllowedToDecode = false;

    if (playback->output_rb.empty()) {
      playback->output_buffer_valid = 0;
    }

    lock.lock();
  }
}

uint32_t XMAPlaybackCreate(uint32_t streams, XMAPLAYBACKINIT *init,
                           uint32_t flags, be<uint32_t> *outPlayback) {
  debug_printf("XMAPlaybackCreate: %x %x %x %x\n", streams, init, flags, outPlayback);
  debug_printf("sampleRate: %d\n", init->sampleRate.get());
  debug_printf("outputBufferSize: %x\n", init->outputBufferSize.get());
  debug_printf("channelCount: %x\n", init->channelCount);
  debug_printf("subframes: %x\n", init->subframes);
  const auto xmaPlayback = g_userHeap.AllocPhysical<XmaPlayback>(
      init->sampleRate.get(), init->outputBufferSize.get(), init->channelCount,
      init->subframes);
  xmaPlayback->decoderThread = std::thread(DecoderThreadFunc, xmaPlayback);
  *outPlayback = g_memory.MapVirtual(xmaPlayback);
  return 0;
}

uint32_t XMAPlaybackRequestModifyLock(XmaPlayback *playback) {
  debug_printf("XMAPlaybackRequestModifyLock\n");
  std::lock_guard<std::mutex> lock(playback->mutex);
  playback->isLocked = true;
  return 0;
}

uint32_t XMAPlaybackWaitUntilModifyLockObtained(XmaPlayback *playback) {
  debug_printf("XMAPlaybackWaitUntilModifyLockObtained\n");
  std::unique_lock<std::mutex> lock(playback->mutex);
  playback->cv.wait(lock, [&playback] { return playback->isLocked.load(); });
  return 0;
}

uint32_t XMAPlaybackQueryReadyForMoreData(XmaPlayback *playback,
                                          uint32_t stream) {
  debug_printf("XMAPlaybackQueryReadyForMoreData %x\n", stream);
  return playback->input_buffer_1_valid == 0 ||
         playback->input_buffer_2_valid == 0;
}

uint32_t XMAPlaybackIsIdle(XmaPlayback *playback, uint32_t stream) {
  debug_printf("XMAPlaybackIsIdle %x %d\n", stream,
         playback->input_buffer_1_valid == 0 &&
             playback->input_buffer_2_valid == 0);
  return playback->input_buffer_1_valid == 0 &&
         playback->input_buffer_2_valid == 0;
}

uint32_t XMAPlaybackQueryContextsAllocated(XmaPlayback *playback) {
  debug_printf("XMAPlaybackQueryContextsAllocated %x\n", playback);
  if (!playback)
    return 0;
  return 1;
}

uint32_t XMAPlaybackResumePlayback(XmaPlayback *playback) {
  debug_printf("XMAPlaybackResumePlayback %x\n", playback);
  std::lock_guard<std::mutex> lock(playback->mutex);
  playback->isLocked = false;
  playback->cv.notify_one();
  debug_printf("XMAPlaybackResumePlayback resumed\n");
  return 0;
}

uint32_t XMAPlaybackQueryInputDataPending(XmaPlayback *playback,
                                          uint32_t stream, uint32_t data) {
  debug_printf("XMAPlaybackQueryInputDataPending %x %x\n", playback, data);
  if (playback->input_buffer_1_valid && playback->inputBuffer1 == data) {
    return 1;
  }
  if (playback->input_buffer_2_valid && playback->inputBuffer2 == data) {
    return 1;
  }
  return 0;
}

uint32_t XMAPlaybackGetErrorBits(XmaPlayback *playback, uint32_t stream) {
  debug_printf("XMAPlaybackGetErrorBits %x\n", playback);
  return 0;
}

uint32_t XMAPlaybackSubmitData(XmaPlayback *playback, uint32_t stream,
                               uint32_t data, uint32_t dataSize) {
  debug_printf("XMAPlaybackSubmitData %x %x %x %x\n", playback, stream, data,
         dataSize);
  if (!playback->isLocked)
    return 1;

  std::lock_guard<std::mutex> lock(playback->mutex);
  uint32_t packet_count = dataSize >> 11;

  debug_printf("is valid1: %d\n", playback->input_buffer_1_valid);
  uint32_t valid_buffer =
      playback->input_buffer_1_valid | (playback->input_buffer_2_valid << 1);
  if (playback->input_buffer_1_valid == 1) {
    if (playback->input_buffer_2_valid == 1) {
      return 0x80070005;
    }
    playback->inputBuffer2 = data;
    playback->inputBuffer2Size = packet_count & 0xFFF;

    playback->input_buffer_2_valid = 1;
    debug_printf("input2 written\n");
  } else {
    playback->inputBuffer1 = data;
    playback->inputBuffer1Size = packet_count & 0xFFF;

    playback->input_buffer_1_valid = 1;
    debug_printf("input1 written\n");
  }
  if (!valid_buffer) {
    uint8_t *current_input_buffer = (uint8_t *)g_memory.Translate(data);
    uint32_t frame_offset =
        XMAPlaybackGetFrameOffsetFromPacketHeader(*current_input_buffer);
    if (frame_offset) {
      playback->input_buffer_read_offset = frame_offset & 0x3FFFFFF;
    }
  }

  playback->bAllowedToDecode = true;
  playback->cv.notify_one();
  return 0;
}

uint32_t XMAPlaybackQueryAvailableData(XmaPlayback *playback, uint32_t stream) {
  debug_printf("XMAPlaybackQueryAvailableData %x %x\n", playback, stream);
  if (!playback->isLocked || (playback->input_buffer_1_valid == 0 &&
                              playback->input_buffer_2_valid == 0))
    return 0;

  uint32_t partial_bytes_read = playback->partialBytesRead;
  uint32_t write_buffer_offset_read =
      playback->output_buffer_read_offset & 0x1F;
  uint32_t offset_write = playback->output_buffer_write_offset;
  uint32_t size_write = playback->output_buffer_block_count & 0x1F;

  uint32_t available_bytes = 0;
  uint32_t is_valid_write = playback->output_buffer_valid;
  if (partial_bytes_read) {
    available_bytes = 256 - partial_bytes_read;
    write_buffer_offset_read++;
    is_valid_write = 1;
  }
  uint32_t available_blocks = 0;
  if (offset_write <= write_buffer_offset_read) {
    if (offset_write < write_buffer_offset_read || !is_valid_write) {
      available_blocks = size_write - write_buffer_offset_read;
    }
  } else {
    available_blocks = offset_write - write_buffer_offset_read;
  }
  uint32_t total_bytes = (available_blocks << 8) + available_bytes;
  uint32_t bytes_per_sample = 1 + 1; // TODO: find all channels and refactor
  debug_printf("samples accessed: %d\n", total_bytes >> bytes_per_sample);
  return total_bytes >> bytes_per_sample;
}


uint32_t XMAPlaybackAccessDecodedData(XmaPlayback *playback, uint32_t stream,
                                      uint32_t **data) {
  debug_printf("XMAPlaybackAccessDecodedData %x %x %x\n", playback, stream, data);
  if (!playback->isLocked)
    return 0;
  uint32_t partial_bytes_read = playback->partialBytesRead;
  debug_printf("data: %x %x\n", data, *data);
  uint32_t addr = reinterpret_cast<uint32_t>(
      playback->outputBuffer +
      ((playback->output_buffer_read_offset << 8) & 0x1F00) +
      partial_bytes_read);
  ;
  *data = (uint32_t *)__builtin_bswap32(addr);
  debug_printf("data2: %x %x\n", data, *data);

  uint32_t write_buffer_offset_read =
      playback->output_buffer_read_offset & 0x1F;
  uint32_t offset_write = playback->output_buffer_write_offset;
  uint32_t size_write = playback->output_buffer_block_count & 0x1F;

  uint32_t available_bytes = 0;
  uint32_t is_valid_write = playback->output_buffer_valid;
  if (partial_bytes_read) {
    available_bytes = 256 - partial_bytes_read;
    write_buffer_offset_read++;
    is_valid_write = 1;
  }
  uint32_t available_blocks = 0;
  if (offset_write <= write_buffer_offset_read) {
    if (offset_write < write_buffer_offset_read || !is_valid_write) {
      available_blocks = size_write - write_buffer_offset_read;
    }
  } else {
    available_blocks = offset_write - write_buffer_offset_read;
  }
  uint32_t total_bytes = (available_blocks << 8) + available_bytes;
  uint32_t bytes_per_sample = 1 + 1; // TODO: find all channels and refactor
  debug_printf("samples accessed: %d\n", total_bytes >> bytes_per_sample);
  return total_bytes >> bytes_per_sample;
}

uint32_t XMAPlaybackConsumeDecodedData(XmaPlayback *playback, uint32_t stream,
                                       uint32_t maxSamples, uint32_t **data) {
  debug_printf("XMAPlaybackConsumeDecodedData %x %x %x %x\n", playback, stream,
         maxSamples, data);
  if (!playback->isLocked)
    return 0;

  uint32_t total_bytes = 0;
  uint32_t partial_bytes_read = playback->partialBytesRead;
  uint32_t addr = reinterpret_cast<uint32_t>(
      playback->outputBuffer +
      ((playback->output_buffer_read_offset << 8) & 0x1F00) +
      partial_bytes_read);
  ;
  *data = (uint32_t *)__builtin_bswap32(addr);
  uint32_t bytes_per_sample = 1 + 1;
  uint32_t bytes_desired = maxSamples << bytes_per_sample;
  if (partial_bytes_read) {
    if (bytes_desired < 256 - partial_bytes_read) {
      total_bytes = bytes_desired;
      playback->partialBytesRead += bytes_desired;
      bytes_desired = 0;
    } else {
      total_bytes = 256 - partial_bytes_read;
      bytes_desired -= 256 - partial_bytes_read;
      playback->partialBytesRead = 0;

      uint32_t write_index = (playback->output_buffer_read_offset + 1) & 0x1F;
      if (write_index >= (playback->output_buffer_block_count & 0x1F))
        write_index = 0;
      playback->output_buffer_read_offset = write_index;
      playback->output_buffer_valid = 1;
    }
  }

  uint32_t write_index = playback->output_buffer_read_offset;
  uint32_t blocks_to_process = bytes_desired >> 8;
  uint32_t available_blocks = 0;

  uint32_t write_size = playback->output_buffer_block_count;
  if (write_size <= write_index) {
    if (write_size < write_index || !playback->output_buffer_valid)
      available_blocks =
          (playback->output_buffer_block_count & 0x1F) - write_index;
  } else {
    available_blocks = write_size - write_index;
  }

  if (blocks_to_process) {
    if (blocks_to_process > available_blocks)
      blocks_to_process = available_blocks;

    total_bytes += blocks_to_process << 8;
    available_blocks -= blocks_to_process;
    write_index = (write_index + blocks_to_process) & 0x1F;
    if (write_index >= (playback->output_buffer_block_count & 0x1F))
      write_index = 0;

    playback->output_buffer_read_offset = write_index;
    playback->output_buffer_valid = 1;
  }

  uint32_t remaining_bytes = bytes_desired & 0xFF;
  if (remaining_bytes && available_blocks) {
    total_bytes += remaining_bytes;
    playback->partialBytesRead = remaining_bytes;
  }

  // playback->bAllowedToDecode = true;
  // playback->cv.notify_one();

  uint32_t samples_consumed = total_bytes >> bytes_per_sample;
  playback->streamPosition += samples_consumed;
  debug_printf("samples desired and consumed: %d %d\n", maxSamples, samples_consumed);
  return samples_consumed;
}

uint32_t XMAPlaybackQueryModifyLockObtained(XmaPlayback *playback) {
  debug_printf("XMAPlaybackQueryModifyLockObtained %x\n", playback);
  return playback->isLocked.load();
}

uint32_t XMAPlaybackDestroy(XmaPlayback *playback) {
  debug_printf("XMAPlaybackDestroy %x\n", playback);
  return 0;
}

uint32_t XMAPlaybackFlushData(XmaPlayback *playback) {
  debug_printf("XMAPlaybackFlushData %x\n", playback);
  // __builtin_debugtrap();
  return 0;
}

struct XMAPLAYBACKLOOP {
  be<uint32_t> loopStartOffset;
  be<uint32_t> loopEndOffset;
  uint8_t loopSubframeEnd;
  uint8_t loopSubframeSkip;
  uint8_t numLoops;
  uint8_t reserved;
};

uint32_t XmaPlaybackSetLoop(XmaPlayback *playback, uint32_t streamIndex,
                            XMAPLAYBACKLOOP *loop) {
  debug_printf("XmaPlaybackSetLoop %x\n", playback);
  debug_printf("loopStartOffset: %x\n", loop->loopStartOffset.get());
  debug_printf("loopEndOffset: %x\n", loop->loopEndOffset.get());
  debug_printf("loopSubframeEnd: %x\n", loop->loopSubframeEnd);
  debug_printf("loopSubframeSkip: %x\n", loop->loopSubframeSkip);
  debug_printf("numLoops: %x\n", loop->numLoops);

  playback->num_loops = loop->numLoops;
  playback->loop_subframe_end = loop->loopSubframeEnd;
  playback->loop_subframe_skip = loop->loopSubframeSkip;
  playback->loop_start_offset = loop->loopStartOffset.get() & 0x3FFFFFF;
  playback->loop_end_offset = loop->loopEndOffset.get() & 0x3FFFFFF;
  // __builtin_debugtrap();
  return 0;
}

uint32_t XMAPlaybackGetRemainingLoopCount(XmaPlayback *playback) {
  debug_printf("XMAPlaybackGetRemainingLoopCount %x\n", playback);
  __builtin_debugtrap();
  return 0;
}

uint32_t XMAPlaybackGetStreamPosition(XmaPlayback *playback) {
  debug_printf("XMAPlaybackGetStreamPosition %x\n", playback);
  return playback->streamPosition;
}

uint32_t XMAPlaybackSetDecodePosition(XmaPlayback *playback,
                                      uint32_t streamIndex, uint32_t bitOffset,
                                      uint32_t subframe) {
  debug_printf("XMAPlaybackSetDecodePosition %x %d %d\n", playback, bitOffset,
         subframe);
  playback->input_buffer_read_offset = bitOffset & 0x3FFFFFF;
  playback->num_subframes_to_skip = subframe & 0x7;
  return 0;
}

uint32_t XMAPlaybackRewindDecodePosition(XmaPlayback *playback,
                                         uint32_t stream_index,
                                         uint32_t num_samples) {
  debug_printf("XMAPlaybackRewindDecodePosition %x %d %d\n", playback, stream_index,
         num_samples);
  uint32_t shift = 7 - (1 != 0);
  uint32_t adjusted_samples = num_samples >> shift;
  uint32_t write_size = playback->output_buffer_block_count & 0x1F;

  uint32_t new_offset;
  if (adjusted_samples >= write_size) {
    new_offset = playback->input_buffer_read_offset & 0x3FFFFFF;
    playback->output_buffer_valid = 1;
    playback->output_buffer_write_offset = new_offset >> 27;
    return 0;
  }

  new_offset =
      (playback->output_buffer_write_offset - adjusted_samples + write_size) &
      0x1F;
  if (new_offset >= write_size) {
    new_offset -= write_size;
  }

  playback->output_buffer_valid = 1;
  playback->output_buffer_write_offset = new_offset;

  return 1;
}

uint32_t XMAPlaybackQueryCurrentPosition(XmaPlayback *playback) {
  debug_printf("XMAPlaybackQueryCurrentPosition %x\n", playback);
  __builtin_debugtrap();
  return 0;
}

GUEST_FUNCTION_HOOK(sub_8255C090, XMAPlaybackCreate);
GUEST_FUNCTION_HOOK(sub_8255CC48, XMAPlaybackRequestModifyLock);
GUEST_FUNCTION_HOOK(sub_8255CCC8, XMAPlaybackWaitUntilModifyLockObtained);
GUEST_FUNCTION_HOOK(sub_8255C4D0, XMAPlaybackQueryReadyForMoreData);
GUEST_FUNCTION_HOOK(sub_8255C520, XMAPlaybackIsIdle);
GUEST_FUNCTION_HOOK(sub_8255C388, XMAPlaybackQueryContextsAllocated);
GUEST_FUNCTION_HOOK(sub_8255CF10, XMAPlaybackResumePlayback);
GUEST_FUNCTION_HOOK(sub_8255C470, XMAPlaybackQueryInputDataPending);
GUEST_FUNCTION_HOOK(sub_8255C9A0, XMAPlaybackGetErrorBits);
GUEST_FUNCTION_HOOK(sub_8255C398, XMAPlaybackSubmitData);
GUEST_FUNCTION_HOOK(sub_8255C578, XMAPlaybackQueryAvailableData);
GUEST_FUNCTION_HOOK(sub_8255C7A8, XMAPlaybackAccessDecodedData);
GUEST_FUNCTION_HOOK(sub_8255C5F0, XMAPlaybackConsumeDecodedData);
GUEST_FUNCTION_HOOK(sub_8255CD90, XMAPlaybackQueryModifyLockObtained);
GUEST_FUNCTION_HOOK(sub_8255C8D8, XMAPlaybackFlushData);
GUEST_FUNCTION_HOOK(sub_8255C9D8, XmaPlaybackSetLoop);
GUEST_FUNCTION_HOOK(sub_8255CA50, XMAPlaybackGetRemainingLoopCount);
GUEST_FUNCTION_HOOK(sub_8255CA90, XMAPlaybackGetStreamPosition);
GUEST_FUNCTION_HOOK(sub_8255CB20, XMAPlaybackSetDecodePosition);
GUEST_FUNCTION_HOOK(sub_8255C850, XMAPlaybackRewindDecodePosition);
GUEST_FUNCTION_HOOK(sub_8255CAB0, XMAPlaybackQueryCurrentPosition);
GUEST_FUNCTION_HOOK(sub_8255C2C0, XMAPlaybackDestroy);
