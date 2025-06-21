MarathonRecomp is an attempt to use the XenonRecomp tool and UnleashedRecomp as a base to run Sonic 2006 from the Xbox 360

The current code renders video playback, correctly renders 2D and incorrect 3D

Unfortunately memory corruption or race condition can still occur, restarting mostly helps

# Build
- Place `:GAME_DIR:/xenon/archives/shader/shader.arc` and `shader_lt.arc`  to `UnleashedRecompLib/private` dir
- Run `sh scripts/unpack_shaders.sh`
- Build works in the same ways as in [UnleashedRecompile](https://github.com/hedge-dev/UnleashedRecomp)

## TO-DO:
- [x] Code execution up to the rendering thread
- [x] Stable rendering thread without stubbing important functions
- [ ] Some cpu things that causes hangs
- [ ] Cleanup of code that depends on Unleashed (and also reset git history)
- [x] Audio (probably should work OOB at some point)
     - [ ] XMA decoding (semi-works)
- [ ] Graphics (current code is very dependent on Unleashed)
    - [x] It's rendering something
    - [x] Correct menu textures rendering (alpha channel)
    - [x] Correct fonts rendering
    - [x] Some 3d rendering
    - [ ] Correct 3d rendering