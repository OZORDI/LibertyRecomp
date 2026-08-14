# Liberty Recompiled installation architecture

## Scope

The installer is part of the current RexGlue GTA IV consumer. It runs from
`GTA4App::OnFinalizePaths` before `Runtime::Setup`, so no guest thread, XEX, or
graphics system is active while files are being validated and published.

The implementation is in:

- `glue/rexglue-sdk-main/gta4-recomp/src/install/gta4_install_dialog.*`
- `glue/rexglue-sdk-main/gta4-recomp/src/install/gta4_installer.*`
- `glue/rexglue-sdk-main/gta4-recomp/src/install/gta4_rpf_extractor.*`
- `glue/rexglue-sdk-main/gta4-recomp/src/gta4_app.*`

The old `LibertyRecomp/install` implementation is not linked into the current
consumer. In particular, the installer never invokes `xextool`, Mono,
SparkCLI, `unzip`, or another process.

## Installed layout

The platform user-data root is supplied by RexGlue and normalized by
`GTA4App::OnConfigurePaths`:

```text
LibertyRecomp/
├── game/
│   ├── default.xex
│   ├── default.xexp
│   ├── aes_key.bin
│   ├── common.rpf
│   ├── xbox360.rpf
│   ├── audio.rpf
│   ├── common/
│   ├── xbox360/
│   ├── audio/
│   ├── update/
│   └── .install-manifest
├── dlc/
│   ├── TLAD/
│   └── TBOGT/
├── shader_cache/
└── saves/
```

Default desktop roots are:

