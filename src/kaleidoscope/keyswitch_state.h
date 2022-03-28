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
// switch debouncing and status

#define INJECTED    0b10000000
#define IS_PRESSED  0b00000010
#define WAS_PRESSED 0b00000001

/* keyIsPressed(): This is true if the key is pressed during this scan cycle.
 * This will be true for several consecutive cycles even for a single tap of
 *   the key.
 * Use this for events which should fire every scan cycle the key is held.
 * If you want an event which fires only once when a key is pressed, use
 *   keyToggledOn() or keyToggledOff() (defined below).
 */
#define keyIsPressed(keyState) ((keyState)&IS_PRESSED)

/* keyWasPressed(): This is true if the key was pressed during the previous
     scan cycle, regardless of whether it is pressed or not in this scan cycle.
 */
#define keyWasPressed(keyState) ((keyState)&WAS_PRESSED)

/* keyToggledOn(): This is true if the key is newly pressed during this scan
 *   cycle, i.e. was not pressed in the previous scan cycle but is now.
 * Use this for events which should fire exactly once per keypress, on a
 *   "key-down" event.
 */
#define keyToggledOn(keyState) (keyIsPressed(keyState) && !keyWasPressed(keyState))

/* keyToggledOff(): This is true if the key is newly not-pressed during this
 *   scan cycle, i.e. is not pressed now but was in the previous scan cycle.
 * Use this for events which should fire exactly once per keypress, on a
 *   "key-up" event.
 */
#define keyToggledOff(keyState) (keyWasPressed(keyState) && !keyIsPressed(keyState))

/* keyIsInjected(): This is true if the key was marked as injected by another
 * plugin, i.e. it was generated artificially instead of corresponding to a
 * "real" keypress.
 */
#define keyIsInjected(keyState) ((keyState)&INJECTED)
