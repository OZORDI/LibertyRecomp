# GTA IV PPC Recomp — RTTI, Debug Strings & Audio/Render Analysis

Source data: `glue/rexglue-sdk-main/gta4-recomp/generated/` (77 recomp files, 172 MB total)
Binary: `gta_iv/default.bin` (17 MB stripped XEX image)

---

## 1. Audio/Streaming String Literals in Recomp Files

The recompiled C++ files contain **no embedded string literals** referencing audio, stream, sound, voice, or mixer. All GTA IV strings are in the binary image and loaded at runtime via `PPC_LOAD_U32/U64` from fixed addresses. The recomp files reference audio only through Xbox 360 kernel import calls.

---

## 2. XAudio Kernel Import Call Sites

### XAudioGetSpeakerConfig (0x82A74DB4)

| File | Line | Enclosing Function |
|-|-|-|
| gta4_recomp.62.cpp | 10855 | sub_8291F508 |
| gta4_recomp.63.cpp | 4457 | sub_82935A70 |

Context: called to query the speaker configuration before audio device setup.

### XAudioGetVoiceCategoryVolumeChangeMask / XAudioGetVoiceCategoryVolume (0x82A75774 / 0x82A75794)

| File | Lines | Enclosing Function |
|-|-|-|
| gta4_recomp.2.cpp | 2134, 2160 | sub_82191228 |

Context: `sub_82191228` is a volume-update loop — it reads a change mask, iterates up to 2 categories, and fetches the current volume for each changed category.

### XAudio Render Driver Client (0x82A757E4 / 0x82A757F4 / 0x82A75804)

| Function | Address | Description |
|-|-|-|
| XAudioSubmitRenderDriverFrame | 0x82A757E4 | Called from sub_82194698 — submits the completed audio mix frame |
| XAudioUnregisterRenderDriverClient | 0x82A757F4 | Called from sub_821945C0 (shutdown) and sub_82194708 (re-register path) |
| XAudioRegisterRenderDriverClient | 0x82A75804 | Called from sub_82194708 (re-register path) |

`sub_82194698` (in gta4_recomp.2.cpp) calls an indirect vtable dispatch at offset +32 of an object then submits the result as an audio render frame — this is the audio mix callback stub.

`sub_82194708` implements unregister+register cycling (teardown/reinit of render client).

---

## 3. XamVoice (Chat Voice) Call Sites

All in **gta4_recomp.70.cpp** — the multiplayer/voice module.

| Kernel Function | Import Address | Enclosing Function | Notes |
|-|-|-|-|
| XamVoiceHeadsetPresent | 0x82A75454 | sub_82A28FF0 | Queries headset presence for user slot |
| XamVoiceClose | 0x82A75464 | sub_82A295F0, sub_82A29C10, sub_82A29D08 | Cleanup path |
| XamVoiceSubmitPacket | 0x82A75474 | sub_82A297F8, sub_82A2D110 | Submit encoded voice packet |
| XamVoiceCreate | 0x82A75484 | sub_82A29D08 | Create voice channel |

---

## 4. Vd (Xenon GPU / Render) Kernel Import Call Sites

All in **gta4_recomp.71.cpp** — the rendering module.

| Kernel Function | Import Address | Enclosing Function |
|-|-|-|
| VdEnableDisableClockGating | 0x82A75824 | sub_82A3F2D8 |
| VdGetSystemCommandBuffer | 0x82A75854 | sub_82A467D8 |
| VdSwap | 0x82A75844 | sub_82A467D8 |
| VdPersistDisplay | 0x82A75834 | sub_82A467D8 |
| VdInitializeRingBuffer | 0x82A75894 | sub_82A49D08 |
| VdEnableRingBufferRPtrWriteBack | 0x82A75884 | sub_82A49D08 |
| VdSetSystemCommandBufferGpuIdentifierAddress | 0x82A758A4 | sub_82A49D08, sub_82A50AF0 |
| VdSetDisplayMode | 0x82A75924 | sub_82A50700 |
| VdGetCurrentDisplayInformation | 0x82A75914 | sub_82A50700 |
| VdInitializeEngines | 0x82A75944 | sub_82A507A8 |
| VdSetGraphicsInterruptCallback | 0x82A75934 | sub_82A507A8, sub_82A50AF0 |
| VdShutdownEngines | 0x82A758F4 | sub_82A50AF0 |
| VdQueryVideoMode | 0x82A75904 | sub_82A507B8 (approx), sub_82A507A8 |
| VdGetCurrentDisplayGamma | 0x82A758B4 | sub_82A45F10 (approx) |
| VdQueryVideoFlags | 0x82A75964 | sub_82A50910 (approx) |
| VdCallGraphicsNotificationRoutines | 0x82A75974 | sub_82A50960 (approx) |
| VdInitializeScalerCommandBuffer | 0x82A75984 | sub_82A60170 (approx) |
| VdRetrainEDRAMWorker | 0x82A759A4 | sub_82A60348 (approx) |
| VdRetrainEDRAM | 0x82A75994 | sub_82A60370 (approx) |
| VdIsHSIOTrainingSucceeded | 0x82A75954 | sub_82A50860 (approx) |

