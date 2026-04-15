#ifdef LIBERTY_RECOMP_PS4
// os/ps4/savedata_ps4.cpp
// PS4 save-data mount helper for LibertyRecomp.
//
// Save data layout on disk (inside the SCE container):
//
//   /savedata0/          ← mount point returned to caller
//     GTAIV00/           ← dir_name passed in
//       saves/           ← GTA IV autosave / manual save files
//       settings.bin     ← LibertyRecomp config that must persist across sessions
//
// The mount point is a plain POSIX directory after sceSaveDataMount2() returns.
// HostPathDevice wraps it directly; no special file-handle magic is needed.

#include "savedata_ps4.h"

#include <orbis/SaveData.h>
#include <orbis/libkernel.h>

#include <cstring>
#include <string>

// Save-data block size is 32 KB (ORBIS_SAVE_DATA_BLOCK_SIZE).
// We request 32768 blocks = 1 GB maximum container.  Actual on-disk usage is
// far less; the OS only allocates what's written.
static constexpr uint64_t kSaveDataBlocks = 32768;

namespace os::savedata {

std::string MountSaveData(int32_t user_id, const char* dir_name)
{
    // ── 1. Initialize the SaveData library ─────────────────────────────────
    // sceSaveDataInitialize3(0): 0 = use default init parameters (no flags).
    int32_t rc = sceSaveDataInitialize3(0);
    // SCE_SAVE_DATA_ERROR_ALREADY_INITIALIZED is not fatal; continue.
    if (rc != 0 && rc != 0x809F0002 /* ALREADY_INITIALIZED */) {
        return {};
    }

    // ── 2. Populate the mount request ──────────────────────────────────────
    OrbisSaveDataMount2 mount = {};
    mount.userId     = user_id;
    mount.blocks     = kSaveDataBlocks;
    // RDWR | CREATE2: mount read/write; create if not already present, or open
    // if it already exists.  This is the standard "first-run safe" mount mode.
    mount.mountMode  = ORBIS_SAVE_DATA_MOUNT_MODE_RDWR
                     | ORBIS_SAVE_DATA_MOUNT_MODE_CREATE2;

    // dir_name identifies the save slot within the encrypted container.
    OrbisSaveDataDirName dir_name_struct = {};
    strncpy(dir_name_struct.data, dir_name, sizeof(dir_name_struct.data) - 1);
    mount.dirName = &dir_name_struct;

    // ── 3. Mount ───────────────────────────────────────────────────────────
    OrbisSaveDataMountResult result = {};
    rc = sceSaveDataMount2(&mount, &result);
    if (rc != 0) {
        return {};
    }

    // result.mountPathName is a null-terminated string like "/savedata0".
    return std::string(result.mountPathName);
}

void UmountSaveData(const std::string& mount_point)
{
    if (mount_point.empty())
        return;

    OrbisSaveDataMountPoint mp = {};
    strncpy(mp.data, mount_point.c_str(), sizeof(mp.data) - 1);
    sceSaveDataUmount(&mp);
}

}  // namespace os::savedata
#endif // LIBERTY_RECOMP_PS4
