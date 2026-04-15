#ifdef LIBERTY_RECOMP_PS4
// os/ps4/savedata_ps4.h
// PS4 save-data mount helper for LibertyRecomp.
//
// PS4 save data lives in a kernel-managed encrypted container.  Before any
// file I/O can happen the container must be explicitly mounted via the SCE
// SaveData API, which returns a POSIX mount point path (e.g. "/savedata0").
// After mounting, that path behaves like a normal filesystem directory —
// open/stat/readdir all work — so HostPathDevice can be pointed directly at it.
//
// Call MountSaveData() once during application startup (after ps4_main sets up
// UserService and NP context) and pass the returned path to Runtime as the
// user_data_root so SetupVfs() can register it as a writable VFS device.
#pragma once

#include <string>

namespace os::savedata {

/// Initializes the SCE SaveData library, creates or opens the save-data
/// container for this title, and mounts it.
///
/// @param user_id   SCE user ID (from sceUserServiceGetInitialUser).
/// @param dir_name  Save data directory name inside the container (≤31 chars,
///                  e.g. "GTAIV00").  Used as a sub-directory so one container
///                  can hold multiple distinct save slots.
///
/// @returns  The POSIX mount-point path (e.g. "/savedata0") on success,
///           or an empty string on failure.  The mount persists until the
///           process exits (or UmountSaveData() is called).
///
/// Internally uses:
///   sceSaveDataInitialize3(0)
///   sceSaveDataMount2( RDWR | CREATE2, blocks = 32768 )   // 1 GB max
std::string MountSaveData(int32_t user_id, const char* dir_name);

/// Unmounts the save-data container previously mounted by MountSaveData().
/// Must be called before the process exits to avoid data loss.
void UmountSaveData(const std::string& mount_point);

}  // namespace os::savedata
#endif // LIBERTY_RECOMP_PS4
