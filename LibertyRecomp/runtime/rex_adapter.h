/**
 * @file    rex_adapter.h
 * @brief   Adapter header to bridge RexGlue SDK types into LibertyRecomp
 *
 * RexGlue's PPC types (PPCContext, PPCRegister, etc.) are authoritative.
 * cpu/ppc_context.h forwards to RexGlue's guest/context.h.
 *
 * RexGlue headers are included directly:
 *   - rex::Runtime, rex::memory::Memory, rex::system::* types are available
 *   - LibertyRecomp's own kernel primitives remain for game-specific code
 */

#pragma once

// =============================================================================
// Core Runtime Headers
// =============================================================================

// RexGlue PPC types (PPCContext, PPCRegister, PPCFunc, etc.)
#include "cpu/ppc_context.h"
#include "cpu/guest_thread.h"

#include <rex/runtime.h>
#include <rex/kernel.h>
#include <rex/kernel/kernel_state.h>
#include <rex/kernel/xobject.h>
#include <rex/kernel/xevent.h>
#include <rex/kernel/xsemaphore.h>
#include <rex/kernel/xmutant.h>
#include <rex/kernel/xthread.h>
#include <rex/kernel/xmemory.h>
#include <rex/runtime/processor.h>
#include <rex/runtime/thread_state.h>
#include <rex/runtime/mmio_handler.h>
#include <rex/exception_handler.h>

// =============================================================================
// Compatibility Layer
// =============================================================================

namespace rex {
namespace adapter {

using Processor = rex::runtime::Processor;
using ThreadState = rex::runtime::ThreadState;
using Memory = rex::memory::Memory;
using KernelState = rex::system::KernelState;
using XObject = rex::system::XObject;
using XEvent = rex::system::XEvent;
using XSemaphore = rex::system::XSemaphore;
using XMutant = rex::system::XMutant;
using XThread = rex::system::XThread;
using XHostThread = rex::system::XHostThread;
template <typename T>
using object_ref = rex::system::object_ref<T>;

} // namespace adapter
} // namespace rex

// =============================================================================
// Convenience Macros
// =============================================================================

#define REX_MEMORY() g_memory.GetRexMemory()
#define REX_MEMBASE() g_memory.base
#define REX_RUNTIME_INSTANCE() rex::Runtime::instance()
#define REX_KERNEL_STATE() rex::Runtime::instance()->kernel_state()
#define REX_PROCESSOR() rex::Runtime::instance()->processor()

// =============================================================================
// Sub-adapter headers
// =============================================================================
#include "rex_sync.h"
#include "rex_runtime.h"
#include "rex_xobject.h"
