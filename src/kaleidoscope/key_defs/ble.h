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

/**
 * @file ble.h
 * @brief Key definitions for Bluetooth Low Energy (BLE) functionality.
 * 
 * This file defines key constants for BLE-related operations such as:
 * - Toggling between USB and BLE modes
 * - Turning off BLE
 * - Starting BLE pairing
 * - Selecting different BLE device slots
 * 
 * These keys are processed by the BLEBluefruit::onKeyEvent handler when
 * the firmware is built with BLE support. 
 */

#include <stdint.h>                 // for uint8_t
#include "kaleidoscope/key_defs.h"  // for Key

/// Toggle between USB and BLE modes
constexpr uint8_t BLE_TOGGLE = 0x70;
/// Turn off BLE and switch to USB
constexpr uint8_t BLE_OFF = 0x71;
/// Start BLE pairing process for the selected device
constexpr uint8_t BLE_PAIR = 0x72;
// 0x73-0x7F reserved for future BLE operation keys
/// Select BLE device slot 1
constexpr uint8_t BLE_SELECT_DEVICE_1 = 0x80;
/// Select BLE device slot 2
constexpr uint8_t BLE_SELECT_DEVICE_2 = 0x81;
/// Select BLE device slot 3
constexpr uint8_t BLE_SELECT_DEVICE_3 = 0x82;
/// Select BLE device slot 4
constexpr uint8_t BLE_SELECT_DEVICE_4 = 0x83;
// 0x84-0x8F reserved for future device slots


/**
 * @name BLE Operation Keys
 * Keys for basic Bluetooth Low Energy operations
 * @{
 */

/** Toggle between USB and BLE modes
 *
 * This key switches the keyboard between USB and BLE modes.
 * In BLE mode, the keyboard will communicate wirelessly with the paired device.
 * In USB mode, the keyboard will communicate via the USB cable.
 */
#define Key_BLEToggle Key(BLE_TOGGLE, KEY_FLAGS | SYNTHETIC)

/** Turn off BLE and switch to USB mode
 *
 * This key turns off BLE functionality and switches the keyboard to USB mode.
 * It will also stop advertising and disconnect from any currently connected device.
 */
#define Key_BLEOff Key(BLE_OFF, KEY_FLAGS | SYNTHETIC)

/** Start BLE pairing process
 *
 * This key starts the BLE pairing process for the currently selected device slot.
 * The keyboard will enter discoverable mode and can be paired with a new device.
 */
#define Key_BLEStartPairing Key(BLE_PAIR, KEY_FLAGS | SYNTHETIC)
/** @} */

/**
 * @name BLE Device Selection Keys
 * Keys for selecting specific BLE device slots
 * @{
 */

/** Select BLE device slot 1
 *
 * This key selects device slot 1, switches to BLE mode, and attempts
 * to connect to the device paired with this slot.
 */
#define Key_BLESelectDevice1 Key(BLE_SELECT_DEVICE_1, KEY_FLAGS | SYNTHETIC)

/** Select BLE device slot 2
 *
 * This key selects device slot 2, switches to BLE mode, and attempts
 * to connect to the device paired with this slot.
 */
#define Key_BLESelectDevice2 Key(BLE_SELECT_DEVICE_2, KEY_FLAGS | SYNTHETIC)

/** Select BLE device slot 3
 *
 * This key selects device slot 3, switches to BLE mode, and attempts
 * to connect to the device paired with this slot.
 */
#define Key_BLESelectDevice3 Key(BLE_SELECT_DEVICE_3, KEY_FLAGS | SYNTHETIC)

/** Select BLE device slot 4
 *
 * This key selects device slot 4, switches to BLE mode, and attempts
 * to connect to the device paired with this slot.
 */
#define Key_BLESelectDevice4 Key(BLE_SELECT_DEVICE_4, KEY_FLAGS | SYNTHETIC)
/** @} */
