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

#include "kaleidoscope/plugin/PloverHID.h"

#include <Arduino.h>                   // for F, __FlashStringHelper
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial
#include <stdint.h>                    // for uint8_t
#include <string.h>                    // for memset

// For HID report ID and HID interface
#include "HID-Settings.h"
#include "HID.h"
#include "MultiReport/PloverHID.h"

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/Runtime.h"               // for Runtime, Runtime_
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/key_defs.h"              // for Key
#include "kaleidoscope/keyswitch_state.h"       // for keyToggledOn, keyToggledOff

namespace kaleidoscope {
namespace plugin {

uint8_t PloverHID::report_[8];

EventHandlerResult PloverHID::onNameQuery() {
  return ::Focus.sendName(F("PloverHID"));
}

EventHandlerResult PloverHID::onKeyEvent(KeyEvent &event) {
  // Check if this is a Plover HID key
  if (event.key < plover_hid::START || event.key > plover_hid::END)
    return EventHandlerResult::OK;

  // Calculate bit position (0-63)
  uint8_t key_index = event.key.getRaw() - plover_hid::START;
  uint8_t byte_index = key_index / 8;
  uint8_t bit_index = key_index % 8;

  // Update the report based on key state
  if (keyToggledOn(event.state)) {
    // Set the bit for key press
    report_[byte_index] |= (1 << bit_index);
  } else if (keyToggledOff(event.state)) {
    // Clear the bit for key release
    report_[byte_index] &= ~(1 << bit_index);
  }

  // Send the report immediately (real-time key state changes)
  sendReport();

  return EventHandlerResult::EVENT_CONSUMED;
}

void PloverHID::sendReport() {
  // Ensure the HID descriptor is registered
  static bool initialized = false;
  if (!initialized) {
    (void)::PloverHID;  // Force instantiation to register descriptor
    initialized = true;
  }

  HID().SendReport(HID_REPORTID_PLOVER_HID, report_, sizeof(report_));
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::PloverHID PloverHID;
