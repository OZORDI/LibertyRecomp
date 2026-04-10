// frontend_state_hooks.cpp
// =============================================================================
// GTA IV Front-End State Machine Hooks
// =============================================================================
// Decompiled from sub_82142230 (the episode selection / title screen state
// machine) and its callees. This file provides two critical hooks:
//
// 1. sub_821406C8 (GetActivePlayerSlot): On PC with keyboard/mouse, the active
//    controller slot index (g_activeControllerSlot @ 0x82A9172C) is -1 because
//    no Xbox controller is enumerated. The original function returns NULL when
//    the index is -1, causing STATE-3 (DLC episode selection) to immediately
//    skip to STATE-4 without letting the player choose an episode. We hook it
//    to return slot 0 as a fallback.
//
// 2. sub_82142230 (FrontEndStateMachine): Full hook that replaces the entire
//    state machine. On Xbox, STATE-3 uses edge-triggered gamepad button
//    detection in the player input slot struct (rising edge = pressed this
//    frame but not last frame). On PC, keyboard/mouse input may not populate
//    the slot struct identically, so we inject direct keyboard-aware input
//    handling for the DPad Up/Down (highlight toggle) and Start/A (confirm)
//    actions.
//
// =============================================================================
// STATE MACHINE OVERVIEW (sub_82142230)
// =============================================================================
//
// STATE 0: Initial sign-in check (sub_822414E8)
//   - Waits for controller sign-in to complete
//   - Result 1 => STATE 1 (clear g_feSubState0)
//   - Result 2 => jump to LABEL_5 (DLC availability check)
//
// STATE 1: Storage device selection (sub_8223DDA8)
//   - Waits for storage device to be selected
//   - Result 1 or 2 => STATE 2
//
// STATE 2: Network/multiplayer session setup (sub_8223DEE8)
//   - Result 1 => LABEL_5 (DLC availability check)
//   - Result 2 => STATE 8 (multiplayer exit path)
//
// LABEL_5 (entered from STATE 0 result=2 or STATE 2 result=1):
//   - Reads DLC manager object (g_dlcManager @ 0x831E4DD4)
//   - Calls sub_82219AC0 to update DLC state
//   - If byte at (dlcManager + 361) == 0: no DLC => STATE 4
//   - If byte at (dlcManager + 361) != 0: DLC available => STATE 3
//
// STATE 3: DLC Episode Selection (the critical state for this hook)
//   - Calls sub_82219AC0 to refresh DLC state
//   - If DLC flag cleared: fall through to STATE 4
//   - Initializes episode highlight (g_episodeHighlight) to 1 if not set
//   - Calls sub_8223CAD8 (scaleform/UI init for episode picker)
//   - Calls sub_821406C8 to get active player input slot
//   - If slot is NULL: skip to STATE 4 (the PC bug!)
//   - Guards: popup active? ack pending? multiplayer check?
//   - Edge-triggered button detection:
//     * DPad Up/Down or A button rising edge => toggle highlight (1<->2)
//       Plays "FRONTEND_MENU_HIGHLIGHT" sound
//     * Start button rising edge => confirm selection
//       Plays "FRONTEND_MENU_SELECT" sound
//       Sets g_episodeSelection based on highlight value:
//         1 => Episode 1 (TLAD), 2 => Episode 2 (TBOGT)
//       Transitions to STATE 4
//   - Back button (sub_826CBD20/sub_826CBD08) => STATE 2
//   - User sign-out detected => STATE 0 with FE_NODLC4 popup
//
// STATE 4: Auto-load save check (sub_822440F8)
//   - Determines which save file to load
//   - Result 1 => STATE 7 (skip to game)
//   - Result 2 => STATE 5
//
// STATE 5: Begin save load (sub_822422E0 + immediate fall-through to STATE 6)
//
// STATE 6: Save load progress (sub_822438B0)
//   - Result 0 => STATE 9 (load complete, start game)
//   - Result 2 => STATE 7 (skip to game)
//   - Result 1 => continue polling
//
// STATES 7, 8, 9: Exit paths (set flags, transition to gameplay)
//   - byte_831D5348 = 0, byte_831D5337 = 1
//   - Calls sub_821B5A68, sub_82220118, sub_8222DB48, sub_8214AD88
//   - Calls sub_821ED6D8, sub_82145820
//   - STATE 7: normal game start
//   - STATE 8: multiplayer game start (checks sub_826CBD08)
//   - STATE 9: loaded save game start
//
// =============================================================================
// GLOBAL VARIABLE MAP
// =============================================================================
//
// 0x82BF9B70  g_popupState        (-1 = no popup, 1 = popup shown)
// 0x82BF9D81  g_popupAckPending   (nonzero = waiting for popup dismiss)
// 0x82A9172C  g_activeCtrlSlot    (-1 = no controller, 0-3 = slot index)
// 0x82B29F18  g_playerSlotArray   (base of 4 player input slot structs)
//             Each slot: int32[47] = 188 bytes
//             [0..19]  = current frame button states
//             [20..39] = previous frame button states
//             [40..46] = analog axis values
// 0x831E4DD4  g_dlcManager        (ptr to DLC manager object)
//             +356 = needs_refresh flag (byte)
//             +361 = dlc_episodes_available flag (byte, 0=no DLC)
// 0x831D5330  g_episodeHighlight  (0=none, 1=Episode1/TLAD, 2=Episode2/TBOGT)
// 0x82A95474  g_episodeSelection  (final selection: 1=TLAD, 2=TBOGT)
// 0x82BF9828  g_feSubState0       (sub-state for STATE 0/1)
// 0x82B2F7E0  g_networkManager    (network/session manager pointer)
// 0x82BCC1F8  g_audioContext      (frontend audio SFX context)
// 0x82BF9858  g_feSceneObject     (frontend scene/camera object)
// 0x831D5327  g_dlcAvailFlag      (tracks DLC availability across sign-in)
// 0x831D5348  g_feExitingFlag     (set to 0 on exit)
// 0x831D5337  g_feTransitionFlag  (set to 1 on exit, 0 after transition)
// 0x831D5324  g_feCompleteFlag    (set to 0 on exit)
// 0x82BFA13C  g_uiScreenState     (current UI screen enum)
// 0x82BFA144  g_uiOverlayActive   (scaleform overlay flag)
// 0x82A95478  g_saveScanSlot      (save file slot from auto-load scan)
// 0x82B39518  g_dlcSaveSlot       (DLC save slot override)
// 0x82B394F8  g_mpPlayerCount     (multiplayer player count)
// 0x82B394FC  g_mpPlayerCountDLC  (multiplayer player count for DLC)
// 0x82BEFA40  g_gameObject1       (used in exit transitions)
// 0x82BEFA54  g_gameObject2       (used in exit transitions, +1376 flag)
// 0x82B978B0  g_worldLoader       (world/streaming loader object)
// 0x8317F66C  g_mpSessionState    (2 = multiplayer active)
//
// =============================================================================
// PLAYER INPUT SLOT STRUCT (int32[47], 188 bytes)
// =============================================================================
//
// Indices used in STATE-3 edge detection:
//   [1]  current A button       [21] previous A button
//   [14] current Start button   [34] previous Start button
//   [17] current DPad Up        [37] previous DPad Up
//   [18] current DPad Down      [38] previous DPad Down
//
// Rising edge detection pattern:
//   pressed_this_frame = (slot[N] != 0) && (slot[N+20] == 0)
//
// =============================================================================

