/*******************************************************************************
 * Copyright (C) 2021-2024 Intel Corporation
 *
 * Intel XeSS SDK - Minimal Header for Integration
 * Based on Intel XeSS SDK 2.1
 *
 * For full SDK, download from: https://github.com/intel/xess/releases
 ******************************************************************************/

#ifndef XESS_H
#define XESS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// XeSS version info
#define XESS_VERSION_MAJOR 2
#define XESS_VERSION_MINOR 1
#define XESS_VERSION_PATCH 1

// Result codes
typedef enum xess_result_t {
    XESS_RESULT_SUCCESS = 0,
    XESS_RESULT_ERROR_UNSUPPORTED_DEVICE = 1,
    XESS_RESULT_ERROR_UNSUPPORTED_DRIVER = 2,
    XESS_RESULT_ERROR_UNINITIALIZED = 3,
    XESS_RESULT_ERROR_INVALID_ARGUMENT = 4,
    XESS_RESULT_ERROR_DEVICE_OUT_OF_MEMORY = 5,
    XESS_RESULT_ERROR_DEVICE = 6,
    XESS_RESULT_ERROR_NOT_IMPLEMENTED = 7,
    XESS_RESULT_ERROR_INVALID_CONTEXT = 8,
    XESS_RESULT_ERROR_OPERATION_IN_PROGRESS = 9,
    XESS_RESULT_ERROR_UNSUPPORTED = 10,
    XESS_RESULT_ERROR_CANT_LOAD_LIBRARY = 11,
    XESS_RESULT_ERROR_UNKNOWN = 1000
} xess_result_t;

// Quality settings
typedef enum xess_quality_settings_t {
    XESS_QUALITY_SETTING_ULTRA_PERFORMANCE = 0,
    XESS_QUALITY_SETTING_PERFORMANCE = 1,
    XESS_QUALITY_SETTING_BALANCED = 2,
    XESS_QUALITY_SETTING_QUALITY = 3,
    XESS_QUALITY_SETTING_ULTRA_QUALITY = 4,
    XESS_QUALITY_SETTING_ULTRA_QUALITY_PLUS = 5,
    XESS_QUALITY_SETTING_AA = 6
} xess_quality_settings_t;

// Network model
typedef enum xess_network_model_t {
    XESS_NETWORK_MODEL_KPSS = 0,
    XESS_NETWORK_MODEL_SPLAT = 1,
    XESS_NETWORK_MODEL_DEFAULT = XESS_NETWORK_MODEL_KPSS
} xess_network_model_t;

// Init flags
typedef enum xess_init_flags_t {
    XESS_INIT_FLAG_NONE = 0,
    XESS_INIT_FLAG_MOTION_VECTORS_HIGH_RES = 1 << 0,
    XESS_INIT_FLAG_MOTION_VECTORS_JITTERED = 1 << 1,
    XESS_INIT_FLAG_EXPOSURE_SCALE_TEXTURE = 1 << 2,
    XESS_INIT_FLAG_RESPONSIVE_PIXEL_MASK = 1 << 3,
    XESS_INIT_FLAG_INVERTED_DEPTH = 1 << 4,
    XESS_INIT_FLAG_HIGH_RES_MV = XESS_INIT_FLAG_MOTION_VECTORS_HIGH_RES,
    XESS_INIT_FLAG_JITTERED_MV = XESS_INIT_FLAG_MOTION_VECTORS_JITTERED,
    XESS_INIT_FLAG_LDR_INPUT_COLOR = 1 << 7,
    XESS_INIT_FLAG_ENABLE_AUTOEXPOSURE = 1 << 8,
    XESS_INIT_FLAG_USE_NDC_VELOCITY = 1 << 9,
    XESS_INIT_FLAG_EXTERNAL_DESCRIPTOR_HEAP = 1 << 10
} xess_init_flags_t;

// Logging levels
typedef enum xess_logging_level_t {
    XESS_LOGGING_LEVEL_DEBUG = 0,
    XESS_LOGGING_LEVEL_INFO = 1,
    XESS_LOGGING_LEVEL_WARNING = 2,
    XESS_LOGGING_LEVEL_ERROR = 3
} xess_logging_level_t;

// 2D structure
typedef struct xess_2d_t {
    uint32_t x;
    uint32_t y;
} xess_2d_t;

// Version structure
typedef struct xess_version_t {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
    uint32_t reserved;
} xess_version_t;

// Properties structure
typedef struct xess_properties_t {
    uint32_t minInputWidth;
    uint32_t minInputHeight;
    uint32_t optimalInputWidth;
    uint32_t optimalInputHeight;
    uint32_t maxInputWidth;
    uint32_t maxInputHeight;
} xess_properties_t;

// Context handle
typedef struct xess_context_t* xess_context_handle_t;

// Callback types
typedef void (*xess_app_log_callback_t)(const char* message, xess_logging_level_t level);

// Core functions (loaded from libxess.dll at runtime)
typedef xess_result_t (*PFN_xessGetVersion)(xess_version_t* pVersion);
typedef xess_result_t (*PFN_xessGetProperties)(xess_context_handle_t hContext, uint32_t* pOutputWidth, uint32_t* pOutputHeight, xess_properties_t* pProps);
typedef xess_result_t (*PFN_xessGetInputResolution)(xess_context_handle_t hContext, uint32_t outputWidth, uint32_t outputHeight, xess_quality_settings_t quality, uint32_t* pInputWidth, uint32_t* pInputHeight);
typedef xess_result_t (*PFN_xessGetOptimalInputResolution)(xess_context_handle_t hContext, uint32_t outputWidth, uint32_t outputHeight, xess_quality_settings_t quality, uint32_t* pOptimalWidth, uint32_t* pOptimalHeight, uint32_t* pMinWidth, uint32_t* pMinHeight, uint32_t* pMaxWidth, uint32_t* pMaxHeight);
typedef xess_result_t (*PFN_xessGetJitterScale)(xess_context_handle_t hContext, float* pX, float* pY);
typedef xess_result_t (*PFN_xessGetVelocityScale)(xess_context_handle_t hContext, float* pX, float* pY);
typedef xess_result_t (*PFN_xessDestroyContext)(xess_context_handle_t hContext);
typedef xess_result_t (*PFN_xessSetLoggingCallback)(xess_context_handle_t hContext, xess_logging_level_t level, xess_app_log_callback_t callback);
typedef xess_result_t (*PFN_xessSetVelocityScale)(xess_context_handle_t hContext, float x, float y);
typedef xess_result_t (*PFN_xessIsOptimalDriver)(xess_context_handle_t hContext);

// Macros for checking results
#define XESS_SUCCEEDED(result) ((result) == XESS_RESULT_SUCCESS)
#define XESS_FAILED(result) ((result) != XESS_RESULT_SUCCESS)

#ifdef __cplusplus
}
#endif

#endif // XESS_H
