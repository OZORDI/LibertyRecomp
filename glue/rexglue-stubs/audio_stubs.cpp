// =============================================================================
// Stub implementations for rex::audio symbols referenced by rexkernel.
// LibertyRecomp provides its own XMA decoding via apu/xma_decoder.cpp;
// these stubs satisfy the linker for the rexaudio XmaDecoder class.
// =============================================================================

#include <cstdint>

// Minimal forward declarations to match the class layout without pulling in
// the full rex::audio header tree.
namespace rex {
namespace runtime {
class FunctionDispatcher;
}
namespace system {
class KernelState;
}
namespace audio {

using X_STATUS = uint32_t;

class XmaDecoder {
 public:
  explicit XmaDecoder(runtime::FunctionDispatcher* function_dispatcher);
  ~XmaDecoder();
  X_STATUS Setup(system::KernelState* kernel_state);
  void Shutdown();
};

// --- Stub definitions --------------------------------------------------------

XmaDecoder::XmaDecoder(runtime::FunctionDispatcher* /*fd*/) {}
XmaDecoder::~XmaDecoder() {}
X_STATUS XmaDecoder::Setup(system::KernelState* /*ks*/) { return 0; }
void XmaDecoder::Shutdown() {}

}  // namespace audio
}  // namespace rex
