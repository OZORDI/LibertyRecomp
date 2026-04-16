/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2020 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <rex/memory/mapped_memory.h>
#include <rex/platform.h>

#if REX_PLATFORM_NX
// ── Switch stub ──────────────────────────────────────────────────────────────
// Switch has no mmap. Provide a read-into-malloc fallback for MappedMemory
// and a nullptr stub for ChunkedMappedMemoryWriter.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>

#include <rex/filesystem.h>
#include <rex/logging.h>

// XELOGE is a Xenia-era macro not available in RexGlue; use fprintf instead.
#ifndef XELOGE
#define XELOGE(fmt, ...) fprintf(stderr, "[mapped_memory] " fmt "\n", ##__VA_ARGS__)
#endif

namespace rex::memory {

class MallocMappedMemory : public MappedMemory {
 public:
  MallocMappedMemory(void* data, size_t size, bool owns,
                     std::string path, bool writable)
      : MappedMemory(data, size), owns_(owns),
        path_(std::move(path)), writable_(writable) {}

  ~MallocMappedMemory() override {
    // Ensure write-back happens even if Close() was not called explicitly.
    if (data_ && owns_) {
      FlushToDisk();
      free(data_);
      data_ = nullptr;
    }
  }

  void Close(uint64_t truncate_size) override {
    if (data_ && owns_) {
      FlushToDisk(truncate_size);
      free(data_);
      data_ = nullptr;
    }
  }

  void Flush() override { FlushToDisk(); }

 private:
  void FlushToDisk(uint64_t truncate_size = 0) {
    if (!writable_ || !data_ || path_.empty()) return;
    size_t write_size = truncate_size ? static_cast<size_t>(truncate_size) : size();
    if (write_size > size()) write_size = size();
    FILE* f = std::fopen(path_.c_str(), "wb");
    if (!f) {
      XELOGE("MallocMappedMemory: failed to open '{}' for write-back", path_);
      return;
    }
    size_t written = std::fwrite(data_, 1, write_size, f);
    std::fclose(f);
    if (written != write_size) {
      XELOGE("MallocMappedMemory: short write to '{}' ({}/{})", path_,
             written, write_size);
    }
  }

  bool owns_;
  std::string path_;
  bool writable_;
};

std::unique_ptr<MappedMemory> MappedMemory::Open(const std::filesystem::path& path, Mode mode,
                                                 size_t offset, size_t length) {
  FILE* f = std::fopen(path.c_str(), (mode == Mode::kReadWrite) ? "r+b" : "rb");
  if (!f) return nullptr;

  // Determine file size if length not specified.
  if (!length) {
    std::fseek(f, 0, SEEK_END);
    long sz = std::ftell(f);
    if (sz <= 0) { std::fclose(f); return nullptr; }
    length = static_cast<size_t>(sz);
    if (offset >= length) { std::fclose(f); return nullptr; }
    length -= offset;
  }

  void* buf = malloc(length);
  if (!buf) { std::fclose(f); return nullptr; }

  std::fseek(f, static_cast<long>(offset), SEEK_SET);
  size_t rd = std::fread(buf, 1, length, f);
  std::fclose(f);

  if (rd != length) {
    free(buf);
    return nullptr;
  }

  bool writable = (mode == Mode::kReadWrite);
  return std::make_unique<MallocMappedMemory>(buf, length, true,
                                              path.string(), writable);
}

std::unique_ptr<ChunkedMappedMemoryWriter> ChunkedMappedMemoryWriter::Open(
    const std::filesystem::path& path, size_t chunk_size, bool low_address_space) {
  XELOGE("ChunkedMappedMemoryWriter::Open not implemented on Switch (no mmap). "
         "Requested path: '{}'", path.string());
  return nullptr;
}

}  // namespace rex::memory

#else  // !REX_PLATFORM_NX — POSIX (Linux / macOS / PS4 / Android)

#include <memory>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <rex/filesystem.h>
#include <rex/memory/mapped_memory.h>

