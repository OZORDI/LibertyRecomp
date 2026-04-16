/**
******************************************************************************
* Xenia : Xbox 360 Emulator Research Project                                 *
******************************************************************************
* Copyright 2022 Ben Vanik. All rights reserved.                             *
* Released under the BSD license - see LICENSE in the root for more details. *
******************************************************************************
*
* @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
*/

// Disable warnings about unused parameters for kernel functions
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <algorithm>

#include <rex/kernel/xboxkrnl/private.h>
#include <rex/logging.h>
#include <rex/platform.h>
#include <rex/hook.h>
#include <rex/types.h>
#include <rex/system/kernel_state.h>
#include <rex/system/xtypes.h>

#include <cstdint>
#include <cstring>
#include <vector>

#include "crypto/TinySHA1.hpp"
#include "crypto/des/des.cpp"
#include "crypto/des/des.h"
#include "crypto/des/des3.h"
#include "crypto/des/descbc.h"
#include "crypto/sha256.cpp"
#include "crypto/sha256.h"

extern "C" {
#include "aes_128/aes.h"
}

namespace rex::kernel::xboxkrnl {

typedef struct {
  uint8_t S[256];  // 0x0
  uint8_t i;       // 0x100
  uint8_t j;       // 0x101
} XECRYPT_RC4_STATE;
static_assert_size(XECRYPT_RC4_STATE, 0x102);

void XeCryptRc4Key_entry(ppc_ptr_t<XECRYPT_RC4_STATE> rc4_ctx, mapped_void key, u32 key_size) {
  // Setup RC4 state
  rc4_ctx->i = rc4_ctx->j = 0;
  for (uint32_t x = 0; x < 0x100; x++) {
    rc4_ctx->S[x] = (uint8_t)x;
  }

  uint32_t idx = 0;
  for (uint32_t x = 0; x < 0x100; x++) {
    idx = (idx + rc4_ctx->S[x] + key[x % 0x10]) % 0x100;
    uint8_t temp = rc4_ctx->S[idx];
    rc4_ctx->S[idx] = rc4_ctx->S[x];
    rc4_ctx->S[x] = temp;
  }
}

void XeCryptRc4Ecb_entry(ppc_ptr_t<XECRYPT_RC4_STATE> rc4_ctx, mapped_void data, u32 size) {
  // Crypt data
  for (uint32_t idx = 0; idx < size; idx++) {
    rc4_ctx->i = (rc4_ctx->i + 1) % 0x100;
    rc4_ctx->j = (rc4_ctx->j + rc4_ctx->S[rc4_ctx->i]) % 0x100;
    uint8_t temp = rc4_ctx->S[rc4_ctx->i];
    rc4_ctx->S[rc4_ctx->i] = rc4_ctx->S[rc4_ctx->j];
    rc4_ctx->S[rc4_ctx->j] = temp;

    uint8_t a = data[idx];
    uint8_t b = rc4_ctx->S[(rc4_ctx->S[rc4_ctx->i] + rc4_ctx->S[rc4_ctx->j]) % 0x100];
    data[idx] = (uint8_t)(a ^ b);
  }
}

void XeCryptRc4_entry(mapped_void key, u32 key_size, mapped_void data, u32 size) {
  XECRYPT_RC4_STATE rc4_ctx;
  XeCryptRc4Key_entry(ppc_ptr_t<XECRYPT_RC4_STATE>::from_host(&rc4_ctx), key, key_size);
  XeCryptRc4Ecb_entry(ppc_ptr_t<XECRYPT_RC4_STATE>::from_host(&rc4_ctx), data, size);
}

typedef struct {
  rex::be<uint32_t> count;     // 0x0
  rex::be<uint32_t> state[5];  // 0x4
  uint8_t buffer[64];          // 0x18
} XECRYPT_SHA_STATE;
static_assert_size(XECRYPT_SHA_STATE, 0x58);

void InitSha1(sha1::SHA1* sha, const XECRYPT_SHA_STATE* state) {
  uint32_t digest[5];
  std::copy(std::begin(state->state), std::end(state->state), digest);

  sha->init(digest, state->buffer, state->count);
}

void StoreSha1(const sha1::SHA1* sha, XECRYPT_SHA_STATE* state) {
  std::copy_n(sha->getDigest(), rex::countof(state->state), state->state);

  state->count = static_cast<uint32_t>(sha->getByteCount());
  std::copy_n(sha->getBlock(), sha->getBlockByteIndex(), state->buffer);
}

void XeCryptShaInit_entry(ppc_ptr_t<XECRYPT_SHA_STATE> sha_state) {
  sha_state.Zero();

  sha_state->state[0] = 0x67452301;
  sha_state->state[1] = 0xEFCDAB89;
  sha_state->state[2] = 0x98BADCFE;
  sha_state->state[3] = 0x10325476;
  sha_state->state[4] = 0xC3D2E1F0;
}

void XeCryptShaUpdate_entry(ppc_ptr_t<XECRYPT_SHA_STATE> sha_state, mapped_void input,
                            u32 input_size) {
  sha1::SHA1 sha;
  InitSha1(&sha, sha_state);

  sha.processBytes(input, input_size);

  StoreSha1(&sha, sha_state);
}

void XeCryptShaFinal_entry(ppc_ptr_t<XECRYPT_SHA_STATE> sha_state, ppc_ptr_t<uint8_t> out,
                           u32 out_size) {
  sha1::SHA1 sha;
  InitSha1(&sha, sha_state);

  uint8_t digest[0x14];
  sha.finalize(digest);

  std::copy_n(digest, std::min<size_t>(rex::countof(digest), out_size), static_cast<uint8_t*>(out));
  std::copy_n(sha.getDigest(), rex::countof(sha_state->state), sha_state->state);
}

void XeCryptSha_entry(mapped_void input_1, u32 input_1_size, mapped_void input_2, u32 input_2_size,
                      mapped_void input_3, u32 input_3_size, mapped_void output, u32 output_size) {
  sha1::SHA1 sha;

  if (input_1 && input_1_size) {
    sha.processBytes(input_1, input_1_size);
  }
  if (input_2 && input_2_size) {
    sha.processBytes(input_2, input_2_size);
  }
  if (input_3 && input_3_size) {
    sha.processBytes(input_3, input_3_size);
  }

  uint8_t digest[0x14];
  sha.finalize(digest);
  std::copy_n(digest, std::min<size_t>(rex::countof(digest), output_size), output.as<uint8_t*>());
}

// TODO: Size of this struct hasn't been confirmed yet.
typedef struct {
  rex::be<uint32_t> count;     // 0x0
  rex::be<uint32_t> state[8];  // 0x4
  uint8_t buffer[64];          // 0x24
} XECRYPT_SHA256_STATE;

void XeCryptSha256Init_entry(ppc_ptr_t<XECRYPT_SHA256_STATE> sha_state) {
  sha_state.Zero();

  sha_state->state[0] = 0x6a09e667;
  sha_state->state[1] = 0xbb67ae85;
  sha_state->state[2] = 0x3c6ef372;
  sha_state->state[3] = 0xa54ff53a;
  sha_state->state[4] = 0x510e527f;
  sha_state->state[5] = 0x9b05688c;
  sha_state->state[6] = 0x1f83d9ab;
  sha_state->state[7] = 0x5be0cd19;
}

void XeCryptSha256Update_entry(ppc_ptr_t<XECRYPT_SHA256_STATE> sha_state, mapped_void input,
                               u32 input_size) {
  sha256::SHA256 sha;
  std::copy(std::begin(sha_state->state), std::end(sha_state->state), sha.getHashValues());
  std::copy(std::begin(sha_state->buffer), std::end(sha_state->buffer), sha.getBuffer());
  sha.setTotalSize(sha_state->count);

  sha.add(input, input_size);

  std::copy_n(sha.getHashValues(), rex::countof(sha_state->state), sha_state->state);
  std::copy_n(sha.getBuffer(), rex::countof(sha_state->buffer), sha_state->buffer);
  sha_state->count = static_cast<uint32_t>(sha.getTotalSize());
}

void XeCryptSha256Final_entry(ppc_ptr_t<XECRYPT_SHA256_STATE> sha_state, ppc_ptr_t<uint8_t> out,
                              u32 out_size) {
  sha256::SHA256 sha;
  std::copy(std::begin(sha_state->state), std::end(sha_state->state), sha.getHashValues());
  std::copy(std::begin(sha_state->buffer), std::end(sha_state->buffer), sha.getBuffer());
  sha.setTotalSize(sha_state->count);

  uint8_t hash[32];
  sha.getHash(hash);

  std::copy_n(hash, std::min<size_t>(rex::countof(hash), out_size), static_cast<uint8_t*>(out));
  std::copy(std::begin(hash), std::end(hash), sha_state->buffer);
}

// Byteswaps each 8 bytes
void XeCryptBnQw_SwapDwQwLeBe_entry(ppc_ptr_t<uint64_t> qw_inp, ppc_ptr_t<uint64_t> qw_out,
                                    u32 size) {
  memory::copy_and_swap<uint64_t>(qw_out, qw_inp, size);
}

typedef struct {
  rex::be<uint32_t> size;  // size of modulus in 8 byte units
  rex::be<uint32_t> public_exponent;
  rex::be<uint64_t> pad_8;

  // followed by modulus, followed by any private-key data
} XECRYPT_RSA;
static_assert_size(XECRYPT_RSA, 0x10);

// -----------------------------------------------------------------------------
// Portable BigNum modexp for XeCryptBnQwNeRsaPubCrypt.
//
// The XECRYPT RSA blob stores the modulus as an array of qwords in "little-
// endian qword" order: least-significant qword first, with each qword stored
// in the platform's native endianness (host). qw_a (ciphertext) is in the same
// layout, and qw_b (plaintext output) is produced in that same layout.
//
// Internally we convert to a uint32_t limb array (least-significant limb at
// index 0, little-endian) so we can do 32x32->64 multiplies portably without
// needing __uint128_t.
// -----------------------------------------------------------------------------
namespace {

// Copy N qwords (host-endian in memory) into 2N uint32 limbs, little-endian
// limb order. qwords are stored LS-qword-first in the source buffer.
static void qw_le_to_limbs(const uint64_t* qw, uint32_t num_qw, uint32_t* limbs) {
  for (uint32_t i = 0; i < num_qw; ++i) {
    uint64_t v = qw[i];
    limbs[2 * i + 0] = static_cast<uint32_t>(v & 0xFFFFFFFFu);
    limbs[2 * i + 1] = static_cast<uint32_t>(v >> 32);
  }
}

static void limbs_to_qw_le(const uint32_t* limbs, uint32_t num_qw, uint64_t* qw) {
  for (uint32_t i = 0; i < num_qw; ++i) {
    uint64_t lo = limbs[2 * i + 0];
    uint64_t hi = limbs[2 * i + 1];
    qw[i] = lo | (hi << 32);
  }
}

// Compare a (n limbs) vs b (n limbs). Returns -1, 0, 1.
static int bn_cmp(const uint32_t* a, const uint32_t* b, uint32_t n) {
  for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
    if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
  }
  return 0;
}

// r = a - b  (a,b,r each n limbs).  Returns final borrow.
static uint32_t bn_sub(uint32_t* r, const uint32_t* a, const uint32_t* b, uint32_t n) {
  uint64_t borrow = 0;
  for (uint32_t i = 0; i < n; ++i) {
    uint64_t t = static_cast<uint64_t>(a[i]) - b[i] - borrow;
    r[i] = static_cast<uint32_t>(t);
    borrow = (t >> 32) & 1;
  }
  return static_cast<uint32_t>(borrow);
}

// r = a * b mod m.  a,b,m are n limbs.  r is n limbs.
// Uses schoolbook multiply into 2n buffer then shift-subtract reduce.
static void bn_mulmod(uint32_t* r, const uint32_t* a, const uint32_t* b, const uint32_t* m,
                      uint32_t n) {
  std::vector<uint32_t> prod(2 * n, 0);
  // schoolbook mul
  for (uint32_t i = 0; i < n; ++i) {
    uint64_t carry = 0;
    uint64_t ai = a[i];
    for (uint32_t j = 0; j < n; ++j) {
      uint64_t cur = prod[i + j] + ai * b[j] + carry;
      prod[i + j] = static_cast<uint32_t>(cur);
      carry = cur >> 32;
    }
    prod[i + n] = static_cast<uint32_t>(carry);
  }

  // Shift-subtract mod reduction.
  // We reduce prod (2n limbs) modulo m (n limbs) by aligning m shifted left
  // to the top of prod and conditionally subtracting, then shifting right by
  // one bit per iteration. Total iterations = n*32 bits.
  std::vector<uint32_t> mshift(2 * n, 0);
  // Start with m shifted so its MSB aligns with bit (2n*32 - 1).
  // Find MSB of m.
  int msb = -1;
  for (int i = static_cast<int>(n) - 1; i >= 0 && msb < 0; --i) {
    if (m[i]) {
      uint32_t v = m[i];
      int bit = 31;
      while (((v >> bit) & 1u) == 0) --bit;
      msb = i * 32 + bit;
    }
  }
  if (msb < 0) {
    // modulus is zero — undefined; zero result
    std::memset(r, 0, n * sizeof(uint32_t));
    return;
  }

  // Shift m left by (2n*32 - 1 - msb) into mshift.
  const int total_bits = static_cast<int>(2 * n * 32);
  int shift_amt = total_bits - 1 - msb;
  // Place m into low part of mshift then shift left by shift_amt.
  for (uint32_t i = 0; i < n; ++i) mshift[i] = m[i];
  // shift left by shift_amt bits in-place
  {
    int word_shift = shift_amt / 32;
    int bit_shift = shift_amt % 32;
    if (word_shift > 0) {
      for (int i = static_cast<int>(2 * n) - 1; i >= 0; --i) {
        mshift[i] = (i - word_shift >= 0) ? mshift[i - word_shift] : 0;
      }
    }
    if (bit_shift > 0) {
      uint32_t carry = 0;
      for (uint32_t i = 0; i < 2 * n; ++i) {
        uint32_t v = mshift[i];
        mshift[i] = (v << bit_shift) | carry;
        carry = v >> (32 - bit_shift);
      }
    }
  }

  // Iterate: at each step, if prod >= mshift, subtract; then shift mshift right 1.
  // We do this shift_amt + 1 times so that the final mshift equals m.
  int iterations = shift_amt + 1;
  for (int it = 0; it < iterations; ++it) {
    // compare prod vs mshift (2n limbs)
    int cmp = 0;
    for (int i = static_cast<int>(2 * n) - 1; i >= 0; --i) {
      if (prod[i] != mshift[i]) {
        cmp = prod[i] < mshift[i] ? -1 : 1;
        break;
      }
    }
    if (cmp >= 0) {
      // prod -= mshift
      uint64_t borrow = 0;
      for (uint32_t i = 0; i < 2 * n; ++i) {
        uint64_t t = static_cast<uint64_t>(prod[i]) - mshift[i] - borrow;
        prod[i] = static_cast<uint32_t>(t);
        borrow = (t >> 32) & 1;
      }
    }
    // shift mshift right 1 bit
    uint32_t carry = 0;
    for (int i = static_cast<int>(2 * n) - 1; i >= 0; --i) {
      uint32_t v = mshift[i];
      mshift[i] = (v >> 1) | (carry << 31);
      carry = v & 1u;
    }
  }

  // prod[0..n-1] now holds r; ensure high half is zero (it should be).
  std::memcpy(r, prod.data(), n * sizeof(uint32_t));
}

// r = base^exp mod m. base, m, r: n limbs. exp is up to 32 bits (matches
// XECRYPT_RSA public_exponent field, which is a u32).
static void bn_modexp_u32(uint32_t* r, const uint32_t* base, uint32_t exp, const uint32_t* m,
                          uint32_t n) {
  // result = 1
  std::vector<uint32_t> result(n, 0);
  result[0] = 1;
  std::vector<uint32_t> cur(base, base + n);
  std::vector<uint32_t> tmp(n, 0);

  // If modulus is 1, result is 0.
  bool mod_is_one = (m[0] == 1);
  if (mod_is_one) {
    for (uint32_t i = 1; i < n; ++i)
      if (m[i]) {
        mod_is_one = false;
        break;
      }
  }
  if (mod_is_one) {
    std::memset(r, 0, n * sizeof(uint32_t));
    return;
  }

  // Reduce initial base mod m in case it's >= m.
  while (bn_cmp(cur.data(), m, n) >= 0) {
    bn_sub(cur.data(), cur.data(), m, n);
  }

  while (exp) {
    if (exp & 1u) {
      bn_mulmod(tmp.data(), result.data(), cur.data(), m, n);
      result.swap(tmp);
    }
    exp >>= 1;
    if (exp) {
      bn_mulmod(tmp.data(), cur.data(), cur.data(), m, n);
      cur.swap(tmp);
    }
  }

  std::memcpy(r, result.data(), n * sizeof(uint32_t));
}

}  // namespace

