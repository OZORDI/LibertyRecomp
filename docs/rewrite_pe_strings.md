# GTA IV PE Binary — Audio & Streaming String Analysis

PE: `tools/extracted/default.pe` (19 MB, 112,166 strings total)
ImageBase: `0x82000000`
Machine: `0x01F2` (PowerPC Big-Endian / Xbox 360)

## Section Layout

| Section | VAddr | GuestBase | RawOffset | RawSize |
|-|-|-|-|-|
| .rdata | 0x000400 | 0x82000400 | 0x000400 | 0x106E00 |
| .text | 0x140000 | 0x82140000 | 0x13B800 | 0x935A00 |
| BINK | 0xA75A00 | 0x82A75A00 | 0xA71200 | 0x10A00 |
| .data | 0xA90000 | 0x82A90000 | 0xA81C00 | 0x99600 |
| .reloc | 0x1200200 | 0x83200200 | 0xB20400 | 0xDB400 |

Address formula: `guest = 0x82000000 + raw_offset` (for .rdata where raw == vaddr). For .text: `guest = 0x82140000 + (raw - 0x13B800)`.

---

## RAGE Audio Class Names (aud*)

Standalone strings found (all from `.rdata`):

```
audController        0x8200A9F0
audEngine            0x8200AA00
audConfig
audRpf
audWaveSlot
audStreamingWaveSlot
audStreamingSound
audSound
audSimpleSound
audLoopingSound
audTwinLoopSound
audEnvelopeSound
audSequentialSound
audMultitrackSound
audRandomizedSound
audSwitchSound
audEnvironmentSound
audCrossfadeSound
audCollapsingStereoSound
audRetriggeredOverlappedSound
audOnStopSound
audWrapperSound
audSpeechSound
audForLoopSound
audIfSound
audVariableBlockSound
audVariableCurveSound
audVariablePrintValueSound
audVariableSetTimeSound
audMathOperationSound
audAssertSound
audEffect
```

---

## RTTI Type Names (.?AV...@@)

74 `aud*` RTTI entries found. Key ones:

**Entity hierarchy:**
```
audEntity@rage@@
audGtaAudioEntity
audVehicleAudioEntity
audPedAudioEntity
audAmbientAudioEntity
audSpeechAudioEntity
audRadioAudioEntity
audFrontendAudioEntity
audScriptAudioEntity
audCollisionAudioEntity
audWeaponAudioEntity
audExplosionAudioEntity
audFireAudioEntity
audDoorAudioEntity
audSmashableAudioEntity
audObjectAudioEntity
audBoatAudioEntity
audWeatherAudioEntity
audCutsceneAudioEntity
audEmitterAudioEntity
```

**Sound/voice hierarchy:**
```
audSound@rage@@
audVoicePhysical@rage@@
audVoiceXenon@rage@@
audVoicePcmXenon@rage@@
audSourceFilterEffectXenon@rage@@
audStreamingSound@rage@@
audSpeechSound@rage@@
audEnvironmentSound@rage@@
audLoopingSound@rage@@
audEnvelopeSound@rage@@
audRandomizedSound@rage@@
audSimpleSound@rage@@
```

**Effects:**
```
audEffect@rage@@
audNullEffect@rage@@
audWaveshaperEffect@rage@@ / audWaveshaperEffectXenon@rage@@
audDelayEffect@rage@@ / audDelayEffectXenon@rage@@
audConvolutionEffect@rage@@ / audConvolutionEffectXenon@rage@@
audCompressorEffect@rage@@ / audCompressorEffectXenon@rage@@
audReverbEffect@rage@@ / audReverbEffectXenon@rage@@
audBiquadFilterEffect@rage@@ / audBiquadFilterEffectXenon@rage@@
```

**Misc:**
```
audSpeechManager@@
audRadioEmitter@@
audStaticRadioEmitter@@
audEntityRadioEmitter@@
audPoliceScanner@@
audPlaceableTracker@@
audTracker@rage@@
audOcclusionGroupManager@rage@@
audOcclusionGroupInterface@rage@@
audGtaOcclusionGroup@@
audGtaOcclusionGroupManager@@
```

---

## Streaming Class Names (fiDevice / pgStream*)

**RTTI entries:**
```
fiDevice@rage@@
fiDeviceLocal@rage@@
fiDeviceXContent@rage@@
fiDeviceRelative@rage@@
fiDeviceMemory@rage@@
fiDeviceTcpIp@rage@@
fiStreamingDevice@rage@@
fiCachedDevice@rage@@
fiPackfile@rage@@
fiBaseTokenizer@rage@@
fiAsciiTokenizer@rage@@
fiBinTokenizer@rage@@
fiTokenizer@rage@@
pgStreamableRefBase@rage@@
pgBasicScheduler@rage@@
pgBase@rage@@
```

