#pragma once

static const uint8_t MOMENTARY_OFFSET __attribute__((deprecated)) = 42;
static const uint8_t LAYER_SHIFT_OFFSET = 42;

#define KEYMAP_0     0
#define KEYMAP_1     1
#define KEYMAP_2     2
#define KEYMAP_3     3
#define KEYMAP_4     4
#define KEYMAP_5     5
#define KEYMAP_6     6
#define KEYMAP_7     7


#define KEYMAP_PREVIOUS  33
#define KEYMAP_NEXT      34


#define Key_Keymap0 (Key) { KEYMAP_0,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap1 (Key) { KEYMAP_1,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap2 (Key) { KEYMAP_2,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap3 (Key) { KEYMAP_3,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap4 (Key) { KEYMAP_4,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap5 (Key) { KEYMAP_5,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap0_Momentary (Key){ KEYMAP_0 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_Keymap1_Momentary (Key){ KEYMAP_1 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_Keymap2_Momentary (Key){ KEYMAP_2 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_Keymap3_Momentary (Key){ KEYMAP_3 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_Keymap4_Momentary (Key){ KEYMAP_4 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_Keymap5_Momentary (Key){ KEYMAP_5 + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }

#define Key_KeymapNext_Momentary (Key) { KEYMAP_NEXT + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_KeymapPrevious_Momentary (Key) { KEYMAP_PREVIOUS + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }


/** Lock/Unlock layer `n`.
 *
 * When locking a layer, it will remain active until unlocked explicitly. `n`
 * can be a number, or an enum value declared previously.
 */
#define LockLayer(n) (Key){ n, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define UnlockLayer(n) LockLayer(n)

/** Temporarily shift to layer `n`.
 *
 * Shifts to layer `n` for as long as the key is held. When the key is
 * released, the layer shifts back too. `n` can be a number, or an enum
 * value declared previously.
 */
#define ShiftToLayer(n) (Key){ n + LAYER_SHIFT_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
