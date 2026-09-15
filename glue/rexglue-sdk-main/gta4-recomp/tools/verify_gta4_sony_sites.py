#!/usr/bin/env python3
"""Check Sony gameplay snapshot and action sites against generated retail PPC."""

from pathlib import Path
import re

title = Path(__file__).resolve().parents[1]
generated = title / "generated"
source = (title / "src/gta4_sony_feedback.cpp").read_text()
policy = (title / "src/gta4_sony_action_policy.h").read_text()


def body(part, address):
    text = (generated / f"gta4_recomp.{part}.cpp").read_text()
    match = re.search(rf"DEFINE_REX_FUNC\(sub_{address}\) \{{(.*?)(?=\nDEFINE_REX_FUNC|\Z)",
                      text, re.S)
    assert match, address
    return match.group(1)


def instructions(part, address, *expected):
    text = body(part, address)
    for instruction in expected:
        assert f"// {instruction}" in text, (address, instruction)


def constant(name, expected):
    match = re.search(rf"constexpr uint32_t {name} = (0x[0-9A-Fa-f]+|[0-9]+);", source)
    assert match and int(match.group(1), 0) == expected, (name, expected)


u32 = lambda value: value & 0xFFFFFFFF
constant("kPrimaryPlayer", u32(-2102788096 - 30856))
constant("kPlayerInfoTable", u32(-2101346304 + 7280))
constant("kGameplayControl", u32(-2102198272 - 23824))
constant("kControlSpan", 4200 + 4)
constant("kControlUserOffset", 3412)
constant("kActionArray", 2328)
constant("kActionStride", 12)
constant("kEpisode", u32(-2102132736 - 27388))
constant("kGameMode", u32(-2102132736 - 27380))
constant("kAliveThreshold", u32(-2113929216 + 3432))
constant("kCutscene", u32(-2101805056 + 30704))
constant("kFrontendWidgetIndex", u32(-2101346304 - 24308))
constant("kFrontendWidgets", u32(-2100559872 + 31696))
constant("kFrontendWidgetFlags", u32(-2100559872 + 31708))
assert (31708 - 31696) // 4 == 3

instructions(3, "821B41E8", "lwz r10,-30856(r11)", "addi r11,r11,7280",
             "lwz r10,1200(r11)", "lwz r10,1400(r11)", "lwz r11,2496(r11)",
             "addi r3,r11,-23824")
instructions(39, "825B6D58", "lwz r11,1400(r3)", "lwz r11,544(r11)",
             "lwz r11,200(r11)")
instructions(40, "825D4CC8", "lwz r11,-27388(r11)")
instructions(4, "821C1328", "lwz r11,-27380(r11)")
instructions(39, "825B56B0", "bl 0x821c1328", "bl 0x82238d40", "lwz r11,1200(r31)")
assert "0xFFFFFFFFFFFFFBFF" in body(8, "82238D40")
assert "0xFFFFFBFFu" in policy
assert u32(0xFFFFFFFFFFFFFBFF) == u32(~1024)
instructions(39, "825B3320", "lfs f13,484(r11)", "lfs f0,3432(r11)",
             "lwz r11,1232(r3)", "cmpwi cr6,r11,2")
instructions(37, "8257EA40", "bl 0x821e3788")
instructions(5, "821E3788", "lwz r11,30704(r11)")
instructions(9, "8224EEF8", "addi r11,r11,-24308", "b 0x8229c4b8")
instructions(11, "8229C4B8", "cmpwi cr6,r3,-90", "addi r11,r11,31708",
             "addi r11,r11,31696", "lbz r3,3109(r11)")
instructions(23, "823D4D60", "lwz r10,20(r11)", "lwz r3,600(r10)",
             "lwz r9,20(r3)", "lwz r11,36(r11)", "lwz r3,32(r11)")
instructions(8, "822343C0", "lwz r11,32(r15)", "addi r11,r11,48", "addi r11,r15,16")

# Input IDs are checked at their actual retail consumers, not by deriving
# addresses from a different executable revision.
for action, offset in [(0, 2328), (8, 2424), (9, 2436), (51, 2940), (52, 2952), (53, 2964)]:
    assert 2328 + action * 12 == offset
instructions(35, "8253C120", "lbz r11,2328(r4)", "lwz r11,860(r3)")
instructions(22, "823CF9C0", "addi r3,r28,2424", "addi r3,r28,2436")
instructions(13, "822D48F0", "bl 0x822d3230")
assert "{0, 8, 9, 51, 52, 53}" in policy

# Existing independent keyboard checks establish these shared global facts.
input_source = (title / "src/gta4_input_hooks.cpp").read_text()
for new_name, old_name in [
    ("kPauseVisible", "kPauseMenuVisibleAddress"),
    ("kPauseTransition", "kPauseMenuTransitionAddress"),
    ("kPhoneCreated", "kPhoneCreatedAddress"),
    ("kPhoneOffscreen", "kPhoneMovingOffscreenAddress"),
    ("kPhoneIndex", "kPhoneRenderIndexAddress"),
    ("kPhoneObjects", "kPhoneRenderObjectTableAddress"),
    ("kInputClock", "kGameInputTimeAddress"),
]:
    match = re.search(rf"constexpr uint32_t {old_name} = (0x[0-9A-Fa-f]+);", input_source)
    assert match, old_name
    constant(new_name, int(match.group(1), 0))
mp_policy = (title / "src/gta4_multiplayer_64_policy.h").read_text()
assert "kLegacyPeerCapacity = 16" in mp_policy
assert "kLegacyPlayerInfoGenerationTableAddress = 0x82C01C30" in mp_policy
constant("kPlayerGenerationTable", 0x82C01C30)
constant("kRetailPlayerCount", 16)

print("GTA IV Sony snapshot/action sites verified against generated retail PPC")