#include <cpu/ppc_context.h>
#include <kernel/function.h>
#include <kernel/memory.h>
#include <os/logger.h>

// ---------------------------------------------------------------------------
// Hook 1: sub_821406C8 - GetActivePlayerInputSlot
// ---------------------------------------------------------------------------
// Original: returns &g_playerSlotArray[47 * g_activeCtrlSlot] or NULL if -1
// Hook: returns slot 0 as fallback when no controller is assigned, ensuring
// the STATE-3 episode selection screen works with keyboard input on PC.
// ---------------------------------------------------------------------------

PPC_FUNC_IMPL(__imp__sub_821406C8);

PPC_FUNC_HOOK(sub_821406C8) {
    static bool s_contentFieldsPopulated = false;
    int32_t slotIndex = static_cast<int32_t>(PPC_LOAD_U32(0x82A9172C));

    uint32_t slotPtr;
    if (slotIndex == -1) {
        // PC fallback: use slot 0 so STATE-3 can read input
        // Slot 0 base = g_playerSlotArray + (0 * 188) = 0x82B29F18
        slotPtr = 0x82B29F18;
    } else {
        // Normal path: return &g_playerSlotArray[47 * slotIndex]
        // 47 ints * 4 bytes = 188 bytes per slot
        uint32_t offset = static_cast<uint32_t>(slotIndex) * 188;
        slotPtr = 0x82B29F18 + offset;
    }

    // Populate player slot content-readiness fields so state 3's "newly set"
    // transition detectors fire. On Xbox 360, XAM notification callbacks
    // write these. In the recomp, we set them here on first call.
    if (!s_contentFieldsPopulated && slotPtr != 0) {
        PPC_STORE_U32(slotPtr + 56, 1);  // DLC/title update content ready
        PPC_STORE_U32(slotPtr + 68, 1);  // Sign-in completion
        PPC_STORE_U32(slotPtr + 72, 1);  // Storage device selected
        PPC_STORE_U32(slotPtr + 4,  1);  // Profile data loaded
        s_contentFieldsPopulated = true;
    }

    ctx.r3.u64 = slotPtr;
}

