#include <rex/input/sdl/physical_device_inventory.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <optional>
#include <string_view>
#include <vector>

#include <SDL3/SDL.h>
#include <rex/input/absolute_pointer.h>
#include <rex/platform.h>

#if REX_PLATFORM_MAC && !REX_PLATFORM_IOS
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <IOKit/hid/IOHIDUsageTables.h>
#include <IOKit/hidsystem/IOHIDParameter.h>
#endif

#if REX_PLATFORM_GNU_LINUX && __has_include(<X11/extensions/XInput2.h>)
#define REX_TOUCH_HAS_XINPUT2_HEADERS 1
#include <X11/Xatom.h>
#include <X11/extensions/XInput2.h>
#else
#define REX_TOUCH_HAS_XINPUT2_HEADERS 0
#endif

namespace rex::input::sdl {
namespace {

constexpr uint64_t kInventoryRefreshIntervalMs = 1000;

bool IsXTestDevice(const char* name) {
  return name && std::string_view(name).find("XTEST") != std::string_view::npos;
}

bool IsDefaultX11Master(const char* name) {
  return name && (std::string_view(name) == "Virtual core pointer" ||
                  std::string_view(name) == "Virtual core keyboard");
}

struct PointerKeyboardInventory {
  std::vector<uint64_t> keyboards;
  std::vector<uint64_t> mice;
};

#if REX_PLATFORM_MAC && !REX_PLATFORM_IOS
bool NumberEquals(CFTypeRef value, int expected) {
  int actual = 0;
  return value && CFGetTypeID(value) == CFNumberGetTypeID() &&
      CFNumberGetValue(static_cast<CFNumberRef>(value), kCFNumberIntType, &actual) &&
      actual == expected;
}

bool HasUsage(CFDictionaryRef properties, int page, int usage) {
  const auto pairs_value = CFDictionaryGetValue(properties, CFSTR(kIOHIDDeviceUsagePairsKey));
  if (pairs_value && CFGetTypeID(pairs_value) == CFArrayGetTypeID()) {
    const auto pairs = static_cast<CFArrayRef>(pairs_value);
    for (CFIndex index = 0; index < CFArrayGetCount(pairs); ++index) {
      const auto value = CFArrayGetValueAtIndex(pairs, index);
      if (!value || CFGetTypeID(value) != CFDictionaryGetTypeID()) continue;
      const auto pair = static_cast<CFDictionaryRef>(value);
      if (NumberEquals(CFDictionaryGetValue(pair, CFSTR(kIOHIDDeviceUsagePageKey)), page) &&
          NumberEquals(CFDictionaryGetValue(pair, CFSTR(kIOHIDDeviceUsageKey)), usage)) return true;
    }
  }
  return NumberEquals(CFDictionaryGetValue(properties, CFSTR(kIOHIDPrimaryUsagePageKey)), page) &&
      NumberEquals(CFDictionaryGetValue(properties, CFSTR(kIOHIDPrimaryUsageKey)), usage);
}

std::optional<PointerKeyboardInventory> QueryCocoaPhysicalDevices() {
  // Cocoa installs default SDL keyboard/mouse IDs even on a Mac with neither
  // device. Registry metadata can be read without opening HID devices, polling
  // reports, scheduling callbacks, or requesting Input Monitoring permission.
  auto matching = IOServiceMatching(kIOHIDDeviceKey);
  if (!matching) return std::nullopt;
  io_iterator_t iterator = IO_OBJECT_NULL;
  if (IOServiceGetMatchingServices(kIOMainPortDefault, matching, &iterator) != KERN_SUCCESS)
    return std::nullopt;
  PointerKeyboardInventory result;
  bool succeeded = true;
  while (const io_registry_entry_t device = IOIteratorNext(iterator)) {
    CFMutableDictionaryRef properties = nullptr;
    uint64_t id = 0;
    if (IORegistryEntryCreateCFProperties(device, &properties, kCFAllocatorDefault, 0) !=
            KERN_SUCCESS || !properties ||
        IORegistryEntryGetRegistryEntryID(device, &id) != KERN_SUCCESS) {
      succeeded = false;
    } else {
      const auto virtual_device = CFDictionaryGetValue(properties, CFSTR(kIOHIDVirtualHIDevice));
      const auto transport_value = CFDictionaryGetValue(properties, CFSTR(kIOHIDTransportKey));
      const bool virtual_transport = transport_value &&
          CFGetTypeID(transport_value) == CFStringGetTypeID() &&
          CFStringFind(static_cast<CFStringRef>(transport_value), CFSTR("virtual"),
                       kCFCompareCaseInsensitive).location != kCFNotFound;
      if (id && virtual_device != kCFBooleanTrue && !NumberEquals(virtual_device, 1) &&
          !virtual_transport) {
        if (HasUsage(properties, kHIDPage_GenericDesktop, kHIDUsage_GD_Keyboard) ||
            HasUsage(properties, kHIDPage_GenericDesktop, kHIDUsage_GD_Keypad))
          result.keyboards.push_back(id);
        if (HasUsage(properties, kHIDPage_GenericDesktop, kHIDUsage_GD_Mouse) ||
            HasUsage(properties, kHIDPage_Digitizer, kHIDUsage_Dig_TouchPad))
          result.mice.push_back(id);
      }
    }
    if (properties) CFRelease(properties);
    IOObjectRelease(device);
  }
  succeeded = succeeded && IOIteratorIsValid(iterator);
  IOObjectRelease(iterator);
  if (!succeeded) return std::nullopt;
  return result;
}
#endif

#if REX_TOUCH_HAS_XINPUT2_HEADERS
struct X11PointerMetadata {
  decltype(&XIGetProperty) get_property = nullptr;
  decltype(&XFree) free_data = nullptr;
  Atom pressure = None;
  Atom wacom_tool_type = None;
  Atom stylus = None;
  Atom eraser = None;
  Atom cursor = None;
  Atom pad = None;
  Atom touch = None;
  Atom tablet_area = None;
  Atom tablet_pressure = None;
  Atom synaptics_edges = None;
  Atom touchpad_tapping = None;
};

struct X11Property {
  bool present = false;
  Atom atom = None;
};

std::optional<X11Property> ReadX11Property(Display* display, int device_id, Atom property,
                                         bool read_atom, const X11PointerMetadata& metadata) {
  if (property == None) return X11Property{};
  Atom type = None;
  int format = 0;
  unsigned long count = 0, remaining = 0;
  unsigned char* data = nullptr;
  const auto status = metadata.get_property(display, device_id, property, 0, read_atom ? 1 : 0,
      False, AnyPropertyType, &type, &format, &count, &remaining, &data);
  X11Property result{.present = type != None};
  bool succeeded = status == Success;
  if (succeeded && result.present && read_atom) {
    succeeded = type == XA_ATOM && format == 32 && count == 1 && remaining == 0 && data;
    // Xlib expands a format-32 property to native longs, including Atom values.
    if (succeeded) std::memcpy(&result.atom, data, sizeof(result.atom));
  }
  if (data) metadata.free_data(data);
  return succeeded ? std::optional(result) : std::nullopt;
}

std::optional<bool> IsX11MousePointer(Display* display, const XIDeviceInfo& device,
                                    const X11PointerMetadata& metadata) {
  bool pressure = false;
  for (int index = 0; index < device.num_classes; ++index) {
    const auto* info = device.classes[index];
    if (!info) continue;
    if (info->type == XITouchClass) {
      const auto* touch = reinterpret_cast<const XITouchClassInfo*>(info);
      return touch->mode == XIDependentTouch;
    }
    if (info->type == XIValuatorClass && metadata.pressure != None) {
      const auto* axis = reinterpret_cast<const XIValuatorClassInfo*>(info);
      pressure |= axis->label == metadata.pressure;
    }
  }

  // These are driver-defined property/atom identifiers, not device names.
  // xf86-input-wacom/include/wacom-properties.h publishes all five tool types.
  const auto tool = ReadX11Property(display, device.deviceid, metadata.wacom_tool_type, true, metadata);
  if (!tool) return std::nullopt;
  if (tool->present && tool->atom != None) {
    if (tool->atom == metadata.stylus || tool->atom == metadata.eraser ||
        tool->atom == metadata.cursor || tool->atom == metadata.pad) return false;
    if (tool->atom == metadata.touch) return true;
  }
  // xf86-input-libinput exports these properties only for tablet tools.
  for (const auto property : {metadata.tablet_area, metadata.tablet_pressure}) {
    const auto value = ReadX11Property(display, device.deviceid, property, false, metadata);
    if (!value) return std::nullopt;
    if (value->present) return false;
  }
  if (!pressure) return true;
  // SDL_x11pen.c identifies generic pens by the Abs Pressure valuator. Keep
  // pressure-sensitive touchpads: Synaptics Edges and libinput Tapping Enabled
  // describe touchpad capabilities even when no XI2 touch class is exposed.
  for (const auto property : {metadata.synaptics_edges, metadata.touchpad_tapping}) {
    const auto value = ReadX11Property(display, device.deviceid, property, false, metadata);
    if (!value) return std::nullopt;
    if (value->present) return true;
  }
  return false;
}
#endif

std::optional<PointerKeyboardInventory> QueryX11SlaveDevices(bool* query_failed) {
  *query_failed = false;
#if REX_TOUCH_HAS_XINPUT2_HEADERS
  int window_count = 0;
  auto windows = SDL_GetWindows(&window_count);
  Display* display = nullptr;
  if (windows) {
    for (int index = 0; index < window_count && !display; ++index)
      display = static_cast<Display*>(SDL_GetPointerProperty(SDL_GetWindowProperties(windows[index]),
          SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr));
    SDL_free(windows);
  }
  if (!display) return std::nullopt;
  // Keep XInput optional, as it is in SDL. Only the server's slave devices are
  // actual input sources; master devices can have arbitrary user-chosen names.
  auto xi = SDL_LoadObject("libXi.so.6");
  auto x11 = SDL_LoadObject("libX11.so.6");
  const auto query_extension = x11 ? reinterpret_cast<decltype(&XQueryExtension)>(
      SDL_LoadFunction(x11, "XQueryExtension")) : nullptr;
  const auto query_devices = xi ? reinterpret_cast<decltype(&XIQueryDevice)>(
      SDL_LoadFunction(xi, "XIQueryDevice")) : nullptr;
  const auto query_version = xi ? reinterpret_cast<decltype(&XIQueryVersion)>(
      SDL_LoadFunction(xi, "XIQueryVersion")) : nullptr;
  const auto free_devices = xi ? reinterpret_cast<decltype(&XIFreeDeviceInfo)>(
      SDL_LoadFunction(xi, "XIFreeDeviceInfo")) : nullptr;
  const auto get_property = xi ? reinterpret_cast<decltype(&XIGetProperty)>(
      SDL_LoadFunction(xi, "XIGetProperty")) : nullptr;
  const auto intern_atoms = x11 ? reinterpret_cast<decltype(&XInternAtoms)>(
      SDL_LoadFunction(x11, "XInternAtoms")) : nullptr;
  const auto free_data = x11 ? reinterpret_cast<decltype(&XFree)>(
      SDL_LoadFunction(x11, "XFree")) : nullptr;
  std::optional<PointerKeyboardInventory> result;
  int opcode = 0, first_event = 0, first_error = 0;
  // SDL negotiates 2.4 on this same Display. Requesting 2.0 afterwards can
  // raise BadValue and would also omit newer touch device class metadata.
  int major = 2, minor = 4;
  if (query_extension && query_devices && query_version && free_devices &&
      get_property && intern_atoms && free_data &&
      query_extension(display, "XInputExtension", &opcode, &first_event, &first_error)) {
    *query_failed = true;
    const bool version_available = query_version(display, &major, &minor) == Success && major >= 2;
    int count = 0;
    if (auto devices = version_available ? query_devices(display, XIAllDevices, &count) : nullptr) {
      const std::array names = {"Abs Pressure", "Wacom Tool Type", "STYLUS", "ERASER", "CURSOR",
          "PAD", "TOUCH", "libinput Tablet Tool Area Ratio", "libinput Tablet Tool Pressurecurve",
          "Synaptics Edges", "libinput Tapping Enabled"};
      std::array<char*, names.size()> arguments{};
      std::array<Atom, names.size()> atoms{};
      for (size_t index = 0; index < names.size(); ++index)
        arguments[index] = const_cast<char*>(names[index]);
      // Existing-only lookup avoids creating properties or changing server state.
      if (count >= 0 && intern_atoms(display, arguments.data(), static_cast<int>(arguments.size()),
                                    True, atoms.data())) {
        const X11PointerMetadata metadata{get_property, free_data, atoms[0], atoms[1], atoms[2],
            atoms[3], atoms[4], atoms[5], atoms[6], atoms[7], atoms[8], atoms[9], atoms[10]};
        result.emplace();
        for (int index = 0; index < count; ++index) {
          const auto& device = devices[index];
          if (!device.enabled || device.deviceid <= 0 || IsXTestDevice(device.name)) continue;
          if (device.use == XISlaveKeyboard) result->keyboards.push_back(uint64_t(device.deviceid));
          if (device.use == XISlavePointer) {
            const auto mouse = IsX11MousePointer(display, device, metadata);
            if (!mouse) {
              result.reset();
              break;
            }
            if (*mouse) result->mice.push_back(uint64_t(device.deviceid));
          }
        }
      }
      *query_failed = !result.has_value();
      free_devices(devices);
    }
  }
  if (xi) SDL_UnloadObject(xi);
  if (x11) SDL_UnloadObject(x11);
  return result;
#else
  return std::nullopt;
#endif
}

bool Contains(const std::vector<uint64_t>& ids, uint64_t id) {
  return std::find(ids.begin(), ids.end(), id) != ids.end();
}

}  // namespace

void PhysicalDeviceInventory::Refresh(uint64_t timestamp_ns, bool force) {
  const uint64_t now_ms = SDL_GetTicks();
  const bool dirty = dirty_.exchange(false, std::memory_order_acq_rel);
  if (!force && !dirty && now_ms < next_refresh_ms_) return;
  next_refresh_ms_ = now_ms + kInventoryRefreshIntervalMs;
  auto& service = GetAbsolutePointerService();
  const char* driver = SDL_GetCurrentVideoDriver();
  const bool x11 = driver && std::string_view(driver) == "x11";

#if REX_PLATFORM_MAC && !REX_PLATFORM_IOS
  const bool cocoa = driver && std::string_view(driver) == "cocoa";
  if (cocoa) {
    if (auto devices = QueryCocoaPhysicalDevices()) {
      service.ReplacePhysicalKeyboards(devices->keyboards, timestamp_ns);
      service.ReplacePhysicalMice(devices->mice, timestamp_ns);
    }
  } else
#endif
  {
    bool native_query_failed = false;
    const auto slaves = x11 ? QueryX11SlaveDevices(&native_query_failed) : std::nullopt;
    int count = 0;
    if (auto ids = native_query_failed ? nullptr : SDL_GetKeyboards(&count)) {
      std::vector<uint64_t> keyboards;
      for (int index = 0; index < count; ++index) {
        const auto id = ids[index];
        if (!id) continue;
        if (x11 && (slaves ? !Contains(slaves->keyboards, id) :
            IsXTestDevice(SDL_GetKeyboardNameForID(id)) ||
            IsDefaultX11Master(SDL_GetKeyboardNameForID(id)))) continue;
        keyboards.push_back(uint64_t(id));
      }
      SDL_free(ids);
      service.ReplacePhysicalKeyboards(keyboards, timestamp_ns);
    }

    std::vector<uint64_t> direct_touch;
    if (x11 && !native_query_failed) {
      if (auto ids = SDL_GetTouchDevices(&count)) {
        for (int index = 0; index < count; ++index)
          if (SDL_GetTouchDeviceType(ids[index]) == SDL_TOUCH_DEVICE_DIRECT)
            direct_touch.push_back(uint64_t(ids[index]));
        SDL_free(ids);
      }
    }
    if (auto ids = native_query_failed ? nullptr : SDL_GetMice(&count)) {
      std::vector<uint64_t> mice;
      for (int index = 0; index < count; ++index) {
        const auto id = ids[index];
        if (!IsPhysicalMouseId(id)) continue;
        if (x11 && (Contains(direct_touch, id) ||
            (slaves ? !Contains(slaves->mice, id) : IsXTestDevice(SDL_GetMouseNameForID(id)) ||
                IsDefaultX11Master(SDL_GetMouseNameForID(id))))) continue;
        mice.push_back(uint64_t(id));
      }
      SDL_free(ids);
      service.ReplacePhysicalMice(mice, timestamp_ns);
    }
  }

  int count = 0;
  if (auto ids = SDL_GetGamepads(&count)) {
    std::vector<uint64_t> gamepads;
    for (int index = 0; index < count; ++index)
      if (ids[index] && !SDL_IsJoystickVirtual(ids[index])) gamepads.push_back(uint64_t(ids[index]));
    SDL_free(ids);
    service.ReplaceGameControllers(gamepads, timestamp_ns);
  }
#if REX_PLATFORM_ANDROID
  // Android's SDL backend exposes a default mouse ID only through events, so
  // JNI InputDevice sources provide the physical keyboard and mouse inventory.
  RefreshAndroidPhysicalKeyboardPresence(timestamp_ns);
#endif
}

}  // namespace rex::input::sdl
