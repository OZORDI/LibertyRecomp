#ifdef LIBERTY_RECOMP_NX
// os/switch/savedata_switch.cpp
// Nintendo Switch save-data mount helper for LibertyRecomp.
//
// Save data layout on disk (inside the journalled container):
//
//   save:/               <-- mount point returned to caller
//     saves/             <-- GTA IV autosave / manual save files
//     settings.bin       <-- LibertyRecomp config that must persist across sessions
//
// After fsdevMountSaveData() the "save:/" prefix behaves like a normal POSIX
// directory.  HostPathDevice wraps it directly; no special handle magic needed.
//
// IMPORTANT: the Switch filesystem does NOT auto-commit writes.  Every batch
// of file writes must be followed by CommitSaveData() or data will be lost on
// a crash / forced shutdown.

#include "savedata_switch.h"

#include <switch.h>

#include <cstdio>
#include <cstring>

// Save container sizes.  Actual on-disk usage only grows as data is written.
// 64 MB data + 64 MB journal is generous for GTA IV saves + settings.
static constexpr s64 kSaveDataSize    = 64 * 1024 * 1024;  // 64 MB
static constexpr s64 kJournalSize     = 64 * 1024 * 1024;  // 64 MB

// The libnx device name used for the mount (produces "save:/" paths).
static constexpr const char* kDeviceName = "save";

// Module-level state.
static bool s_mounted = false;

namespace os::savedata {

// ---------------------------------------------------------------------------
// Internal: ensure the save-data container exists before attempting to mount.
// On first boot there is no container yet; fsdevMountSaveData would fail with
// 0x221002 (TargetNotFound).  We create one using fsCreateSaveDataFileSystem.
// If the container already exists the create call returns 0x802 (PathAlreadyExists)
// which we treat as success.
// ---------------------------------------------------------------------------
static Result EnsureSaveDataExists(uint64_t application_id, AccountUid uid)
{
    FsSaveDataAttribute attr = {};
    attr.application_id = application_id;
    attr.uid            = uid;
    attr.save_data_type = FsSaveDataType_Account;

    FsSaveDataCreationInfo creation = {};
    creation.save_data_size    = kSaveDataSize;
    creation.journal_size      = kJournalSize;
    creation.available_size    = 0;
    creation.owner_id          = application_id;
    creation.flags             = 0;
    creation.save_data_space_id = FsSaveDataSpaceId_User;

    FsSaveDataMetaInfo meta = {};
    meta.size = 0;
    meta.type = 0;

    Result rc = fsCreateSaveDataFileSystem(&attr, &creation, &meta);

    // 0x802 = PathAlreadyExists — container was created on a previous boot.
    if (rc == 0x802)
        return 0;

    return rc;
}

std::string MountSaveData(uint64_t application_id, AccountUid uid)
{
    if (s_mounted)
        return std::string(kDeviceName) + ":/";

    // Step 1: create the container if this is the first run.
    Result rc = EnsureSaveDataExists(application_id, uid);
    if (R_FAILED(rc)) {
        fprintf(stderr, "[savedata] EnsureSaveDataExists failed: 0x%X\n",
                static_cast<unsigned>(rc));
        return {};
    }

    // Step 2: mount via libnx devoptab so "save:/" works with stdio.
    rc = fsdevMountSaveData(kDeviceName, application_id, uid);
    if (R_FAILED(rc)) {
        fprintf(stderr, "[savedata] fsdevMountSaveData failed: 0x%X\n",
                static_cast<unsigned>(rc));
        return {};
    }

    s_mounted = true;
    return std::string(kDeviceName) + ":/";
}

bool CommitSaveData()
{
    if (!s_mounted)
        return false;

    Result rc = fsdevCommitDevice(kDeviceName);
    if (R_FAILED(rc)) {
        fprintf(stderr, "[savedata] fsdevCommitDevice failed: 0x%X\n",
                static_cast<unsigned>(rc));
        return false;
    }
    return true;
}

void UnmountSaveData()
{
    if (!s_mounted)
        return;

    // Commit outstanding writes before unmounting.
    fsdevCommitDevice(kDeviceName);
    fsdevUnmountDevice(kDeviceName);
    s_mounted = false;
}

bool IsSaveDataMounted()
{
    return s_mounted;
}

}  // namespace os::savedata
#endif
