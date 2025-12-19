# VFS Advanced Features - Feasibility Study

## Overview

This document analyzes two advanced features for the Virtual File System (VFS):
1. **Memory-mapped files (mmap)** for large files
2. **Parallel extraction** for multi-threaded RPF unpacking

---

## 1. Memory-Mapped Files (mmap)

### What It Is
Memory-mapping allows the OS to map a file directly into the process's virtual address space. Instead of explicit read() calls, the application accesses file data through pointer dereferences, and the OS handles paging data in/out transparently.

### Benefits
| Benefit | Impact |
|---------|--------|
| **Zero-copy reads** | No buffer allocation or memcpy for large reads |
| **OS-managed caching** | Kernel optimizes page-in/page-out automatically |
| **Random access efficiency** | Seeking is free (just pointer arithmetic) |
| **Reduced syscall overhead** | No read() calls after initial mapping |
| **Shared memory potential** | Multiple handles can share same physical pages |

### GTA IV Use Case Analysis

**Large files in GTA IV:**
- `xbox360.rpf` - ~60MB (if serving raw RPF)
- Texture dictionaries (`.xtd`) - 1-10MB each
- Model files (`.wdr`) - 100KB-5MB each
- Audio banks - 10-50MB each

**Current I/O pattern (from `imports.cpp`):**
```cpp
// NtReadFile uses std::fstream with explicit seeks
hFile->stream.seekg(offset);
hFile->stream.read(buffer, length);
```

**Access patterns observed:**
- Sequential reads for streaming (models, textures)
- Random seeks for TOC/header parsing
- Small reads (12-4096 bytes) for metadata
- Large reads (64KB-1MB) for asset data

### Implementation Path

#### Phase 1: MappedFile Class
```cpp
// kernel/mapped_file.h
class MappedFile {
public:
    bool Open(const std::filesystem::path& path);
    void Close();
    
    const uint8_t* Data() const { return m_data; }
    size_t Size() const { return m_size; }
    
    // Read interface compatible with current usage
    size_t Read(uint64_t offset, void* buffer, size_t length);
    
private:
    uint8_t* m_data = nullptr;
    size_t m_size = 0;
#ifdef _WIN32
    HANDLE m_fileHandle = INVALID_HANDLE_VALUE;
    HANDLE m_mapHandle = nullptr;
#else
    int m_fd = -1;
#endif
};
```

#### Phase 2: Platform-Specific Implementation

**macOS/Linux:**
```cpp
bool MappedFile::Open(const std::filesystem::path& path) {
    m_fd = open(path.c_str(), O_RDONLY);
    if (m_fd < 0) return false;
    
    struct stat st;
    if (fstat(m_fd, &st) < 0) {
        close(m_fd);
        return false;
    }
    m_size = st.st_size;
    
    m_data = (uint8_t*)mmap(nullptr, m_size, PROT_READ, MAP_PRIVATE, m_fd, 0);
    if (m_data == MAP_FAILED) {
        close(m_fd);
        return false;
    }
    
    // Hint to kernel about access pattern
    madvise(m_data, m_size, MADV_SEQUENTIAL);  // or MADV_RANDOM
    
    return true;
}
```

**Windows:**
```cpp
bool MappedFile::Open(const std::filesystem::path& path) {
    m_fileHandle = CreateFileW(path.c_str(), GENERIC_READ, 
        FILE_SHARE_READ, nullptr, OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, nullptr);
    if (m_fileHandle == INVALID_HANDLE_VALUE) return false;
    
    LARGE_INTEGER size;
    GetFileSizeEx(m_fileHandle, &size);
    m_size = size.QuadPart;
    
    m_mapHandle = CreateFileMappingW(m_fileHandle, nullptr, 
        PAGE_READONLY, 0, 0, nullptr);
    if (!m_mapHandle) {
        CloseHandle(m_fileHandle);
        return false;
    }
    
    m_data = (uint8_t*)MapViewOfFile(m_mapHandle, FILE_MAP_READ, 0, 0, 0);
    if (!m_data) {
        CloseHandle(m_mapHandle);
        CloseHandle(m_fileHandle);
        return false;
    }
    
    return true;
}
```

#### Phase 3: VFS Integration
```cpp
// In vfs.h - add new handle type
struct VfsMappedHandle {
    MappedFile mapping;
    std::filesystem::path path;
    uint64_t position = 0;  // Current read position for sequential access
};

// In NtCreateFile - use mmap for large files
if (fileSize > MMAP_THRESHOLD) {  // e.g., 1MB
    auto* handle = new VfsMappedHandle();
    if (handle->mapping.Open(vfsResolved)) {
        // Return mapped handle
    }
}
```

### Feasibility Assessment

| Criteria | Rating | Notes |
|----------|--------|-------|
| **Complexity** | Medium | Platform-specific code required |
| **Performance gain** | High | 2-10x for large sequential reads |
| **Risk** | Low | Fallback to fstream is trivial |
| **Dependencies** | None | Uses OS primitives only |
| **Compatibility** | High | Works on all target platforms |

### Recommendation
**✅ RECOMMENDED** - Start with files >1MB, expand threshold based on profiling.

---

## 2. Parallel RPF Extraction

### What It Is
Extract multiple files from RPF archives simultaneously using worker threads, significantly reducing installation time.

### Current State
From `rpf_extractor.cpp`:
- Single-threaded extraction
- Falls back to SparkCLI (external process)
- Sequential file writes

### Benefits
| Benefit | Impact |
|---------|--------|
| **Faster installation** | 3-8x speedup on multi-core systems |
| **Better I/O utilization** | Overlap decompression with disk writes |
| **User experience** | Shorter wait during first-run setup |

