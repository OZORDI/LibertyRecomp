#include "postprocess_cache.h"

// Post-process shader cache entries — populated with hashes from postprocess_aa.h ShaderHash namespace.
// Actual shader binary loading is handled by PostProcessRenderer via embedded headers;
// this cache is used for hash-based lookup/validation by the AA system.
PostProcessShaderEntry g_postprocessShaderEntries[] = {
    // Fullscreen vertex shader
    { 0x2E0C3E65F83551D4ULL, nullptr, 0, nullptr, 0, nullptr, 0 },
    // SMAA shaders
    { 0xBB3171DE755EADDAULL, nullptr, 0, nullptr, 0, nullptr, 0 },  // SMAAEdgeDetectPS
    { 0xCBF1A03E73FF920FULL, nullptr, 0, nullptr, 0, nullptr, 0 },  // SMAABlendPS
    // TAA shader
    { 0x23B41EED42846B3FULL, nullptr, 0, nullptr, 0, nullptr, 0 },  // TAAPS
    // FSR 1.0 shaders
    { 0xDB4863B0C31A22EFULL, nullptr, 0, nullptr, 0, nullptr, 0 },  // FSR1EASUPS
    { 0xB6836B8A33B66FB0ULL, nullptr, 0, nullptr, 0, nullptr, 0 },  // FSR1RCASPS
    // Motion blur shaders
    { 0x8ADA07D974D45D0FULL, nullptr, 0, nullptr, 0, nullptr, 0 },  // MotionBlurCameraPS
    { 0x26C20A94DAD70E23ULL, nullptr, 0, nullptr, 0, nullptr, 0 },  // MotionBlurEnhancedPS
    // Chromatic aberration
    { 0xC8212C3B45221B3DULL, nullptr, 0, nullptr, 0, nullptr, 0 },  // ChromaticAberrationPS
    // Film grain
    { 0x0BBBBA40C868BCC1ULL, nullptr, 0, nullptr, 0, nullptr, 0 },  // FilmGrainPS
    // Depth of field
    { 0xD1ED440EAEF01028ULL, nullptr, 0, nullptr, 0, nullptr, 0 },  // DOFPS
};

const size_t g_postprocessShaderCount = sizeof(g_postprocessShaderEntries) / sizeof(g_postprocessShaderEntries[0]);
