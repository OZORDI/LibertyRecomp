#include "camera_extract.h"
#include <cstring>

namespace Camera {

// Global camera data instance
ExtractedCameraData g_cameraData = {};

// Frame counter for tracking updates
static uint32_t s_lastFrameIndex = 0;

void ExtractCameraFromGuest(uint8_t* guestBase) {
    if (!guestBase) {
        g_cameraData.isValid = false;
        return;
    }
    
    // Read global projection parameters (big-endian)
    float nearClip = ByteSwapFloat(*reinterpret_cast<float*>(guestBase + GuestAddress::NearClipPlane));
    float farClip = ByteSwapFloat(*reinterpret_cast<float*>(guestBase + GuestAddress::FarClipPlane));
    float fov = ByteSwapFloat(*reinterpret_cast<float*>(guestBase + GuestAddress::FieldOfView));
    float aspect = ByteSwapFloat(*reinterpret_cast<float*>(guestBase + GuestAddress::AspectRatio));
    
    // Validate extracted values
    bool validNear = (nearClip > 0.01f && nearClip < 10.0f);
    bool validFar = (farClip > 50.0f && farClip < 10000.0f);
    bool validFov = (fov > 0.3f && fov < 2.5f);  // ~17 to ~143 degrees
    bool validAspect = (aspect > 0.5f && aspect < 3.0f);
    
    if (validNear && validFar && validFov && validAspect) {
        g_cameraData.nearClip = nearClip;
        g_cameraData.farClip = farClip;
        g_cameraData.fovY = fov;
        g_cameraData.aspectRatio = aspect;
        
        // Build projection matrix from extracted parameters
        BuildPerspectiveMatrix(g_cameraData.projMatrix, fov, aspect, nearClip, farClip);
        
        // Compute inverse projection
        InvertMatrix4x4(g_cameraData.projMatrix, g_cameraData.invProjMatrix);
        
        g_cameraData.isValid = true;
    } else {
        // Use fallback defaults if extraction failed
        g_cameraData.nearClip = 0.1f;
        g_cameraData.farClip = 1000.0f;
        g_cameraData.fovY = 1.22173f;  // ~70 degrees
        g_cameraData.aspectRatio = 16.0f / 9.0f;
        
        BuildPerspectiveMatrix(g_cameraData.projMatrix, 
                               g_cameraData.fovY, 
                               g_cameraData.aspectRatio, 
                               g_cameraData.nearClip, 
                               g_cameraData.farClip);
        InvertMatrix4x4(g_cameraData.projMatrix, g_cameraData.invProjMatrix);
        
        g_cameraData.isValid = false;
    }
    
    g_cameraData.frameIndex++;
}

void ExtractCameraFromShaderConstants(const float* vsConstants, ExtractedCameraData& out) {
    if (!vsConstants) {
        out.isValid = false;
        return;
    }
    
    // GTA IV stores matrices in vertex shader constant registers:
    // - View matrix at c0-c3 (registers 0-15 as floats, row-major)
    // - Projection matrix at c4-c7 (registers 16-31 as floats, row-major)
    const float* viewMatrix = &vsConstants[0];   // c0-c3
    const float* projMatrix = &vsConstants[16];  // c4-c7
    
    // Copy matrices
    memcpy(out.viewMatrix, viewMatrix, 16 * sizeof(float));
    memcpy(out.projMatrix, projMatrix, 16 * sizeof(float));
    
    // Check if projection matrix is valid (not all zeros)
    bool projValid = (projMatrix[0] != 0.0f || projMatrix[5] != 0.0f);
    
    if (projValid) {
        // Extract FOV from projection matrix: P[1][1] = 1 / tan(fovY/2)
        float p11 = projMatrix[5];  // [1][1] in row-major
        if (p11 > 0.1f && p11 < 10.0f) {
            out.fovY = 2.0f * atanf(1.0f / p11);
        } else {
            out.fovY = 1.22173f;  // Default ~70 degrees
        }
        
        // Extract near/far from projection matrix
        // For right-handed perspective: P[2][2] = f/(n-f), P[3][2] = nf/(n-f)
        float p22 = projMatrix[10]; // [2][2]
        float p32 = projMatrix[14]; // [3][2]
        
        if (p32 != 0.0f && p22 != 1.0f && p22 != 0.0f) {
            // Solve for near and far
            // p22 = f/(n-f) => f = p22*(n-f) = p22*n - p22*f => f*(1+p22) = p22*n
            // p32 = nf/(n-f)
            // Dividing: p32/p22 = n
            out.nearClip = fabsf(p32 / (p22 + 1.0f));
            out.farClip = fabsf(p32 / p22);
            
            // Clamp to reasonable values
            if (out.nearClip < 0.01f || out.nearClip > 10.0f) out.nearClip = 0.1f;
            if (out.farClip < 100.0f || out.farClip > 10000.0f) out.farClip = 1000.0f;
        } else {
            out.nearClip = 0.1f;
            out.farClip = 1000.0f;
        }
        
        // Extract aspect ratio: P[0][0] / P[1][1] = 1/aspect
        float p00 = projMatrix[0];
        if (p00 > 0.0f && p11 > 0.0f) {
            out.aspectRatio = p11 / p00;
        } else {
            out.aspectRatio = 16.0f / 9.0f;
        }
        
        // Compute inverse matrices
        InvertMatrix4x4(out.viewMatrix, out.invViewMatrix);
        InvertMatrix4x4(out.projMatrix, out.invProjMatrix);
        
        // Compute combined view-projection
        MultiplyMatrix4x4(out.viewMatrix, out.projMatrix, out.viewProjMatrix);
        
        // Extract camera position from inverse view matrix (translation column)
        out.position[0] = out.invViewMatrix[12];
        out.position[1] = out.invViewMatrix[13];
        out.position[2] = out.invViewMatrix[14];
        
        // Extract camera vectors from inverse view matrix
        out.right[0] = out.invViewMatrix[0];
        out.right[1] = out.invViewMatrix[1];
        out.right[2] = out.invViewMatrix[2];
        
        out.up[0] = out.invViewMatrix[4];
        out.up[1] = out.invViewMatrix[5];
        out.up[2] = out.invViewMatrix[6];
        
        out.forward[0] = -out.invViewMatrix[8];   // Negate for RH coordinate system
        out.forward[1] = -out.invViewMatrix[9];
        out.forward[2] = -out.invViewMatrix[10];
        
        out.isValid = true;
    } else {
        // Projection matrix is invalid, use defaults
        out.nearClip = 0.1f;
        out.farClip = 1000.0f;
        out.fovY = 1.22173f;
        out.aspectRatio = 16.0f / 9.0f;
        
        // Identity matrices
        memset(out.viewMatrix, 0, sizeof(out.viewMatrix));
        out.viewMatrix[0] = out.viewMatrix[5] = out.viewMatrix[10] = out.viewMatrix[15] = 1.0f;
        
        BuildPerspectiveMatrix(out.projMatrix, out.fovY, out.aspectRatio, out.nearClip, out.farClip);
        
        memcpy(out.invViewMatrix, out.viewMatrix, sizeof(out.invViewMatrix));
        InvertMatrix4x4(out.projMatrix, out.invProjMatrix);
        MultiplyMatrix4x4(out.viewMatrix, out.projMatrix, out.viewProjMatrix);
        
        out.position[0] = out.position[1] = out.position[2] = 0.0f;
        out.forward[0] = 0.0f; out.forward[1] = 0.0f; out.forward[2] = -1.0f;
        out.up[0] = 0.0f; out.up[1] = 1.0f; out.up[2] = 0.0f;
        out.right[0] = 1.0f; out.right[1] = 0.0f; out.right[2] = 0.0f;
        
        out.isValid = false;
    }
    
    out.frameIndex++;
}

} // namespace Camera
