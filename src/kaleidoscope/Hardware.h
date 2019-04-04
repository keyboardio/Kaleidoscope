/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware -- Kaleidoscope Hardware Base class
 * Copyright (C) 2017, 2018  Keyboard.io, Inc
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

#ifndef CRGB
#error cRGB and CRGB *must* be defined before including this header!
#endif

/* All hardware libraries must define the following macros:
 * HARDWARE_IMPLEMENTATION - the name of your public object conforming to
 *   the 'class Hardware' interface below.
 * CRGB(r,g,b) - explained below
 */

/**
 * Forward declaration of the cRGB structure.
 *
 * The base class does not define the cRGB structure, that is left up to the
 * individual Hardware libraries. We do a forward declaration here, because the
 * base hardware has methods that depend on a cRGB structure.
 *
 * The structure will need to have at least three members: r, g, and b -
 * compilation will fail otherwise.
 *
 * Despite its name, the members do not need to be in the order r g b -- most
 * likely they will be in an order that is convenient for the hardware. So
 * initializing a cRGB with a struct literal will give surprising results for any
 * colors where r, g, and b do not have the same value. Each Hardware library
 * defines a CRGB(r,g,b) macro which returns a literal cRGB with the given values.
 */
typedef struct cRGB cRGB;

namespace kaleidoscope {
/** Kaleidoscope Hardware base class.
 * Essential methods all hardware libraries must implement.
 */
class Hardware {
 public:
  /**
   * @defgroup kaleidoscope_hardware_leds Kaleidoscope::Hardware/LEDs
   * @{
   */
  /**
   * Sync the LEDs with the underlying hardware. This should make sure that
   * changes made before this call are reflected on the device.
   */
  void syncLeds(void) {}
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
  void setCrgbAt(byte row, byte col, cRGB color) {}
  /**
   * Set the color of a per-key LED at a given LED index.
   *
   * Setting the color does not need to take effect immediately, it can be
   * delayed until @ref syncLeds is called.
   *
   * @param i is the LED index to change the color of.
   * @param color is the color to set it to.
   */
  void setCrgbAt(uint8_t i, cRGB color) {}
  /**
   * Returns the color of the LED at a given index.
   *
   * @param i is the index of the LED to return the color of.
   *
   * @returns The color at the given position.
   */
  cRGB getCrgbAt(uint8_t i) {
    cRGB c = {
      0, 0, 0
    };
    return c;
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
  int8_t getLedIndex(uint8_t row, byte col) {
    return -1;
  }
  /** @} */

  /** @defgroup kaleidoscope_hardware_matrix Kaleidoscope::Hardware/Matrix
   * @{
   */
  /**
   * Scan the keyboard matrix, and act on it.
   */
  void scanMatrix(void) {}
  /**
   * Read the state of the keyboard matrix.
   *
   * Do whatever is necessary to read the current keyboard state - but without
   * acting on it.
   *
   * This is primarily used by @ref scanMatrix, but may have other uses too.
   */
  void readMatrix(void) {}
  /**
   * Act on the scanned keyboard matrix.
   *
   * Iterate through the scanned state (@see readMatrix), and act on any events.
   */
  void actOnMatrixScan(void) {}
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
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   */
  void maskKey(byte row, byte col) {}
  /**
   * Unmask a key.
   *
   * Remove the mask - if any - for a given key. To be used when the mask
   * needs to be removed without the key being released.
   *
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   */
  void unMaskKey(byte row, byte col) {}
  /**
   * Check whether a key is masked or not.
   *
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   *
   * @returns true if the key is masked, false otherwise.
   */
  bool isKeyMasked(byte row, byte col) {
    return false;
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
    UDCON |= _BV(DETACH);
  }
  /**
   * Attack the device to the host.
   *
   * Must restore the link detachFromHost severed.
   */
  void attachToHost() {
    UDCON &= ~_BV(DETACH);
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
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   *
   * @returns true if the key is pressed, false otherwise.
   */
  bool isKeyswitchPressed(byte row, byte col) {
    return false;
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
    return false;
  }
  /**
   * Check the number of key switches currently pressed.
   *
   * @returns the number of keys pressed.
   */
  uint8_t pressedKeyswitchCount() {
    return 0;
  }

  /**
   * Check if a key was pressed at a given position on the previous scan
   *
   * @param row is the row the key is located at in the matrix.
   * @param col is the column the key is located at in the matrix.
   *
   * @returns true if the key was pressed, false otherwise.
   */
  bool wasKeyswitchPressed(byte row, byte col) {
    return false;
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
    return false;
  }

  /**
   * Check the number of key switches pressed in the previous scan.
   *
   * @returns the number of keys pressed.
   */
  uint8_t previousPressedKeyswitchCount() {
    return 0;
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
  void setup() {}

  /**
   * Method to configure the device for a hardware test mode
   *
   * Called by the Kaleidoscope Hardware test plugin, this method should
   * do any device-specific initialization needed for factory hardware testing
   *
   */
  void enableHardwareTestMode() {}

  /** @} */
};
}
