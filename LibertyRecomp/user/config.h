#pragma once

#include <locale/locale.h>

class IConfigDef
{
public:
    virtual ~IConfigDef() = default;
    virtual bool IsHidden() = 0;
    virtual void SetHidden(bool hidden) = 0;
    virtual void ReadValue(toml::v3::ex::parse_result& toml) = 0;
    virtual void MakeDefault() = 0;
    virtual std::string_view GetSection() const = 0;
    virtual std::string_view GetName() const = 0;
    virtual std::string GetNameLocalised(ELanguage language) const = 0;
    virtual std::string GetDescription(ELanguage language) const = 0;
    virtual bool IsDefaultValue() const = 0;
    virtual const void* GetValue() const = 0;
    virtual std::string GetValueLocalised(ELanguage language) const = 0;
    virtual std::string GetValueDescription(ELanguage language) const = 0;
    virtual std::string GetDefinition(bool withSection = false) const = 0;
    virtual std::string ToString(bool strWithQuotes = true) const = 0;
    virtual void GetLocaleStrings(std::vector<std::string_view>& localeStrings) const = 0;
    virtual void SnapToNearestAccessibleValue(bool searchUp) = 0;
    virtual bool RequiresRestart() = 0;
    virtual void UpdateStore() = 0;
    virtual bool IsValueChanged() = 0;
};

#define CONFIG_LOCALE            std::unordered_map<ELanguage, std::tuple<std::string, std::string>>
#define CONFIG_ENUM_LOCALE(type) std::unordered_map<ELanguage, std::unordered_map<type, std::tuple<std::string, std::string>>>

#define CONFIG_CALLBACK(name)       if (name.Callback) name.Callback(&name)
#define CONFIG_LOCK_CALLBACK(name)  if (name.LockCallback) name.LockCallback(&name)
#define CONFIG_APPLY_CALLBACK(name) if (name.ApplyCallback) name.ApplyCallback(&name)

#define WINDOWPOS_CENTRED        0x2FFF0000

extern std::vector<IConfigDef*> g_configDefinitions;

enum class EVoiceLanguage : uint32_t
{
    English,
    Japanese
};

enum class ECameraRotationMode : uint32_t
{
    Normal,
    Reverse
};

enum class ELightDash : uint32_t
{
    X,
    Y
};

enum class ESlidingAttack : uint32_t
{
    B,
    X
};

enum class EControllerIcons : uint32_t
{
    Auto,
    Xbox,
    PlayStation
};

enum class EChannelConfiguration : uint32_t
{
    Stereo,
    Surround
};

enum class EGraphicsAPI : uint32_t
{
    Auto,
#ifdef LIBERTY_RECOMP_D3D12
    D3D12,
#endif
#ifdef LIBERTY_RECOMP_METAL
    Metal,
#endif
    Vulkan
};

enum class EWindowState : uint32_t
{
    Normal,
    Maximised
};

enum class EAspectRatio : uint32_t
{
    Auto,
    Original
};

enum class ETripleBuffering : uint32_t
{
    Auto,
    On,
    Off
};

static constexpr int32_t FPS_MIN = 15;
static constexpr int32_t FPS_MAX = 241;

enum class EAntiAliasing : uint32_t
{
    Off = 0,
    MSAA2x = 2,
    MSAA4x = 4,
    MSAA8x = 8
};

enum class EShadowResolution : int32_t
{
    Original = -1,
    x512 = 512,
    x1024 = 1024,
    x2048 = 2048,
    x4096 = 4096,
    x8192 = 8192
};

enum class EShadowFilter : uint32_t
{
    Off,        // Hard shadows (no filtering)
    PCF3x3,     // 3x3 PCF (9 samples)
    PCF5x5,     // 5x5 PCF (25 samples)
    PCF7x7,     // 7x7 Gaussian PCF
    PCSS        // Contact hardening (soft shadows)
};

enum class EReflectionResolution : int32_t
{
    Eighth,
    Quarter,
    Half,
    Full
};

