# Liberty Recompiled

An unofficial PC port of Grand Theft Auto IV (Xbox 360) via static recompilation.

## Features

- **Static Recompilation** - Xbox 360 PowerPC code recompiled to native x86-64/ARM64
- **Cross-Platform** - Windows, Linux, macOS support
- **FusionFix-Compatible Mods** - Drop-in mod support via `update/` folder
- **Native Shader Pipeline** - Xbox 360 shaders converted to DXIL/SPIR-V/Metal

## Requirements

- GTA IV Xbox 360 disc image (`default.xex` + `xbox360.rpf`)
- See [BUILDING.md](docs/BUILDING.md) for build instructions

## Mod Support

Liberty Recompiled supports **FusionFix-style mod loading**:

```
LibertyRecomp/
├── game/           # Extracted game files
└── update/         # Place mods here (overrides game files)
    └── common/
        └── data/
            └── handling.dat
```

**Supported locations** (highest priority first):
- `mods/update/` 
- `update/`
- `GTAIV.EFLC.FusionFix/update/`

See [MOD_SUPPORT.md](docs/MOD_SUPPORT.md) for details.

## Documentation

- [BUILDING.md](docs/BUILDING.md) - Build instructions
- [INSTALLATION_ARCHITECTURE.md](docs/INSTALLATION_ARCHITECTURE.md) - Installation flow
- [MOD_SUPPORT.md](docs/MOD_SUPPORT.md) - Mod loading guide
- [RPF_EXTRACTION_DESIGN.md](docs/RPF_EXTRACTION_DESIGN.md) - VFS design

## License

See [COPYING](COPYING) for license information.
