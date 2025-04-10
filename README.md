MarathonRecomp is an attempt to use the XenonRecomp and UnleashedRecomp tool as a base to run Sonic 2006 from the Xbox 360

The current code renders video playback and renders some textures in the menu

Unfortunately some wrong things are ignored and also memory corruption or race condition can still occur, restarting mostly helps

# Build
- Place `:GAME_DIR:/xenon/archives/shader/shader.arc` to `UnleashedRecompLib/private` dir
- Run `sh scripts/unpack_shaders.sh`
- Build works in the same ways as in [UnleashedRecompile](https://github.com/hedge-dev/UnleashedRecomp)

## TO-DO:
- [x] Code execution up to the rendering thread
- [x] Stable rendering thread without stubbing important functions
- [ ] Cleanup of code that depends on Unleashed (and also reset git history)
- [x] Audio (probably should work OOB at some point)
- [ ] Graphics (current code is very dependent on Unleashed)
    - [x] It's rendering something
    - [ ] Correct texture rendering (alpha)
    - [ ] Correct fonts rendering
    - [ ] Probably everything 3D