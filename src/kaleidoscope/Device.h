/* -*- mode: c++ -*-
 * kaleidoscope::Device -- Kaleidoscope device Base class
 * Copyright (C) 2017, 2018, 2019  Keyboard.io, Inc
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

/** @file kaleidoscope/Hardware.h
 * Base class for Kaleidoscope hardware libraries.
 */

#pragma once

#include "kaleidoscope/MatrixAddr.h"
#include "kaleidoscope_internal/deprecations.h"
#include "kaleidoscope/macro_helpers.h"

#ifndef CRGB
#error cRGB and CRGB *must* be defined before including this header!
#endif

#define _DEPRECATED_MESSAGE_HARDWARE_RESETDEVICE                       \
  "`KeyboardHardware.resetDevice()` is deprecated, please use " __NL__ \
  "`KeyboardHardware.rebootBootloader()` instead."

/* All hardware libraries must define the following macros:
 * HARDWARE_IMPLEMENTATION - the name of your public object conforming to
 *   the 'class Hardware' interface below.
 * CRGB(r,g,b) - explained below
 * cRGB, a structure with at least three members: r, g, and b -
 * compilation will fail otherwise.
 *
 * Despite its name, the members do not need to be in the order r g b -- most
 * likely they will be in an order that is convenient for the hardware. So
 * initializing a cRGB with a struct literal will give surprising results for any
 * colors where r, g, and b do not have the same value. Each Hardware library
 * defines a CRGB(r,g,b) macro which returns a literal cRGB with the given values.
 */

namespace kaleidoscope {

/** Kaleidoscope Hardware base class.
 * Essential methods all hardware libraries must implement.
 */

template<typename _DeviceDescription>
class Device {
 private:
  class DummySerial {
   public:
    DummySerial() {}
  };

  static DummySerial dummy_serial_;

 public:

  typedef _DeviceDescription Description;

  typedef typename _DeviceDescription::KeyScanner KeyScanner;
  typedef typename _DeviceDescription::KeyScannerDescription::KeyAddr KeyAddr;
  typedef typename _DeviceDescription::LEDs LEDs;
  typedef typename _DeviceDescription::MCU MCU;
  typedef typename _DeviceDescription::BootLoader BootLoader;
  typedef typename _DeviceDescription::Storage Storage;

  static constexpr uint8_t matrix_rows = _DeviceDescription::KeyScannerDescription::matrix_rows;
  static constexpr uint8_t matrix_columns = _DeviceDescription::KeyScannerDescription::matrix_columns;
  static constexpr typename _DeviceDescription::LEDs::LedCountType led_count = _DeviceDescription::LEDs::led_count;

  /**
   * @returns the number of keys on the keyboard.
   */
  static constexpr int8_t numKeys() {
    return matrix_columns * matrix_rows;
  }

  /**
   * Returns the storage driver used by the keyboard.
   */
  Storage &storage() {
    return storage_;
  }

  /**
   * Returns the serial port driver used by the keyboard.
   */
  DummySerial &serialPort() {
    return dummy_serial_;
  }

  /**
   * @defgroup kaleidoscope_hardware_leds Kaleidoscope::Hardware/LEDs
   * @{
   */
  /**
   * Sync the LEDs with the underlying hardware. This should make sure that
   * changes made before this call are reflected on the device.
   */
  void syncLeds(void) {
    leds_.syncLeds();
  }
  /**
   * Set the color of a per-key LED at a given row and column.
   *
   * Setting the color does not need to take effect immediately, it can be
   * delayed until @ref syncLeds is called.
   *
   * @param key_addr is the matrix address of the LED.
   * @param color is the color to set the LED to.
   */
  void setCrgbAt(KeyAddr key_addr, cRGB color) {
    setCrgbAt(getLedIndex(key_addr), color);
  }
  /**
   * Set the color of a per-key LED at a given row and column.
   *
   * Setting the color does not need to take effect immediately, it can be
   * delayed until @ref syncLeds is called.
   *
   * @param row is the logical row position of the key.
   * @param col is the logical column position of the key.
   * @param color is the color to set the LED to.
   */
  DEPRECATED(ROW_COL_FUNC) void setCrgbAt(byte row, byte col, cRGB color) {
    setCrgbAt(KeyAddr(row, col), color);
  }
  /**
   * Set the color of a per-key LED at a given LED index.
   *
   * Setting the color does not need to take effect immediately, it can be
   * delayed until @ref syncLeds is called.
   *
   * @param i is the LED index to change the color of.
   * @param color is the color to set it to.
   */
  void setCrgbAt(uint8_t i, cRGB color) {
    leds_.setCrgbAt(i, color);
  }
  /**
   * Returns the color of the LED at a given index.
   *
   * @param i is the index of the LED to return the color of.
   *
   * @returns The color at the given position.
   */
  cRGB getCrgbAt(uint8_t i) {
    return leds_.getCrgbAt(i);
  }
  /**
   * Returns the color of the LED at a given index.
   *
   * @param key_addr is the key address of the LED to return the color of.
   *
   * @returns The color at the given position.
   */
  cRGB getCrgbAt(KeyAddr key_addr) {
    return getCrgbAt(getLedIndex(key_addr));
  }
  /**
  * Returns the index of the LED at a given row & column.
  *
  * @param key_addr is the matrix address of the LED.
  *
  * @returns The index of the LED at the given position, or -1 if there are no
  * LEDs there.
  */
  int8_t getLedIndex(KeyAddr key_addr) {
    return leds_.getLedIndex(key_addr);
  }
  /**
   * Returns the index of the LED at a given row & column.
   *
   * @param row is the logical row position of the key.
   * @param col is the logical column position of the key.
   *
   * @returns The index of the LED at the given position, or -1 if there are no
   * LEDs there.
   */
  DEPRECATED(ROW_COL_FUNC) int8_t getLedIndex(uint8_t row, byte col) {
    return leds_.getLedIndex(KeyAddr(row, col));
  }
  /** @} */

