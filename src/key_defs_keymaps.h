#pragma once

#define MOMENTARY_OFFSET 42

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
#define Key_Keymap0_Momentary (Key){ KEYMAP_0 + MOMENTARY_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_Keymap1_Momentary (Key){ KEYMAP_1 + MOMENTARY_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_Keymap2_Momentary (Key){ KEYMAP_2 + MOMENTARY_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_Keymap3_Momentary (Key){ KEYMAP_3 + MOMENTARY_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_Keymap4_Momentary (Key){ KEYMAP_4 + MOMENTARY_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_Keymap5_Momentary (Key){ KEYMAP_5 + MOMENTARY_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }

#define Key_KeymapNext_Momentary (Key) { KEYMAP_NEXT + MOMENTARY_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
#define Key_KeymapPrevious_Momentary (Key) { KEYMAP_PREVIOUS + MOMENTARY_OFFSET, KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP }
