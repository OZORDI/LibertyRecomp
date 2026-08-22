#!/usr/bin/env python3
"""Verify the generated PPC sites and calculate constants used by physics tracing."""

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
GENERATED = ROOT / "generated"


def function_body(source: Path, name: str) -> str:
    text = source.read_text()
    marker = f"DEFINE_REX_FUNC({name})"
    start = text.index(marker)
    end = text.find("DEFINE_REX_FUNC(", start + len(marker))
    return text[start:] if end < 0 else text[start:end]


def require(source_part: int, name: str, fragments: tuple[str, ...]) -> None:
    source = GENERATED / f"gta4_recomp.{source_part}.cpp"
    body = function_body(source, name)
    for fragment in fragments:
        if fragment not in body:
            raise RuntimeError(f"{name}: generated instruction changed: {fragment}")


def guest_address(lis_immediate: int, displacement: int) -> int:
    high = (lis_immediate & 0xFFFF) << 16
    return (high + displacement) & 0xFFFFFFFF


def main() -> None:
    require(33, "sub_82511890", (
        "// mr r31,r3",
        "// mr r30,r4",
        "// lwz r11,8(r31)",
        "// rlwinm r11,r11,2,30,31",
        "// rlwimi r11,r30,30,0,1",
        "// stw r11,8(r31)",
    ))
    require(41, "sub_825FCD68", (
        "// lwz r11,24768(r11)",
        "// stw r4,0(r31)",
        "sub_825FC660(ctx, base);",
    ))
    require(41, "sub_825FC660", (
        "// lwz r9,12(r11)",
        "// lwz r10,0(r11)",
        "// stw r3,48(r28)",
        "sub_825FC4F0(ctx, base);",
    ))
    require(41, "sub_825FC4F0", (
        "// mr r29,r3",
        "// mr r28,r4",
        "sub_82956478(ctx, base);",
        "// mr r3,r30",
    ))
    require(41, "sub_825FC448", (
        "// lwz r11,24768(r11)",
        "sub_825FC198(ctx, base);",
        "// stw r11,0(r29)",
    ))
    require(70, "sub_82956478", (
        "// mr r31,r3",
        "// mr r29,r4",
        "sub_82951980(ctx, base);",
        "// lhz r11,172(r31)",
        "// lwz r8,168(r31)",
        "// lhz r10,4(r11)",
        "// lwz r8,0(r11)",
    ))
    require(70, "sub_82951980", (
        "// mr r30,r7",
        "// mr r31,r3",
        "// mr r29,r4",
        "sub_829517D8(ctx, base);",
        "sub_8294E568(ctx, base);",
        "// mr r3,r30",
    ))
    require(70, "sub_829561D0", (
        "// lhz r5,188(r31)",
        "// lwz r4,184(r31)",
        "// lhz r5,172(r31)",
        "// lwz r6,176(r31)",
        "// lwz r4,168(r31)",
        "sub_8294E6D0(ctx, base);",
    ))
    require(70, "sub_82956510", (
        "// mr r31,r3",
        "// mr r30,r4",
        "// mr r29,r5",
        "// lhz r11,188(r31)",
        "// lwz r9,184(r31)",
    ))
    require(70, "sub_82958128", (
        "// mr r31,r3",
        "// fmr f31,f1",
        "sub_829561D0(ctx, base);",
    ))
    require(76, "sub_829C5888", (
        "// mr r17,r3",
        "// lwz r11,4(r17)",
        "// lhz r31,16(r11)",
    ))
    require(62, "sub_82869620", (
        "// mr r31,r3",
        "// mr r9,r4",
        "// mr r30,r5",
        "// vmsum3fp128 v9,v13,v13",
        "// vrefp v9,v9",
    ))

    calculated = {
        "bounds_store_global": guest_address(-31991, 24768),
        "streaming_entries_global": guest_address(-31997, 10052),
        "physics_simulator_global": guest_address(-32001, 21348),
        "streaming_state_shift": 30,
        "streaming_state_mask": (1 << 2) - 1,
        "streaming_entry_stride": 3 * 8,
    }
    for name, value in calculated.items():
        print(f"{name}=0x{value:08X}" if "global" in name else f"{name}={value}")


if __name__ == "__main__":
    main()