u32 XeCryptBnQwNeRsaPubCrypt_entry(ppc_ptr_t<uint64_t> qw_a, ppc_ptr_t<uint64_t> qw_b,
                                   ppc_ptr_t<XECRYPT_RSA> rsa) {
  // 0 indicates failure (but not a BOOL return value)
  const uint32_t num_qw = rsa->size;
  if (num_qw == 0 || num_qw > 64) {  // sanity: up to 4096-bit
    REXKRNL_ERROR("XeCryptBnQwNeRsaPubCrypt: invalid modulus size {}", num_qw);
    return 0;
  }

  const uint32_t num_limbs = num_qw * 2;  // uint32 limbs

  // XECRYPT_RSA stores modulus immediately after the header, as num_qw qwords
  // in LS-qword-first order (host-endian per qword).
  uint64_t* modulus_qw = reinterpret_cast<uint64_t*>(&rsa[1]);

  std::vector<uint32_t> mod_limbs(num_limbs, 0);
  std::vector<uint32_t> a_limbs(num_limbs, 0);
  std::vector<uint32_t> r_limbs(num_limbs, 0);

  qw_le_to_limbs(modulus_qw, num_qw, mod_limbs.data());
  qw_le_to_limbs(static_cast<uint64_t*>(qw_a), num_qw, a_limbs.data());

  uint32_t exponent = rsa->public_exponent.value;
  if (exponent == 0) exponent = 65537;  // safety default

  bn_modexp_u32(r_limbs.data(), a_limbs.data(), exponent, mod_limbs.data(), num_limbs);

  limbs_to_qw_le(r_limbs.data(), num_qw, static_cast<uint64_t*>(qw_b));
  return 1;
}