`sub_82A467D8` is the **frame present / swap** function — it calls GetSystemCommandBuffer, dispatches a command buffer, then calls VdSwap and VdPersistDisplay.

`sub_82A507A8` is the **Vd init** function — calls VdInitializeEngines, VdSetGraphicsInterruptCallback.

`sub_82A50AF0` is the **Vd shutdown** function — calls VdSetGraphicsInterruptCallback (clear), VdShutdownEngines.

---

## 5. Thread Creation Call Sites

| File | Lines | Enclosing Function | Calls |
|-|-|-|-|
| gta4_recomp.2.cpp | 1496, 1516, 1521 | sub_82190B48 | ExCreateThread + KeSetBasePriorityThread + KeResumeThread |
| gta4_recomp.69.cpp | 34383 | sub_82A11478 | KeSetBasePriorityThread |
| gta4_recomp.69.cpp | 34544 | sub_82A11580 | KeSetAffinityThread |
| gta4_recomp.69.cpp | 50721 | sub_82A1A4B8 | ExCreateThread |
| gta4_recomp.69.cpp | 54393 | sub_82A1BD40 | ExCreateThread + KeSetBasePriorityThread |

`sub_82190B48` is the primary thread-spawn helper: creates thread, sets priority 15, resumes, and dereferences the handle. It is in the 0x8219xxxx audio block.

---

## 6. RTTI Class Names from Binary (.?AV prefix, MSVC mangling)

2820 total RTTI class names found in `default.bin`. Relevant subset below.

### Audio Classes (audXxx / rage::audXxx)

```
audAmbientAudioEntity
audAssertSound (rage)
audBoatAudioEntity
audCollapsingStereoSound (rage)
audCollisionAudioEntity
audCrossfadeSound (rage)
audCutsceneAudioEntity
audDoorAudioEntity
audEmitterAudioEntity
audEnvelopeSound (rage)
audEnvironmentSound (rage)
audExplosionAudioEntity
audFireAudioEntity
audForLoopSound (rage)
audFrontendAudioEntity
audGtaAudioEntity
audIfSound (rage)
audLoopingSound (rage)
audMathOperationSound (rage)
audMultitrackSound (rage)
audObjectAudioEntity
audOnStopSound (rage)
audPedAudioEntity
audRadioAudioEntity
audRandomizedSound (rage)
audRetriggeredOverlappedSound (rage)
audScriptAudioEntity
audSequentialSound (rage)
audSimpleSound (rage)
audSmashableAudioEntity
audSound (rage)                         <- base class
audSpeechAudioEntity
audSpeechSound (rage)
audStreamingSound (rage)
audSwitchSound (rage)
audTwinLoopSound (rage)
audVariableBlockSound (rage)
audVariableCurveSound (rage)
audVariablePrintValueSound (rage)
audVariableSetTimeSound (rage)
audVehicleAudioEntity
audVoicePcmXenon (rage)
audVoicePhysical (rage)
audVoiceXenon (rage)
audWeaponAudioEntity
audWeatherAudioEntity
audWrapperSound (rage)
crmtManagerMixer (rage)                 <- mixer
```

### Streaming / File Device Classes

