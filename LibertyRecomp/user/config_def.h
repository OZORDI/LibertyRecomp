// This file gets included in both config.h and config.cpp, with their own macros changing
// the preprocessed output. The header is only going to have the declarations this way.

CONFIG_DEFINE_ENUM_LOCALISED("System", ELanguage, Language, ELanguage::English, true);
CONFIG_DEFINE_ENUM_LOCALISED("System", EVoiceLanguage, VoiceLanguage, EVoiceLanguage::English, false);
CONFIG_DEFINE_LOCALISED("System", bool, Subtitles, true, false);
CONFIG_DEFINE_LOCALISED("System", bool, Hints, true, false);
CONFIG_DEFINE_LOCALISED("System", bool, ControlTutorial, true, false);
CONFIG_DEFINE_LOCALISED("System", bool, Autosave, true, false);
CONFIG_DEFINE_LOCALISED("System", bool, AchievementNotifications, true, false);
CONFIG_DEFINE("System", bool, ShowConsole, false, false);

CONFIG_DEFINE_ENUM_LOCALISED("Input", ECameraRotationMode, HorizontalCamera, ECameraRotationMode::Reverse, false);
CONFIG_DEFINE_ENUM_LOCALISED("Input", ECameraRotationMode, VerticalCamera, ECameraRotationMode::Normal, false);
CONFIG_DEFINE_LOCALISED("Input", bool, AllowBackgroundInput, false, false);
CONFIG_DEFINE_ENUM_LOCALISED("Input", EControllerIcons, ControllerIcons, EControllerIcons::Auto, false);
CONFIG_DEFINE_ENUM_LOCALISED("Input", ELightDash, LightDash, ELightDash::X, false);
CONFIG_DEFINE_ENUM_LOCALISED("Input", ESlidingAttack, SlidingAttack, ESlidingAttack::X, false);

// GTA IV Control Scheme - Face Buttons
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_A, SDL_SCANCODE_LSHIFT, false);      // Sprint (on foot) / Handbrake (in car)
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_B, SDL_SCANCODE_LCTRL, false);       // Crouch/Duck
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_X, SDL_SCANCODE_SPACE, false);       // Jump
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_Y, SDL_SCANCODE_F, false);           // Enter/Exit Vehicle

// GTA IV Control Scheme - D-Pad (weapon switching via mouse wheel)
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_DPadUp, SDL_SCANCODE_UP, false);     // Phone
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_DPadDown, SDL_SCANCODE_T, false);    // Radar Zoom
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_DPadLeft, SDL_SCANCODE_UNKNOWN, false);  // Previous Weapon (Mouse Wheel Down)
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_DPadRight, SDL_SCANCODE_UNKNOWN, false); // Next Weapon (Mouse Wheel Up)

// GTA IV Control Scheme - Start/Back
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_Start, SDL_SCANCODE_ESCAPE, false);  // Pause Menu
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_Back, SDL_SCANCODE_V, false);        // Change Camera

// GTA IV Control Scheme - Triggers (handled by mouse buttons)
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_LeftTrigger, SDL_SCANCODE_UNKNOWN, false);  // Aim (RMB)
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_RightTrigger, SDL_SCANCODE_UNKNOWN, false); // Shoot (LMB)

// GTA IV Control Scheme - Shoulders
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_LeftBumper, SDL_SCANCODE_E, false);  // Action (hail taxi, etc.)
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_RightBumper, SDL_SCANCODE_Q, false); // Take Cover

// GTA IV Control Scheme - Movement (WASD)
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_LeftStickUp, SDL_SCANCODE_W, false);    // Move Forward
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_LeftStickDown, SDL_SCANCODE_S, false);  // Move Backward
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_LeftStickLeft, SDL_SCANCODE_A, false);  // Move Left
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_LeftStickRight, SDL_SCANCODE_D, false); // Move Right

// GTA IV Control Scheme - Camera (handled by mouse)
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_RightStickUp, SDL_SCANCODE_UNKNOWN, false);    // Mouse Y
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_RightStickDown, SDL_SCANCODE_UNKNOWN, false);  // Mouse Y
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_RightStickLeft, SDL_SCANCODE_UNKNOWN, false);  // Mouse X
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_RightStickRight, SDL_SCANCODE_UNKNOWN, false); // Mouse X

// GTA IV Additional Keys
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_Reload, SDL_SCANCODE_R, false);       // Reload
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_LookBehind, SDL_SCANCODE_C, false);   // Look Behind
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_Horn, SDL_SCANCODE_H, false);         // Horn (in vehicle)
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_Headlight, SDL_SCANCODE_G, false);    // Headlights (in vehicle)
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_RadioNext, SDL_SCANCODE_X, false);    // Next Radio Station
CONFIG_DEFINE_ENUM("Bindings", SDL_Scancode, Key_RadioPrev, SDL_SCANCODE_Z, false);    // Previous Radio Station

CONFIG_DEFINE_LOCALISED("Input", float, MouseSensitivityX, 1.0f, false);
CONFIG_DEFINE_LOCALISED("Input", float, MouseSensitivityY, 1.0f, false);
CONFIG_DEFINE_LOCALISED("Input", bool, MouseInvertY, false, false);
CONFIG_DEFINE_LOCALISED("Input", float, MouseSmoothing, 0.5f, false);

