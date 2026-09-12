#pragma once

#include <cerrno>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/error.h"
}

namespace rex::audio {

// Called under the owning XmaContext lock, never from the CoreAudio callback.
// The bundled raw-frame XMA codec has no complete flush callback. An explicit
// guest context clear/release therefore requires a fresh codec, even when the
// next voice uses the same format. Buffer refills, pause/resume and loop wraps
// are stream continuations and must not set reset_pending.
//
// Returns 1 for a new decoder, 0 for continuation, or a negative FFmpeg error.
// A failed replacement keeps reset_pending set: the old stream cannot be used
// as a fallback. Construct before replacing so partial init cannot leak state.
inline int PrepareXmaDecoderForStream(AVCodec* codec, AVCodecContext*& current,
                                     AVFrame* frame, bool& reset_pending,
                                     int sample_rate, int channels) {
  if (!codec || !frame || sample_rate <= 0 || channels < 1 || channels > 2) {
    reset_pending = true;
    return AVERROR(EINVAL);
  }
  if (!reset_pending && current && avcodec_is_open(current) &&
      current->sample_rate == sample_rate && current->channels == channels) {
    return 0;
  }

  reset_pending = true;
  AVCodecContext* replacement = avcodec_alloc_context3(codec);
  if (!replacement) return AVERROR(ENOMEM);
  replacement->sample_rate = sample_rate;
  replacement->channels = channels;
  const int result = avcodec_open2(replacement, codec, nullptr);
  if (result < 0) {
    avcodec_free_context(&replacement);
    return result;
  }

  av_frame_unref(frame);
  avcodec_free_context(&current);
  current = replacement;
  reset_pending = false;
  return 1;
}

}  // namespace rex::audio