u32 XeCryptBnDwLePkcs1Verify_entry(mapped_void hash, mapped_void sig, u32 size) {
  // BOOL return value
  return 1;
}

void XeCryptRandom_entry(mapped_void buf, u32 buf_size) {
  std::memset(buf, 0xFD, buf_size);
}

struct XECRYPT_DES_STATE {
  uint32_t keytab[16][2];
};

// Sets bit 0 to make the parity odd
void XeCryptDesParity_entry(mapped_void inp, u32 inp_size, mapped_void out_ptr) {
  DES::set_parity(inp, inp_size, out_ptr);
}

struct XECRYPT_DES3_STATE {
  XECRYPT_DES_STATE des_state[3];
};

void XeCryptDes3Key_entry(ppc_ptr_t<XECRYPT_DES3_STATE> state_ptr, mapped_u64 key) {
  DES3 des3(key[0], key[1], key[2]);
  DES* des = des3.getDES();

  // Store our DES state into the state.
  for (int i = 0; i < 3; i++) {
    std::memcpy(state_ptr->des_state[i].keytab, des[i].get_sub_key(), 128);
  }
}

void XeCryptDes3Ecb_entry(ppc_ptr_t<XECRYPT_DES3_STATE> state_ptr, mapped_u64 inp, mapped_u64 out,
                          u32 encrypt) {
  DES3 des3((ui64*)state_ptr->des_state[0].keytab, (ui64*)state_ptr->des_state[1].keytab,
            (ui64*)state_ptr->des_state[2].keytab);

  if (encrypt) {
    *out = des3.encrypt(*inp);
  } else {
    *out = des3.decrypt(*inp);
  }
}