CONFIG_DEFINE_LOCALISED("Audio", float, MasterVolume, 1.0f, false);
CONFIG_DEFINE_LOCALISED("Audio", float, MusicVolume, 0.6f, false);
CONFIG_DEFINE_LOCALISED("Audio", float, EffectsVolume, 0.6f, false);
CONFIG_DEFINE_ENUM_LOCALISED("Audio", EChannelConfiguration, ChannelConfiguration, EChannelConfiguration::Stereo, true);
CONFIG_DEFINE_LOCALISED("Audio", bool, MuteOnFocusLost, true, false);
CONFIG_DEFINE_LOCALISED("Audio", bool, MusicAttenuation, false, false);

CONFIG_DEFINE("Video", std::string, GraphicsDevice, "", true);
CONFIG_DEFINE_ENUM("Video", EGraphicsAPI, GraphicsAPI, EGraphicsAPI::Auto, true);
CONFIG_DEFINE("Video", int32_t, WindowX, WINDOWPOS_CENTRED, false);
CONFIG_DEFINE("Video", int32_t, WindowY, WINDOWPOS_CENTRED, false);
CONFIG_DEFINE_LOCALISED("Video", int32_t, WindowSize, -1, false);
CONFIG_DEFINE("Video", int32_t, WindowWidth, 1280, false);
CONFIG_DEFINE("Video", int32_t, WindowHeight, 720, false);
CONFIG_DEFINE_ENUM("Video", EWindowState, WindowState, EWindowState::Normal, false);
CONFIG_DEFINE_LOCALISED("Video", int32_t, Monitor, 0, false);
CONFIG_DEFINE_ENUM_LOCALISED("Video", EAspectRatio, AspectRatio, EAspectRatio::Auto, false);
CONFIG_DEFINE_LOCALISED("Video", float, ResolutionScale, 1.0f, false);
CONFIG_DEFINE_LOCALISED("Video", bool, Fullscreen, true, false);
CONFIG_DEFINE_LOCALISED("Video", bool, VSync, true, false);
CONFIG_DEFINE_ENUM("Video", ETripleBuffering, TripleBuffering, ETripleBuffering::Auto, false);
CONFIG_DEFINE_LOCALISED("Video", int32_t, FPS, 60, false);
CONFIG_DEFINE("Video", bool, ShowFPS, false, false);
CONFIG_DEFINE("Video", uint32_t, MaxFrameLatency, 2, false);
CONFIG_DEFINE_LOCALISED("Video", float, Brightness, 0.5f, false);
CONFIG_DEFINE_ENUM_LOCALISED("Video", EAntiAliasing, AntiAliasing, EAntiAliasing::MSAA4x, false);
CONFIG_DEFINE_LOCALISED("Video", bool, TransparencyAntiAliasing, true, false);
CONFIG_DEFINE("Video", uint32_t, AnisotropicFiltering, 16, false);
CONFIG_DEFINE_ENUM_LOCALISED("Video", EShadowResolution, ShadowResolution, EShadowResolution::x4096, false);
CONFIG_DEFINE_ENUM_LOCALISED("Video", EReflectionResolution, ReflectionResolution, EReflectionResolution::Half, false);
CONFIG_DEFINE_ENUM_LOCALISED("Video", ERadialBlur, RadialBlur, ERadialBlur::Original, false);
CONFIG_DEFINE_ENUM_LOCALISED("Video", ECutsceneAspectRatio, CutsceneAspectRatio, ECutsceneAspectRatio::Original, false);
CONFIG_DEFINE_ENUM_LOCALISED("Video", EUIAlignmentMode, UIAlignmentMode, EUIAlignmentMode::Edge, false);

CONFIG_DEFINE_HIDDEN("Codes", bool, AntigravityRetainsMomentum, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, ControllableBoundAttack, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, ControllableSpinkick, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, ControllableTeleportDash, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, DisableDWMRoundedCorners, false, true);
CONFIG_DEFINE_HIDDEN("Codes", bool, DisableEdgeGrabLeftover, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, DisableKingdomValleyMist, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, DisableLowResolutionFontOnCustomUI, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, DisablePushState, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, DisableTitleInputDelay, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, EnableDebugMode, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, FixPowerUpJingleDuration, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, HUDToggleKey, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, InfiniteLives, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, MidairControlForMachSpeed, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, MidairControlForSnowboards, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, RestoreChainJumpFlips, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, RestoreChaosBoostJump, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, RestoreChaosSpearFlips, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, RestoreContextualHUDColours, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, RestoreDemoCameraMode, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, RestoreSonicActionGauge, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, SkipIntroLogos, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, TailsGauge, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, UnlimitedAntigravity, false, false);
CONFIG_DEFINE_HIDDEN("Codes", bool, UseOfficialTitleOnTitleBar, false, true);

CONFIG_DEFINE("Update", time_t, LastChecked, 0, false);

CONFIG_DEFINE_HIDDEN("Install", uint32_t, InstalledTitleUpdateVersion, 0, false);

// Online Multiplayer (Nebula VPN)
CONFIG_DEFINE_HIDDEN("Network", std::string, NetworkName, "", false);
CONFIG_DEFINE_HIDDEN("Network", std::string, LighthouseAddress, "", false);
CONFIG_DEFINE_HIDDEN("Network", std::string, VirtualIP, "192.168.100.2/24", false);
CONFIG_DEFINE_HIDDEN("Network", int32_t, ListenPort, 4242, false);
CONFIG_DEFINE_HIDDEN("Network", bool, AutoConnect, false, false);
CONFIG_DEFINE_HIDDEN("Network", bool, ShowStatusOverlay, true, false);