enum class ERadialBlur : uint32_t
{
    Off,
    Original,
    Enhanced
};

enum class ECutsceneAspectRatio : uint32_t
{
    Original,
    Unlocked
};

enum class EUIAlignmentMode : uint32_t
{
    Edge,
    Centre
};

enum class EHDRMode : uint32_t
{
    Off,        // SDR output (B8G8R8A8_UNORM)
    scRGB,      // scRGB linear (R16G16B16A16_FLOAT) - Windows HDR
    HDR10       // HDR10 PQ (R10G10B10A2_UNORM) - TV HDR
};

enum class EModernAA : uint32_t
{
    Off,        // No modern AA (use legacy if available)
    TAA,        // Temporal Anti-Aliasing
    SMAA,       // Subpixel Morphological Anti-Aliasing
    FSR1        // AMD FidelityFX Super Resolution 1.0 (used as AA)
};

enum class EDynamicResolution : uint32_t
{
    Off,        // Fixed internal resolution
    Quality,    // Target 90% GPU utilization
    Balanced,   // Target 75% GPU utilization  
    Performance // Target 60% GPU utilization
};

enum class EMotionBlur : uint32_t
{
    Off,        // No motion blur
    Camera,     // Camera-only motion blur (reprojection-based)
    Enhanced    // Camera + object motion blur (velocity buffer)
};

enum class ESSAA : uint32_t
{
    Off,        // Native resolution (1.0x)
    x1_5,       // 1.5x supersampling (2.25x pixels)
    x2,         // 2x supersampling (4x pixels)
    x4          // 4x supersampling (16x pixels) - very demanding
};

enum class EDepthOfField : uint32_t
{
    Off,        // No DOF
    Low,        // 3x3 blur kernel
    Medium,     // 5x5 blur kernel
    High,       // 7x7 blur kernel
    Ultra       // 9x9 blur kernel + bokeh
};

enum class ESSAO : uint32_t
{
    Off,        // No SSAO
    Low,        // 4 directions, 2 steps (fast)
    Medium,     // 6 directions, 4 steps (balanced)
    High,       // 8 directions, 6 steps (quality)
    Ultra       // 12 directions, 8 steps (max quality)
};

enum class EFilmGrain : uint32_t
{
    Off,        // No film grain
    Light,      // Subtle grain
    Medium,     // Moderate grain
    Heavy       // Strong grain (cinematic)
};

enum class EChromaticAberration : uint32_t
{
    Off,        // No chromatic aberration
    Subtle,     // Light fringe effect
    Normal,     // Standard CA
    Strong      // Heavy lens distortion
};

enum class EUpscaler : uint32_t
{
    Off,        // Native resolution
    FSR1,       // AMD FidelityFX Super Resolution 1.0
    FSR3,       // AMD FidelityFX Super Resolution 3.0 (Frame Gen)
    DLSS,       // NVIDIA Deep Learning Super Sampling
    XeSS,       // Intel Xe Super Sampling
    MetalFX     // Apple MetalFX (macOS only)
};

enum class EUpscaleQuality : uint32_t
{
    UltraQuality,   // 77% render scale
    Quality,        // 67% render scale  
    Balanced,       // 58% render scale
    Performance,    // 50% render scale
    UltraPerformance // 33% render scale
};

enum class EFrameGeneration : uint32_t
{
    Off,        // No frame generation
    FSR3FG,     // AMD FSR 3 Frame Generation
    DLSSFG      // NVIDIA DLSS Frame Generation
};

enum class EPlayerCharacter : uint32_t
{
    Sonic,
    Shadow,
    Silver,
    Blaze,
    Amy,
    Tails,
    Rouge,
    Knuckles
};

enum class EMultiplayerBackend : uint32_t
{
    Community,  // Default - uses community-hosted session tracker
    Firebase,   // Self-hosted Firebase for private communities
    LAN         // LAN only - no internet matchmaking
};

