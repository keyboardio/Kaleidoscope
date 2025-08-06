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

#include <Kaleidoscope-Ranges.h>  // for PLOVER_HID_FIRST
#include <stdint.h>               // for uint8_t

#include "kaleidoscope/KeyEvent.h"              // for KeyEvent
#include "kaleidoscope/event_handler_result.h"  // for EventHandlerResult
#include "kaleidoscope/plugin.h"                // for Plugin

// Macro for defining Plover HID keys in keymaps
#define PH(n) Key(kaleidoscope::plugin::plover_hid::n)

namespace kaleidoscope {
namespace plugin {

class PloverHID : public kaleidoscope::Plugin {
 public:
  EventHandlerResult onNameQuery();
  EventHandlerResult onKeyEvent(KeyEvent &event);

 private:
  static uint8_t report_[8];  // 64-bit report data
  static void sendReport();
};

// Plover HID key definitions
// Based on the Plover HID protocol specification
namespace plover_hid {
enum {
  START = kaleidoscope::ranges::PLOVER_HID_FIRST,
  
  // Standard steno keys (first 23 keys)
  S_L = START,    // S- (left)
  T_L,            // T- (left)
  K_L,            // K- (left)
  P_L,            // P- (left)
  W_L,            // W- (left)
  H_L,            // H- (left)
  R_L,            // R- (left)
  A,              // A
  O,              // O
  STAR,           // *
  E,              // -E
  U,              // -U
  F_R,            // -F (right)
  R_R,            // -R (right)
  P_R,            // -P (right)
  B_R,            // -B (right)
  L_R,            // -L (right)
  G_R,            // -G (right)
  T_R,            // -T (right)
  S_R,            // -S (right)
  D_R,            // -D (right)
  Z_R,            // -Z (right)
  NUM,            // # (number)
  
  // Extended keys X1-X26 (24-49)
  X1, X2, X3, X4, X5, X6, X7, X8, X9, X10,
  X11, X12, X13, X14, X15, X16, X17, X18, X19, X20,
  X21, X22, X23, X24, X25, X26,
  
  // Additional extended keys X27-X41 (50-64)
  X27, X28, X29, X30, X31, X32, X33, X34, X35, X36,
  X37, X38, X39, X40, X41,
  
  END = X41,
};
}  // namespace plover_hid

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::PloverHID PloverHID;