// ---------------------------------------------------------------------------
// Hook 2: sub_82142230 - FrontEndStateMachine
// ---------------------------------------------------------------------------
// We hook the entire state machine to fix the STATE-3 input handling for PC.
// The original code's edge detection reads from the player input slot struct
// which requires Xbox gamepad polling to populate. On PC with keyboard/mouse,
// the slot struct may be populated by the HID translation layer, but the
// critical issue is that g_activeCtrlSlot == -1 causes GetActivePlayerSlot
// to return NULL, skipping STATE-3 entirely.
//
// With Hook 1 above fixing the NULL return, the original recompiled code
// for sub_82142230 should work correctly because:
// - The HID layer (XInputGetState shim) maps keyboard to XINPUT_STATE
// - The game's own input processing (sub_821B42B8 etc.) fills the slot struct
// - The edge detection logic in the recompiled STATE-3 code reads those slots
//
// Therefore we do NOT need to replace the entire state machine. Hook 1 is the
// minimal, surgical fix. We call the original __imp__ and let it run.
//
// However, we add diagnostic logging around the state transitions so we can
// track the frontend flow during development.
// ---------------------------------------------------------------------------

PPC_FUNC_IMPL(__imp__sub_82142230);

PPC_FUNC_HOOK(sub_82142230) {
    // Log entry with current state values for diagnostics
    int32_t episodeHighlight = static_cast<int32_t>(PPC_LOAD_U32(0x831D5330));
    int32_t episodeSelection = static_cast<int32_t>(PPC_LOAD_U32(0x82A95474));
    int32_t activeSlot       = static_cast<int32_t>(PPC_LOAD_U32(0x82A9172C));
    int32_t popupState       = static_cast<int32_t>(PPC_LOAD_U32(0x82BF9B70));
    uint8_t popupAckPending  = PPC_LOAD_U8(0x82BF9D81);

    static bool s_loggedOnce = false;
    if (!s_loggedOnce) {
        LOGF_INFO("[FE-SM] FrontEndStateMachine entered: ctrlSlot={} highlight={} "
                  "selection={} popup={} ackPending={}",
                  activeSlot, episodeHighlight, episodeSelection,
                  popupState, popupAckPending);
        s_loggedOnce = true;
    }

    // Ensure controller slot is valid for PC (belt-and-suspenders with Hook 1)
    if (activeSlot == -1) {
        // Force slot 0 so the state machine's internal call to sub_821406C8
        // (via the recompiled code) returns a valid pointer
        PPC_STORE_U32(0x82A9172C, 0);
    }

    // Call the original recompiled state machine
    __imp__sub_82142230(ctx, base);

    // Log exit state
    int32_t exitHighlight  = static_cast<int32_t>(PPC_LOAD_U32(0x831D5330));
    int32_t exitSelection  = static_cast<int32_t>(PPC_LOAD_U32(0x82A95474));
    uint8_t exitTransition = PPC_LOAD_U8(0x831D5337);

    static int32_t s_lastSelection = -999;
    if (exitSelection != s_lastSelection) {
        LOGF_INFO("[FE-SM] FrontEndStateMachine exited: highlight={} selection={} "
                  "transitionFlag={}",
                  exitHighlight, exitSelection, exitTransition);
        s_lastSelection = exitSelection;
    }
}

// ---------------------------------------------------------------------------
// Hook 3: sub_8224FA48 - IsPopupActive
// ---------------------------------------------------------------------------
// Trivial function (returns dword_82BF9B70 != -1) but called 14 times.
// No hook needed - works correctly as recompiled. Documented here for
// completeness of the state machine analysis.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Hook 4: sub_821B6FD0 - IsMultiplayerRestricted
// ---------------------------------------------------------------------------
// Checks if the current network session restricts episode selection.
// Uses a callback table to iterate network session members.
// On PC (single-player focused), this should always return false.
// No hook needed unless multiplayer is active.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Hook 5: sub_8223CAD8 - EpisodePickerUIInit
// ---------------------------------------------------------------------------
// Called at the start of STATE-3 to initialize the scaleform episode picker.
// Checks byte_82BF984C (a one-shot init flag) and configures the UI overlay.
// If g_uiOverlayActive (0x82BFA144) is set, uses a different UI path.
// No hook needed - works correctly as recompiled.
// ---------------------------------------------------------------------------
