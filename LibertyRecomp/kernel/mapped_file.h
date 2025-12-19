#pragma once

#include <filesystem>
#include <cstdint>
#include <cstddef>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace VFS
{
    /**
     * Memory-mapped file for efficient large file access.
     * Uses mmap on Unix/macOS and MapViewOfFile on Windows.
     * 
     * Benefits:
     * - Zero-copy reads (no buffer allocation)
     * - OS-managed caching
     * - Efficient random access
     * - Reduced syscall overhead
     */
    class MappedFile
    {
    public:
        MappedFile() = default;
        ~MappedFile();
        
        // Non-copyable
        MappedFile(const MappedFile&) = delete;
        MappedFile& operator=(const MappedFile&) = delete;
        
        // Movable
        MappedFile(MappedFile&& other) noexcept;
        MappedFile& operator=(MappedFile&& other) noexcept;
        
        /**
         * Open and map a file into memory.
         * @param path Path to the file to map
         * @return true if successful, false otherwise
         */
        bool Open(const std::filesystem::path& path);
        
        /**
         * Close the mapping and release resources.
         */
        void Close();
        
        /**
         * Check if file is currently mapped.
         */
        bool IsOpen() const { return m_data != nullptr; }
        
        /**
         * Get pointer to mapped data.
         */
        const uint8_t* Data() const { return m_data; }
        
        /**
         * Get size of mapped file.
         */
        size_t Size() const { return m_size; }
        
        /**
         * Get the file path.
         */
        const std::filesystem::path& Path() const { return m_path; }
        
        /**
         * Read data from the mapped file.
         * @param offset Byte offset from start of file
         * @param buffer Destination buffer
         * @param length Number of bytes to read
         * @return Number of bytes actually read
         */
        size_t Read(uint64_t offset, void* buffer, size_t length) const;
        
        /**
         * Hint to the OS about expected access pattern.
         */
        enum class AccessHint
        {
            Sequential,  // Will read sequentially
            Random,      // Will access randomly
            WillNeed,    // Will need this data soon
            DontNeed     // Won't need this data soon
        };
        
        /**
         * Provide access pattern hint to the OS.
         */
        void SetAccessHint(AccessHint hint);
        
        /**
         * Minimum file size to use mmap (smaller files use fstream).
         * Default: 1MB
         */
        static constexpr size_t MMAP_THRESHOLD = 1024 * 1024;
        
    private:
        std::filesystem::path m_path;
        uint8_t* m_data = nullptr;
        size_t m_size = 0;
        
#ifdef _WIN32
        HANDLE m_fileHandle = INVALID_HANDLE_VALUE;
        HANDLE m_mapHandle = nullptr;
#else
        int m_fd = -1;
#endif
    };
    
} // namespace VFS
