/*
 * XTD CLI Tool - Complete Xbox 360 XTD texture replacement via CLI
 *
 * Usage:
 *   xtd_cli.exe list <input.xtd>           - List textures in XTD
 *   xtd_cli.exe export <input.xtd> <dir>   - Export all textures as DDS
 *   xtd_cli.exe replace <input.xtd> <output.xtd> <mapping.json> - Replace
 * textures xtd_cli.exe replace <input.xtd> <output.xtd> <name> <dds>   -
 * Replace single texture
 *
 * Compile with mingw-w64:
 *   i686-w64-mingw32-gcc -o xtd_cli.exe xtd_cli.c -Wall -O2
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// LZX function pointers
typedef int(__cdecl *LZXinit_t)(int window);
typedef int(__cdecl *LZXdecompress_t)(const char *inData, int inlen,
                                      char *outData, int outlen);

typedef HRESULT(__stdcall *XMemCreateCompressionContext_t)(int CodecType,
                                                           void *pCodecParams,
                                                           int Flags,
                                                           int *pContext);
typedef void(__stdcall *XMemDestroyCompressionContext_t)(int Context);
typedef HRESULT(__stdcall *XMemResetCompressionContext_t)(int Context);
typedef HRESULT(__stdcall *XMemCompress_t)(int Context, void *pDest,
                                           int *pDestSize, void *pSrc,
                                           int SrcSize);

#define XMEMCODEC_LZX 1
#define XMEMCOMPRESS_STREAM 0x00000001

static HMODULE hXCompressCpp = NULL;
static HMODULE hXCompress = NULL;
static LZXinit_t pfnLZXinit = NULL;
static LZXdecompress_t pfnLZXdecompress = NULL;
static XMemCreateCompressionContext_t pfnXMemCreateCompressionContext = NULL;
static XMemDestroyCompressionContext_t pfnXMemDestroyCompressionContext = NULL;
static XMemResetCompressionContext_t pfnXMemResetCompressionContext = NULL;
static XMemCompress_t pfnXMemCompress = NULL;

// Structures
#pragma pack(push, 1)
typedef struct {
  uint32_t magic;         // RSC\x05 = 0x52534305
  uint32_t resource_type; // 0x07000000 = texture dictionary
  uint32_t flags;
  uint32_t reserved; // Usually 0x0F or compressed size marker
  uint32_t compressed_size;
} RSCHeader;

typedef struct {
  uint32_t vmt;
  uint32_t offset_map_offset;
  uint32_t hash_table_offset;
  uint32_t f_C;
  uint32_t f_10;
  uint16_t texture_count;
  uint16_t texture_count2;
  uint32_t texture_list_offset;
  uint16_t texture_count3;
  uint16_t texture_count4;
} XTDHeader;

typedef struct {
  uint32_t vmt;
  uint32_t f4;
  uint8_t f8;
  uint8_t f9;
  uint16_t fA;
  uint32_t fC;
  uint32_t f10;
  uint32_t name_ptr;
  uint32_t d3d_base_texture;
  uint16_t width;
  uint16_t height;
  uint32_t f20;
} GrcTextureXenon;

typedef struct {
  uint32_t dwords[13];
} D3DBaseTextureDWORDs;
#pragma pack(pop)

typedef struct {
  char name[64];
  uint32_t name_offset;
  uint32_t d3d_offset;
  uint16_t width;
  uint16_t height;
  uint32_t texture_type;
  uint32_t gpu_offset;
  uint32_t endian;
} TextureInfo;

// Utility functions
static uint32_t swap32(uint32_t val) {
  return ((val >> 24) & 0xFF) | ((val >> 8) & 0xFF00) |
         ((val << 8) & 0xFF0000) | ((val << 24) & 0xFF000000);
}

static uint16_t swap16(uint16_t val) {
  return ((val >> 8) & 0xFF) | ((val << 8) & 0xFF00);
}

static uint32_t get_offset(uint32_t offset) {
  if (offset == 0)
    return 0;
  uint32_t section = offset >> 28;
  if (section != 5 && section != 6)
    return 0;
  return offset & 0x0FFFFFFF;
}

static uint32_t data_offset(uint32_t val) {
  int j = val;
  j >>= 8;
  j <<= 16;
  j >>= 8;
  return j;
}

// Xbox 360 texture swizzling
static uint32_t xg_address_2d_tiled_offset(uint32_t x, uint32_t y, uint32_t w,
                                           uint32_t texel_pitch) {
  uint32_t aligned_width = (w + 31) & ~31;
  uint32_t log_bpp =
      (texel_pitch >> 2) + ((texel_pitch >> 1) >> (texel_pitch >> 2));
  uint32_t macro = ((x >> 5) + (y >> 5) * (aligned_width >> 5))
                   << (log_bpp + 7);
  uint32_t micro = (((x & 7) + ((y & 6) << 2)) << log_bpp);
  uint32_t offset = macro + ((micro & ~15) << 1) + (micro & 15) +
                    ((y & 8) << (3 + log_bpp)) + ((y & 1) << 4);
  return (((offset & ~511) << 3) + ((offset & 448) << 2) + (offset & 63) +
          ((y & 16) << 7) + ((((((y & 8) >> 2) + (x >> 3)) & 3) << 6))) >>
         log_bpp;
}

int load_dlls(void) {
  hXCompressCpp = LoadLibraryA("xcompress_cpp.dll");
  if (!hXCompressCpp) {
    fprintf(stderr, "Failed to load xcompress_cpp.dll\n");
    return 0;
  }

  pfnLZXinit = (LZXinit_t)GetProcAddress(hXCompressCpp, "LZXinit");
  pfnLZXdecompress =
      (LZXdecompress_t)GetProcAddress(hXCompressCpp, "LZXdecompress");

  if (!pfnLZXinit || !pfnLZXdecompress) {
    fprintf(stderr, "Failed to get LZX functions\n");
    return 0;
  }

  hXCompress = LoadLibraryA("xcompress.dll");
  if (hXCompress) {
    pfnXMemCreateCompressionContext =
        (XMemCreateCompressionContext_t)GetProcAddress(
            hXCompress, "XMemCreateCompressionContext");
    pfnXMemDestroyCompressionContext =
        (XMemDestroyCompressionContext_t)GetProcAddress(
            hXCompress, "XMemDestroyCompressionContext");
    pfnXMemResetCompressionContext =
        (XMemResetCompressionContext_t)GetProcAddress(
            hXCompress, "XMemResetCompressionContext");
    pfnXMemCompress =
        (XMemCompress_t)GetProcAddress(hXCompress, "XMemCompress");
  }

  return 1;
}

void unload_dlls(void) {
  if (hXCompressCpp)
    FreeLibrary(hXCompressCpp);
  if (hXCompress)
    FreeLibrary(hXCompress);
}

// Read LZX block size
int read_block_header(const uint8_t **ptr, const uint8_t *end,
                      uint16_t *uncompressed, uint16_t *compressed) {
  if (*ptr >= end)
    return 0;

  uint8_t b0 = *(*ptr)++;
  if (b0 == 0xFF) {
    if (*ptr + 4 > end)
      return -1;
    uint8_t b1 = *(*ptr)++;
    uint8_t b2 = *(*ptr)++;
    uint8_t b3 = *(*ptr)++;
    uint8_t b4 = *(*ptr)++;
    *uncompressed = (b1 << 8) | b2;
    *compressed = (b3 << 8) | b4;
  } else {
    if (*ptr >= end)
      return -1;
    uint8_t b1 = *(*ptr)++;
    *uncompressed = 0x8000;
    *compressed = (b0 << 8) | b1;
  }
  return 1;
}

// Decompress LZX data
uint8_t *decompress_lzx(const uint8_t *input, size_t input_size,
                        size_t *output_size) {
  pfnLZXinit(17);

  // First pass: calculate output size
  const uint8_t *ptr = input;
  const uint8_t *end = input + input_size;
  size_t total_size = 0;

  while (ptr < end) {
    uint16_t uncompressed, compressed;
    int result = read_block_header(&ptr, end, &uncompressed, &compressed);
    if (result <= 0)
      break;
    total_size += uncompressed;
    ptr += compressed;
  }

  if (total_size == 0)
    return NULL;

  // Second pass: decompress
  uint8_t *output = (uint8_t *)malloc(total_size);
  if (!output)
    return NULL;

  ptr = input;
  uint8_t *out_ptr = output;
  pfnLZXinit(17); // Reset

  while (ptr < end) {
    uint16_t uncompressed, compressed;
    int result = read_block_header(&ptr, end, &uncompressed, &compressed);
    if (result <= 0)
      break;

    pfnLZXdecompress((const char *)ptr, compressed, (char *)out_ptr,
                     uncompressed);
    ptr += compressed;
    out_ptr += uncompressed;
  }

  *output_size = total_size;
  return output;
}

// Compress data using LZX
uint8_t *compress_lzx(const uint8_t *input, size_t input_size,
                      size_t *output_size) {
  if (!pfnXMemCompress) {
    fprintf(stderr, "Compression not available\n");
    return NULL;
  }

  int context = 0;
  HRESULT hr = pfnXMemCreateCompressionContext(XMEMCODEC_LZX, NULL,
                                               XMEMCOMPRESS_STREAM, &context);
  if (FAILED(hr)) {
    fprintf(stderr, "Failed to create compression context\n");
    return NULL;
  }

  pfnXMemResetCompressionContext(context);

  int out_size = input_size * 2;
  uint8_t *output = (uint8_t *)malloc(out_size);

  hr = pfnXMemCompress(context, output, &out_size, (void *)input, input_size);
  pfnXMemDestroyCompressionContext(context);

  if (FAILED(hr)) {
    free(output);
    fprintf(stderr, "Compression failed\n");
    return NULL;
  }

  *output_size = out_size;
  return output;
}

// Parse XTD header and extract texture list
int parse_xtd(uint8_t *data, size_t size, TextureInfo **textures,
              int *texture_count) {
  if (size < sizeof(XTDHeader))
    return 0;

  XTDHeader *header = (XTDHeader *)data;

  // Swap big-endian values
  uint32_t vmt = swap32(header->vmt);
  uint32_t texture_list_offset = swap32(header->texture_list_offset);
  texture_list_offset = get_offset(texture_list_offset);
  uint16_t count = swap16(header->texture_count);

  if (count == 0 || texture_list_offset >= size) {
    fprintf(stderr, "Invalid XTD header: count=%d, offset=%u\n", count,
            texture_list_offset);
    return 0;
  }

  *texture_count = count;
  *textures = (TextureInfo *)calloc(count, sizeof(TextureInfo));

  // Read texture offsets
  uint32_t *offsets = (uint32_t *)malloc(count * sizeof(uint32_t));
  uint8_t *ptr = data + texture_list_offset;

  for (int i = 0; i < count; i++) {
    offsets[i] = get_offset(swap32(*(uint32_t *)(ptr + i * 4)));
  }

  // Read each texture info
  for (int i = 0; i < count; i++) {
    TextureInfo *tex = &(*textures)[i];
    GrcTextureXenon *grc = (GrcTextureXenon *)(data + offsets[i]);

    uint32_t name_offset = get_offset(swap32(grc->name_ptr));
    uint32_t d3d_offset = get_offset(swap32(grc->d3d_base_texture));
    tex->width = swap16(grc->width);
    tex->height = swap16(grc->height);
    tex->name_offset = name_offset;
    tex->d3d_offset = d3d_offset;

    // Read name
    if (name_offset < size) {
      char *name_ptr = (char *)(data + name_offset);
      // Remove "pack:/" prefix if present
      if (strncmp(name_ptr, "pack:/", 6) == 0) {
        name_ptr += 6;
      }
      strncpy(tex->name, name_ptr, 63);
      // Remove .dds extension for base name
      char *ext = strstr(tex->name, ".dds");
      if (ext)
        *ext = '\0';
    }

    // Read D3DBaseTexture info
    if (d3d_offset + 52 <= size) {
      uint8_t *d3d = data + d3d_offset;

      // D3DBaseTexture layout (big-endian):
      // +0x00: Common_Flags, Common_Type
      // +0x04: ReferenceCount
      // +0x08: Fence
      // +0x0C: ReadFence
      // +0x10: Identifier
      // +0x14: BaseFlush
      // +0x18: MipFlush
      // +0x1C: DWORD 8 - first GPU format DWORD (Tiled, Pitch, Clamp, Sign,
      // Type) +0x20: DWORD 9 - (BaseAddress, ClampPolicy, Stacked, RequestSize,
      // Endian, DataFormat) DataFormat is in bits 0-5 of DWORD 9 (after endian
      // swap)

      // Read DWORD at offset +0x20 (DWORD 9)
      uint32_t dword9 = swap32(*(uint32_t *)(d3d + 0x20));
      // DataFormat is in bits 0-5
      tex->texture_type = dword9 & 0x3F;

      // Endian is in bits 4-5 of DWORD 9
      tex->endian = (dword9 >> 4) & 0x3;

      // GPU offset is in DWORD at +0x20 (DWORD 9)
      // Swap to big-endian first, then extract BaseAddress bits
      // BaseAddress is in bits 8-27 (after swap), extracted via data_offset
      tex->gpu_offset = data_offset(dword9);
    }
  }

  free(offsets);
  return 1;
}

const char *get_texture_format_name(uint32_t type) {
  switch (type) {
  case 18:
    return "DXT1";
  case 19:
    return "DXT2/3";
  case 20:
    return "DXT4/5";
  case 2:
    return "8-bit";
  case 6:
    return "8888";
  case 49:
    return "DXN";
  case 59:
    return "DXT5A";
  default:
    return "Unknown";
  }
}

// Decompress the resource payload once so a host-side batch transformer can
// update every texture before a single recompression.
int cmd_unpack(const char *xtd_path, const char *output_path) {
  FILE *fin = fopen(xtd_path, "rb");
  if (!fin) {
    fprintf(stderr, "Cannot open: %s\n", xtd_path);
    return 1;
  }

  fseek(fin, 0, SEEK_END);
  size_t size = ftell(fin);
  fseek(fin, 0, SEEK_SET);
  if (size < 20) {
    fclose(fin);
    fprintf(stderr, "XTD is truncated: %s\n", xtd_path);
    return 1;
  }

  uint8_t *data = (uint8_t *)malloc(size);
  if (!data || fread(data, 1, size, fin) != size) {
    fclose(fin);
    free(data);
    fprintf(stderr, "Cannot read: %s\n", xtd_path);
    return 1;
  }
  fclose(fin);

  if (memcmp(data, "RSC\x05", 4) != 0) {
    free(data);
    fprintf(stderr, "Invalid RSC5 header: %s\n", xtd_path);
    return 1;
  }

  size_t decompressed_size = 0;
  uint8_t *decompressed =
      decompress_lzx(data + 20, size - 20, &decompressed_size);
  free(data);
  if (!decompressed) {
    fprintf(stderr, "Decompression failed\n");
    return 1;
  }

  FILE *fout = fopen(output_path, "wb");
  if (!fout || fwrite(decompressed, 1, decompressed_size, fout) !=
                   decompressed_size) {
    if (fout)
      fclose(fout);
    free(decompressed);
    fprintf(stderr, "Cannot write: %s\n", output_path);
    return 1;
  }
  fclose(fout);
  free(decompressed);
  printf("Unpacked %zu bytes to %s\n", decompressed_size, output_path);
  return 0;
}

// Recompress a host-modified resource payload while preserving the original
// 16-byte RSC header. The compressed-size field is regenerated.
int cmd_pack(const char *template_path, const char *input_path,
             const char *output_path) {
  FILE *ftemplate = fopen(template_path, "rb");
  if (!ftemplate) {
    fprintf(stderr, "Cannot open template: %s\n", template_path);
    return 1;
  }
  uint8_t header[16];
  if (fread(header, 1, sizeof(header), ftemplate) != sizeof(header)) {
    fclose(ftemplate);
    fprintf(stderr, "Template is truncated: %s\n", template_path);
    return 1;
  }
  fclose(ftemplate);
  if (memcmp(header, "RSC\x05", 4) != 0) {
    fprintf(stderr, "Invalid RSC5 template: %s\n", template_path);
    return 1;
  }

  FILE *fin = fopen(input_path, "rb");
  if (!fin) {
    fprintf(stderr, "Cannot open payload: %s\n", input_path);
    return 1;
  }
  fseek(fin, 0, SEEK_END);
  size_t input_size = ftell(fin);
  fseek(fin, 0, SEEK_SET);
  uint8_t *input = (uint8_t *)malloc(input_size);
  if (!input || fread(input, 1, input_size, fin) != input_size) {
    fclose(fin);
    free(input);
    fprintf(stderr, "Cannot read payload: %s\n", input_path);
    return 1;
  }
  fclose(fin);

  size_t compressed_size = 0;
  uint8_t *compressed = compress_lzx(input, input_size, &compressed_size);
  free(input);
  if (!compressed) {
    fprintf(stderr, "Compression failed\n");
    return 1;
  }

  FILE *fout = fopen(output_path, "wb");
  if (!fout) {
    free(compressed);
    fprintf(stderr, "Cannot create output: %s\n", output_path);
    return 1;
  }
  uint32_t compressed_size_be = swap32((uint32_t)compressed_size);
  int write_failed =
      fwrite(header, 1, sizeof(header), fout) != sizeof(header) ||
      fwrite(&compressed_size_be, 1, sizeof(compressed_size_be), fout) !=
          sizeof(compressed_size_be) ||
      fwrite(compressed, 1, compressed_size, fout) != compressed_size;
  fclose(fout);
  free(compressed);
  if (write_failed) {
    fprintf(stderr, "Cannot write output: %s\n", output_path);
    return 1;
  }
  printf("Packed %zu bytes to %s\n", input_size, output_path);
  return 0;
}

// List textures in XTD
int cmd_list(const char *xtd_path) {
  FILE *fin = fopen(xtd_path, "rb");
  if (!fin) {
    fprintf(stderr, "Cannot open: %s\n", xtd_path);
    return 1;
  }

  fseek(fin, 0, SEEK_END);
  size_t size = ftell(fin);
  fseek(fin, 0, SEEK_SET);

  uint8_t *data = (uint8_t *)malloc(size);
  fread(data, 1, size, fin);
  fclose(fin);

  // Decompress
  size_t decompressed_size;
  uint8_t *decompressed =
      decompress_lzx(data + 20, size - 20, &decompressed_size);
  free(data);

  if (!decompressed) {
    fprintf(stderr, "Decompression failed\n");
    return 1;
  }

  // Parse
  TextureInfo *textures;
  int texture_count;

  if (!parse_xtd(decompressed, decompressed_size, &textures, &texture_count)) {
    free(decompressed);
    return 1;
  }

  printf("Textures in %s:\n", xtd_path);
  printf("%-4s %-32s %6s %6s %-10s %s\n", "#", "Name", "Width", "Height",
         "Format", "GPU Offset");
  printf("----------------------------------------------------------------\n");

  for (int i = 0; i < texture_count; i++) {
    printf("%-4d %-32s %6d %6d %-10s 0x%08X\n", i, textures[i].name,
           textures[i].width, textures[i].height,
           get_texture_format_name(textures[i].texture_type),
           textures[i].gpu_offset);
  }

  printf("----------------------------------------------------------------\n");
  printf("Total: %d textures\n", texture_count);

  free(textures);
  free(decompressed);
  return 0;
}

// De-swizzle Xbox 360 texture (reverse of xg_address_2d_tiled_offset)
void deswizzle_texture(const uint8_t *src, uint8_t *dst, uint32_t width,
                       uint32_t height, uint32_t texel_pitch) {
  uint32_t block_w = width / 4;
  uint32_t block_h = height / 4;

  for (uint32_t y = 0; y < block_h; y++) {
    for (uint32_t x = 0; x < block_w; x++) {
      uint32_t swizzled_offset =
          xg_address_2d_tiled_offset(x, y, block_w, texel_pitch) * texel_pitch;
      uint32_t linear_offset = (x + y * block_w) * texel_pitch;
      memcpy(dst + linear_offset, src + swizzled_offset, texel_pitch);
    }
  }
}

// Write DDS file header for DXT5
void write_dds_header(FILE *f, uint32_t width, uint32_t height) {
  uint8_t header[128] = {0};

  // DDS magic
  header[0] = 'D';
  header[1] = 'D';
  header[2] = 'S';
  header[3] = ' ';

  // DDS_HEADER size (124)
  *(uint32_t *)(header + 4) = 124;

  // Flags: DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT |
  // DDSD_LINEARSIZE
  *(uint32_t *)(header + 8) = 0x1007;

  // Height, Width
  *(uint32_t *)(header + 12) = height;
  *(uint32_t *)(header + 16) = width;

  // Linear size (for DXT5: width * height)
  *(uint32_t *)(header + 20) = width * height;

  // Pixel format offset 76, size 32
  *(uint32_t *)(header + 76) = 32;  // Size
  *(uint32_t *)(header + 80) = 0x4; // Flags: DDPF_FOURCC
  header[84] = 'D';                 // FourCC: DXT5
  header[85] = 'X';
  header[86] = 'T';
  header[87] = '5';

  // Caps
  *(uint32_t *)(header + 108) = 0x1000; // DDSCAPS_TEXTURE

  fwrite(header, 1, 128, f);
}

// Export textures from XTD
int cmd_export(const char *xtd_path, const char *output_dir) {
  FILE *fin = fopen(xtd_path, "rb");
  if (!fin) {
    fprintf(stderr, "Cannot open: %s\n", xtd_path);
    return 1;
  }

  fseek(fin, 0, SEEK_END);
  size_t size = ftell(fin);
  fseek(fin, 0, SEEK_SET);

  uint8_t *data = (uint8_t *)malloc(size);
  fread(data, 1, size, fin);
  fclose(fin);

  // Keep original header
  uint8_t header[20];
  memcpy(header, data, 20);

  // Decompress
  size_t decompressed_size;
  uint8_t *decompressed =
      decompress_lzx(data + 20, size - 20, &decompressed_size);
  free(data);

  if (!decompressed) {
    fprintf(stderr, "Decompression failed\n");
    return 1;
  }

  // Parse RSC header to get CPU size
  uint32_t magic = *(uint32_t *)(header);
  uint32_t flags = *(uint32_t *)(header + 8);
  if (magic == 0x52534305) {
    flags = swap32(flags);
  }
  uint32_t cpu_size = (flags & 0x7FF) << (((flags >> 11) & 0xF) + 8);

  // Parse textures
  TextureInfo *textures;
  int texture_count;

  if (!parse_xtd(decompressed, decompressed_size, &textures, &texture_count)) {
    free(decompressed);
    return 1;
  }

  // Create output directory
  CreateDirectoryA(output_dir, NULL);

  printf("Exporting %d textures to %s\n", texture_count, output_dir);

  for (int i = 0; i < texture_count; i++) {
    TextureInfo *tex = &textures[i];
    char output_path[512];
    snprintf(output_path, sizeof(output_path), "%s\\%s.dds", output_dir,
             tex->name);

    // Calculate texture parameters
    uint32_t w = tex->width;
    uint32_t h = tex->height;
    uint32_t texel_pitch = 16; // DXT5
    uint32_t aligned_w = w, aligned_h = h;

    if (aligned_w % 128)
      aligned_w += 128 - (aligned_w % 128);
    if (aligned_h % 128)
      aligned_h += 128 - (aligned_h % 128);

    uint32_t tex_size = aligned_w * aligned_h; // DXT5
    uint32_t linear_size = (w / 4) * (h / 4) * texel_pitch;

    // Get GPU data offset
    uint32_t gpu_offset = cpu_size + tex->gpu_offset;

    if (gpu_offset + tex_size > decompressed_size) {
      printf("  [%d] %s - SKIP (offset out of bounds)\n", i, tex->name);
      continue;
    }

    // Allocate buffers
    uint8_t *swizzled = decompressed + gpu_offset;
    uint8_t *linear = (uint8_t *)calloc(linear_size, 1);

    // De-swizzle
    deswizzle_texture(swizzled, linear, w, h, texel_pitch);

    // Reverse endian swap (Xbox 360 stores data big-endian)
    for (size_t j = 0; j + 1 < linear_size; j += 2) {
      uint8_t tmp = linear[j];
      linear[j] = linear[j + 1];
      linear[j + 1] = tmp;
    }

    // Write DDS file
    FILE *fout = fopen(output_path, "wb");
    if (fout) {
      write_dds_header(fout, w, h);
      fwrite(linear, 1, linear_size, fout);
      fclose(fout);
      printf("  [%d] %s (%dx%d) -> OK\n", i, tex->name, w, h);
    } else {
      printf("  [%d] %s - FAIL (cannot write)\n", i, tex->name);
    }

    free(linear);
  }

  free(textures);
  free(decompressed);

  printf("Export complete.\n");
  return 0;
}

// Replace textures in XTD
int cmd_replace(const char *xtd_path, const char *output_path, const char *name,
                const char *dds_path) {
  FILE *fin = fopen(xtd_path, "rb");
  if (!fin) {
    fprintf(stderr, "Cannot open: %s\n", xtd_path);
    return 1;
  }

  fseek(fin, 0, SEEK_END);
  size_t size = ftell(fin);
  fseek(fin, 0, SEEK_SET);

  uint8_t *data = (uint8_t *)malloc(size);
  fread(data, 1, size, fin);
  fclose(fin);

  // Keep original header
  uint8_t header[20];
  memcpy(header, data, 20);

  // Decompress
  size_t decompressed_size;
  uint8_t *decompressed =
      decompress_lzx(data + 20, size - 20, &decompressed_size);
  free(data);

  if (!decompressed) {
    fprintf(stderr, "Decompression failed\n");
    return 1;
  }

  // Parse
  TextureInfo *textures;
  int texture_count;

  if (!parse_xtd(decompressed, decompressed_size, &textures, &texture_count)) {
    free(decompressed);
    return 1;
  }

  // Find texture
  int found = -1;
  for (int i = 0; i < texture_count; i++) {
    if (strcmp(textures[i].name, name) == 0) {
      found = i;
      break;
    }
  }

  if (found < 0) {
    fprintf(stderr, "Texture '%s' not found in XTD\n", name);
    free(textures);
    free(decompressed);
    return 1;
  }

  TextureInfo *tex = &textures[found];
  printf("Replacing texture: %s (%dx%d, %s)\n", tex->name, tex->width,
         tex->height, get_texture_format_name(tex->texture_type));

  // Read DDS file
  FILE *fdds = fopen(dds_path, "rb");
  if (!fdds) {
    fprintf(stderr, "Cannot open DDS: %s\n", dds_path);
    free(textures);
    free(decompressed);
    return 1;
  }

  fseek(fdds, 0, SEEK_END);
  size_t dds_size = ftell(fdds);
  fseek(fdds, 128, SEEK_SET); // Skip DDS header
  size_t dds_data_size = dds_size - 128;

  uint8_t *dds_data = (uint8_t *)malloc(dds_data_size);
  fread(dds_data, 1, dds_data_size, fdds);
  fclose(fdds);

  // Calculate texture size and parameters based on format
  uint32_t w = tex->width;
  uint32_t h = tex->height;
  uint32_t tex_size, block_w, block_h, texel_pitch;
  int use_swizzle = 1;
  int use_endian_swap = 1;

  switch (tex->texture_type) {
  case 18: // DXT1
    if (w % 128)
      w += 128 - (w % 128);
    if (h % 128)
      h += 128 - (h % 128);
    tex_size = w * h / 2;
    block_w = tex->width / 4;
    block_h = tex->height / 4;
    texel_pitch = 8;
    break;
  case 19: // DXT2/3
  case 20: // DXT4/5
    if (w % 128)
      w += 128 - (w % 128);
    if (h % 128)
      h += 128 - (h % 128);
    tex_size = w * h;
    block_w = tex->width / 4;
    block_h = tex->height / 4;
    texel_pitch = 16;
    break;
  default:
    fprintf(stderr, "Unsupported texture format: %s\n",
            get_texture_format_name(tex->texture_type));
    free(dds_data);
    free(textures);
    free(decompressed);
    return 1;
  }

  printf("  Texture size: %u bytes, DDS data size: %zu bytes\n", tex_size,
         dds_data_size);

  // Allocate swizzled buffer
  uint8_t *swizzled = (uint8_t *)calloc(tex_size, 1);

  // Perform endian swap first (for DXT, swap 2 bytes at a time)
  if (use_endian_swap) {
    for (size_t i = 0; i + 1 < dds_data_size; i += 2) {
      uint8_t tmp = dds_data[i];
      dds_data[i] = dds_data[i + 1];
      dds_data[i + 1] = tmp;
    }
  }

  // Perform swizzling
  if (use_swizzle) {
    for (uint32_t y = 0; y < block_h; y++) {
      for (uint32_t x = 0; x < block_w; x++) {
        uint32_t src_offset = (x + y * block_w) * texel_pitch;
        uint32_t dst_offset =
            xg_address_2d_tiled_offset(x, y, block_w, texel_pitch) *
            texel_pitch;

        if (src_offset + texel_pitch <= dds_data_size &&
            dst_offset + texel_pitch <= tex_size) {
          memcpy(swizzled + dst_offset, dds_data + src_offset, texel_pitch);
        }
      }
    }
  } else {
    memcpy(swizzled, dds_data,
           dds_data_size < tex_size ? dds_data_size : tex_size);
  }

  // Write swizzled data back to decompressed buffer
  // GPU data is stored after CPU segment in decompressed data
  // For GTA IV XTD, GPU offset is relative to GPU segment start
  // CPU segment is typically at start, GPU segment follows

  // Parse RSC header to get CPU/GPU sizes
  // Check signature byte order to determine if flags need swapping
  uint32_t magic = *(uint32_t *)(header);
  uint32_t flags = *(uint32_t *)(header + 8);

  // If signature is big-endian (0x52534305), swap the flags
  // If signature is little-endian (0x05435352), use flags as-is
  if (magic == 0x52534305) {
    flags = swap32(flags);
  }

  uint32_t cpu_size = (flags & 0x7FF) << (((flags >> 11) & 0xF) + 8);
  uint32_t gpu_size = ((flags >> 15) & 0x7FF) << (((flags >> 26) & 0xF) + 8);

  // The gpu_offset from texture info is the offset within the GPU segment
  // GPU segment starts at cpu_size in the decompressed data
  uint32_t write_offset = cpu_size + tex->gpu_offset;

  printf("  Writing to offset: 0x%08X (CPU size=%u, GPU size=%u, tex GPU "
         "offset=0x%X)\n",
         write_offset, cpu_size, gpu_size, tex->gpu_offset);

  if (write_offset + tex_size <= decompressed_size) {
    memcpy(decompressed + write_offset, swizzled, tex_size);
    printf("  ✓ Texture data replaced\n");
  } else {
    fprintf(stderr, "  ERROR: Offset out of bounds (0x%08X + %u > %zu)\n",
            write_offset, tex_size, decompressed_size);
    free(swizzled);
    free(dds_data);
    free(textures);
    free(decompressed);
    return 1;
  }

  free(swizzled);
  free(dds_data);

  // Recompress
  printf("Recompressing...\n");
  size_t compressed_size;
  uint8_t *compressed =
      compress_lzx(decompressed, decompressed_size, &compressed_size);

  if (!compressed) {
    fprintf(stderr, "Compression failed\n");
    free(textures);
    free(decompressed);
    return 1;
  }

  // Write output
  FILE *fout = fopen(output_path, "wb");
  if (!fout) {
    fprintf(stderr, "Cannot create output: %s\n", output_path);
    free(compressed);
    free(textures);
    free(decompressed);
    return 1;
  }

  // Write RSC header (16 bytes) + compressed size (4 bytes big-endian) +
  // compressed data
  fwrite(header, 1, 16, fout);
  uint32_t comp_size_be = swap32((uint32_t)compressed_size);
  fwrite(&comp_size_be, 4, 1, fout);
  fwrite(compressed, 1, compressed_size, fout);
  fclose(fout);

  printf("Output saved: %s (%zu bytes)\n", output_path, 20 + compressed_size);

  free(compressed);
  free(textures);
  free(decompressed);
  return 0;
}

void print_usage(const char *prog) {
  printf("XTD CLI - Xbox 360 Texture Dictionary Tool\n\n");
  printf("Usage:\n");
  printf("  %s list <input.xtd>\n", prog);
  printf("      List all textures in an XTD file\n\n");
  printf("  %s export <input.xtd> <output_dir>\n", prog);
  printf("      Export texture info (names, dimensions, formats)\n\n");
  printf("  %s unpack <input.xtd> <output.bin>\n", prog);
  printf("      Decompress an XTD resource payload for host-side batch edits\n\n");
  printf("  %s pack <template.xtd> <input.bin> <output.xtd>\n", prog);
  printf("      Recompress a batch-edited payload using the template header\n\n");
  printf("  %s replace <input.xtd> <output.xtd> <texture_name> <dds_file>\n",
         prog);
  printf("      Replace a single texture with a DDS file\n\n");
  printf("Examples:\n");
  printf("  %s list buttons_360.xtd\n", prog);
  printf("  %s replace buttons_360.xtd output.xtd a_butt new_button.dds\n",
         prog);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  if (!load_dlls()) {
    fprintf(stderr, "Failed to load required DLLs\n");
    return 1;
  }

  int result = 1;

  if (strcmp(argv[1], "list") == 0 && argc >= 3) {
    result = cmd_list(argv[2]);
  } else if (strcmp(argv[1], "export") == 0 && argc >= 4) {
    result = cmd_export(argv[2], argv[3]);
  } else if (strcmp(argv[1], "unpack") == 0 && argc >= 4) {
    result = cmd_unpack(argv[2], argv[3]);
  } else if (strcmp(argv[1], "pack") == 0 && argc >= 5) {
    result = cmd_pack(argv[2], argv[3], argv[4]);
  } else if (strcmp(argv[1], "replace") == 0 && argc >= 6) {
    result = cmd_replace(argv[2], argv[3], argv[4], argv[5]);
  } else {
    print_usage(argv[0]);
  }

  unload_dlls();
  return result;
}
