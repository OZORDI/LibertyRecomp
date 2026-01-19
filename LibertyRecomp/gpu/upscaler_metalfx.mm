// =============================================================================
// MetalFX Temporal Upscaler Implementation (Objective-C++)
// Uses MTLFXTemporalScaler with motion vectors for high-quality upscaling
// =============================================================================

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import <MetalFX/MetalFX.h>
#include "upscaler.h"
#include <os/logger.h>

namespace Upscaler {

// =============================================================================
// MetalFX Native Implementation
// =============================================================================
class MetalFXNativeContext {
public:
    id<MTLDevice> device = nil;
    id<MTLCommandQueue> commandQueue = nil;
    id<MTLFXTemporalScaler> temporalScaler = nil;
    id<MTLFXSpatialScaler> spatialScaler = nil;
    
    // Render dimensions
    uint32_t inputWidth = 0;
    uint32_t inputHeight = 0;
    uint32_t outputWidth = 0;
    uint32_t outputHeight = 0;
    
    // Temporal state
    bool useTemporalScaler = true;
    bool resetHistory = true;
    
    ~MetalFXNativeContext() {
        Shutdown();
    }
    
    bool Initialize(id<MTLDevice> mtlDevice, uint32_t inWidth, uint32_t inHeight,
                    uint32_t outWidth, uint32_t outHeight) {
        if (!mtlDevice) {
            LOG_ERROR("[MetalFX] No Metal device provided");
            return false;
        }
        
        device = mtlDevice;
        inputWidth = inWidth;
        inputHeight = inHeight;
        outputWidth = outWidth;
        outputHeight = outHeight;
        
        // Create command queue
        commandQueue = [device newCommandQueue];
        if (!commandQueue) {
            LOG_ERROR("[MetalFX] Failed to create command queue");
            return false;
        }
        
        // Try to create temporal scaler first (better quality with motion vectors)
        if (@available(macOS 13.0, iOS 16.0, *)) {
            MTLFXTemporalScalerDescriptor* temporalDesc = [[MTLFXTemporalScalerDescriptor alloc] init];
            temporalDesc.inputWidth = inputWidth;
            temporalDesc.inputHeight = inputHeight;
            temporalDesc.outputWidth = outputWidth;
            temporalDesc.outputHeight = outputHeight;
            temporalDesc.colorTextureFormat = MTLPixelFormatRGBA16Float;
            temporalDesc.depthTextureFormat = MTLPixelFormatDepth32Float;
            temporalDesc.motionTextureFormat = MTLPixelFormatRG16Float;
            temporalDesc.outputTextureFormat = MTLPixelFormatRGBA16Float;
            // Note: autoExposureEnabled property may not exist in all SDK versions
            
            temporalScaler = [temporalDesc newTemporalScalerWithDevice:device];
            
            if (temporalScaler) {
                useTemporalScaler = true;
                LOGF_INFO("[MetalFX] Temporal scaler created ({}x{} -> {}x{})",
                          inputWidth, inputHeight, outputWidth, outputHeight);
            } else {
                LOG_WARNING("[MetalFX] Temporal scaler not available, falling back to spatial");
                useTemporalScaler = false;
            }
            
            // Also create spatial scaler as fallback
            MTLFXSpatialScalerDescriptor* spatialDesc = [[MTLFXSpatialScalerDescriptor alloc] init];
            spatialDesc.inputWidth = inputWidth;
            spatialDesc.inputHeight = inputHeight;
            spatialDesc.outputWidth = outputWidth;
            spatialDesc.outputHeight = outputHeight;
            spatialDesc.colorTextureFormat = MTLPixelFormatRGBA16Float;
            spatialDesc.outputTextureFormat = MTLPixelFormatRGBA16Float;
            spatialDesc.colorProcessingMode = MTLFXSpatialScalerColorProcessingModeLinear;
            
            spatialScaler = [spatialDesc newSpatialScalerWithDevice:device];
            
            if (!spatialScaler && !temporalScaler) {
                LOG_ERROR("[MetalFX] Failed to create any scaler");
                return false;
            }
        } else {
            LOG_ERROR("[MetalFX] Requires macOS 13.0+ or iOS 16.0+");
            return false;
        }
        
        resetHistory = true;
        return true;
    }
    
    void Shutdown() {
        temporalScaler = nil;
        spatialScaler = nil;
        commandQueue = nil;
        device = nil;
    }
    
