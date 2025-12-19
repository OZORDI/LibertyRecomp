#include "mapped_file.h"
#include <os/logger.h>
#include <cstring>
#include <algorithm>

namespace VFS
{
    MappedFile::~MappedFile()
    {
        Close();
    }
    
    MappedFile::MappedFile(MappedFile&& other) noexcept
        : m_path(std::move(other.m_path))
        , m_data(other.m_data)
        , m_size(other.m_size)
#ifdef _WIN32
        , m_fileHandle(other.m_fileHandle)
        , m_mapHandle(other.m_mapHandle)
#else
        , m_fd(other.m_fd)
#endif
    {
        other.m_data = nullptr;
        other.m_size = 0;
#ifdef _WIN32
        other.m_fileHandle = INVALID_HANDLE_VALUE;
        other.m_mapHandle = nullptr;
#else
        other.m_fd = -1;
#endif
    }
    
    MappedFile& MappedFile::operator=(MappedFile&& other) noexcept
    {
        if (this != &other)
        {
            Close();
            
            m_path = std::move(other.m_path);
            m_data = other.m_data;
            m_size = other.m_size;
#ifdef _WIN32
            m_fileHandle = other.m_fileHandle;
            m_mapHandle = other.m_mapHandle;
            other.m_fileHandle = INVALID_HANDLE_VALUE;
            other.m_mapHandle = nullptr;
#else
            m_fd = other.m_fd;
            other.m_fd = -1;
#endif
            other.m_data = nullptr;
            other.m_size = 0;
        }
        return *this;
    }
    
    bool MappedFile::Open(const std::filesystem::path& path)
    {
        Close();
        
        m_path = path;
        
#ifdef _WIN32
        // Windows implementation
        m_fileHandle = CreateFileW(
            path.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
            nullptr);
        
        if (m_fileHandle == INVALID_HANDLE_VALUE)
        {
            LOG_ERROR("[MappedFile] Failed to open file: {}", path.string());
            return false;
        }
        
        LARGE_INTEGER fileSize;
        if (!GetFileSizeEx(m_fileHandle, &fileSize))
        {
            LOG_ERROR("[MappedFile] Failed to get file size: {}", path.string());
            CloseHandle(m_fileHandle);
            m_fileHandle = INVALID_HANDLE_VALUE;
            return false;
        }
        m_size = static_cast<size_t>(fileSize.QuadPart);
        
        if (m_size == 0)
        {
            // Empty file - nothing to map
            return true;
        }
        
        m_mapHandle = CreateFileMappingW(
            m_fileHandle,
            nullptr,
            PAGE_READONLY,
            0,
            0,
            nullptr);
        
        if (!m_mapHandle)
        {
            LOG_ERROR("[MappedFile] Failed to create file mapping: {}", path.string());
            CloseHandle(m_fileHandle);
            m_fileHandle = INVALID_HANDLE_VALUE;
            return false;
        }
        
        m_data = static_cast<uint8_t*>(MapViewOfFile(
            m_mapHandle,
            FILE_MAP_READ,
            0,
            0,
            0));
        
        if (!m_data)
        {
            LOG_ERROR("[MappedFile] Failed to map view of file: {}", path.string());
            CloseHandle(m_mapHandle);
            CloseHandle(m_fileHandle);
            m_mapHandle = nullptr;
            m_fileHandle = INVALID_HANDLE_VALUE;
            return false;
        }
        
#else
        // Unix/macOS implementation
        m_fd = open(path.c_str(), O_RDONLY);
        if (m_fd < 0)
        {
            LOG_ERROR("[MappedFile] Failed to open file: {}", path.string());
            return false;
        }
        
        struct stat st;
        if (fstat(m_fd, &st) < 0)
        {
            LOG_ERROR("[MappedFile] Failed to stat file: {}", path.string());
            close(m_fd);
            m_fd = -1;
            return false;
        }
        m_size = static_cast<size_t>(st.st_size);
        
        if (m_size == 0)
        {
            // Empty file - nothing to map
            return true;
        }
        
        m_data = static_cast<uint8_t*>(mmap(
            nullptr,
            m_size,
            PROT_READ,
            MAP_PRIVATE,
            m_fd,
            0));
        
        if (m_data == MAP_FAILED)
        {
            LOG_ERROR("[MappedFile] Failed to mmap file: {}", path.string());
            m_data = nullptr;
            close(m_fd);
            m_fd = -1;
            return false;
        }
        
        // Default to sequential access hint
        madvise(m_data, m_size, MADV_SEQUENTIAL);
#endif
        
        LOG_UTILITY("[MappedFile] Mapped {} ({} bytes)", path.string(), m_size);
        return true;
    }
    
    void MappedFile::Close()
    {
        if (!m_data && m_size == 0)
        {
            // Nothing to close
#ifdef _WIN32
            if (m_fileHandle != INVALID_HANDLE_VALUE)
            {
                CloseHandle(m_fileHandle);
                m_fileHandle = INVALID_HANDLE_VALUE;
            }
#else
            if (m_fd >= 0)
            {
                close(m_fd);
                m_fd = -1;
            }
#endif
            return;
        }
        
#ifdef _WIN32
        if (m_data)
        {
            UnmapViewOfFile(m_data);
            m_data = nullptr;
        }
        if (m_mapHandle)
        {
            CloseHandle(m_mapHandle);
            m_mapHandle = nullptr;
        }
        if (m_fileHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_fileHandle);
            m_fileHandle = INVALID_HANDLE_VALUE;
        }
#else
        if (m_data)
        {
            munmap(m_data, m_size);
            m_data = nullptr;
        }
        if (m_fd >= 0)
        {
            close(m_fd);
            m_fd = -1;
        }
#endif
        
        m_size = 0;
        m_path.clear();
    }
    
    size_t MappedFile::Read(uint64_t offset, void* buffer, size_t length) const
    {
        if (!m_data || offset >= m_size)
        {
            return 0;
        }
        
        size_t available = m_size - static_cast<size_t>(offset);
        size_t toRead = std::min(length, available);
        
        std::memcpy(buffer, m_data + offset, toRead);
        return toRead;
    }
    
    void MappedFile::SetAccessHint(AccessHint hint)
    {
        if (!m_data || m_size == 0)
        {
            return;
        }
        
#ifdef _WIN32
        // Windows doesn't have madvise equivalent for mapped files
        // Could use PrefetchVirtualMemory for WillNeed hint on Windows 8+
        (void)hint;
#else
        int advice = MADV_NORMAL;
        switch (hint)
        {
            case AccessHint::Sequential:
                advice = MADV_SEQUENTIAL;
                break;
            case AccessHint::Random:
                advice = MADV_RANDOM;
                break;
            case AccessHint::WillNeed:
                advice = MADV_WILLNEED;
                break;
            case AccessHint::DontNeed:
                advice = MADV_DONTNEED;
                break;
        }
        madvise(m_data, m_size, advice);
#endif
    }
    
} // namespace VFS