void XeCryptDes3Cbc_entry(ppc_ptr_t<XECRYPT_DES3_STATE> state_ptr, mapped_u64 inp, u32 inp_size,
                          mapped_u64 out, mapped_u64 feed, u32 encrypt) {
  DES3 des3((ui64*)state_ptr->des_state[0].keytab, (ui64*)state_ptr->des_state[1].keytab,
            (ui64*)state_ptr->des_state[2].keytab);

  // DES can only do 8-byte chunks at a time!
  assert_true(inp_size % 8 == 0);

  uint64_t last_block = *feed;
  for (uint32_t i = 0; i < inp_size / 8; i++) {
    uint64_t block = inp[i];
    if (encrypt) {
      last_block = des3.encrypt(block ^ last_block);
      out[i] = last_block;
    } else {
      out[i] = des3.decrypt(block) ^ last_block;
      last_block = block;
    }
  }

  *feed = last_block;
}

struct XECRYPT_AES_STATE {
  uint8_t keytabenc[11][4][4];  // 0x0
  uint8_t keytabdec[11][4][4];  // 0xB0
};
static_assert_size(XECRYPT_AES_STATE, 0x160);

static inline uint8_t xeXeCryptAesMul2(uint8_t a) {
  return (a & 0x80) ? ((a << 1) ^ 0x1B) : (a << 1);
}

void XeCryptAesKey_entry(ppc_ptr_t<XECRYPT_AES_STATE> state_ptr, mapped_void key) {
  aes_key_schedule_128(key, reinterpret_cast<uint8_t*>(state_ptr->keytabenc));
  // Decryption key schedule not needed by openluopworld/aes_128, but generated
  // to fill the context structure properly.
  std::memcpy(state_ptr->keytabdec[0], state_ptr->keytabenc[10], 16);
  // Inverse MixColumns.
  for (uint32_t i = 1; i < 10; ++i) {
    const uint8_t* enc = reinterpret_cast<const uint8_t*>(state_ptr->keytabenc[10 - i]);
    uint8_t* dec = reinterpret_cast<uint8_t*>(state_ptr->keytabdec[i]);
    uint8_t t, u, v;
    t = enc[0] ^ enc[1] ^ enc[2] ^ enc[3];
    dec[0] = t ^ enc[0] ^ xeXeCryptAesMul2(enc[0] ^ enc[1]);
    dec[1] = t ^ enc[1] ^ xeXeCryptAesMul2(enc[1] ^ enc[2]);
    dec[2] = t ^ enc[2] ^ xeXeCryptAesMul2(enc[2] ^ enc[3]);
    dec[3] = t ^ enc[3] ^ xeXeCryptAesMul2(enc[3] ^ enc[0]);
    u = xeXeCryptAesMul2(xeXeCryptAesMul2(enc[0] ^ enc[2]));
    v = xeXeCryptAesMul2(xeXeCryptAesMul2(enc[1] ^ enc[3]));
    t = xeXeCryptAesMul2(u ^ v);
    dec[0] ^= t ^ u;
    dec[1] ^= t ^ v;
    dec[2] ^= t ^ u;
    dec[3] ^= t ^ v;
    t = enc[4] ^ enc[5] ^ enc[6] ^ enc[7];
    dec[4] = t ^ enc[4] ^ xeXeCryptAesMul2(enc[4] ^ enc[5]);
    dec[5] = t ^ enc[5] ^ xeXeCryptAesMul2(enc[5] ^ enc[6]);
    dec[6] = t ^ enc[6] ^ xeXeCryptAesMul2(enc[6] ^ enc[7]);
    dec[7] = t ^ enc[7] ^ xeXeCryptAesMul2(enc[7] ^ enc[4]);
    u = xeXeCryptAesMul2(xeXeCryptAesMul2(enc[4] ^ enc[6]));
    v = xeXeCryptAesMul2(xeXeCryptAesMul2(enc[5] ^ enc[7]));
    t = xeXeCryptAesMul2(u ^ v);
    dec[4] ^= t ^ u;
    dec[5] ^= t ^ v;
    dec[6] ^= t ^ u;
    dec[7] ^= t ^ v;
    t = enc[8] ^ enc[9] ^ enc[10] ^ enc[11];
    dec[8] = t ^ enc[8] ^ xeXeCryptAesMul2(enc[8] ^ enc[9]);
    dec[9] = t ^ enc[9] ^ xeXeCryptAesMul2(enc[9] ^ enc[10]);
    dec[10] = t ^ enc[10] ^ xeXeCryptAesMul2(enc[10] ^ enc[11]);
    dec[11] = t ^ enc[11] ^ xeXeCryptAesMul2(enc[11] ^ enc[8]);
    u = xeXeCryptAesMul2(xeXeCryptAesMul2(enc[8] ^ enc[10]));
    v = xeXeCryptAesMul2(xeXeCryptAesMul2(enc[9] ^ enc[11]));
    t = xeXeCryptAesMul2(u ^ v);
    dec[8] ^= t ^ u;
    dec[9] ^= t ^ v;
    dec[10] ^= t ^ u;
    dec[11] ^= t ^ v;
    t = enc[12] ^ enc[13] ^ enc[14] ^ enc[15];
    dec[12] = t ^ enc[12] ^ xeXeCryptAesMul2(enc[12] ^ enc[13]);
    dec[13] = t ^ enc[13] ^ xeXeCryptAesMul2(enc[13] ^ enc[14]);
    dec[14] = t ^ enc[14] ^ xeXeCryptAesMul2(enc[14] ^ enc[15]);
    dec[15] = t ^ enc[15] ^ xeXeCryptAesMul2(enc[15] ^ enc[12]);
    u = xeXeCryptAesMul2(xeXeCryptAesMul2(enc[12] ^ enc[14]));
    v = xeXeCryptAesMul2(xeXeCryptAesMul2(enc[13] ^ enc[15]));
    t = xeXeCryptAesMul2(u ^ v);
    dec[12] ^= t ^ u;
    dec[13] ^= t ^ v;
    dec[14] ^= t ^ u;
    dec[15] ^= t ^ v;
  }
  std::memcpy(state_ptr->keytabdec[10], state_ptr->keytabenc[0], 16);
  // TODO(Triang3l): Verify the order in keytabenc and everything in keytabdec.
}

