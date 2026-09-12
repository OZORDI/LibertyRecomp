// Test-only access to the bundled decoder's overlap storage. This translation
// unit replaces wmaprodec.o in the standalone regression executable, not the app.
// The valid silent bitstream and retained overlap are synthetic test inputs.
#include "../glue/rexglue-sdk-main/thirdparty/FFmpeg/libavcodec/wmaprodec.c"

void xma_reset_test_seed_overlap(AVCodecContext* codec) {
    WMAProDecodeCtx* s = codec->priv_data;
    for (int c = 0; c < s->nb_channels; ++c) {
        for (int i = 0; i < s->samples_per_frame / 2; ++i) {
            s->channel[c].out[i] = (float)((i % 17) - 8) * 0.04f;
        }
        s->channel[c].prev_block_len = s->samples_per_frame;
    }
    s->frame_num = 422;
    codec->frame_number = 422;
}

int xma_reset_test_silent_packet(AVCodecContext* codec, uint8_t* bytes, int capacity) {
    WMAProDecodeCtx* s = codec->priv_data;
    PutBitContext bits;
    if (capacity < 128 || s->nb_channels < 1 || s->nb_channels > 2) return -1;
    memset(bytes, 0, capacity);
    init_put_bits(&bits, bytes + 1, capacity - 1);
    put_bits(&bits, s->log2_frame_size, 0); // patched below
    if (s->max_num_subframes != 1) put_bits(&bits, 1, 1); // shared layout
    if (s->max_subframe_len_bit) put_bits(&bits, 1, 0); // full frame
    else put_bits(&bits, s->subframe_len_bits, 0);
    if (s->nb_channels > 1) put_bits(&bits, 1, 0); // no postproc transform
    if (s->dynamic_range_compression) put_bits(&bits, 8, 0);
    put_bits(&bits, 1, 0); // no start/end skip fields
    put_bits(&bits, 1, 0); // no extended subframe header
    put_bits(&bits, 1, 0); // reserved bit
    if (s->nb_channels > 1) {
        put_bits(&bits, 1, 0); // transform reserved bit
        put_bits(&bits, 1, 1); // no decorrelation
        put_bits(&bits, 1, 0);
    }
    for (int c = 0; c < s->nb_channels; ++c) put_bits(&bits, 1, 0); // no coefficients
    put_bits(&bits, 1, 1); // termination bit
    put_bits(&bits, 1, 0); // no following frame
    const int frame_bits = put_bits_count(&bits);
    flush_put_bits(&bits);
    // Patch the frame length without clearing adjacent bits.
    for (int i = 0; i < s->log2_frame_size; ++i) {
        const int bit = (frame_bits >> (s->log2_frame_size - i - 1)) & 1;
        const int mask = 1 << (7 - (i & 7));
        bytes[1 + (i >> 3)] = (bytes[1 + (i >> 3)] & ~mask) | (bit ? mask : 0);
    }
    const int padding = (8 - (frame_bits & 7)) & 7;
    bytes[0] = padding << 2;
    return 1 + ((frame_bits + 7) >> 3);
}
