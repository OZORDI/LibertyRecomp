MarathonRecomp is an attempt to use the XenonRecomp and UnleashedRecomp tool as a base to run Sonic 2006 from the Xbox 360

The current code works with many caveats, I had to stub some important methods that cause the game to essentially not "render" and there is also a race condition/memory corruption that can cause the game to crash before rendering thread or a bit after rendering thread

## TO-DO:
- [x] Code execution up to the rendering thread
- [ ] Stable rendering thread without stubbing important functions
- [ ] Cleanup of code that depends on Unleashed (and also reset git history)
- [ ] Audio (probably should work OOB at some point)
- [ ] Graphics (current code is very dependent on Unleashed)