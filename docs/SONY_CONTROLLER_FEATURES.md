# PlayStation controller features

Liberty uses SDL's real controller type and reported capabilities to enable the
DualShock 4 and DualSense features below. SDL keeps ownership of the connection.
DualSense trigger effects use SDL's device-specific effect API and its transport
framing; they do not open a second HID connection.

| Input or output | Behaviour |
| --- | --- |
| Touchpad click | Cycle the existing GTA IV camera views |
| Swipe up / down on foot | Previous / next weapon |
| Swipe left / right while driving | Previous / next radio station |
| Hold two fingers in a vehicle | Hold the existing cinematic camera action |
| Controller light | Blue for GTA IV, orange for TLAD, purple for TBOGT |
| Wanted light | Alternate red and blue while wanted |
| Added vibration | Confirmed local gunshots, applied damage and nearby accepted explosions |
| DualSense triggers | Weapon aim/fire resistance and brief shot recoil; driver brake/accelerator resistance |

The episode colours are an authored Liberty mapping. GTA IV has no GTA V
three-character switching mechanic. Added vibration uses conventional SDL rumble;
it does not synthesize the PS5 game's waveform haptics.

The native frontend has one **DualSense Features** On/Off option for touchpad
gestures, controller lighting, added vibration and adaptive triggers. Turning it
on enables every supported feature, including any previously disabled individual
switches. Turning it off disables the bundle. Motion Controls remains separate.
Changes apply live and use the existing Save action. The setting also covers the
supported features on DualShock 4 controllers.

The persistent CVARs remain `gta4_sony_enabled`, `gta4_sony_gestures`,
`gta4_sony_lighting`, `gta4_sony_rumble` and `gta4_sony_triggers`; the frontend
updates them together. Feedback intensity remains configurable through
`gta4_sony_intensity` (0–1, default 0.5). The older desktop host can use the same
CVAR configuration, with its existing SDL owner forwarding snapshots to Rex input.

Menus, phone use, cutscenes, minigames, death, focus loss, disconnected controllers
and stale gameplay snapshots cancel added gameplay input/effects. A held touchpad
click keeps its original ownership until release so a context change cannot turn
the same held click into a new Back press. Unsupported adaptive-trigger output is
disabled independently. Output writes are bounded and neutralized at shutdown.

## Implementation boundaries

`gta4_sony_feedback.cpp` reads checked guest spans on a guest thread and publishes
copied state to `rex::input::sony::Service`. The output worker never reads guest
memory. The existing macOS player-info alias remains around local-player reads
and damage handling.

| Retail hook | Purpose |
| --- | --- |
| `828CCD60` | Publish the current gameplay context after the native input poll |
| `822B7DD0` | Merge gestures into actions 0, 8, 9, 51, 52 and 53 |
| `8226F428` | Observe a successful shot by the locally controlled player |
| `824DC670` | Observe accepted health/armour loss after native damage calculation |
| `822343C0` | Observe an accepted nearby explosion |

Each observer calls the original exactly once. Radio gets separate press and
release polls, matching its native release-edge consumer. Input overlays retain
the native action polarity and physical-input baseline. Generated PPC is unchanged.

Not included: a new first-person camera, quick grenade task creation, controller
speaker audio, waveform haptics, directional damage, road-surface/weather effects,
or vehicle weapon swipes. These need additional game logic or verified transport
support beyond the implemented hooks.

## Automated verification

Build `LibertyRecomp` and `sony_feedback_tests` with the `macos-release` preset's
build tree. Run the focused Catch2 test executable and the Python suite:

```sh
python3 glue/rexglue-sdk-main/tools/run_sony_feedback_tests.py --sanitize
python3 glue/rexglue-sdk-main/gta4-recomp/tools/verify_gta4_sony_sites.py
```

The Python suite compiles the production service against Python-generated packet
and timing fixtures, tests actual SDL virtual callbacks, and checks event-watch
routing. Physical output is excluded by default; `--physical-unique-usb` opts into
a brief low-intensity probe only when exactly one wired Sony DualSense is found,
followed by explicit neutral output.

Automated USB command acceptance does not establish tactile quality. DualShock 4,
Bluetooth, Windows and Linux require their own hardware/runtime verification.