    bool UpscaleTemporal(id<MTLTexture> colorTexture,
                         id<MTLTexture> depthTexture,
                         id<MTLTexture> motionTexture,
                         id<MTLTexture> outputTexture,
                         float jitterX, float jitterY,
                         bool reset) {
        if (!temporalScaler) return false;
        
        @autoreleasepool {
            id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
            if (!commandBuffer) {
                LOG_ERROR("[MetalFX] Failed to create command buffer");
                return false;
            }
            
            // Configure temporal scaler
            temporalScaler.colorTexture = colorTexture;
            temporalScaler.depthTexture = depthTexture;
            temporalScaler.motionTexture = motionTexture;
            temporalScaler.outputTexture = outputTexture;
            
            // Set jitter offsets (in pixels)
            temporalScaler.jitterOffsetX = jitterX;
            temporalScaler.jitterOffsetY = jitterY;
            
            // Reset temporal history if needed
            temporalScaler.reset = reset || resetHistory;
            resetHistory = false;
            
            // Motion vector scale (pixels to NDC)
            // Motion vectors should be in pixel space, scale to -1..1 range
            temporalScaler.motionVectorScaleX = 1.0f;
            temporalScaler.motionVectorScaleY = 1.0f;
            
            // Encode upscaling
            [temporalScaler encodeToCommandBuffer:commandBuffer];
            
            // Commit and wait
            [commandBuffer commit];
            [commandBuffer waitUntilCompleted];
        }
        
        return true;
    }
    
    bool UpscaleSpatial(id<MTLTexture> colorTexture,
                        id<MTLTexture> outputTexture) {
        if (!spatialScaler) return false;
        
        @autoreleasepool {
            id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
            if (!commandBuffer) {
                LOG_ERROR("[MetalFX] Failed to create command buffer");
                return false;
            }
            
            // Configure spatial scaler
            spatialScaler.colorTexture = colorTexture;
            spatialScaler.outputTexture = outputTexture;
            
            // Encode upscaling
            [spatialScaler encodeToCommandBuffer:commandBuffer];
            
            // Commit and wait
            [commandBuffer commit];
            [commandBuffer waitUntilCompleted];
        }
        
        return true;
    }
    
    void ResetHistory() {
        resetHistory = true;
    }
    
    bool IsTemporalAvailable() const {
        return temporalScaler != nil;
    }
    
    bool IsSpatialAvailable() const {
        return spatialScaler != nil;
    }
};

// =============================================================================
// C++ Wrapper for MetalFX
// =============================================================================
static MetalFXNativeContext* g_metalFXContext = nullptr;

bool MetalFX_Initialize(void* mtlDevice, uint32_t inputWidth, uint32_t inputHeight,
                        uint32_t outputWidth, uint32_t outputHeight) {
    if (g_metalFXContext) {
        delete g_metalFXContext;
    }
    
    g_metalFXContext = new MetalFXNativeContext();
    return g_metalFXContext->Initialize((__bridge id<MTLDevice>)mtlDevice,
                                         inputWidth, inputHeight,
                                         outputWidth, outputHeight);
}

void MetalFX_Shutdown() {
    if (g_metalFXContext) {
        delete g_metalFXContext;
        g_metalFXContext = nullptr;
    }
}

bool MetalFX_UpscaleTemporal(void* colorTexture, void* depthTexture,
                              void* motionTexture, void* outputTexture,
                              float jitterX, float jitterY, bool reset) {
    if (!g_metalFXContext) return false;
    
    return g_metalFXContext->UpscaleTemporal(
        (__bridge id<MTLTexture>)colorTexture,
        (__bridge id<MTLTexture>)depthTexture,
        (__bridge id<MTLTexture>)motionTexture,
        (__bridge id<MTLTexture>)outputTexture,
        jitterX, jitterY, reset);
}

bool MetalFX_UpscaleSpatial(void* colorTexture, void* outputTexture) {
    if (!g_metalFXContext) return false;
    
    return g_metalFXContext->UpscaleSpatial(
        (__bridge id<MTLTexture>)colorTexture,
        (__bridge id<MTLTexture>)outputTexture);
}

void MetalFX_ResetHistory() {
    if (g_metalFXContext) {
        g_metalFXContext->ResetHistory();
    }
}

bool MetalFX_IsTemporalAvailable() {
    return g_metalFXContext && g_metalFXContext->IsTemporalAvailable();
}

bool MetalFX_IsSpatialAvailable() {
    return g_metalFXContext && g_metalFXContext->IsSpatialAvailable();
}

} // namespace Upscaler
