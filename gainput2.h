
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


typedef enum GpResult {
	GP_SUCCESS,
	GP_GENERAL_ERROR,
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

typedef struct GpFloat2 {
	GpFloat x;
	GpFloat y;
} GpFloat2;

typedef struct GpFloat3 {
	GpFloat x;
	GpFloat y;
	GpFloat z;
} GpFloat3;

typedef struct GpInputState {
	union {
		GpFloat f;
		GpFloat2 f2;
		GpFloat3 f3;
		GpBool b;
	} data;
} GpInputState;

typedef struct GpDeviceInputState {

} GpDeviceInputState;

typedef uint64_t GpContext;
typedef uint64_t GpDevice;

typedef void* (*PFN_gpAllocateMemory)(void* userData, size_t size);
typedef void (*PFN_gpFreeMemory)(void* userData, void* ptr);

typedef struct GpContextCreateInfo {
	PFN_gpAllocateMemory allocateMemory;
	PFN_gpFreeMemory freeMemory;
	void* memoryUserData;
} GpContextCreateInfo;

typedef void (*PFN_gpPollInputDevice)(GpDevice device, void* userData);

typedef struct GpRegisterInputDeviceInfo {
	GpContext context;
	GpDeviceType deviceType;
	PFN_gpPollInputDevice pollInputDevice;
	void* userData;
} GpRegisterInputDeviceInfo;



GpResult gpCreateContext(const GpContextCreateInfo* pCreateInfo, GpContext* context);
void gpDestroyContext(GpContext context);
void gpProcessContext(GpContext context);

GpResult gpRegisterDevice(const GpRegisterInputDeviceInfo* pRegisterInfo, GpDevice* device);
void gpUnregisterDevice(GpDevice device);
GpResult gpGetDeviceStatus(GpDevice device, GpDeviceStatus* state);

#ifdef __cplusplus
}
#endif

#endif