#if defined(__APPLE__) || REX_PLATFORM_PS4
// macOS and PS4 (FreeBSD-based) use 64-bit file offsets natively; no *64 variants needed.
#  define stat64      stat
#  define fstat64     fstat
#  define ftruncate64 ftruncate
typedef off_t off64_t;
#endif  // __APPLE__ || REX_PLATFORM_PS4

namespace rex::memory {

class PosixMappedMemory : public MappedMemory {
 public:
  PosixMappedMemory(void* data, size_t size, int file_descriptor)
      : MappedMemory(data, size), file_descriptor_(file_descriptor) {}

  ~PosixMappedMemory() override {
    if (data_) {
      munmap(data_, size());
    }
    if (file_descriptor_ >= 0) {
      close(file_descriptor_);
    }
  }

  static std::unique_ptr<PosixMappedMemory> WrapFileDescriptor(int file_descriptor, Mode mode,
                                                               size_t offset = 0,
                                                               size_t length = 0) {
    int protection = 0;
    switch (mode) {
      case Mode::kRead:
        protection |= PROT_READ;
        break;
      case Mode::kReadWrite:
        protection |= PROT_READ | PROT_WRITE;
        break;
    }

    size_t map_length = length;
    if (!length) {
      struct stat64 file_stat;
      if (fstat64(file_descriptor, &file_stat)) {
        close(file_descriptor);
        return nullptr;
      }
      map_length = size_t(file_stat.st_size);
    }

    void* data =
        mmap(0, map_length, protection, MAP_SHARED, file_descriptor, static_cast<off_t>(offset));
    if (!data || data == MAP_FAILED) {
      close(file_descriptor);
      return nullptr;
    }

    return std::make_unique<PosixMappedMemory>(data, map_length, file_descriptor);
  }

  void Close(uint64_t truncate_size) override {
    if (data_) {
      munmap(data_, size());
      data_ = nullptr;
    }
    if (file_descriptor_ >= 0) {
      if (truncate_size) {
        ftruncate64(file_descriptor_, off64_t(truncate_size));
      }
      close(file_descriptor_);
      file_descriptor_ = -1;
    }
  }

  void Flush() override { msync(data(), size(), MS_ASYNC); }

 private:
  int file_descriptor_;
};

std::unique_ptr<MappedMemory> MappedMemory::Open(const std::filesystem::path& path, Mode mode,
                                                 size_t offset, size_t length) {
  int open_flags = 0;
  switch (mode) {
    case Mode::kRead:
      open_flags |= O_RDONLY;
      break;
    case Mode::kReadWrite:
      open_flags |= O_RDWR;
      break;
  }
  int file_descriptor = open(path.c_str(), open_flags);
  if (file_descriptor < 0) {
    return nullptr;
  }
  return PosixMappedMemory::WrapFileDescriptor(file_descriptor, mode, offset, length);
}

#if REX_PLATFORM_ANDROID
std::unique_ptr<MappedMemory> MappedMemory::OpenForAndroidContentUri(const std::string_view uri,
                                                                     Mode mode, size_t offset,
                                                                     size_t length) {
  const char* open_mode = nullptr;
  switch (mode) {
    case Mode::kRead:
      open_mode = "r";
      break;
    case Mode::kReadWrite:
      open_mode = "rw";
      break;
  }
  int file_descriptor = rex::filesystem::OpenAndroidContentFileDescriptor(uri, open_mode);
  if (file_descriptor < 0) {
    return nullptr;
  }
  return PosixMappedMemory::WrapFileDescriptor(file_descriptor, mode, offset, length);
}
#endif  // REX_PLATFORM_ANDROID

std::unique_ptr<ChunkedMappedMemoryWriter> ChunkedMappedMemoryWriter::Open(
    const std::filesystem::path& path, size_t chunk_size, bool low_address_space) {
  // TODO: Implement if needed
  return nullptr;
}

}  // namespace rex::memory

#endif  // REX_PLATFORM_NX
