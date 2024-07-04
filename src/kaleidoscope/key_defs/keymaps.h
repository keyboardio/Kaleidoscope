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

#include <stdint.h>  // for uint8_t


// Layer lock keys
#define Key_Keymap0 Key(KEYMAP_0, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define Key_Keymap1 Key(KEYMAP_1, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define Key_Keymap2 Key(KEYMAP_2, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define Key_Keymap3 Key(KEYMAP_3, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define Key_Keymap4 Key(KEYMAP_4, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define Key_Keymap5 Key(KEYMAP_5, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)

// Layer shift keys
#define Key_Keymap0_Momentary Key(KEYMAP_0 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define Key_Keymap1_Momentary Key(KEYMAP_1 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define Key_Keymap2_Momentary Key(KEYMAP_2 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define Key_Keymap3_Momentary Key(KEYMAP_3 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define Key_Keymap4_Momentary Key(KEYMAP_4 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define Key_Keymap5_Momentary Key(KEYMAP_5 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)

// Next/previous layer shift keys
#define Key_KeymapNext_Momentary     Key(KEYMAP_NEXT + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define Key_KeymapPrevious_Momentary Key(KEYMAP_PREVIOUS + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)


/** Lock/Unlock layer `n`.
 *
 * When locking a layer, it will remain active until unlocked explicitly. `n`
 * can be a number, or an enum value declared previously.
 */
#define LockLayer(n)   Key(n, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)
#define UnlockLayer(n) LockLayer(n)

/** Temporarily shift to layer `n`.
 *
 * Shifts to layer `n` for as long as the key is held. When the key is
 * released, the layer shifts back too. `n` can be a number, or an enum
 * value declared previously.
 */
#define ShiftToLayer(n) Key(n + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)

/** Move to layer `n`.
 *
 * Moving to a layer disables all other layers. Unlike locking and shifting to,
 * this is a one-way operation.
 */
#define MoveToLayer(n) Key(n + LAYER_MOVE_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP)

// Shorthand for keymap entry (e.g. `ML(LeftAlt, 3)`)
#define ML(mod, layer) kaleidoscope::modLayerKey(Key_##mod, layer)
