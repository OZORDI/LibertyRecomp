#include "persistent_state.h"

#include <array>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <limits>
#include <system_error>
#include <utility>
#include <vector>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace libserver {
namespace {

constexpr const char* kEnvelopeFormat = "libserver-state";
constexpr std::size_t kIoBufferSize = 16384;
#if defined(_WIN32)
constexpr std::size_t kWindowsMaximumWrite = 4294967295ULL;
#endif

#if !defined(_WIN32)
std::string SystemErrorDetail(const std::string& operation, int error) {
    return operation + ": " + std::error_code(error, std::generic_category()).message();
}
#endif

#if defined(_WIN32)
std::string WindowsErrorDetail(const std::string& operation, DWORD error) {
    return operation + ": win32 error " + std::to_string(error);
}

bool IsReparsePoint(const std::filesystem::path& path, DWORD* error) {
    const DWORD attributes = GetFileAttributesW(path.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        *error = GetLastError();
        return false;
    }
    *error = ERROR_SUCCESS;
    return (attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
}
#endif

} // namespace

PersistentState::PersistentState(Options options) : options_(std::move(options)) {}

PersistentState::~PersistentState() {
    ReleaseLock();
}

bool PersistentState::Open() {
    health_ = Health::kInitializing;
    error_code_ = ErrorCode::kNone;
    error_detail_.clear();
    payload_ = nlohmann::json::object();
    opened_ = false;
    durability_ambiguous_ = false;
    has_snapshot_ = false;
    ReleaseLock();

    if (options_.data_directory.empty() || options_.state_filename.empty() ||
        options_.schema_version == 0 || options_.maximum_snapshot_bytes == 0) {
        return Fail(ErrorCode::kInvalidOptions,
                    "data directory, state filename, schema version, and snapshot limit are required");
    }
    const std::filesystem::path filename(options_.state_filename);
    if (filename.is_absolute() || filename.has_parent_path() || filename.filename() != filename) {
        return Fail(ErrorCode::kInvalidOptions, "state filename must be a single relative path component");
    }
    if (ShouldFault(FaultPoint::kBeforeDirectoryCreate)) {
        return Fail(ErrorCode::kFaultInjected, "fault before directory creation");
    }
    if (!EnsureDataDirectory()) {
        return false;
    }

    state_path_ = options_.data_directory / filename;
    lock_path_ = options_.data_directory / ".libserver-state.lock";
    if (!AcquireLock()) {
        return false;
    }
    if (ShouldFault(FaultPoint::kAfterLockAcquire)) {
        ReleaseLock();
        return Fail(ErrorCode::kFaultInjected, "fault after lock acquisition");
    }
    if (!LoadSnapshot()) {
        ReleaseLock();
        return false;
    }
    opened_ = true;
    Ready();
    return true;
}

bool PersistentState::EnsureDataDirectory() {
    std::error_code error;
    const std::filesystem::file_status initial_status =
        std::filesystem::symlink_status(options_.data_directory, error);
    if (error && error != std::errc::no_such_file_or_directory) {
        return Fail(ErrorCode::kDirectoryCreateFailed,
                    "cannot inspect data directory: " + error.message());
    }
    if (!error && std::filesystem::exists(initial_status) &&
        (std::filesystem::is_symlink(initial_status) ||
         !std::filesystem::is_directory(initial_status))) {
        return Fail(ErrorCode::kDirectoryInvalid, "data directory is not a real directory");
    }

    error.clear();
    std::filesystem::create_directories(options_.data_directory, error);
    if (error) {
        return Fail(ErrorCode::kDirectoryCreateFailed,
                    "cannot create data directory: " + error.message());
    }

    error.clear();
    const std::filesystem::file_status final_status =
        std::filesystem::symlink_status(options_.data_directory, error);
    if (error || std::filesystem::is_symlink(final_status) ||
        !std::filesystem::is_directory(final_status)) {
        return Fail(ErrorCode::kDirectoryInvalid, "data directory changed during creation");
    }

#if !defined(_WIN32)
    if (::chmod(options_.data_directory.c_str(), S_IRWXU) != 0) {
        return Fail(ErrorCode::kDirectoryPermissionsFailed,
                    SystemErrorDetail("chmod data directory", errno));
    }
#endif
    return true;
}

bool PersistentState::AcquireLock() {
#if defined(_WIN32)
    const HANDLE handle =
        CreateFileW(lock_path_.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
    if (handle == INVALID_HANDLE_VALUE) {
        const DWORD error = GetLastError();
        if (error == ERROR_SHARING_VIOLATION || error == ERROR_LOCK_VIOLATION) {
            return Fail(ErrorCode::kLockAlreadyHeld, WindowsErrorDetail("open lock", error));
        }
        return Fail(ErrorCode::kLockOpenFailed, WindowsErrorDetail("open lock", error));
    }
    BY_HANDLE_FILE_INFORMATION information{};
    if (!GetFileInformationByHandle(handle, &information) ||
        (information.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0) {
        const DWORD error = GetLastError();
        CloseHandle(handle);
        return Fail(ErrorCode::kLockOpenFailed,
                    (information.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0
                        ? "lock target is a reparse point"
                        : WindowsErrorDetail("validate lock", error));
    }
    lock_handle_ = handle;
    return true;
#else
    lock_fd_ = ::open(lock_path_.c_str(), O_CREAT | O_RDWR | O_CLOEXEC | O_NOFOLLOW,
                      S_IRUSR | S_IWUSR);
    if (lock_fd_ < 0) {
        return Fail(ErrorCode::kLockOpenFailed, SystemErrorDetail("open lock", errno));
    }
    if (::flock(lock_fd_, LOCK_EX | LOCK_NB) != 0) {
        const int error = errno;
        ::close(lock_fd_);
        lock_fd_ = -1;
        if (error == EWOULDBLOCK || error == EAGAIN) {
            return Fail(ErrorCode::kLockAlreadyHeld, SystemErrorDetail("lock already held", error));
        }
        return Fail(ErrorCode::kLockAcquireFailed, SystemErrorDetail("acquire lock", error));
    }
    if (::fchmod(lock_fd_, S_IRUSR | S_IWUSR) != 0) {
        const int error = errno;
        ReleaseLock();
        return Fail(ErrorCode::kLockOpenFailed, SystemErrorDetail("chmod lock", error));
    }
    return true;
#endif
}

bool PersistentState::RejectStateSymlink() {
#if defined(_WIN32)
    DWORD error = ERROR_SUCCESS;
    if (IsReparsePoint(state_path_, &error)) {
        return Fail(ErrorCode::kStateTargetIsSymlink, "state target is a reparse point");
    }
    if (error != ERROR_SUCCESS && error != ERROR_FILE_NOT_FOUND && error != ERROR_PATH_NOT_FOUND) {
        return Fail(ErrorCode::kStateOpenFailed, WindowsErrorDetail("inspect state target", error));
    }
    return true;
#else
    struct stat status {};
    if (::lstat(state_path_.c_str(), &status) == 0) {
        if (S_ISLNK(status.st_mode)) {
            return Fail(ErrorCode::kStateTargetIsSymlink, "state target is a symbolic link");
        }
        return true;
    }
    if (errno == ENOENT) {
        return true;
    }
    return Fail(ErrorCode::kStateOpenFailed, SystemErrorDetail("inspect state target", errno));
#endif
}

bool PersistentState::LoadSnapshot() {
    if (!RejectStateSymlink()) {
        return false;
    }
    if (ShouldFault(FaultPoint::kBeforeStateRead)) {
        return Fail(ErrorCode::kFaultInjected, "fault before state read");
    }

    std::string serialized;
#if defined(_WIN32)
    const HANDLE file = CreateFileW(state_path_.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                                    OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        const DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND || error == ERROR_PATH_NOT_FOUND) {
            payload_ = nlohmann::json::object();
            has_snapshot_ = false;
            return true;
        }
        return Fail(ErrorCode::kStateOpenFailed, WindowsErrorDetail("open state", error));
    }
    BY_HANDLE_FILE_INFORMATION information{};
    if (!GetFileInformationByHandle(file, &information) ||
        (information.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0) {
        const DWORD error = GetLastError();
        CloseHandle(file);
        return Fail(ErrorCode::kStateTargetIsSymlink, WindowsErrorDetail("validate state", error));
    }
    LARGE_INTEGER file_size{};
    if (!GetFileSizeEx(file, &file_size)) {
        const DWORD error = GetLastError();
        CloseHandle(file);
        return Fail(ErrorCode::kStateReadFailed,
                    WindowsErrorDetail("inspect state size", error));
    }
    if (file_size.QuadPart < 0 ||
        static_cast<std::uint64_t>(file_size.QuadPart) > options_.maximum_snapshot_bytes) {
        CloseHandle(file);
        return Fail(ErrorCode::kSnapshotTooLarge, "snapshot exceeds configured byte limit");
    }
    std::array<char, kIoBufferSize> buffer{};
    for (;;) {
        DWORD count = 0;
        if (!ReadFile(file, buffer.data(), static_cast<DWORD>(buffer.size()), &count, nullptr)) {
            const DWORD error = GetLastError();
            CloseHandle(file);
            return Fail(ErrorCode::kStateReadFailed, WindowsErrorDetail("read state", error));
        }
        if (count == 0) {
            break;
        }
        serialized.append(buffer.data(), count);
        if (serialized.size() > options_.maximum_snapshot_bytes) {
            CloseHandle(file);
            return Fail(ErrorCode::kSnapshotTooLarge, "snapshot exceeds configured byte limit");
        }
    }
    CloseHandle(file);
#else
    const int descriptor = ::open(state_path_.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
    if (descriptor < 0) {
        if (errno == ENOENT) {
            payload_ = nlohmann::json::object();
            has_snapshot_ = false;
            return true;
        }
        if (errno == ELOOP) {
            return Fail(ErrorCode::kStateTargetIsSymlink, "state target is a symbolic link");
        }
        return Fail(ErrorCode::kStateOpenFailed, SystemErrorDetail("open state", errno));
    }
    struct stat file_status {};
    if (::fstat(descriptor, &file_status) != 0) {
        const int error = errno;
        ::close(descriptor);
        return Fail(ErrorCode::kStateReadFailed,
                    SystemErrorDetail("inspect state size", error));
    }
    if (file_status.st_size < 0 ||
        static_cast<std::uint64_t>(file_status.st_size) > options_.maximum_snapshot_bytes) {
        ::close(descriptor);
        return Fail(ErrorCode::kSnapshotTooLarge, "snapshot exceeds configured byte limit");
    }
    std::array<char, kIoBufferSize> buffer{};
    for (;;) {
        const ssize_t count = ::read(descriptor, buffer.data(), buffer.size());
        if (count < 0) {
            if (errno == EINTR) {
                continue;
            }
            const int error = errno;
            ::close(descriptor);
            return Fail(ErrorCode::kStateReadFailed, SystemErrorDetail("read state", error));
        }
        if (count == 0) {
            break;
        }
        serialized.append(buffer.data(), static_cast<std::size_t>(count));
        if (serialized.size() > options_.maximum_snapshot_bytes) {
            ::close(descriptor);
            return Fail(ErrorCode::kSnapshotTooLarge, "snapshot exceeds configured byte limit");
        }
    }
    if (::close(descriptor) != 0) {
        return Fail(ErrorCode::kStateReadFailed, SystemErrorDetail("close state", errno));
    }
#endif

    nlohmann::json envelope;
    try {
        envelope = nlohmann::json::parse(serialized);
    } catch (const nlohmann::json::exception& exception) {
        return Fail(ErrorCode::kStateParseFailed, exception.what());
    }
    if (!envelope.is_object() || !envelope.contains("format") ||
        !envelope["format"].is_string() || envelope["format"] != kEnvelopeFormat ||
        !envelope.contains("schema_version") ||
        !envelope["schema_version"].is_number_unsigned() || !envelope.contains("payload")) {
        return Fail(ErrorCode::kEnvelopeInvalid, "snapshot envelope is invalid");
    }
    for (const auto& item : envelope.items()) {
        if (item.key() != "format" && item.key() != "schema_version" &&
            item.key() != "payload") {
            return Fail(ErrorCode::kEnvelopeInvalid, "snapshot envelope contains unknown fields");
        }
    }
    const std::uint64_t schema_version = envelope["schema_version"].get<std::uint64_t>();
    if (schema_version != static_cast<std::uint64_t>(options_.schema_version)) {
        return Fail(ErrorCode::kSchemaMismatch, "snapshot schema version does not match");
    }
    payload_ = envelope["payload"];
    has_snapshot_ = true;
    return true;
}

bool PersistentState::Save(const nlohmann::json& payload) {
    if (!opened_) {
        return Fail(ErrorCode::kInvalidOptions, "persistent state is not open");
    }
    if (durability_ambiguous_) {
        return Fail(ErrorCode::kDurabilityAmbiguous,
                    "snapshot durability is ambiguous; reopen before saving");
    }
    if (!RejectStateSymlink()) {
        return false;
    }
    if (ShouldFault(FaultPoint::kBeforeTempCreate)) {
        return Fail(ErrorCode::kFaultInjected, "fault before temporary file creation");
    }

    const nlohmann::json envelope = {
        {"format", kEnvelopeFormat},
        {"schema_version", options_.schema_version},
        {"payload", payload},
    };
    std::string serialized;
    try {
        serialized = envelope.dump();
    } catch (const nlohmann::json::exception& exception) {
        return Fail(ErrorCode::kSerializationFailed, exception.what());
    }
    serialized.push_back('\n');
    if (serialized.size() > options_.maximum_snapshot_bytes) {
        return Fail(ErrorCode::kSnapshotTooLarge, "snapshot exceeds configured byte limit");
    }

    std::filesystem::path temporary_path;
#if defined(_WIN32)
    wchar_t temporary_buffer[MAX_PATH]{};
    if (GetTempFileNameW(options_.data_directory.c_str(), L"lss", 0, temporary_buffer) == 0) {
        return Fail(ErrorCode::kTempCreateFailed,
                    WindowsErrorDetail("create temporary state", GetLastError()));
    }
    temporary_path = temporary_buffer;
    const HANDLE file =
        CreateFileW(temporary_path.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        std::error_code ignored;
        std::filesystem::remove(temporary_path, ignored);
        return Fail(ErrorCode::kTempCreateFailed,
                    WindowsErrorDetail("open temporary state", GetLastError()));
    }
    BY_HANDLE_FILE_INFORMATION temporary_information{};
    if (!GetFileInformationByHandle(file, &temporary_information) ||
        (temporary_information.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0) {
        const DWORD error = GetLastError();
        CloseHandle(file);
        std::error_code ignored;
        std::filesystem::remove(temporary_path, ignored);
        return Fail(ErrorCode::kTempCreateFailed,
                    (temporary_information.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0
                        ? "temporary state target is a reparse point"
                        : WindowsErrorDetail("validate temporary state", error));
    }
    auto abandon_temporary = [&]() {
        CloseHandle(file);
        std::error_code ignored;
        std::filesystem::remove(temporary_path, ignored);
    };
    if (ShouldFault(FaultPoint::kBeforeTempWrite)) {
        abandon_temporary();
        return Fail(ErrorCode::kFaultInjected, "fault before temporary state write");
    }
    if (serialized.size() > kWindowsMaximumWrite) {
        abandon_temporary();
        return Fail(ErrorCode::kTempWriteFailed, "snapshot exceeds one Windows write");
    }
    DWORD written = 0;
    if (!WriteFile(file, serialized.data(), static_cast<DWORD>(serialized.size()), &written,
                   nullptr) || written != serialized.size()) {
        const DWORD error = GetLastError();
        abandon_temporary();
        return Fail(ErrorCode::kTempWriteFailed,
                    WindowsErrorDetail("write temporary state", error));
    }
    if (ShouldFault(FaultPoint::kBeforeTempSync)) {
        abandon_temporary();
        return Fail(ErrorCode::kFaultInjected, "fault before temporary state sync");
    }
    if (!FlushFileBuffers(file)) {
        const DWORD error = GetLastError();
        abandon_temporary();
        return Fail(ErrorCode::kTempSyncFailed,
                    WindowsErrorDetail("sync temporary state", error));
    }
    CloseHandle(file);
    if (ShouldFault(FaultPoint::kBeforeRename)) {
        std::error_code ignored;
        std::filesystem::remove(temporary_path, ignored);
        return Fail(ErrorCode::kFaultInjected, "fault before state rename");
    }
    if (!MoveFileExW(temporary_path.c_str(), state_path_.c_str(),
                     MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        const DWORD error = GetLastError();
        std::error_code ignored;
        std::filesystem::remove(temporary_path, ignored);
        return Fail(ErrorCode::kRenameFailed, WindowsErrorDetail("replace state", error));
    }
    payload_ = payload;
    if (ShouldFault(FaultPoint::kBeforeDirectorySync)) {
        return FailAfterRename(ErrorCode::kFaultInjected,
                               "fault before directory durability boundary");
    }
#else
    std::string template_text =
        (options_.data_directory / ("." + options_.state_filename + ".tmp.XXXXXX")).string();
    std::vector<char> template_buffer(template_text.begin(), template_text.end());
    template_buffer.push_back('\0');
    const int descriptor = ::mkstemp(template_buffer.data());
    if (descriptor < 0) {
        return Fail(ErrorCode::kTempCreateFailed,
                    SystemErrorDetail("create temporary state", errno));
    }
    temporary_path = template_buffer.data();
    if (::fchmod(descriptor, S_IRUSR | S_IWUSR) != 0) {
        const int error = errno;
        ::close(descriptor);
        ::unlink(temporary_path.c_str());
        return Fail(ErrorCode::kTempCreateFailed,
                    SystemErrorDetail("chmod temporary state", error));
    }
    FILE* file = ::fdopen(descriptor, "wb");
    if (file == nullptr) {
        const int error = errno;
        ::close(descriptor);
        ::unlink(temporary_path.c_str());
        return Fail(ErrorCode::kTempCreateFailed,
                    SystemErrorDetail("open temporary state stream", error));
    }
    auto abandon_temporary = [&]() {
        std::fclose(file);
        ::unlink(temporary_path.c_str());
    };
    if (ShouldFault(FaultPoint::kBeforeTempWrite)) {
        abandon_temporary();
        return Fail(ErrorCode::kFaultInjected, "fault before temporary state write");
    }
    if (std::fwrite(serialized.data(), serialized.size(), 1, file) != 1 ||
        std::fflush(file) != 0) {
        const int error = errno;
        abandon_temporary();
        return Fail(ErrorCode::kTempWriteFailed,
                    SystemErrorDetail("write temporary state", error));
    }
    if (ShouldFault(FaultPoint::kBeforeTempSync)) {
        abandon_temporary();
        return Fail(ErrorCode::kFaultInjected, "fault before temporary state sync");
    }
    if (::fsync(descriptor) != 0) {
        const int error = errno;
        abandon_temporary();
        return Fail(ErrorCode::kTempSyncFailed,
                    SystemErrorDetail("sync temporary state", error));
    }
    if (std::fclose(file) != 0) {
        const int error = errno;
        ::unlink(temporary_path.c_str());
        return Fail(ErrorCode::kTempWriteFailed,
                    SystemErrorDetail("close temporary state", error));
    }
    if (ShouldFault(FaultPoint::kBeforeRename)) {
        ::unlink(temporary_path.c_str());
        return Fail(ErrorCode::kFaultInjected, "fault before state rename");
    }
    if (!RejectStateSymlink()) {
        ::unlink(temporary_path.c_str());
        return false;
    }
    if (::rename(temporary_path.c_str(), state_path_.c_str()) != 0) {
        const int error = errno;
        ::unlink(temporary_path.c_str());
        return Fail(ErrorCode::kRenameFailed, SystemErrorDetail("replace state", error));
    }
    payload_ = payload;
    if (ShouldFault(FaultPoint::kBeforeDirectorySync)) {
        return FailAfterRename(ErrorCode::kFaultInjected, "fault before directory sync");
    }
    const int directory =
        ::open(options_.data_directory.c_str(), O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if (directory < 0) {
        return FailAfterRename(ErrorCode::kDirectorySyncFailed,
                               SystemErrorDetail("open data directory for sync", errno));
    }
    if (::fsync(directory) != 0) {
        const int error = errno;
        ::close(directory);
        return FailAfterRename(ErrorCode::kDirectorySyncFailed,
                               SystemErrorDetail("sync data directory", error));
    }
    if (::close(directory) != 0) {
        return FailAfterRename(ErrorCode::kDirectorySyncFailed,
                               SystemErrorDetail("close data directory", errno));
    }
#endif

    Ready();
    has_snapshot_ = true;
    return true;
}

bool PersistentState::readable() const noexcept {
    return opened_ && !durability_ambiguous_ &&
           (health_ == Health::kReady || health_ == Health::kDegraded);
}

PersistentState::Health PersistentState::health() const noexcept {
    return health_;
}

PersistentState::ErrorCode PersistentState::error_code() const noexcept {
    return error_code_;
}

const std::string& PersistentState::error_detail() const noexcept {
    return error_detail_;
}

const nlohmann::json& PersistentState::payload() const noexcept {
    return payload_;
}

bool PersistentState::has_snapshot() const noexcept {
    return has_snapshot_;
}

nlohmann::json PersistentState::HealthJson() const {
    return {
        {"status", HealthName(health_)},
        {"error", ErrorCodeName(error_code_)},
    };
}

const char* PersistentState::HealthName(Health health) noexcept {
    switch (health) {
    case Health::kInitializing:
        return "initializing";
    case Health::kReady:
        return "ready";
    case Health::kDegraded:
        return "degraded";
    case Health::kError:
        return "error";
    }
    return "error";
}

const char* PersistentState::ErrorCodeName(ErrorCode error) noexcept {
    switch (error) {
    case ErrorCode::kNone:
        return "none";
    case ErrorCode::kInvalidOptions:
        return "invalid_options";
    case ErrorCode::kDirectoryCreateFailed:
        return "directory_create_failed";
    case ErrorCode::kDirectoryInvalid:
        return "directory_invalid";
    case ErrorCode::kDirectoryPermissionsFailed:
        return "directory_permissions_failed";
    case ErrorCode::kLockOpenFailed:
        return "lock_open_failed";
    case ErrorCode::kLockAlreadyHeld:
        return "lock_already_held";
    case ErrorCode::kLockAcquireFailed:
        return "lock_acquire_failed";
    case ErrorCode::kStateTargetIsSymlink:
        return "state_target_is_symlink";
    case ErrorCode::kStateOpenFailed:
        return "state_open_failed";
    case ErrorCode::kStateReadFailed:
        return "state_read_failed";
    case ErrorCode::kStateParseFailed:
        return "state_parse_failed";
    case ErrorCode::kEnvelopeInvalid:
        return "envelope_invalid";
    case ErrorCode::kSchemaMismatch:
        return "schema_mismatch";
    case ErrorCode::kSerializationFailed:
        return "serialization_failed";
    case ErrorCode::kSnapshotTooLarge:
        return "snapshot_too_large";
    case ErrorCode::kTempCreateFailed:
        return "temp_create_failed";
    case ErrorCode::kTempWriteFailed:
        return "temp_write_failed";
    case ErrorCode::kTempSyncFailed:
        return "temp_sync_failed";
    case ErrorCode::kRenameFailed:
        return "rename_failed";
    case ErrorCode::kDirectorySyncFailed:
        return "directory_sync_failed";
    case ErrorCode::kDurabilityAmbiguous:
        return "durability_ambiguous";
    case ErrorCode::kFaultInjected:
        return "fault_injected";
    }
    return "unknown";
}

bool PersistentState::ShouldFault(FaultPoint point) {
    return options_.fault_injector && options_.fault_injector(point);
}

bool PersistentState::Fail(ErrorCode error, std::string detail) {
    const bool retryable_save = error == ErrorCode::kSerializationFailed ||
        error == ErrorCode::kSnapshotTooLarge || error == ErrorCode::kTempCreateFailed ||
        error == ErrorCode::kTempWriteFailed || error == ErrorCode::kTempSyncFailed ||
        error == ErrorCode::kRenameFailed || error == ErrorCode::kFaultInjected;
    health_ = opened_ && !durability_ambiguous_ && retryable_save
                  ? Health::kDegraded : Health::kError;
    error_code_ = error;
    error_detail_ = std::move(detail);
    return false;
}

bool PersistentState::FailAfterRename(ErrorCode error, std::string detail) {
    durability_ambiguous_ = true;
    return Fail(error, std::move(detail));
}

void PersistentState::Ready() {
    health_ = Health::kReady;
    error_code_ = ErrorCode::kNone;
    error_detail_.clear();
}

void PersistentState::ReleaseLock() noexcept {
    opened_ = false;
#if defined(_WIN32)
    if (lock_handle_ != nullptr) {
        CloseHandle(static_cast<HANDLE>(lock_handle_));
        lock_handle_ = nullptr;
    }
#else
    if (lock_fd_ >= 0) {
        ::flock(lock_fd_, LOCK_UN);
        ::close(lock_fd_);
        lock_fd_ = -1;
    }
#endif
}

} // namespace libserver