```
fiCachedDevice (rage)
fiDevice (rage)                         <- base class
fiDeviceLocal (rage)
fiDeviceMemory (rage)
fiDeviceRelative (rage)
fiDeviceTcpIp (rage)
fiDeviceXContent
fiStreamingDevice
parStream (rage)                        <- parameter serialization stream
parStreamIn (rage)
parStreamInRbf (rage)
parStreamInXml (rage)
parStreamOut (rage)
parStreamOutRbf (rage)
parStreamOutXml (rage)
pgStreamableRef<frag::fragType> (rage)
pgStreamableRefBase (rage)
MemoryStream (NMutils)
MemoryStreamReader (NMutils)
```

### Render Classes (Render phases, DCs, grc)

```
CDrawCommandAllocator
CRenderPhase                            <- base class
CRenderPhaseBlit
CRenderPhaseCloudGeneration
CRenderPhaseDeferredLighting_LightsToScreen
CRenderPhaseDeferredLighting_SceneToGBuffer
CRenderPhaseDrawScene
CRenderPhaseFrontEnd
CRenderPhaseHeight
CRenderPhaseHtml
CRenderPhaseInteriorReflection
CRenderPhaseMirrorReflection
CRenderPhasePhoneModel
CRenderPhasePlayerSettings
CRenderPhasePostRenderViewport
CRenderPhasePreRenderViewport
CRenderPhaseRadar
CRenderPhaseRainUpdate
CRenderPhaseReflection
CRenderPhaseScript2d
CRenderPhaseSetDefaultRenderState
CRenderPhaseTreeImposters
CRenderPhaseWarpShadow
CRenderPhaseWaterReflection
CRenderPhaseWaterSurface
CViewport3DScene
CViewportFrontend3DScene
grcRenderTarget (rage)
grcRenderTargetXenon (rage)
ptxGpuRenderShader (rage)
ptxRenderSetup (rage)
ProceduralTextureRenderTargetDef (rage)
```

### Thread / Script Classes

```
GtaThread
scrThread (rage)
```

### Misc Notable

```
CEventSoundDynamic
CTaskSimpleSayAudio
CTaskComplexCarDriveMissionFleeScene
CCamCutscene
CCutsceneObject
```

---

## 7. Debug / Assert Strings Revealing Function Names

Extracted from binary string table:

```
"Invalid bank ID in audWaveSlot::RequestLoad (%u)"
"Invalid bank ID in audStreamingWaveSlot::RequestLoad (%u)"
"Failed to obtain load block from streaming Wave Bank %u with a start offset of %dms"
"Failed to prepare StreamingSound from Wave Bank %u with a start offset of %dms"
"Error loading block from streaming Wave Bank %s"
"audAssertSound: %u"
"Failed to load audio extra content %s"
"Failed to load extra audio content [%s]: %s"
"Failed to prepare audio track for cutscene: %s"
"Failed to create radio track sound %u"
"Failed to find smash sound: \"%s\""
"PFailed to load audio extra content %s"

"pgStreamer::Open - file '%s' isn't really a resource, or the file format is out of date..."
"pgStreamer::Open - unable to find file '%s'"
"pgStreamer::Open - out of handles"
"pgStreamer::Close - invalid handle"
"[RAGE] pgStreamer::Worker"
"fiStream::Open(%s,%d) %s"
"fiDevice::GetDevice - Filename '%s' not in mount list, no default device"
"fiDevice::Mount - mount name must end in a slash"
"fiDevice::Mount - mount name too long"
"fiAssetManager::SetPath - Setting write path to '%s'"
"fiDiskCache::Worker - copy of '%s' completed."
"fiPackfile::Open(%s) - inflateInit failed."
"fiPackfile::OpenBulk - '%s' is externally compressed, cannot open with OpenBulk."

"scrThread::Validate - NATIVE definition for %x is missing, PC = %d"
"via scrThread::Kill on myself"

"Bad resource type %d in grcTextureFactoryXenon::PlaceTexture"

"ART::ARTContext::init"
"ART::Rockstar::NmRsCBUTaskManager::init"
"ART::Rockstar::NmRsEngine::initialiseAgent"
"NMutils::MemoryStream::MemoryStream"
"NMutils::MemoryStream::~MemoryStream"
"NMutils::MemoryStream::reAlloc"
"NMutils::MemoryStream::setExternalDataBlock"
```

---

## 8. Vtable Data

