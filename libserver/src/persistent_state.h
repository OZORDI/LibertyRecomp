#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <cstddef>
#include <string>

#include <nlohmann/json.hpp>

namespace libserver {

class PersistentState final {
public:
    enum class Health {
        kInitializing,
        kReady,
        kDegraded,
        kError,
    };

    enum class ErrorCode {
        kNone,
        kInvalidOptions,
        kDirectoryCreateFailed,
        kDirectoryInvalid,
        kDirectoryPermissionsFailed,
        kLockOpenFailed,
        kLockAlreadyHeld,
        kLockAcquireFailed,
        kStateTargetIsSymlink,
        kStateOpenFailed,
        kStateReadFailed,
        kStateParseFailed,
        kEnvelopeInvalid,
        kSchemaMismatch,
        kSerializationFailed,
        kSnapshotTooLarge,
        kTempCreateFailed,
        kTempWriteFailed,
        kTempSyncFailed,
        kRenameFailed,
        kDirectorySyncFailed,
        kDurabilityAmbiguous,
        kFaultInjected,
    };

    enum class FaultPoint {
        kBeforeDirectoryCreate,
        kAfterLockAcquire,
        kBeforeStateRead,
        kBeforeTempCreate,
        kBeforeTempWrite,
        kBeforeTempSync,
        kBeforeRename,
        kBeforeDirectorySync,
    };

    using FaultInjector = std::function<bool(FaultPoint)>;

    struct Options {
        std::filesystem::path data_directory;
        std::string state_filename = "state.json";
        unsigned int schema_version = 1;
        std::size_t maximum_snapshot_bytes = 67108864;
        FaultInjector fault_injector;
    };

    explicit PersistentState(Options options);
    ~PersistentState();

    PersistentState(const PersistentState&) = delete;
    PersistentState& operator=(const PersistentState&) = delete;
    PersistentState(PersistentState&&) = delete;
    PersistentState& operator=(PersistentState&&) = delete;

    bool Open();
    bool Save(const nlohmann::json& payload);

    [[nodiscard]] Health health() const noexcept;
    [[nodiscard]] bool readable() const noexcept;
    [[nodiscard]] ErrorCode error_code() const noexcept;
    [[nodiscard]] const std::string& error_detail() const noexcept;
    [[nodiscard]] const nlohmann::json& payload() const noexcept;
    [[nodiscard]] bool has_snapshot() const noexcept;
    [[nodiscard]] nlohmann::json HealthJson() const;

    [[nodiscard]] static const char* HealthName(Health health) noexcept;
    [[nodiscard]] static const char* ErrorCodeName(ErrorCode error) noexcept;

private:
    bool EnsureDataDirectory();
    bool AcquireLock();
    bool LoadSnapshot();
    bool RejectStateSymlink();
    bool ShouldFault(FaultPoint point);
    bool Fail(ErrorCode error, std::string detail);
    bool FailAfterRename(ErrorCode error, std::string detail);
    void Ready();
    void ReleaseLock() noexcept;

    Options options_;
    std::filesystem::path state_path_;
    std::filesystem::path lock_path_;
    nlohmann::json payload_ = nlohmann::json::object();
    Health health_ = Health::kInitializing;
    ErrorCode error_code_ = ErrorCode::kNone;
    std::string error_detail_;
    bool opened_ = false;
    bool durability_ambiguous_ = false;
    bool has_snapshot_ = false;

#if defined(_WIN32)
    void* lock_handle_ = nullptr;
#else
    int lock_fd_ = -1;
#endif
};

} // namespace libserver