void XeCryptAesEcb_entry(ppc_ptr_t<XECRYPT_AES_STATE> state_ptr, mapped_void inp_ptr,
                         mapped_void out_ptr, u32 encrypt) {
  const uint8_t* keytab = reinterpret_cast<const uint8_t*>(state_ptr->keytabenc);
  if (encrypt) {
    aes_encrypt_128(keytab, inp_ptr, out_ptr);
  } else {
    aes_decrypt_128(keytab, inp_ptr, out_ptr);
  }
}

void XeCryptAesCbc_entry(ppc_ptr_t<XECRYPT_AES_STATE> state_ptr, mapped_void inp_ptr, u32 inp_size,
                         mapped_void out_ptr, mapped_void feed_ptr, u32 encrypt) {
  const uint8_t* keytab = reinterpret_cast<const uint8_t*>(state_ptr->keytabenc);
  const uint8_t* inp = inp_ptr.as<const uint8_t*>();
  uint8_t* out = out_ptr.as<uint8_t*>();
  uint8_t* feed = feed_ptr.as<uint8_t*>();
  if (encrypt) {
    for (uint32_t i = 0; i < inp_size; i += 16) {
      for (uint32_t j = 0; j < 16; ++j) {
        feed[j] ^= inp[j];
      }
      aes_encrypt_128(keytab, feed, feed);
      std::memcpy(out, feed, 16);
      inp += 16;
      out += 16;
    }
  } else {
    for (uint32_t i = 0; i < inp_size; i += 16) {
      // In case inp == out.
      uint8_t tmp[16];
      std::memcpy(tmp, inp, 16);
      aes_decrypt_128(keytab, inp, out);
      for (uint32_t j = 0; j < 16; ++j) {
        out[j] ^= feed[j];
      }
      std::memcpy(feed, tmp, 16);
      inp += 16;
      out += 16;
    }
  }
}

void XeCryptHmacSha_entry(mapped_void key, u32 key_size_in, mapped_void inp_1, u32 inp_1_size,
                          mapped_void inp_2, u32 inp_2_size, mapped_void inp_3, u32 inp_3_size,
                          mapped_void out, u32 out_size) {
  uint32_t key_size = key_size_in;
  sha1::SHA1 sha;
  uint8_t kpad_i[0x40];
  uint8_t kpad_o[0x40];
  uint8_t tmp_key[0x40];
  std::memset(kpad_i, 0x36, 0x40);
  std::memset(kpad_o, 0x5C, 0x40);

  // Setup HMAC key
  // If > block size, use its hash
  if (key_size > 0x40) {
    sha1::SHA1 sha_key;
    sha_key.processBytes(key, key_size);
    sha_key.finalize((uint8_t*)tmp_key);

    key_size = 0x14u;
  } else {
    std::memcpy(tmp_key, key, key_size);
  }

  for (uint32_t i = 0; i < key_size; i++) {
    kpad_i[i] = tmp_key[i] ^ 0x36;
    kpad_o[i] = tmp_key[i] ^ 0x5C;
  }

  // Inner
  sha.processBytes(kpad_i, 0x40);

  if (inp_1_size) {
    sha.processBytes(inp_1, inp_1_size);
  }

  if (inp_2_size) {
    sha.processBytes(inp_2, inp_2_size);
  }

  if (inp_3_size) {
    sha.processBytes(inp_3, inp_3_size);
  }

  uint8_t digest[0x14];
  sha.finalize(digest);
  sha.reset();

  // Outer
  sha.processBytes(kpad_o, 0x40);
  sha.processBytes(digest, 0x14);
  sha.finalize(digest);

  std::memcpy(out, digest, std::min((uint32_t)out_size, 0x14u));
}

// Keys
// TODO: Array of keys we need

// Retail key 0x19
static const uint8_t key19[] = {0xE1, 0xBC, 0x15, 0x9C, 0x73, 0xB1, 0xEA, 0xE9,
                                0xAB, 0x31, 0x70, 0xF3, 0xAD, 0x47, 0xEB, 0xF3};

