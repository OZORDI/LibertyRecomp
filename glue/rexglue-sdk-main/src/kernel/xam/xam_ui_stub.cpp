/**
 ******************************************************************************
 * ReXGlue runtime - XAM UI stubs for headless console builds.
 ******************************************************************************
 *
 * When REXGLUE_ENABLE_GRAPHICS=OFF (PS4 GNM-only, etc.) xam_ui.cpp cannot
 * compile because it pulls in ImGui and rex::ui::Window. Console builds
 * don't have a host-side ImGui surface to render into, so instead we stub
 * out the exports here. The game sees every XAM UI call immediately
 * complete with a "cancelled"-equivalent status.
 */

#include <rex/hook.h>
#include <rex/kernel/xam/private.h>
#include <rex/system/kernel_state.h>
#include <rex/system/xtypes.h>

namespace rex {
namespace kernel {
namespace xam {

// XamIsUIActive: the UI is never up on headless builds.
static uint32_t XamIsUIActive_entry() { return 0; }

// XamShowMessageBoxUI: fill the result-pointer with a "cancel" selection
// (button 0) and return X_ERROR_SUCCESS so the calling thread sees the
// dialog as dismissed.
static uint32_t XamShowMessageBoxUI_entry(uint32_t /*user_index*/,
                                          uint32_t /*title_ptr*/,
                                          uint32_t /*text_ptr*/,
                                          uint32_t /*button_count*/,
                                          uint32_t /*button_ptrs*/,
                                          uint32_t /*active_button*/,
                                          uint32_t /*flags*/,
                                          uint32_t result_ptr,
                                          uint32_t overlapped_ptr) {
    if (result_ptr) {
        auto* result = reinterpret_cast<uint32_t*>(
            rex::system::kernel_state()->memory()->TranslateVirtual(result_ptr));
        if (result) {
            *result = 0; // first button = default = "cancel"
        }
    }
    (void)overlapped_ptr;
    return 0; // X_ERROR_SUCCESS
}

static uint32_t XamShowMessageBoxUIEx_entry(uint32_t a, uint32_t b, uint32_t c,
                                            uint32_t d, uint32_t e, uint32_t f,
                                            uint32_t g, uint32_t h, uint32_t i,
                                            uint32_t j, uint32_t k,
                                            uint32_t result_ptr,
                                            uint32_t overlapped_ptr) {
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
    (void)g; (void)h; (void)i; (void)j; (void)k; (void)overlapped_ptr;
    if (result_ptr) {
        auto* result = reinterpret_cast<uint32_t*>(
            rex::system::kernel_state()->memory()->TranslateVirtual(result_ptr));
        if (result) {
            *result = 0;
        }
    }
    return 0;
}

// XamShowKeyboardUI: "user cancelled" return so we don't overwrite the
// default text buffer with garbage.
static uint32_t XamShowKeyboardUI_entry(uint32_t /*user_index*/,
                                        uint32_t /*flags*/,
                                        uint32_t /*default_text*/,
                                        uint32_t /*title*/,
                                        uint32_t /*description*/,
                                        uint32_t /*buffer*/,
                                        uint32_t /*buffer_length*/,
                                        uint32_t /*overlapped*/) {
    return 0x00000490; // ERROR_CANCELLED
}

// XamShowDeviceSelectorUI: write the default storage device ID and succeed.
static uint32_t XamShowDeviceSelectorUI_entry(uint32_t /*user_index*/,
                                              uint32_t /*content_type*/,
                                              uint32_t /*content_flags*/,
                                              uint64_t /*total_requested*/,
                                              uint32_t device_id_ptr,
                                              uint32_t /*overlapped*/) {
    if (device_id_ptr) {
        auto* id = reinterpret_cast<uint32_t*>(
            rex::system::kernel_state()->memory()->TranslateVirtual(device_id_ptr));
        if (id) {
            *id = 0x00000001; // HDD
        }
    }
    return 0;
}

static uint32_t XamShowDirtyDiscErrorUI_entry(uint32_t /*user_index*/) {
    return 0;
}

static uint32_t XamShowPartyUI_entry(uint32_t /*a*/) {
    return 0;
}

static uint32_t XamShowCommunitySessionsUI_entry(uint32_t /*a*/, uint32_t /*b*/) {
    return 0;
}

}  // namespace xam
}  // namespace kernel
}  // namespace rex

REX_EXPORT(__imp__XamIsUIActive, rex::kernel::xam::XamIsUIActive_entry)
REX_EXPORT(__imp__XamShowMessageBoxUI, rex::kernel::xam::XamShowMessageBoxUI_entry)
REX_EXPORT(__imp__XamShowKeyboardUI, rex::kernel::xam::XamShowKeyboardUI_entry)
REX_EXPORT(__imp__XamShowDeviceSelectorUI, rex::kernel::xam::XamShowDeviceSelectorUI_entry)
REX_EXPORT(__imp__XamShowDirtyDiscErrorUI, rex::kernel::xam::XamShowDirtyDiscErrorUI_entry)
REX_EXPORT(__imp__XamShowPartyUI, rex::kernel::xam::XamShowPartyUI_entry)
REX_EXPORT(__imp__XamShowCommunitySessionsUI, rex::kernel::xam::XamShowCommunitySessionsUI_entry)
REX_EXPORT(__imp__XamShowMessageBoxUIEx, rex::kernel::xam::XamShowMessageBoxUIEx_entry)

REX_EXPORT_STUB(__imp__XamIsGuideDisabled);
REX_EXPORT_STUB(__imp__XamIsMessageBoxActive);
REX_EXPORT_STUB(__imp__XamIsNuiUIActive);
REX_EXPORT_STUB(__imp__XamIsSysUiInvokedByTitle);
REX_EXPORT_STUB(__imp__XamIsSysUiInvokedByXenonButton);
REX_EXPORT_STUB(__imp__XamIsUIThread);
REX_EXPORT_STUB(__imp__XamNavigate);
REX_EXPORT_STUB(__imp__XamNavigateBack);
REX_EXPORT_STUB(__imp__XamOverrideHudOpenType);
REX_EXPORT_STUB(__imp__XamShowAchievementDetailsUI);
REX_EXPORT_STUB(__imp__XamShowAchievementsUI);
REX_EXPORT_STUB(__imp__XamShowAchievementsUIEx);
REX_EXPORT_STUB(__imp__XamShowAndWaitForMessageBoxEx);
REX_EXPORT_STUB(__imp__XamShowAvatarAwardGamesUI);
REX_EXPORT_STUB(__imp__XamShowAvatarAwardsUI);
REX_EXPORT_STUB(__imp__XamShowAvatarMiniCreatorUI);
REX_EXPORT_STUB(__imp__XamShowBadDiscErrorUI);
REX_EXPORT_STUB(__imp__XamShowBeaconsUI);
REX_EXPORT_STUB(__imp__XamShowBrandedKeyboardUI);
REX_EXPORT_STUB(__imp__XamShowChangeGamerTileUI);
REX_EXPORT_STUB(__imp__XamShowComplaintUI);
REX_EXPORT_STUB(__imp__XamShowCreateProfileUI);
