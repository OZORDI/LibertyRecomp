#ifdef LIBERTY_RECOMP_NX
// os/switch/savedata_switch.h
// Nintendo Switch save-data mount helper for LibertyRecomp.
//
// Switch save data lives in a journalled container managed by the OS.  Before
// any file I/O can happen the container must be created (if first run) and
// mounted via libnx's fsdev wrappers, which register a POSIX device name
// (e.g. "save") accessible as "save:/" from standard stdio calls.
//
// Call MountSaveData() once during application startup (after __appInit has
// initialized fs/account services) and use "save:/" as the user_data_root so
// HostPathDevice can point directly at it.  CommitSaveData() must be called
// after every batch of writes to flush the journal; the OS does NOT auto-commit.
#pragma once

#include <string>

namespace os::savedata {

/// Creates the save-data container if it does not already exist, then mounts
/// it as the POSIX device "save" so that save:/ paths work with stdio.
///
/// @param application_id  Title application ID (from NACP / NSP metadata).
///                        Pass 0 to use the running title's own ID.
/// @param uid             AccountUid of the user whose save to mount.
///
/// @returns  The device-prefixed path "save:/" on success, or an empty string
///           on failure.  The mount persists until UnmountSaveData() is called
///           or the process exits.
std::string MountSaveData(uint64_t application_id, AccountUid uid);

/// Flushes the save-data journal so all pending writes are persisted.
/// Must be called after every batch of file writes (close is not enough).
/// @returns true on success.
bool CommitSaveData();

/// Unmounts the save-data device previously mounted by MountSaveData().
/// Automatically commits before unmounting.
void UnmountSaveData();

/// @returns true if the save-data device is currently mounted.
bool IsSaveDataMounted();

}  // namespace os::savedata
#endif
