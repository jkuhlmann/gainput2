
#ifndef GAINPUT2_H_
#define GAINPUT2_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool GpBool;
typedef float GpFloat;
typedef size_t GpSize;

typedef uint64_t GpContext;
typedef uint64_t GpDevice;
typedef uint64_t GpDeviceButton;

typedef void* (*PFN_gpAllocateMemory)(void* userData, size_t size);
typedef void (*PFN_gpFreeMemory)(void* userData, void* ptr);


typedef enum GpResult {
	GP_SUCCESS,
	GP_GENERIC_ERROR,
} GpResult;

typedef enum GpDeviceType {
	GP_DEVICE_TYPE_KEYBOARD,
	GP_DEVICE_TYPE_MOUSE,
	GP_DEVICE_TYPE_GAMEPAD,
	GP_DEVICE_TYPE_TOUCH,
} GpDeviceType;

typedef enum GpDeviceStatus {
	GP_DEVICE_STATUS_UNAVAILABLE,
	GP_DEVICE_STATUS_OK,
	GP_DEVICE_STATUS_LOW_BATTERY,
} GpDeviceStatus;

typedef enum GpDeviceButtonType {
	GP_DEVICE_BUTTON_TYPE_FLOAT,
	GP_DEVICE_BUTTON_TYPE_FLOAT2,
	GP_DEVICE_BUTTON_TYPE_FLOAT3,
	GP_DEVICE_BUTTON_TYPE_BOOL,
} GpDeviceButtonType;

typedef struct GpFloat2 {
	GpFloat x;
	GpFloat y;
} GpFloat2;

typedef struct GpFloat3 {
	GpFloat x;
	GpFloat y;
	GpFloat z;
} GpFloat3;

typedef struct GpButtonState {
	union {
		GpFloat f;
		GpFloat2 f2;
		GpFloat3 f3;
		GpBool b;
	} data;
} GpButtonState;

typedef struct GpDeviceInputState {
	GpSize numButtons;
	GpButtonState buttons[];
} GpDeviceInputState;


typedef void (*PFN_gpPollInputDevice)(GpDevice device, void* userData, GpDeviceInputState* inputState);

typedef struct GpContextCreateInfo {
	PFN_gpAllocateMemory allocateMemory;
	PFN_gpFreeMemory freeMemory;
	void* memoryUserData;
} GpContextCreateInfo;


typedef struct GpRegisterInputDeviceInfo {
	GpDeviceType deviceType;
	GpSize deviceNumber;
	PFN_gpPollInputDevice pollInputDevice;
	void* userData;
} GpRegisterInputDeviceInfo;

typedef struct GpDeviceButtonInfo {
	GpBool isValid;
	GpDevice device;
	GpDeviceButton button;
	GpDeviceButtonType type;
} GpDeviceButtonInfo;

// Context/global
GpResult gpCreateContext(const GpContextCreateInfo* pCreateInfo, GpContext* context);
void gpDestroyContext(GpContext context);
void gpSetContextDisplaySize(GpContext context, GpSize width, GpSize height);
void gpProcessContext(GpContext context);
GpResult gpGetAnyButtonDown(GpContext context, GpSize maxButtonNum, GpDeviceButtonInfo* deviceButtonInfos);
GpResult gpGetDeviceByType(GpContext context, GpDeviceType deviceType, GpSize deviceNumber);
// TODO device sync?

// Device
GpResult gpRegisterDevice(GpContext context, const GpRegisterInputDeviceInfo* pRegisterInfo, GpDevice* device);
GpResult gpCreateAndRegisterPlatformDevice(GpContext context, GpDeviceType type, GpDevice* device);
void gpUnregisterDevice(GpDevice device);
GpResult gpGetDeviceStatus(GpDevice device, GpDeviceStatus* status);
GpResult gpAllocateDeviceInputState(GpDevice device, GpDeviceInputState** inputState);
void gpFreeDeviceInputState(GpDevice device, GpDeviceInputState* inputState);
GpResult gpUpdateDeviceInputState(GpDevice device, GpDeviceInputState* inputState);
GpResult gpGetDeviceButtonInfo(GpDevice device, GpDeviceButton deviceButton, GpDeviceButtonInfo* deviceButtonInfo);
GpResult gpGetDeviceButtonName(GpDevice device, GpDeviceButton deviceButton, GpSize maxNameLen, char* name);
GpResult gpGetDeviceButtonByName(GpDevice device, const char* name, GpDeviceButton* deviceButton);
GpResult gpGetDeviceAnyButtonDown(GpDevice device, GpSize maxButtonNum, GpDeviceButtonInfo* deviceButtonInfos);
// TODO deadzones

// State helpers
GpBool gpIsDown(const GpDeviceInputState* inputState, GpDeviceButton deviceButton);
GpFloat gpGetFloat(const GpDeviceInputState* inputState, GpDeviceButton deviceButton);

// TODO button change events/listeners

// TODO checking for button changes/deltas

// TODO input mapping

// TODO input processor chaining/graph

// Cut feature (for now): network sync, gestures, thread-safe adding of button events, 
// debug rendering, input recording/playback.

#ifdef __cplusplus
}
#endif

#endif