  /** @defgroup kaleidoscope_hardware_matrix Kaleidoscope::Hardware/Matrix
   * @{
   */
  /**
   * Scan the keyboard matrix, and act on it.
   */
  void scanMatrix(void) {
    key_scanner_.scanMatrix();
  }
  /**
   * Read the state of the keyboard matrix.
   *
   * Do whatever is necessary to read the current keyboard state - but without
   * acting on it.
   *
   * This is primarily used by @ref scanMatrix, but may have other uses too.
   */
  void readMatrix(void) {
    key_scanner_.readMatrix();
  }
  /**
   * Act on the scanned keyboard matrix.
   *
   * Iterate through the scanned state (@see readMatrix), and act on any events.
   */
  void actOnMatrixScan(void) {
    key_scanner_.actOnMatrixScan();
  }
  /** @} */

  /** @defgroup kaleidoscope_hardware_masking Kaleidoscope::Hardware/Key masking
   *
   * Sometimes there are situations when one wants to ignore key events for a
   * while, to mask them out. Masked keys will be ignored until they are
   * released.
   *
   * This is implemented in the Hardware library because that knows best how
   * to mask efficiently, as this requires a deeper knowledge of the hardware,
   * which is all but hidden from the rest of the plugins.
   * @{
   */
  /**
   * Mask out a key.
   *
   * Masking a key out means that any other event than a release will be
   * ignored until said release.
   *
   * @param key_addr is the matrix address of the key.
   */
  void maskKey(KeyAddr key_addr) {
    key_scanner_.maskKey(key_addr);
  }
  /**
   * Mask out a key.
   *
   * Masking a key out means that any other event than a release will be
   * ignored until said release.
   *
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   */
  DEPRECATED(ROW_COL_FUNC) void maskKey(byte row, byte col) {
    key_scanner_.maskKey(KeyAddr(row, col));
  }
  /**
   * Unmask a key.
   *
   * Remove the mask - if any - for a given key. To be used when the mask
   * needs to be removed without the key being released.
   *
   * @param key_addr is the matrix address of the key.
   */
  void unMaskKey(KeyAddr key_addr) {
    key_scanner_.unMaskKey(key_addr);
  }
  /**
   * Unmask a key.
   *
   * Remove the mask - if any - for a given key. To be used when the mask
   * needs to be removed without the key being released.
   *
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   */
  DEPRECATED(ROW_COL_FUNC) void unMaskKey(byte row, byte col) {
    key_scanner_.unMaskKey(KeyAddr(row, col));
  }
  /**
   * Check whether a key is masked or not.
   *
   * @param key_addr is the matrix address of the key.
   *
   * @returns true if the key is masked, false otherwise.
   */
  bool isKeyMasked(KeyAddr key_addr) {
    return key_scanner_.isKeyMasked(key_addr);
  }
  /**
   * Check whether a key is masked or not.
   *
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   *
   * @returns true if the key is masked, false otherwise.
   */
  DEPRECATED(ROW_COL_FUNC) bool isKeyMasked(byte row, byte col) {
    return key_scanner_.isKeyMasked(KeyAddr(row, col));
  }
  /** @} */

