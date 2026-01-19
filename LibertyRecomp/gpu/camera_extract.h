#pragma once

#include <cstdint>
#include <cmath>

namespace Camera {

// ============================================================================
// GTA IV Camera Data Extraction
// Based on execution trace of PPC recompiled code
// ============================================================================

// Global constant addresses discovered via execution trace
namespace GuestAddress {
    // Camera projection parameters (global constants)
    constexpr uint32_t NearClipPlane = 0x81802648;
    constexpr uint32_t FarClipPlane  = 0x82053CD8;
    constexpr uint32_t FieldOfView   = 0x8180D8EC;
    constexpr uint32_t AspectRatio   = 0x8180F4D8;
    
    // CCam class structure offsets
    namespace CCamOffset {
        constexpr uint32_t ViewMatrix     = 16;   // 48 bytes, 3x4 matrix
        constexpr uint32_t CameraPosition = 80;   // 12 bytes, float3
        constexpr uint32_t FOV            = 96;   // 4 bytes, float (radians)
        constexpr uint32_t NearClip       = 100;  // 4 bytes, float
        constexpr uint32_t FarClip        = 104;  // 4 bytes, float
        constexpr uint32_t CameraUp       = 108;  // 12 bytes, float3
        constexpr uint32_t CameraRight    = 120;  // 12 bytes, float3
        constexpr uint32_t CameraForward  = 132;  // 12 bytes, float3
    }
    
    // Matrix initialization structure offsets (sub_823D8B30)
    namespace MatrixInitOffset {
        constexpr uint32_t NearClip    = 80;
        constexpr uint32_t FarClip     = 84;
        constexpr uint32_t FOV         = 88;
        constexpr uint32_t AspectRatio = 92;
    }
    
    // Device context (TLS + 1676, ~19KB structure)
    namespace DeviceContext {
        constexpr uint32_t TLSOffset           = 1676;
        constexpr uint32_t FrameCounter        = 16544;
        constexpr uint32_t CurrentVertexShader = 10932;
        constexpr uint32_t CurrentPixelShader  = 10936;
    }
}

// Extracted camera data structure
struct ExtractedCameraData {
    // Matrices (column-major, suitable for shaders)
    float viewMatrix[16];
    float projMatrix[16];
    float invViewMatrix[16];
    float invProjMatrix[16];
    float viewProjMatrix[16];
    
    // Camera vectors (world space)
    float position[3];
    float forward[3];
    float up[3];
    float right[3];
    
    // Projection parameters
    float nearClip;
    float farClip;
    float fovY;          // Vertical FOV in radians
    float aspectRatio;
    
    // Frame info
    uint32_t frameIndex;
    
    // Validity flag
    bool isValid;
};

// Byte swap for big-endian PPC data
inline float ByteSwapFloat(float f) {
    union {
        float f;
        uint32_t u;
    } val;
    val.f = f;
    val.u = ((val.u & 0xFF000000) >> 24) |
            ((val.u & 0x00FF0000) >> 8)  |
            ((val.u & 0x0000FF00) << 8)  |
            ((val.u & 0x000000FF) << 24);
    return val.f;
}

inline uint32_t ByteSwapU32(uint32_t u) {
    return ((u & 0xFF000000) >> 24) |
           ((u & 0x00FF0000) >> 8)  |
           ((u & 0x0000FF00) << 8)  |
           ((u & 0x000000FF) << 24);
}

// Build perspective projection matrix (row-major)
inline void BuildPerspectiveMatrix(float* outMatrix, float fovY, float aspect, float nearZ, float farZ) {
    float yScale = 1.0f / tanf(fovY * 0.5f);
    float xScale = yScale / aspect;
    float zRange = farZ / (nearZ - farZ);
    
    // Zero initialize
    for (int i = 0; i < 16; i++) outMatrix[i] = 0.0f;
    
    // Row-major perspective matrix (right-handed, depth 0 to 1)
    outMatrix[0]  = xScale;
    outMatrix[5]  = yScale;
    outMatrix[10] = zRange;
    outMatrix[11] = -1.0f;
    outMatrix[14] = nearZ * zRange;
}

// Expand 3x4 view matrix to 4x4
inline void ExpandViewMatrix3x4To4x4(const float* mat3x4, float* mat4x4) {
    // Input: 3 rows x 4 columns (rotation + translation)
    // Output: 4x4 with [0,0,0,1] as last row
    mat4x4[0]  = mat3x4[0];  mat4x4[1]  = mat3x4[1];  mat4x4[2]  = mat3x4[2];  mat4x4[3]  = mat3x4[3];
    mat4x4[4]  = mat3x4[4];  mat4x4[5]  = mat3x4[5];  mat4x4[6]  = mat3x4[6];  mat4x4[7]  = mat3x4[7];
    mat4x4[8]  = mat3x4[8];  mat4x4[9]  = mat3x4[9];  mat4x4[10] = mat3x4[10]; mat4x4[11] = mat3x4[11];
    mat4x4[12] = 0.0f;       mat4x4[13] = 0.0f;       mat4x4[14] = 0.0f;       mat4x4[15] = 1.0f;
}

// Matrix multiplication (4x4)
inline void MultiplyMatrix4x4(const float* a, const float* b, float* out) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            out[row * 4 + col] = 
                a[row * 4 + 0] * b[0 * 4 + col] +
                a[row * 4 + 1] * b[1 * 4 + col] +
                a[row * 4 + 2] * b[2 * 4 + col] +
                a[row * 4 + 3] * b[3 * 4 + col];
        }
    }
}

// Invert 4x4 matrix (general case)
inline bool InvertMatrix4x4(const float* m, float* out) {
    float inv[16], det;
    
    inv[0] = m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15] + 
             m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
    inv[4] = -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15] - 
             m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
    inv[8] = m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15] + 
             m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
    inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14] - 
              m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
    inv[1] = -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15] - 
             m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
    inv[5] = m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15] + 
             m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
    inv[9] = -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15] - 
             m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
    inv[13] = m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14] + 
              m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
    inv[2] = m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15] + 
             m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
    inv[6] = -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15] - 
             m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
    inv[10] = m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15] + 
              m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
    inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14] - 
              m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
    inv[3] = -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11] - 
             m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
    inv[7] = m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11] + 
             m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
    inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11] - 
              m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
    inv[15] = m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10] + 
              m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];
    
    det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
    if (det == 0.0f) return false;
    
    det = 1.0f / det;
    for (int i = 0; i < 16; i++) out[i] = inv[i] * det;
    return true;
}

// Global camera data (updated each frame)
extern ExtractedCameraData g_cameraData;

// Extract camera data from guest memory
// Call this once per frame before post-processing
void ExtractCameraFromGuest(uint8_t* guestBase);

// Fallback extraction from vertex shader constants
void ExtractCameraFromShaderConstants(const float* vsConstants, ExtractedCameraData& out);

} // namespace Camera
