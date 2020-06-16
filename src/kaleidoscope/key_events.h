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

#include "kaleidoscope/driver/keyscanner/Base.h"
#include "kaleidoscope/device/device.h"
#include "kaleidoscope/key_defs.h"
#include "kaleidoscope/keyswitch_state.h"
#include "kaleidoscope/KeyAddr.h"

// UnknownKeyswitchLocation represents an invalid (as default constructed)
// key address. Note: This is not a constexpr as it turned out
// that the compiler would instanciate it and store it in RAM if
// not made a temporary.
//
#define UnknownKeyswitchLocation KeyAddr(KeyAddr::invalid_state)

// Conversely, if an injected event *is* tied to a physical keyswitch and should
// be resolved by the current keymap, code can use Key_NoKey on the injected event
// with a real key address

// sending events to the computer
/* The event handling starts with the Scanner calling handleKeyswitchEvent() for
 * every non-idle key in the matrix, and it is the task of this method to figure
 * out what to do, it is the main entry point. Keys that were off in the
 * previous scan cycle, and are still off now, are considered idle, and
 * handleKeyswitchEvent() is *not* called on them.
 *
 * This function will iterate through an array of handler functions, and stop as
 * soon as one of them signals that the event has been handled. To make it
 * possible to inject synthetic events, one can call handleKeyswitchEvent from
 * within a custom handler (making the event handling recursive), with a
 * different keycode.
 *
 * This is useful for example for one-shot modifiers, where we would like to
 * temporarily disable the one-shot functionality, and have them work as a
 * normal modifier instead. In this case, the keymap would contain a key with
 * OSM flags set, and the event handler would remove the OSM flags, and let the
 * system handle the key as it would have, without the OSM flags. So we simply
 * clear the flags, and call handleKeyswitchEvent again, with the modifier keycode
 * as the first argument. This way, we could insert an event, and have the whole
 * chain re-process it, instead of registering the keycode ourselves with HID
 * ourselves. Injecting allows any and all custom handlers to have a chance,
 * too.
 *
 * For this reason, the handleKeyswitchEvent receives four arguments: the mapped key
 * (or Key_NoKey if we do not want to override what is in the keymap), the matrix
 * address of the key, so we can look up the code for it, and the current and
 * previous state of the key, so we can determine what the event is. The
 * currentState may be flagged INJECTED, which signals that the event was
 * injected, and is not a direct result of a keypress, coming from the scanner.
 */
void handleKeyswitchEvent(Key mappedKey, kaleidoscope::Device::Props::KeyScannerProps::KeyAddr key_addr, uint8_t keyState);