**Class type strings in .data (for device factory/registry):**

| Class | GuestAddr |
|-|-|
| fiDeviceLocal | 0x82AA2C5C |
| fiDeviceXContent | 0x82AA2C80 |
| fiDeviceMemory | 0x82B16414 |
| fiDeviceTcpIp | 0x82B17728 |
| fiStreamingDevice | 0x82AA8E4C |

---

## C++ Method Name Strings (Class::Method)

```
audWaveSlot::RequestLoad          0x8209BB50
audStreamingWaveSlot::RequestLoad 0x8209EA38
fiDevice::GetDevice               0x820859A8
fiDevice::Mount                   0x82085A34
fiDiskCache::Worker               0x8208534C
fiStream::Open                    0x82085D6C
pgStreamer::Open                  0x82085178
pgStreamer::Close                 0x82085244
pgStreamer::Worker                0x82085268
```

---

## Key Audio Error/Assert Strings with Guest Addresses

| String | GuestAddr |
|-|-|
| `[RAGE Audio] - Engine Thread` | 0x8209B43C |
| `noaudio` | 0x8209B45C |
| `noaudiothread` | 0x8209B46C |
| `xaudiothreads` | 0x8209E4B0 |
| `nospuaudio` | 0x8209E2EC |
| `audiotestslot` | 0x8209C114 |
| `audStreamingSound with no SimpleSound children` | 0x8209DCD8 |
| `Invalid bank ID in audWaveSlot::RequestLoad` | 0x8209BB50 |
| `Invalid bank ID in audStreamingWaveSlot::RequestLoad` | 0x8209EA38 |
| `audRpf: Unable to mount RPF file` | 0x8209D330 |
| `audRpf: Unable to unmount RPF file` | 0x8209D308 |
| `An XAudio voice has frozen playing wave name hash` | 0x820A0E60 |
| `Waiting for pending audio loads to finish...` | (rdata) |
| `radio track sound not of type streamingsound` | 0x82044BBC |

---

## Audio Config Keys (from xaudiothreads context at 0x8209E4B0)

These are configuration key names read by the audio engine from `audConfig`:
```
xaudiothreads
NumOutputSpeakers
NumNewPhysicalVoiceBufferVoices
NumPhysicalVoices
NumVirtualVoices
EffectsRootPath
WaveRootPath
RpfDirectory
RpfConfig
WaveSlotsConfig
```

---

## Streaming Error Strings

| String | GuestAddr |
|-|-|
| `pgStreamer::Open - file '...' isn't really a resource` | 0x82085178 |
| `pgStreamer::Open - unable to find file` | 0x820851F4 |
| `pgStreamer::Open - out of handles` | 0x82085220 |
| `pgStreamer::Close - invalid handle` | 0x82085244 |
| `[RAGE] pgStreamer::Worker` | 0x82085268 |
| `fiStream::Open(%s,%d) %s` | 0x82085D6C |
| `fiAsynchStream file` | 0x82086370 |
| `pgStreamable - Unable to allocate %dK memory` | 0x82087990 |
| `fullstreamingspew` | 0x820879E4 |
| `stream error` | 0x8208B3A0 |
| `Scripted stream %s prepared` | 0x8204014C |

---

## Audio File Paths

| Path | GuestAddr |
|-|-|
| `game:/audio.rpf` | 0x82000B78 |
| `audio:/` | (rdata) |
| `audio:/config/` | 0x8200A65C |
| `audio:/sfx/` | 0x8200A8E0 |
| `game:/xbox360/audio/` | (rdata) |
| `platform:/stream.ini` | (rdata) |

---

## Sub_8285DC40 / Sub_8285D500 Context

- `sub_8285D500` likely formats thread names — the thread name `[RAGE Audio] - Engine Thread` at `0x8209B43C` is the primary audio engine thread name.
- `sub_8285DC40` likely compares device type strings; candidates: `fiDeviceLocal`, `fiDeviceXContent`, `fiDeviceRelative` (device factory dispatch). `fiDevice::GetDevice` at `0x820859A8` and `fiDevice::Mount` at `0x82085A34` are the relevant method strings.
- `audVoicePhysical@rage@@` RTTI entry at raw `0xB1A564` (guest `0x82B28964` in .data region — note: falls outside normal .data mapping, likely BSS/late data init).

---

## Voice/SPU Notes

- `nospuaudio` (`0x8209E2EC`) — flag to disable SPU (hardware DSP) audio processing
- `xaudiothreads` (`0x8209E4B0`) — controls number of XAudio2 mixer threads
- `audVoiceXenon` / `audVoicePcmXenon` / `audSourceFilterEffectXenon` — Xenon (Xbox 360) hardware voice path; these are the concrete platform implementations of `audVoicePhysical`
- XAudio2 is the backing audio API (no WASAPI/DirectSound/SDL strings found in binary)