- Windows: `%LOCALAPPDATA%\LibertyRecomp\`
- Linux: `$XDG_DATA_HOME/LibertyRecomp/`, falling back to
  `~/.local/share/LibertyRecomp/`
- macOS: `~/Library/Application Support/LibertyRecomp/`

## Accepted sources

### Base game

The base-game picker accepts:

- an Xbox 360 ISO containing a GDFX filesystem;
- an extracted directory;
- an STFS/SVOD package supported by RexGlue.

The source must contain exactly one `default.xex`. The installer validates the
XEX2 header, required optional headers, title-module flag, GTA IV title ID
`545407F2`, and bounded header offsets before copying anything.

### Required title update

The retail XEX requires the GTA IV v8 patch. The update picker accepts:

- an STFS/SVOD package containing exactly one `default.xexp`; or
- a raw `default.xexp`.

The installer validates the patch-module and delta flags, source and target
versions, patch digest, exact supported patch hash, and the SHA-1 binding from
the XEXP delta descriptor to the base XEX RSA signature.

The original executable and patch are staged as siblings:

```text
game/default.xex
game/default.xexp
```

For an STFS/SVOD update, the directory containing the validated patch is
normalized into `game/update/`; no package wrapper or unrelated top-level file
is allowed to overlay the base-game tree. The validated patch is also copied to
the required sibling path above. A raw XEXP produces only the sibling file.

The installer does not create a permanently patched XEX. RexGlue resolves the
sibling by appending `p` to the XEX path in
`glue/rexglue-sdk-main/src/system/user_module.cpp`, then applies the LZX delta
in memory through `XexModule::ApplyPatch` in
`glue/rexglue-sdk-main/src/system/xex_module.cpp`. A previously installed,
known v8 XEX is accepted for backward compatibility.

### Episodes

Each episode picker accepts an extracted directory or an STFS/SVOD package.
Package title metadata, when present, must match GTA IV. Detection uses content
identity rather than filenames:

| Episode | Required payload markers |
|---|---|
| The Lost and Damned | `setup2.xml`, `content.dat`, `e1_audio.xml`, and no `e2_audio.xml` |
| The Ballad of Gay Tony | `setup2.xml`, `content.dat`, `e2_audio.xml`, and no `e1_audio.xml` |

The matching payload directory is copied to `dlc/TLAD` or `dlc/TBOGT`. DLC can
be added later with `--install-dlc` without replacing the base installation.
ZIP-wrapped packages are intentionally not accepted.

## Installation sequence

1. `GTA4App::OnFinalizePaths` verifies the current XEX/XEXP pair.
2. A missing install, `--install`, or `--install-dlc` opens the ImGui setup
   dialog before runtime setup.
3. SDL's asynchronous native file/folder picker collects source paths.
4. Sources are mounted read-only with `HostPathDevice`, `DiscImageDevice`, or
   `StfsContainerDevice`.
5. The complete source tree is validated and measured. Unsafe components,
   case-insensitive path collisions, excessive entry counts, and insufficient
   free space fail before publication.
6. Files are copied to `LibertyRecomp/.install-staging`. The base tree, update
   payload, sibling XEXP, and selected DLC are never published piecemeal.
7. The bundled 32-byte RPF key is copied into the staged game root.
8. `common.rpf`, `xbox360.rpf`, and `audio.rpf` are extracted to their matching
   loose directories. Original archives are preserved. Nested RPF2 archives
   are expanded beside themselves into directories named after their stems.
9. The staged XEX/XEXP pair and DLC marker files are revalidated, and the game
   manifest is written.
10. Existing destinations are renamed to private backups. Staged directories
    are renamed into place. Any publish failure restores the backups.
11. Only after successful publication does the dialog resume RexGlue setup.

Cancellation is checked during chunked source copies and RPF entry extraction.
A cancelled or failed operation removes only the private staging directory.

## Parser behavior and provenance

### GDFX ISO

`DiscImageDevice` is derived from the in-repository Xenia implementation at
`tools/xenia-master-1/src/xenia/vfs/devices/disc_image_device.cc`. The RexGlue
version adds installer-facing hardening:

- the full volume descriptor must fit in the mapped image;
- sector multiplication and offset addition are overflow checked;
- root, directory, and file extents must remain inside the image;
- directory entry headers and names must remain inside their directory table;
- cyclic or duplicate tree ordinals are rejected;
- recursion depth and total entry count are bounded.

Only the GDFX filesystem is parsed. The installer does not emulate disc
security sectors or physical-drive behavior.

### STFS/SVOD

The installer uses RexGlue's current
`StfsContainerDevice::ReadPackageHeader` and `StfsContainerDevice` rather than
the dead installer's separate parser. Package metadata and every mounted entry
remain read-only; extraction passes through the same bounded copy and safe-path
checks as ISO and directory sources.

### XEX/XEXP

Installer validation uses the same structures from
`rex/system/util/xex2_info.h` as the runtime loader. The expected sibling
placement and in-memory patch lifecycle are corroborated by RexGlue's
`user_module.cpp`, `xex_module.cpp`, and `lzx.cpp`. The local xextool reference
is used only to cross-check XEX/XEXP structure and version semantics; xextool is
not shipped or executed.

### RPF2

The RPF2 reader is grounded in the in-repository SparkIV `RageLib` reference
and the current `rpf_button_prompts.cpp` parser:

- the 20-byte little-endian header is plaintext;
- the table of contents begins at `0x800`;
- a nonzero encryption flag means the TOC is encrypted;
- encrypted TOCs use the GTA IV 32-byte AES key and the game's repeated ECB
  transform;
- non-resource entries marked compressed use raw deflate;
- resource entries are copied byte-for-byte;
- directory ranges, names, data extents, graph cycles, expansion sizes, nested
  depth, and output paths are validated.

Failed decompression is fatal. Compressed bytes are never silently installed
as if extraction succeeded.

## Shader handling

The installer materializes the original RPF contents, including `.fxc` and
`.sps` assets, but does not run the dead installer's offline DXIL/SPIR-V/AIR
conversion pipeline.

The current `gta4-native` renderer owns shader loading and caching. Its compiled
Liberty shader cache is loaded by
`glue/rexglue-sdk-main/src/graphics/gta4_native/graphics_system.cpp`, while
driver-native pipeline caches are stored under the configured
`shader_cache/` root. Installation and GPU initialization therefore remain
separate phases.

## Verification and launch arguments

| Argument | Behavior |
|---|---|
| `--install` | Force a complete install wizard before runtime setup. |
| `--install-dlc` | Open episode-only installation when the base pair is valid. |
| `--install-check` | Revalidate the installed XEX/XEXP pair and any present episode roots. |

Launch readiness requires either the supported base-XEX/v8-XEXP pair or the
known backward-compatible v8 XEX. An existing episode directory is rejected if
its required marker files are incomplete.

## Loose portable save files

GTA IV saved-game packages are redirected to `saves/`. RexGlue mounts a
writable host directory at the Xbox saved-game package boundary, keeping files
loose rather than embedding them in an emulator-specific container.

```text
saves/<XUID>/<title-id>/<content-type>/<package-name>/
saves/<XUID>/<title-id>/Headers/<content-type>/<package-name>.header
```

The `saved_game_root` runtime override may select a custom location. Save
package creation, open, delete, and mount events are logged; verbose logs expose
individual file operations under the `[PortableSave]` marker.

## Out of scope

Multiplayer backend configuration, graphics settings, controller selection,
and save migration are runtime concerns and are not installer wizard steps.