u32 XeKeysHmacSha_entry(u32 key_num, mapped_void inp_1, u32 inp_1_size, mapped_void inp_2,
                        u32 inp_2_size, mapped_void inp_3, u32 inp_3_size, mapped_void out,
                        u32 out_size) {
  const uint8_t* key = nullptr;
  if (key_num == 0x19) {
    key = key19;
  }

  if (key) {
    XeCryptHmacSha_entry(mapped_void::from_host((void*)key), 0x10, inp_1, inp_1_size, inp_2,
                         inp_2_size, inp_3, inp_3_size, out, out_size);

    return X_STATUS_SUCCESS;
  }

  return X_STATUS_UNSUCCESSFUL;
}

static const uint8_t xe_key_obfuscation_key[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

u32 XeKeysAesCbcUsingKey_entry(mapped_void obscured_key, mapped_void inp_ptr, u32 inp_size,
                               mapped_void out_ptr, mapped_void feed_ptr, u32 encrypt) {
  uint8_t key[16];

  // Deobscure key
  XECRYPT_AES_STATE aes;
  XeCryptAesKey_entry(ppc_ptr_t<XECRYPT_AES_STATE>::from_host(&aes),
                      mapped_void::from_host((void*)xe_key_obfuscation_key));
  XeCryptAesEcb_entry(ppc_ptr_t<XECRYPT_AES_STATE>::from_host(&aes), obscured_key,
                      mapped_void::from_host(key), 0);

  // Run CBC using deobscured key
  XeCryptAesKey_entry(ppc_ptr_t<XECRYPT_AES_STATE>::from_host(&aes), mapped_void::from_host(key));
  XeCryptAesCbc_entry(ppc_ptr_t<XECRYPT_AES_STATE>::from_host(&aes), inp_ptr, inp_size, out_ptr,
                      feed_ptr, encrypt);

  return X_STATUS_SUCCESS;
}

u32 XeKeysObscureKey_entry(mapped_void input, mapped_void output) {
  // Based on HvxKeysObscureKey
  // Seems to encrypt input with per-console KEY_OBFUSCATION_KEY (key 0x18)

  XECRYPT_AES_STATE aes;
  XeCryptAesKey_entry(ppc_ptr_t<XECRYPT_AES_STATE>::from_host(&aes),
                      mapped_void::from_host((void*)xe_key_obfuscation_key));
  XeCryptAesEcb_entry(ppc_ptr_t<XECRYPT_AES_STATE>::from_host(&aes), input, output, 1);

  return X_STATUS_SUCCESS;
}

u32 XeKeysHmacShaUsingKey_entry(mapped_void obscured_key, mapped_void inp_1, u32 inp_1_size,
                                mapped_void inp_2, u32 inp_2_size, mapped_void inp_3,
                                u32 inp_3_size, mapped_void out, u32 out_size) {
  if (!obscured_key) {
    return X_STATUS_INVALID_PARAMETER;
  }

  uint8_t key[16];

  // Deobscure key
  XECRYPT_AES_STATE aes;
  XeCryptAesKey_entry(ppc_ptr_t<XECRYPT_AES_STATE>::from_host(&aes),
                      mapped_void::from_host((void*)xe_key_obfuscation_key));
  XeCryptAesEcb_entry(ppc_ptr_t<XECRYPT_AES_STATE>::from_host(&aes), obscured_key,
                      mapped_void::from_host(key), 0);

  XeCryptHmacSha_entry(mapped_void::from_host(key), 0x10, inp_1, inp_1_size, inp_2, inp_2_size,
                       inp_3, inp_3_size, out, out_size);
  return X_STATUS_SUCCESS;
}

u32 XeKeysConsolePrivateKeySign_entry(mapped_void hash, mapped_void signature) {
  REXKRNL_DEBUG("XeKeysConsolePrivateKeySign - stub");
  return 0;  // Success
}

u32 XeKeysConsoleSignatureVerification_entry(mapped_void hash, mapped_void signature,
                                             mapped_void pubkey) {
  REXKRNL_DEBUG("XeKeysConsoleSignatureVerification - stub");
  return 0;  // Success (signature valid)
}

REX_EXPORT_STUB(__imp__XeKeysGetConsoleCertificate);
REX_EXPORT_STUB(__imp__XeCryptBnDwLeDhEqualBase);
REX_EXPORT_STUB(__imp__XeCryptBnDwLeDhInvalBase);
REX_EXPORT_STUB(__imp__XeCryptBnDwLeDhModExp);
REX_EXPORT_STUB(__imp__XeCryptBnDw_Copy);
REX_EXPORT_STUB(__imp__XeCryptBnDw_SwapLeBe);
REX_EXPORT_STUB(__imp__XeCryptBnDw_Zero);
REX_EXPORT_STUB(__imp__XeCryptBnDwLePkcs1Format);
REX_EXPORT_STUB(__imp__XeCryptBnQwBeSigCreate);
REX_EXPORT_STUB(__imp__XeCryptBnQwBeSigFormat);
REX_EXPORT_STUB(__imp__XeCryptBnQwBeSigVerify);
REX_EXPORT_STUB(__imp__XeCryptBnQwNeModExp);
REX_EXPORT_STUB(__imp__XeCryptBnQwNeModExpRoot);
REX_EXPORT_STUB(__imp__XeCryptBnQwNeModInv);
REX_EXPORT_STUB(__imp__XeCryptBnQwNeModMul);
REX_EXPORT_STUB(__imp__XeCryptBnQwNeRsaKeyGen);
REX_EXPORT_STUB(__imp__XeCryptBnQwNeRsaPrvCrypt);
REX_EXPORT_STUB(__imp__XeCryptBnQw_Copy);
REX_EXPORT_STUB(__imp__XeCryptBnQw_SwapDwQw);
REX_EXPORT_STUB(__imp__XeCryptBnQw_SwapLeBe);
REX_EXPORT_STUB(__imp__XeCryptBnQw_Zero);
REX_EXPORT_STUB(__imp__XeCryptChainAndSumMac);
REX_EXPORT_STUB(__imp__XeCryptDesKey);
REX_EXPORT_STUB(__imp__XeCryptDesEcb);
REX_EXPORT_STUB(__imp__XeCryptDesCbc);
REX_EXPORT_STUB(__imp__XeCryptHmacMd5Init);
REX_EXPORT_STUB(__imp__XeCryptHmacMd5Update);
REX_EXPORT_STUB(__imp__XeCryptHmacMd5Final);
REX_EXPORT_STUB(__imp__XeCryptHmacMd5);
REX_EXPORT_STUB(__imp__XeCryptHmacShaInit);
REX_EXPORT_STUB(__imp__XeCryptHmacShaUpdate);
REX_EXPORT_STUB(__imp__XeCryptHmacShaFinal);
REX_EXPORT_STUB(__imp__XeCryptHmacShaVerify);
REX_EXPORT_STUB(__imp__XeCryptMd5Init);
REX_EXPORT_STUB(__imp__XeCryptMd5Update);
REX_EXPORT_STUB(__imp__XeCryptMd5Final);
REX_EXPORT_STUB(__imp__XeCryptMd5);
REX_EXPORT_STUB(__imp__XeCryptParveEcb);
REX_EXPORT_STUB(__imp__XeCryptParveCbcMac);
REX_EXPORT_STUB(__imp__XeCryptRotSumSha);
REX_EXPORT_STUB(__imp__XeCryptSha256);
REX_EXPORT_STUB(__imp__XeCryptSha384Init);
REX_EXPORT_STUB(__imp__XeCryptSha384Update);
REX_EXPORT_STUB(__imp__XeCryptSha384Final);
REX_EXPORT_STUB(__imp__XeCryptSha384);
REX_EXPORT_STUB(__imp__XeCryptSha512Init);
REX_EXPORT_STUB(__imp__XeCryptSha512Update);
REX_EXPORT_STUB(__imp__XeCryptSha512Final);
REX_EXPORT_STUB(__imp__XeCryptSha512);
REX_EXPORT_STUB(__imp__XeCryptBnQwNeCompare);
REX_EXPORT_STUB(__imp__XeKeysGetFactoryChallenge);
REX_EXPORT_STUB(__imp__XeKeysSetFactoryResponse);
REX_EXPORT_STUB(__imp__XeKeysInitializeFuses);
REX_EXPORT_STUB(__imp__XeKeysSaveBootLoader);
REX_EXPORT_STUB(__imp__XeKeysSaveKeyVault);
REX_EXPORT_STUB(__imp__XeKeysGetStatus);
REX_EXPORT_STUB(__imp__XeKeysGeneratePrivateKey);
REX_EXPORT_STUB(__imp__XeKeysGetKeyProperties);
REX_EXPORT_STUB(__imp__XeKeysSetKey);
REX_EXPORT_STUB(__imp__XeKeysGenerateRandomKey);
REX_EXPORT_STUB(__imp__XeKeysGetKey);
REX_EXPORT_STUB(__imp__XeKeysGetDigest);
REX_EXPORT_STUB(__imp__XeKeysGetConsoleID);
REX_EXPORT_STUB(__imp__XeKeysGetConsoleType);
REX_EXPORT_STUB(__imp__XeKeysQwNeRsaPrvCrypt);
REX_EXPORT_STUB(__imp__XeKeysAesCbc);
REX_EXPORT_STUB(__imp__XeKeysDes2Cbc);
REX_EXPORT_STUB(__imp__XeKeysDesCbc);
REX_EXPORT_STUB(__imp__XeKeysSaveBootLoaderEx);
REX_EXPORT_STUB(__imp__XeKeysDes2CbcUsingKey);
REX_EXPORT_STUB(__imp__XeKeysDesCbcUsingKey);
REX_EXPORT_STUB(__imp__XeKeysObfuscate);
REX_EXPORT_STUB(__imp__XeKeysUnObfuscate);
REX_EXPORT_STUB(__imp__XeKeysVerifyRSASignature);
REX_EXPORT_STUB(__imp__XeKeysSaveSystemUpdate);
REX_EXPORT_STUB(__imp__XeKeysLockSystemUpdate);
REX_EXPORT_STUB(__imp__XeKeysExecute);
REX_EXPORT_STUB(__imp__XeKeysGetVersions);
REX_EXPORT_STUB(__imp__XeKeysSetRevocationList);
REX_EXPORT_STUB(__imp__XeKeysExSaveKeyVault);
REX_EXPORT_STUB(__imp__XeKeysExSetKey);
REX_EXPORT_STUB(__imp__XeKeysExGetKey);
REX_EXPORT_STUB(__imp__XeKeysSecurityInitialize);
REX_EXPORT_STUB(__imp__XeKeysSecurityLoadSettings);
REX_EXPORT_STUB(__imp__XeKeysSecuritySaveSettings);
REX_EXPORT_STUB(__imp__XeKeysSecuritySetDetected);
REX_EXPORT_STUB(__imp__XeKeysSecurityGetDetected);
REX_EXPORT_STUB(__imp__XeKeysSecuritySetActivated);
REX_EXPORT_STUB(__imp__XeKeysSecurityGetActivated);
REX_EXPORT_STUB(__imp__XeKeysDvdAuthAP25InstallTable);
REX_EXPORT_STUB(__imp__XeKeysDvdAuthAP25GetTableVersion);
REX_EXPORT_STUB(__imp__XeKeysGetProtectedFlag);
REX_EXPORT_STUB(__imp__XeKeysSetProtectedFlag);
REX_EXPORT_STUB(__imp__XeKeysGetUpdateSequence);
REX_EXPORT_STUB(__imp__XeKeysDvdAuthExActivate);
REX_EXPORT_STUB(__imp__XeKeysRevokeSaveSettings);
REX_EXPORT_STUB(__imp__XeKeysGetMediaID);
REX_EXPORT_STUB(__imp__XeKeysLoadKeyVault);
REX_EXPORT_STUB(__imp__XeKeysRevokeUpdateDynamic);
REX_EXPORT_STUB(__imp__XeKeysSecuritySetStat);
REX_EXPORT_STUB(__imp__XeKeysFcrtLoad);
REX_EXPORT_STUB(__imp__XeKeysFcrtSave);
REX_EXPORT_STUB(__imp__XeKeysFcrtSet);
REX_EXPORT_STUB(__imp__XeKeysRevokeIsDeviceRevoked);
REX_EXPORT_STUB(__imp__XeKeysDvdAuthExSave);
REX_EXPORT_STUB(__imp__XeKeysDvdAuthExInstall);
REX_EXPORT_STUB(__imp__XeKeysObfuscateEx);
REX_EXPORT_STUB(__imp__XeKeysUnObfuscateEx);
REX_EXPORT_STUB(__imp__XeKeysVerifyPIRSSignature);
REX_EXPORT_STUB(__imp__XeCryptAesCtr);
REX_EXPORT_STUB(__imp__XeCryptAesCbcMac);
REX_EXPORT_STUB(__imp__XeCryptAesDmMac);
REX_EXPORT_STUB(__imp__XeKeysGetTruncatedSecondaryConsoleId);
REX_EXPORT_STUB(__imp__XeCryptSha224Init);
REX_EXPORT_STUB(__imp__XeCryptAesCreateKeySchedule);
REX_EXPORT_STUB(__imp__XeCryptAesEncryptOne);
REX_EXPORT_STUB(__imp__XeCryptAesDecryptOne);
REX_EXPORT_STUB(__imp__XeCryptAesCbcEncrypt);
REX_EXPORT_STUB(__imp__XeCryptAesCbcDecrypt);
REX_EXPORT_STUB(__imp__XeCryptAesGcmInitialize);
REX_EXPORT_STUB(__imp__XeCryptAesGcmUpdate);
REX_EXPORT_STUB(__imp__XeCryptAesGcmFinalize);
REX_EXPORT_STUB(__imp__XeCryptEccGetCurveParameters);
REX_EXPORT_STUB(__imp__XeCryptEccEcdhGenerateKeypair);
REX_EXPORT_STUB(__imp__XeCryptEccEcdhExponentiate);
REX_EXPORT_STUB(__imp__XeCryptEccEcdsaGenerateSignature);
REX_EXPORT_STUB(__imp__XeCryptEccEcdsaVerifySignature);

}  // namespace rex::kernel::xboxkrnl

