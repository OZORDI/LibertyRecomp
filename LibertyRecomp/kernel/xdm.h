#pragma once

// Shim header — Liberty's custom kernel layer was removed (rexglue provides
// Xbox 360 kernel services natively). This header now re-exports the
// equivalent constants from rexglue's xtypes.
//
// If you're adding NEW code, prefer including rex/system/xtypes.h directly.

#include <cstdint>
#include <rex/system/xtypes.h>