template<typename T, bool isHidden = false>
class ConfigDef final : public IConfigDef
{
    T m_storedValue{};

public:
    std::string Section{};
    std::string Name{};
    CONFIG_LOCALE* Locale{};
    T DefaultValue{};
    T Value{ DefaultValue };
    std::set<T> InaccessibleValues{};
    std::unordered_map<std::string, T>* EnumTemplate{};
    std::map<T, std::string> EnumTemplateReverse{};
    CONFIG_ENUM_LOCALE(T)* EnumLocale{};
    std::function<void(ConfigDef<T, isHidden>*)> Callback;
    std::function<void(ConfigDef<T, isHidden>*)> LockCallback;
    std::function<void(ConfigDef<T, isHidden>*)> ApplyCallback;
    bool IsLoadedFromConfig{};
    bool IsRestartRequired{};

    // CONFIG_DEFINE
    ConfigDef(std::string section, std::string name, T defaultValue, bool requiresRestart);

    // CONFIG_DEFINE_LOCALISED
    ConfigDef(std::string section, std::string name, CONFIG_LOCALE* nameLocale, T defaultValue, bool requiresRestart);

    // CONFIG_DEFINE_ENUM
    ConfigDef(std::string section, std::string name, T defaultValue, bool requiresRestart, std::unordered_map<std::string, T>* enumTemplate);

    // CONFIG_DEFINE_ENUM_LOCALISED
    ConfigDef(std::string section, std::string name, CONFIG_LOCALE* nameLocale, T defaultValue, bool requiresRestart, std::unordered_map<std::string, T>* enumTemplate, CONFIG_ENUM_LOCALE(T)* enumLocale);

    ConfigDef(const ConfigDef&) = delete;
    ConfigDef(ConfigDef&&) = delete;
    ~ConfigDef();

    bool IsHidden() override;
    void SetHidden(bool hidden) override;
    void ReadValue(toml::v3::ex::parse_result& toml) override;
    void MakeDefault() override;
    std::string_view GetSection() const override;
    std::string_view GetName() const override;
    std::string GetNameLocalised(ELanguage language) const override;
    std::string GetDescription(ELanguage language) const override;
    bool IsDefaultValue() const override;
    const void* GetValue() const override;
    std::string GetValueLocalised(ELanguage language) const override;
    std::string GetValueDescription(ELanguage language) const override;
    std::string GetDefinition(bool withSection = false) const override;
    std::string ToString(bool strWithQuotes = true) const override;
    void GetLocaleStrings(std::vector<std::string_view>& localeStrings) const override;
    void SnapToNearestAccessibleValue(bool searchUp) override;
    bool RequiresRestart() override;
    void UpdateStore() override;
    bool IsValueChanged() override;

    operator T() const
    {
        return Value;
    }

    void operator=(const T& other)
    {
        Value = other;
    }
};

#define CONFIG_DECLARE(type, name)                                                       static ConfigDef<type>       name;
#define CONFIG_DECLARE_HIDDEN(type, name)                                                static ConfigDef<type, true> name;

#define CONFIG_DEFINE(section, type, name, defaultValue, requiresRestart)                CONFIG_DECLARE(type, name)
#define CONFIG_DEFINE_HIDDEN(section, type, name, defaultValue, requiresRestart)         CONFIG_DECLARE_HIDDEN(type, name)
#define CONFIG_DEFINE_LOCALISED(section, type, name, defaultValue, requiresRestart)      CONFIG_DECLARE(type, name)
#define CONFIG_DEFINE_ENUM(section, type, name, defaultValue, requiresRestart)           CONFIG_DECLARE(type, name)
#define CONFIG_DEFINE_ENUM_LOCALISED(section, type, name, defaultValue, requiresRestart) CONFIG_DECLARE(type, name)

class Config
{
public:
    #include "config_def.h"

    static inline bool s_isCallbacksCreated;

    static std::filesystem::path GetConfigPath();

    static void CreateCallbacks();
    static void Load();
    static void Save();

    static bool IsControllerIconsPS3();
};