### GTA IV RPF Analysis

**Archive sizes:**
- `common.rpf` - ~200MB, ~500 files
- `xbox360.rpf` - ~60MB, ~300 files  
- `audio.rpf` - ~50MB, ~100 files

**Bottlenecks:**
1. **I/O bound**: Reading from source RPF (single file)
2. **CPU bound**: AES decryption + zlib decompression
3. **I/O bound**: Writing extracted files

### Implementation Path

#### Phase 1: Thread Pool
```cpp
// install/thread_pool.h
class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
    ~ThreadPool();
    
    template<typename F>
    std::future<void> Enqueue(F&& task);
    
    void Wait();  // Wait for all tasks
    
private:
    std::vector<std::thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    bool m_stop = false;
};
```

#### Phase 2: Parallel Extraction Strategy

**Option A: File-level parallelism**
```cpp
ExtractionResult ExtractParallel(
    const std::filesystem::path& rpfPath,
    const std::filesystem::path& outputDir,
    const std::vector<uint8_t>& aesKey,
    size_t numThreads)
{
    // 1. Read TOC (single-threaded, fast)
    auto entries = ListEntries(rpfPath, aesKey);
    
    // 2. Pre-create directory structure
    for (auto& entry : entries) {
        if (entry.isDirectory) {
            std::filesystem::create_directories(outputDir / entry.name);
        }
    }
    
    // 3. Open RPF file with shared handle
    std::ifstream rpfFile(rpfPath, std::ios::binary);
    std::mutex rpfMutex;  // Protect seeks
    
    // 4. Process files in parallel
    ThreadPool pool(numThreads);
    std::atomic<uint64_t> filesExtracted{0};
    std::atomic<uint64_t> bytesExtracted{0};
    
    for (auto& entry : entries) {
        if (entry.isDirectory) continue;
        
        pool.Enqueue([&, entry]() {
            // Read compressed data (locked)
            std::vector<uint8_t> compressed(entry.compressedSize);
            {
                std::lock_guard<std::mutex> lock(rpfMutex);
                rpfFile.seekg(entry.offset);
                rpfFile.read((char*)compressed.data(), entry.compressedSize);
            }
            
            // Decrypt if needed (unlocked, CPU-bound)
            if (entry.isEncrypted) {
                DecryptAES256(compressed, aesKey);
            }
            
            // Decompress if needed (unlocked, CPU-bound)
            std::vector<uint8_t> decompressed;
            if (entry.isCompressed) {
                decompressed = DecompressZlib(compressed);
            } else {
                decompressed = std::move(compressed);
            }
            
            // Write output (no lock needed, different files)
            std::ofstream out(outputDir / entry.name, std::ios::binary);
            out.write((char*)decompressed.data(), decompressed.size());
            
            filesExtracted++;
            bytesExtracted += decompressed.size();
        });
    }
    
    pool.Wait();
    return {true, outputDir, filesExtracted, bytesExtracted, ""};
}
```

**Option B: Pipeline parallelism (more complex but higher throughput)**
```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│   Reader    │───▶│  Decryptor  │───▶│Decompressor │───▶│   Writer    │
│  (1 thread) │    │ (N threads) │    │ (N threads) │    │ (N threads) │
└─────────────┘    └─────────────┘    └─────────────┘    └─────────────┘
```

#### Phase 3: Memory Management
```cpp
// Limit concurrent memory usage
class MemoryBudget {
    std::atomic<size_t> m_used{0};
    const size_t m_limit;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    
public:
    explicit MemoryBudget(size_t limit) : m_limit(limit) {}
    
    void Acquire(size_t bytes) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [&]{ return m_used + bytes <= m_limit; });
        m_used += bytes;
    }
    
    void Release(size_t bytes) {
        m_used -= bytes;
        m_cv.notify_all();
    }
};
```

### Feasibility Assessment

| Criteria | Rating | Notes |
|----------|--------|-------|
| **Complexity** | Medium-High | Thread synchronization, memory management |
| **Performance gain** | High | 3-8x on 4+ core systems |
| **Risk** | Medium | Race conditions, memory pressure |
| **Dependencies** | C++17 | `<thread>`, `<mutex>`, `<future>` |
| **Compatibility** | High | Standard C++ threading |

### Recommendation
**✅ RECOMMENDED** - Implement Option A first (simpler), consider Option B for >8 cores.

---

## Implementation Priority

| Priority | Feature | Effort | Impact | Recommendation |
|----------|---------|--------|--------|----------------|
| **1** | Memory-mapped files | 2-3 days | High | Start here |
| **2** | Parallel extraction | 3-5 days | High | Second priority |

## Quick Start Implementation

### Step 1: Add mmap support (immediate win)
```cpp
// Add to kernel/mapped_file.h and kernel/mapped_file.cpp
// Integrate into NtCreateFile for files > 1MB
```

### Step 2: Add thread pool for extraction
```cpp
// Add to install/thread_pool.h
// Modify ExtractParallel() in rpf_extractor.cpp
```

---

## Dependencies Summary

| Feature | New Dependencies | Existing Dependencies |
|---------|------------------|----------------------|
| mmap | None (OS APIs) | - |
| Parallel | C++17 threading | - |

## Risks and Mitigations

| Risk | Mitigation |
|------|------------|
| mmap fails on large files | Fallback to fstream |
| Thread contention | Tune thread count, add profiling |
| Memory exhaustion | Memory budget limiter |

---

*Document created: December 2024*
*Status: Implemented - mmap and parallel extraction*