146 vtables extracted from binary (see `gta_iv/gta4_vtables.txt` and `gta_iv/vtable_prepopulate.h`). All are in the 0x8214xxxx–0x8225xxxx range. No RTTI-to-vtable mapping is present in the binary (the RTTI `.?AV` names are in `.rdata` but the vtable-to-typeinfo pointers were not extracted by Ghidra's current analysis).

Key large vtables (likely base classes):

| Address | Entry Count | Likely Class |
|-|-|-|
| 0x821498C8 | 254 | Unknown large base (possibly CEntity or CObject) |
| 0x82149DC4 | 160 | Unknown |
| 0x8215DB18 | 228 | Unknown |
| 0x821518A0 | 205 | Unknown |
| 0x821602FC | 205 | Unknown |
| 0x82210018 | 9 | Small interface |
| 0x822109FC | 183 | Unknown |
| 0x82210D58 | 154 | Unknown |
| 0x82241FB8 | 101 | Unknown |
| 0x822503B8 | 61 | Unknown |
| 0x82233F00 | 78 | Unknown |

No vtables were found above 0x8225xxxx. The audio (0x8219xxxx) and render (0x82A4xxxx–0x82A5xxxx) modules reference vtables that are either in unextracted regions or are stored only as raw function pointers loaded at runtime via `PPC_CALL_INDIRECT_FUNC`.

---

## 9. Complete XAudio / Vd / Thread Import Address Table

### Audio & Voice

| Import | Address |
|-|-|
| XAudioGetSpeakerConfig | 0x82A74DB4 |
| XAudioGetVoiceCategoryVolumeChangeMask | 0x82A75774 |
| XAudioGetVoiceCategoryVolume | 0x82A75794 |
| XAudioSubmitRenderDriverFrame | 0x82A757E4 |
| XAudioUnregisterRenderDriverClient | 0x82A757F4 |
| XAudioRegisterRenderDriverClient | 0x82A75804 |
| XamVoiceHeadsetPresent | 0x82A75454 |
| XamVoiceClose | 0x82A75464 |
| XamVoiceSubmitPacket | 0x82A75474 |
| XamVoiceCreate | 0x82A75484 |

### Vd (GPU/Display)

| Import | Address |
|-|-|
| VdEnableDisableClockGating | 0x82A75824 |
| VdPersistDisplay | 0x82A75834 |
| VdSwap | 0x82A75844 |
| VdGetSystemCommandBuffer | 0x82A75854 |
| VdEnableRingBufferRPtrWriteBack | 0x82A75884 |
| VdInitializeRingBuffer | 0x82A75894 |
| VdSetSystemCommandBufferGpuIdentifierAddress | 0x82A758A4 |
| VdGetCurrentDisplayGamma | 0x82A758B4 |
| VdShutdownEngines | 0x82A758F4 |
| VdQueryVideoMode | 0x82A75904 |
| VdGetCurrentDisplayInformation | 0x82A75914 |
| VdSetDisplayMode | 0x82A75924 |
| VdSetGraphicsInterruptCallback | 0x82A75934 |
| VdInitializeEngines | 0x82A75944 |
| VdIsHSIOTrainingSucceeded | 0x82A75954 |
| VdQueryVideoFlags | 0x82A75964 |
| VdCallGraphicsNotificationRoutines | 0x82A75974 |
| VdInitializeScalerCommandBuffer | 0x82A75984 |
| VdRetrainEDRAM | 0x82A75994 |
| VdRetrainEDRAMWorker | 0x82A759A4 |

### Threads & Sync

| Import | Address |
|-|-|
| ExCreateThread | 0x82A752E4 |
| ExTerminateThread | 0x82A75064 |
| KeDelayExecutionThread | 0x82A752D4 |
| KeInitializeSemaphore | 0x82A75754 |
| KeReleaseSemaphore | 0x82A75784 |
| KeResumeThread | 0x82A75764 |
| KeSetAffinityThread | 0x82A74EC4 |
| KeSetBasePriorityThread | 0x82A74E94 |
| KeSetDisableBoostThread | 0x82A74EB4 |
| NtCreateSemaphore | 0x82A74FD4 |
| NtReleaseSemaphore | 0x82A74FE4 |
| NtResumeThread | 0x82A75024 |
| RtlInitializeCriticalSection | 0x82A74D74 |