  /** @defgroup kaleidoscope_hardware_reattach Kaleidoscope::Hardware/Attach & Detach
   *
   * In situations where one wants to re-initialize the devices, perhaps to
   * change settings inbetween, detaching from and then attaching back to the
   * host is a desirable feature to have. Especially if this does not cut power,
   * nor reboot the device.
   *
   * Because different hardware has different ways to accomplish this, the
   * hardware plugin must provide these functions. Kaleidoscope will wrap them,
   * so user code does not have to deal with KeyboardHardware.
   * @{
   */
  /**
   * Detach the device from the host.
   *
   * Must detach the device, without rebooting or cutting power. Only the end
   * points should get detached, the device must remain powered on.
   */
  void detachFromHost() {
    mcu_.detachFromHost();
  }
  /**
   * Attack the device to the host.
   *
   * Must restore the link detachFromHost severed.
   */
  void attachToHost() {
    mcu_.attachToHost();
  }
  /** @} */

  /**
   * @defgroup kaleidoscope_hardware_keyswitch_state Kaleidoscope::Hardware/Key-switch state
   *
   * These methods offer a way to peek at the key switch states, for those cases
   * where we need to deal with the state closest to the hardware. Some methods
   * offer a way to check if a key is pressed, others return the number of
   * pressed keys.
   *
   * @{
   */
  /**
   * Check if a key is pressed at a given position.
   *
   * @param key_addr is the matrix address of the key.
   *
   * @returns true if the key is pressed, false otherwise.
   */
  bool isKeyswitchPressed(KeyAddr key_addr) {
    return key_scanner_.isKeyswitchPressed(key_addr);
  }
  /**
   * Check if a key is pressed at a given position.
   *
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   *
   * @returns true if the key is pressed, false otherwise.
   */
  DEPRECATED(ROW_COL_FUNC) bool isKeyswitchPressed(byte row, byte col) {
    return key_scanner_.isKeyswitchPressed(KeyAddr(row, col));
  }
  /**
   * Check if a key is pressed at a given position.
   *
   * @param keyIndex is the key index, as calculated by `keyIndex`.
   *
   * @note Key indexes start at 1, not 0!
   *
   * @returns true if the key is pressed, false otherwise.
   */
  bool isKeyswitchPressed(uint8_t keyIndex) {
    return key_scanner_.isKeyswitchPressed(KeyAddr(--keyIndex));
  }
  /**
   * Check the number of key switches currently pressed.
   *
   * @returns the number of keys pressed.
   */
  uint8_t pressedKeyswitchCount() {
    return key_scanner_.pressedKeyswitchCount();
  }

  /**
   * Check if a key was pressed at a given position on the previous scan
   *
   * @param key_addr is the matrix address of the key.
   *
   * @returns true if the key was pressed, false otherwise.
   */
  bool wasKeyswitchPressed(KeyAddr key_addr) {
    return key_scanner_.wasKeyswitchPressed(key_addr);
  }
  /**
   * Check if a key was pressed at a given position on the previous scan
   *
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   *
   * @returns true if the key was pressed, false otherwise.
   */
  DEPRECATED(ROW_COL_FUNC) bool wasKeyswitchPressed(byte row, byte col) {
    return key_scanner_.wasKeyswitchPressed(KeyAddr(row, col));
  }
  /**
   * Check if a key was pressed at a given position on the previous scan.
   *
   * @param keyIndex is the key index, as calculated by `keyIndex`.
   *
   * @note Key indexes start at 1, not 0!
   *
   * @returns true if the key was pressed, false otherwise.
   */
  bool wasKeyswitchPressed(uint8_t keyIndex) {
    return key_scanner_.wasKeyswitchPressed(KeyAddr(--keyIndex));
  }

  /**
   * Check the number of key switches pressed in the previous scan.
   *
   * @returns the number of keys pressed.
   */
  uint8_t previousPressedKeyswitchCount() {
    return key_scanner_.previousPressedKeyswitchCount();
  }


  /** @} */

  /**
   * @defgroup kaleidoscope_hardware_misc Kaleidoscope::Hardware/Miscellaneous methods
   * @{
   */
  /**
   * Method to do any hardware-specific initialization.
   *
   * Called once when the device boots, this should initialize the device, and
   * bring it up into a useful state.
   */
  void setup() {
    key_scanner_.setup();
    leds_.setup();
    mcu_.setup();
  }

  /**
   * Method to configure the device for a hardware test mode
   *
   * Called by the Kaleidoscope Hardware test plugin, this method should
   * do any device-specific initialization needed for factory hardware testing
   *
   */
  void enableHardwareTestMode() {}

  /**
   * Method to put the device into programmable/bootloader mode.
   *
   * This is the old, legacy name of the method.
   */
  DEPRECATED(HARDWARE_RESETDEVICE) void resetDevice() {
    bootloader_.rebootBootloader();
  }

  /**
   * Method to put the device into programmable/bootloader mode.
   */
  void rebootBootloader() {
    bootloader_.rebootBootloader();
  }

  /** @} */

 protected:
  KeyScanner key_scanner_;
  LEDs leds_;
  MCU mcu_;
  BootLoader bootloader_;
  Storage storage_;
};
}
