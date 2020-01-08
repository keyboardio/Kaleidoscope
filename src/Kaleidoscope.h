/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2018  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <Arduino.h>

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project KeyboardIO here

#define TS(X) //Serial.print(micros() );Serial.print("\t");Serial.println(X);

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "kaleidoscope/version.h"
#include "kaleidoscope/device/device.h"
#include "kaleidoscope/device/key_indexes.h"
#include "kaleidoscope_internal/device.h"
#include "kaleidoscope_internal/deprecations.h"

#include "kaleidoscope/hid.h"

static constexpr DEPRECATED(KEYBOARDHARDWARE) kaleidoscope::Device &KeyboardHardware = kaleidoscope_internal::device;

#ifdef PER_KEY_DATA_STACKED
#define KEYMAP_STACKED(...) { PER_KEY_DATA_STACKED(XXX, __VA_ARGS__) }
#endif

#ifdef PER_KEY_DATA
#define KEYMAP(...) { PER_KEY_DATA(XXX, __VA_ARGS__) }
#endif

static constexpr DEPRECATED(ROWS) uint8_t ROWS = kaleidoscope_internal::device.matrix_rows;
static constexpr DEPRECATED(COLS) uint8_t COLS = kaleidoscope_internal::device.matrix_columns;
static constexpr DEPRECATED(LED_COUNT) uint8_t LED_COUNT = kaleidoscope_internal::device.led_count;

#include "kaleidoscope/KeyAddr.h"
#include "kaleidoscope/key_events.h"
#include "kaleidoscope/layers.h"
#include "kaleidoscope_internal/sketch_exploration/sketch_exploration.h"
#include "kaleidoscope/macro_map.h"
#include "kaleidoscope_internal/event_dispatch.h"
#include "kaleidoscope_internal/LEDModeManager.h"
#include "kaleidoscope/macro_helpers.h"
#include "kaleidoscope/plugin.h"
#include "kaleidoscope/Runtime.h"

#define HOOK_MAX 64

#ifndef VERSION
#define VERSION "locally-built"
#endif

/** Kaleidoscope API (major) version.
 *
 * The API is guaranteed to be backwards compatible for the entire duration of a
 * major version. However, breaking changes may come, and result in a major
 * version bump. To help migration, the `KALEIDOSCOPE_API_VERSION` macro can be
 * used to check the major version provided by the Kaleidoscope we are compiling
 * against. This can be used to error out with a helpful message, or change how
 * the API is used - it is entirely up to the plugin or sketch author. The point
 * of this macro is to let them easily check the version.
 */
#define KALEIDOSCOPE_API_VERSION 2

/** Required Kaleidoscope major version.
 *
 * For the sake of convenience, defining `KALEIDOSCOPE_REQUIRED_API_VERSION`
 * before including `Kaleidoscope.h` itself will result in comparing its value
 * to `KALEIDOSCOPE_API_VERSION`. If they differ, a helpful error message is
 * printed.
 *
 * Done so that a new API version would result in a helpful error message,
 * instead of cryptic compile errors.
 */
#if defined(KALEIDOSCOPE_REQUIRED_API_VERSION) && (KALEIDOSCOPE_REQUIRED_API_VERSION != KALEIDOSCOPE_API_VERSION)
#define xstr(a) str(a)
#define str(a) #a
static_assert(KALEIDOSCOPE_REQUIRED_API_VERSION == KALEIDOSCOPE_API_VERSION,
              "Kaleidoscope API version mismatch! We have version " xstr(KALEIDOSCOPE_API_VERSION)
              " available, but version " xstr(KALEIDOSCOPE_REQUIRED_API_VERSION) " is required.");
#endif

// Use this function macro to register plugins with Kaleidoscope's
// hooking system. The macro accepts a list of plugin instances that
// must have been instantiated at global scope.
//
#define KALEIDOSCOPE_INIT_PLUGINS(...) _KALEIDOSCOPE_INIT_PLUGINS(__VA_ARGS__)

// For compatibility reasons we export class Runtime_ as Kaleidoscope_
// in global namespace.
//
DEPRECATED(GLOBAL_TYPENAME_KALEIDOSCOPE)
typedef kaleidoscope::Runtime_  Kaleidoscope_;

// For compatibility/usability reasons we enable the global variable
// Kaleidoscope in global namespace.
//
extern kaleidoscope::Runtime_ &Kaleidoscope;
