/**
 * gainput2 - C input library for games.
 *
 * Copyright (c) 2013-2020 Johannes Kuhlmann.
 * Licensed under the MIT license. See LICENSE file.
 */

#ifndef GAINPUT2_H_
#define GAINPUT2_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool gp_bool;
typedef float gp_float;
typedef size_t gp_size;

typedef uint64_t gp_context;
typedef uint64_t gp_device;
typedef uint64_t gp_device_button;

typedef enum gp_result {
	GP_SUCCESS,
	GP_GENERIC_ERROR,
} gp_result;

typedef enum gp_device_type {
	GP_DEVICE_TYPE_KEYBOARD,
	GP_DEVICE_TYPE_MOUSE,
	GP_DEVICE_TYPE_GAMEPAD,
	GP_DEVICE_TYPE_TOUCH,
} gp_device_type;

typedef enum gp_device_status {
	GP_DEVICE_STATUS_UNAVAILABLE,
	GP_DEVICE_STATUS_OK,
	GP_DEVICE_STATUS_LOW_BATTERY,
} gp_device_status;

typedef enum gp_device_button_type {
	GP_DEVICE_BUTTON_TYPE_FLOAT,
	GP_DEVICE_BUTTON_TYPE_FLOAT2,
	GP_DEVICE_BUTTON_TYPE_FLOAT3,
	GP_DEVICE_BUTTON_TYPE_BOOL,
} gp_device_button_type;

typedef struct gp_float2 {
	gp_float x;
	gp_float y;
} gp_float2;

typedef struct gp_float3 {
	gp_float x;
	gp_float y;
	gp_float z;
} gp_float3;

typedef struct gp_button_state {
	union {
		gp_float f;
		gp_float2 f2;
		gp_float3 f3;
		gp_bool b;
	} data;
} gp_button_state;

typedef struct gp_device_input_state {
	gp_size num_buttons;
	gp_button_state buttons[];
} gp_device_input_state;

typedef void* (*gp_PFN_allocate_memory)(void* user_data, size_t size);
typedef void (*gp_PFN_free_memory)(void* user_data, void* ptr);
typedef void (*gp_PFN_poll_input_device)(gp_device device, void* user_data, gp_device_input_state* input_state);

typedef struct gp_context_create_desc {
	gp_PFN_allocate_memory allocate_memory;
	gp_PFN_free_memory free_memory;
	void* memory_user_data;
} gp_context_create_desc;

typedef struct gp_register_input_device_desc {
	gp_device_type device_type;
	gp_size device_number;
	gp_PFN_poll_input_device poll_input_device;
	void* user_data;
} gp_register_input_device_desc;

typedef struct gp_device_button_info {
	gp_bool is_valid;
	gp_device device;
	gp_device_button button;
	gp_device_button_type type;
} gp_device_button_info;

// Context/global
gp_result gp_create_context(const gp_context_create_desc* create_desc, gp_context* context);
void gp_destroy_context(gp_context context);
void gp_set_context_display_size(gp_context context, gp_size width, gp_size height);
void gp_process_context(gp_context context);
gp_result gp_get_any_button_down(gp_context context, gp_size max_button_num, gp_device_button_info* device_button_infos);
gp_result gp_get_device_by_type(gp_context context, gp_device_type device_type, gp_size device_number);

// Device
gp_result gp_register_device(gp_context context, const gp_register_input_device_desc* register_desc, gp_device* device);
gp_result gp_create_and_register_platform_device(gp_context context, gp_device_type type, gp_device* device);
void gp_unregister_device(gp_device device);
gp_result gp_get_device_status(gp_device device, gp_device_status* status);
gp_result gp_allocate_device_input_state(gp_device device, gp_device_input_state** input_state);
void gp_free_device_input_state(gp_device device, gp_device_input_state* input_state);
gp_result gp_update_device_input_state(gp_device device, gp_device_input_state* input_state);
gp_result gp_get_device_button_info(gp_device device, gp_device_button device_button, gp_device_button_info* device_button_info);
gp_result gp_get_device_button_name(gp_device device, gp_device_button device_button, gp_size max_name_len, char* name);
gp_result gp_get_device_button_by_name(gp_device device, const char* name, gp_device_button* device_button);
gp_result gp_get_device_any_button_down(gp_device device, gp_size max_button_num, gp_device_button_info* device_button_infos);
// TODO deadzones

// State helpers
gp_bool gp_is_down(const gp_device_input_state* input_state, gp_device_button device_button);
gp_float gp_get_float(const gp_device_input_state* input_state, gp_device_button device_button);

// TODO button change events/listeners

// TODO checking for button changes/deltas

// TODO input mapping

// TODO new: input processor chaining/graph

// Cut feature (for now): network sync, gestures, thread-safe adding of button events, 
// debug rendering, input recording/playback.

#ifdef __cplusplus
}
#endif

#endif

