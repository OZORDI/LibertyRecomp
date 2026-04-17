# 14 — Frame-by-Frame Chronology of the AFTER-Fix Run

Crash: first `CRASH DETECTED` at AFTER log line **1809** (after CreateRAGERT #5).
Scope: boot through crash, covering lines 1–1809 of `/tmp/liberty_AFTER.log`.
Compared against `/tmp/liberty_BEFORE.log` (crashed near line 5903).

All timestamps are `17:07:23.xxx`; offsets relative to **T+0 = 23.240** (XEX load).

## Phase map

|phase|log lines|wall time|
|-|-|-|
|Host boot (pre-guest)|1–50|21.528 → 21.992|
|Embedded shader cache load|51–53|21.992 → 21.993|
|PostProcess pipeline init|59–67|23.008 → 23.012|
|Video device up + KiSystemStartup|68–80|23.012 → 23.240|
|XEX load, module prep|82–92|23.240 → 23.286|
|Guest thread start, early syscall init|93–96|23.286|
|Semaphore burst (F80001xx → F800098C)|97–1200|23.374 → 23.386|
|Event burst (F80008xx → F8000C4Cish)|1200–1681|23.386 → 23.389|
|First asset stream (common.rpf → update)|1685–1722|23.389 → 23.412|
|GPU asset upload start (SetTex NULL, CreateTexture, VD-FIX)|1729–1784|23.413|
|Shader warm (gta_im cache hits)|1785–1798|23.413 → 23.414|
|Render target creation|1799–1807|23.414|
|**Crash in sub_82A00DC0+0xB44**|1809|23.414|

## Narrative (T+ms from XEX load)

|offset|log line|event|
|-|-|-|
|pre T|46|embedded shader cache opened (33 515 275 B)|
|pre T|59–67|PostProcess pipelines + SSAO/DoF/bloom/sun-shaft/SMAA buffers up (3456×2104 RT)|
|T+0|82|`runtime.cpp:300 Loading XEX image: game:\default.xex`|
|T+0|84|VFS: game:\default.xex resolved|
|T+45|85|VFS warn: `default.xexp` not found (expected — no delta patch)|
|T+46|93|Start Guest Thread, `main.cpp:1036`|
|T+48|96|GPU GOT patched: `0x82000768` → `0x83124900`|
|T+134|97|First `NtCreateSemaphore` (F8000018) at guest LR 82A12F08 — thread pool init|
|T+146|~1200|transition to `NtCreateEvent` burst (LR 82A12C84) — sync object pool|
|T+149|1685|**first gameplay-VFS read**: `game:\common.rpf` attrs + FindFirstFile|
|T+149|1689–1693|pool worker threads spawned (ExCreateThread, start=0x82849940)|
|T+163|1694|`game:\xbox360.rpf` opened|
|T+171|1705|`game:\audio.rpf` opened|
|T+172|1716–1720|`game:\update` probed, `update:\data\effects` enumerated|
|T+172|1721|more ExCreateThread (start=0x82A4F0E0) — streaming workers|
|T+173|1729|`SetTex` #1…#50 all NULL — shader stage reset|
|T+173|1778|`GTAIV_CreateTexture` #1–#3 (720×1, fmt 0x28280106) — UI/HUD strip textures|
|T+173|1781|`GTAIV_CreateVertexBuffer` #1–#2 (0x2d0 bytes, usage 0x1a220197/0x1a2201bf)|
|T+173|1783|**VD-FIX Create** seq=1 @ guest_vd=0x20000360, host_vd=0x4ff193000, inputCount=20|
|T+173|1784|VD-FIX Create seq=2 @ guest_vd=0x20094460, host_vd=0x4ff192000|
|T+173|1785–1790|shader cache HIT: `gta_im_vs0..vs5`|
|T+173|1791–1798|shader cache HIT: `gta_im_ps0..ps7`|
|T+174|1799|`CreateRAGERT` #1 1×1 fmt=0x0 → 0xd906a3d8|
|T+174|1800|`CreateRAGERT` #2 1×1 fmt=0x1a220197 → 0xd906a438|
|T+174|1801|`CreateRAGERT` #3 32×1 fmt=0x18280186 msaa=1 → 0xd9004738|
|T+174|1802|`CreateRAGERT` #4 32×1 fmt=0x18280186 msaa=1 → 0xd9004778|
|T+174|1803|`sub_82A55DC0` rect 0,0,240,1 → 240×1, caller=0x82A567B0|
|T+174|1804|**`Unknown format 0x8 — using RGBA8 fallback`** (video.cpp:3976)|
|T+174|1807|`CreateRAGERT` #5 8×1 fmt=0x18280186 msaa=1 → 0xd90047b8|
|**T+174**|**1809**|**CRASH** `sub_82A00DC0 + 0xB44`, fault `0x40000C000` (guest 0xC000)|

## Last event immediately before crash

```
[23.414] [CreateRAGERT] #5 8x1 fmt=0x18280186 type=0 msaa=1 out=0xd90047b8
============================================================
CRASH DETECTED (RexGlue): ACCESS_VIOLATION
Fault address: 0x000000040000C000       (guest 0x0000C000)
PC:            0x0000000102960A28
Symbol:        sub_82A00DC0 + 0xB44
```

PPC state at fault:
```
r1 =0x7010F820   r3 =0x0000BFF8   r4 =0xD906A5C8   r5 =0x00000100
r8 =0xFF35FDFF   r11=0xD906A5C0   r12=0x0000C0F8   lr =0x828D9D0C
```
`r3 = 0xBFF8` + a `+0x08` byte offset reaches guest **0xC000**, where the process has nothing mapped. This is the same `sub_82A00DC0` (memset/memcpy-style CRT path) previously analyzed — caller at LR `0x828D9D0C` passed a tiny (near-zero) base pointer, not a heap block. The VD-FIX Create of seq=1/2 plus 5 RAGE RTs is the RAGE graphics-device init; `sub_82A55DC0` at 240×1 and the `Unknown format 0x8` pass suggest this is a per-format scratch buffer init path where a length or dest pointer was read from an uninitialized slot (r3=0xBFF8 ≈ "structure-base + field" with structure-base near zero).

## Divergence from BEFORE-log

BEFORE and AFTER traces are **byte-for-byte identical through line 1797** (CreateRAGERT #1). They are the same game boot path.

|line|BEFORE (16:32:04.280)|AFTER (17:07:23.414)|
|-|-|-|
|1805|`GTAIV_CreateVertexBuffer #3 device=0x1 length=0x1 usage=0x18280186`|(same event missing — crash before it)|
|1806|`CreateRAGERT #6 1x1 fmt=0x18280186`|—|
|1807|`CreateRAGERT #7 64x1 fmt=0x1a200152 msaa=1`|—|
|1808+|second shader batch, `gta_default_vs0..vs7`|CRASH|

**Divergence point: between CreateRAGERT #5 and the next `GTAIV_CreateVertexBuffer #3`.**
AFTER dies inside the code path that BEFORE executed cleanly. What BEFORE did next (`CreateVertexBuffer #3` with `device=0x1 length=0x1 usage=0x18280186`) is what `sub_82A00DC0+0xB44` is *trying* to service in AFTER — but with a near-zero destination.

BEFORE eventually crashed ~4000 lines later (line 5903, `PC=0x10297B1E8`, fault `0x400000020`) inside the **draw path** while processing the first RAGE `DrawPrimUP` (color-marker `0xFFE1E1E1`), then again at `0x100947028` (fault `0x45`) inside `CreateGPL` / pipeline-state build. BEFORE was further into rendering; AFTER regresses and fails earlier in the same RT-setup loop.

## Interpretation

The AFTER-fix run regressed the exact section of GPU init that BEFORE previously completed. The "shader loaded + RT creation going well" narrative is correct up to CreateRAGERT #5; the regression is that the very next expected call (`GTAIV_CreateVertexBuffer #3`) is now serviced by a `sub_82A00DC0` (CRT copy/clear helper) with a degenerate pointer, hitting unmapped guest memory at 0xC000. The fix changed *something* in the allocator or RT→VB handoff between CreateRAGERT #5 and the follow-up VertexBuffer create — producing a zeroed struct-base that the CRT helper blindly dereferences.

## Appendix — events that never fire in AFTER (but do in BEFORE)

- `GTAIV_CreateVertexBuffer #3` (usage=0x18280186, length=0x1)
- `CreateRAGERT #6` 1×1 fmt=0x18280186
- `CreateRAGERT #7` 64×1 fmt=0x1a200152 msaa=1
- `CreatePSFromBytecode #7–#20` (second shader warm batch incl. `gta_default_vs*`)
- All `DrawPrimUP`, `WATCH`, `SetVertexShader`, `SetPixelShader`, `ProcDP`, `Flush`, `CreateGPL` events
- `game:\xbox360\audio\config*` VFS enumeration

These represent ~4100 lines of additional progress in BEFORE. The post-fix code never reaches rendering at all.