REX_EXPORT(__imp__XeCryptRc4Key, rex::kernel::xboxkrnl::XeCryptRc4Key_entry)
REX_EXPORT(__imp__XeCryptRc4Ecb, rex::kernel::xboxkrnl::XeCryptRc4Ecb_entry)
REX_EXPORT(__imp__XeCryptRc4, rex::kernel::xboxkrnl::XeCryptRc4_entry)
REX_EXPORT(__imp__XeCryptShaInit, rex::kernel::xboxkrnl::XeCryptShaInit_entry)
REX_EXPORT(__imp__XeCryptShaUpdate, rex::kernel::xboxkrnl::XeCryptShaUpdate_entry)
REX_EXPORT(__imp__XeCryptShaFinal, rex::kernel::xboxkrnl::XeCryptShaFinal_entry)
REX_EXPORT(__imp__XeCryptSha, rex::kernel::xboxkrnl::XeCryptSha_entry)
REX_EXPORT(__imp__XeCryptSha256Init, rex::kernel::xboxkrnl::XeCryptSha256Init_entry)
REX_EXPORT(__imp__XeCryptSha256Update, rex::kernel::xboxkrnl::XeCryptSha256Update_entry)
REX_EXPORT(__imp__XeCryptSha256Final, rex::kernel::xboxkrnl::XeCryptSha256Final_entry)
REX_EXPORT(__imp__XeCryptBnQw_SwapDwQwLeBe, rex::kernel::xboxkrnl::XeCryptBnQw_SwapDwQwLeBe_entry)
REX_EXPORT(__imp__XeCryptBnQwNeRsaPubCrypt, rex::kernel::xboxkrnl::XeCryptBnQwNeRsaPubCrypt_entry)
REX_EXPORT(__imp__XeCryptBnDwLePkcs1Verify, rex::kernel::xboxkrnl::XeCryptBnDwLePkcs1Verify_entry)
REX_EXPORT(__imp__XeCryptRandom, rex::kernel::xboxkrnl::XeCryptRandom_entry)
REX_EXPORT(__imp__XeCryptDesParity, rex::kernel::xboxkrnl::XeCryptDesParity_entry)
REX_EXPORT(__imp__XeCryptDes3Key, rex::kernel::xboxkrnl::XeCryptDes3Key_entry)
REX_EXPORT(__imp__XeCryptDes3Ecb, rex::kernel::xboxkrnl::XeCryptDes3Ecb_entry)
REX_EXPORT(__imp__XeCryptDes3Cbc, rex::kernel::xboxkrnl::XeCryptDes3Cbc_entry)
REX_EXPORT(__imp__XeCryptAesKey, rex::kernel::xboxkrnl::XeCryptAesKey_entry)
REX_EXPORT(__imp__XeCryptAesEcb, rex::kernel::xboxkrnl::XeCryptAesEcb_entry)
REX_EXPORT(__imp__XeCryptAesCbc, rex::kernel::xboxkrnl::XeCryptAesCbc_entry)
REX_EXPORT(__imp__XeCryptHmacSha, rex::kernel::xboxkrnl::XeCryptHmacSha_entry)
REX_EXPORT(__imp__XeKeysHmacSha, rex::kernel::xboxkrnl::XeKeysHmacSha_entry)
REX_EXPORT(__imp__XeKeysAesCbcUsingKey, rex::kernel::xboxkrnl::XeKeysAesCbcUsingKey_entry)
REX_EXPORT(__imp__XeKeysObscureKey, rex::kernel::xboxkrnl::XeKeysObscureKey_entry)
REX_EXPORT(__imp__XeKeysHmacShaUsingKey, rex::kernel::xboxkrnl::XeKeysHmacShaUsingKey_entry)
REX_EXPORT(__imp__XeKeysConsolePrivateKeySign,
           rex::kernel::xboxkrnl::XeKeysConsolePrivateKeySign_entry)
REX_EXPORT(__imp__XeKeysConsoleSignatureVerification,
           rex::kernel::xboxkrnl::XeKeysConsoleSignatureVerification_entry)

REX_EXPORT_STUB(__imp__DevAuthGetStatistics);
REX_EXPORT_STUB(__imp__DevAuthShouldAlwaysEnforce);
