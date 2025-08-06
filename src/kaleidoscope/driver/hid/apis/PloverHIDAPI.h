/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2025 Keyboard.io, inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
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

#include "kaleidoscope/driver/hid/HIDDefs.h"

// Plover HID Protocol Descriptor
// Based on the specification from https://github.com/dnaq/plover-machine-hid
// Uses vendor-defined usage page 0xFF50 ("S") with usage 0x564C ("TN")
// Together they spell "STN" for stenography
#define DESCRIPTOR_PLOVER_HID(...)                                    \
  HID_USAGE_PAGE_N(0xFF50, 2),        /* Vendor-defined page "S" */  \
  HID_USAGE_N(0x564C, 2),             /* Usage "TN" */               \
  HID_COLLECTION(HID_COLLECTION_LOGICAL),                            \
                                                                     \
  /* Report ID, if any */                                            \
  __VA_ARGS__                                                        \
                                                                     \
  /* 64 bits representing key states */                             \
  HID_LOGICAL_MIN(0),                                                \
  HID_LOGICAL_MAX(1),                                                \
  HID_REPORT_SIZE(1),                                                \
  HID_REPORT_COUNT(64),                                              \
  HID_USAGE_PAGE(HID_USAGE_PAGE_ORDINAL),                           \
  HID_USAGE_MIN(0),                                                  \
  HID_USAGE_MAX(63),                                                 \
  HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),                 \
                                                                     \
  HID_COLLECTION_END

// Report data structure for Plover HID
typedef struct {
  uint8_t keys[8];  // 64 bits = 8 bytes
} __attribute__((packed)) HID_PloverHIDReport_Data_t;
