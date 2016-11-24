#pragma once

#include "key_defs.h"

#define IS_MOUSE_KEY     		   B00010000

// Synthetic, not internal
#define KEY_MOUSE_BTN_L 0x01 // Synthetic key
#define KEY_MOUSE_BTN_M 0x02 // Synthetic key
#define KEY_MOUSE_BTN_R 0x04 // Synthetic key


#define KEY_MOUSE_UP            B0000001
#define KEY_MOUSE_DOWN          B0000010
#define KEY_MOUSE_LEFT          B0000100
#define KEY_MOUSE_RIGHT         B0001000
#define KEY_MOUSE_CENTER        B0010000
#define KEY_MOUSE_WARP          B0100000
#define KEY_MOUSE_WARP_END      B1000000


#define Key_mouseWarpNW   (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_WARP| KEY_MOUSE_UP | KEY_MOUSE_LEFT }
#define Key_mouseWarpNE   (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_WARP| KEY_MOUSE_UP | KEY_MOUSE_RIGHT }
#define Key_mouseWarpSW   (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_WARP| KEY_MOUSE_DOWN | KEY_MOUSE_LEFT }
#define Key_mouseWarpSE   (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_WARP| KEY_MOUSE_DOWN | KEY_MOUSE_RIGHT }
#define Key_mouseWarpEnd   (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_WARP| KEY_MOUSE_WARP_END}


#define Key_mouseUpL   (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_UP | KEY_MOUSE_LEFT }
#define Key_mouseUp    (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_UP }
#define Key_mouseUpR  (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_UP | KEY_MOUSE_RIGHT }
#define Key_mouseL    (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_LEFT }
#define Key_mouseR    (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_RIGHT }
#define Key_mouseDnL   (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_DOWN | KEY_MOUSE_LEFT  }
#define Key_mouseDn    (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_DOWN }
#define Key_mouseDnR    (Key){ KEY_FLAGS|SYNTHETIC|IS_MOUSE_KEY, KEY_MOUSE_DOWN | KEY_MOUSE_RIGHT  }
#define Key_mouseScrollUp
#define Key_mouseScrollDn
#define Key_mouseScrollL
#define Key_mouseScrollR
#define Key_mouseBtnL    (Key){ KEY_FLAGS | SYNTHETIC, KEY_MOUSE_BTN_L }
#define Key_mouseBtnM    (Key){ KEY_FLAGS | SYNTHETIC, KEY_MOUSE_BTN_M }
#define Key_mouseBtnR    (Key){ KEY_FLAGS | SYNTHETIC, KEY_MOUSE_BTN_R }
