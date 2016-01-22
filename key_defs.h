#include "HIDTables.h"
#include "HIDAliases.h"
typedef struct {
  byte flags;
  byte rawKey;
} Key;



#define KEY_FLAGS       B00000000
#define CTRL_HELD       B00000001
#define ALT_HELD        B00000010
#define SHIFT_HELD      B00000100
#define GUI_HELD        B00001000
#define SWITCH_TO_KEYMAP B00010000
#define MOMENTARY       B00100000
#define MOUSE_KEY       B01000000
#define SYNTHETIC_KEY   B10000000

// we assert that synthetic keys can never have keys held, so we reuse the _HELD bits
#define IS_MACRO       B00000001
#define IS_SYSCTL      B00000010
#define IS_CONSUMER    B00000100
#define IS_INTERNAL    B00001000


// IS_INTERNAL key table:

#define LED_TOGGLE 1


#define MOUSE_UP            B0000001
#define MOUSE_DN            B0000010
#define MOUSE_L             B0000100
#define MOUSE_R             B0001000
#define MOUSE_CENTER        B0010000
#define MOUSE_WARP          B0100000
#define MOUSE_END_WARP      B1000000

#define KEYMAP_0     0
#define KEYMAP_1     1
#define KEYMAP_2     2
#define KEYMAP_3     3
#define KEYMAP_4     4
#define KEYMAP_5     5
#define KEYMAP_6     6
#define KEYMAP_7     7



#define KEYMAP_PREVIOUS  253
#define KEYMAP_NEXT      254


#define Key_NoKey (Key){ KEY_FLAGS,0 }
#define Key_skip (Key){ KEY_FLAGS,0 }

#define Key_macroKey1 (Key){ KEY_FLAGS|SYNTHETIC_KEY|IS_MACRO, 1}


#define Key_mouseWarpNW   (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_WARP| MOUSE_UP | MOUSE_L }
#define Key_mouseWarpNE   (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_WARP| MOUSE_UP | MOUSE_R }
#define Key_mouseWarpSW   (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_WARP| MOUSE_DN | MOUSE_L }
#define Key_mouseWarpSE   (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_WARP| MOUSE_DN | MOUSE_R }
#define Key_mouseWarpEnd   (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_WARP| MOUSE_END_WARP}



#define Key_mouseUpL   (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_UP | MOUSE_L }
#define Key_mouseUp    (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_UP }
#define Key_mouseUpR  (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_UP | MOUSE_R }
#define Key_mouseL    (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_L }
#define Key_mouseR    (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_R }
#define Key_mouseDnL   (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_DN | MOUSE_L  }
#define Key_mouseDn    (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_DN }
#define Key_mouseDnR    (Key){ KEY_FLAGS|MOUSE_KEY, MOUSE_DN | MOUSE_R  }
#define Key_mouseScrollUp
#define Key_mouseScrollDn
#define Key_mouseScrollL
#define Key_mouseScrollR
#define KEY_MOUSE_BTN_L 0x01 // Synthetic key
#define KEY_MOUSE_BTN_M 0x04 // Synthetic key
#define KEY_MOUSE_BTN_R 0x02 // Synthetic key
#define Key_mouseBtnL    (Key){ KEY_FLAGS | SYNTHETIC_KEY, KEY_MOUSE_BTN_L }
#define Key_mouseBtnM    (Key){ KEY_FLAGS | SYNTHETIC_KEY , KEY_MOUSE_BTN_M }
#define Key_mouseBtnR    (Key){ KEY_FLAGS | SYNTHETIC_KEY, KEY_MOUSE_BTN_R }





#define Key_powerDown (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_POWER_DOWN }
#define Key_sleep (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_SLEEP }
#define Key_wakeup (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_WAKEUP }
#define Key_coldRestart (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_COLD_RESTART }
#define Key_warmRestart (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_WARM_RESTART }
#define Key_dock (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_DOCK }
#define Key_undock (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_UNDOCK }
#define Key_speakerMute (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_SPEAKER_MUTE }
#define Key_hibernate (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_HIBERNATE }
#define Key_displayInvert (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_DISPLAY_INVERT }
#define Key_displayInternal (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_DISPLAY_INTERNAL }
#define Key_displayExternal (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_DISPLAY_EXTERNAL }
#define Key_displayBoth (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_DISPLAY_BOTH }
#define Key_displayDual (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_DISPLAY_DUAL }
#define Key_displayToggle (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_DISPLAY_TOGGLE_INT_EXT
#define Key_displaySwap (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,HID_SYSTEM_DISPLAY_SWAP }


#define Key_volumeMute (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, HID_CONSUMER_VOLUME_MUTE}
#define Key_volumeUp (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, HID_CONSUMER_VOLUME_INCREMENT }
#define Key_volumeDown (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, HID_CONSUMER_VOLUME_DECREMENT }
#define Key_playPause (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, HID_CONSUMER_PLAY_SLASH_PAUSE}
#define Key_stop (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, HID_CONSUMER_STOP}
#define Key_prevTrack (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, HID_CONSUMER_SCAN_PREVIOUS_TRACK}
#define Key_nextTrack (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, HID_CONSUMER_SCAN_NEXT_TRACK}
#define Key_Eject (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, HID_CONSUMER_EJECT}

//#define Key_Cut (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_AC_CUT }
//#define Key_Copy (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_AC_COPY}
//#define Key_Paste (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_AC_PASTE}
//#define Key_Undo (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_AC_UNDO }

#define Key_LCtrl (Key){ KEY_FLAGS, HID_KEYBOARD_LEFT_CONTROL }
#define Key_LShift (Key){ KEY_FLAGS, HID_KEYBOARD_LEFT_SHIFT }
#define Key_LAlt (Key){ KEY_FLAGS, HID_KEYBOARD_LEFT_ALT }
#define Key_LGUI (Key){ KEY_FLAGS, HID_KEYBOARD_LEFT_GUI }
#define Key_RCtrl (Key){ KEY_FLAGS, HID_KEYBOARD_RIGHT_CONTROL }
#define Key_RShift (Key){ KEY_FLAGS, HID_KEYBOARD_RIGHT_SHIFT }
#define Key_RAlt (Key){ KEY_FLAGS, HID_KEYBOARD_RIGHT_ALT }
#define Key_RGUI (Key){ KEY_FLAGS, HID_KEYBOARD_RIGHT_GUI }

#define Key_UpArrow (Key){ KEY_FLAGS, HID_KEYBOARD_UP_ARROW }
#define Key_DnArrow (Key){ KEY_FLAGS, HID_KEYBOARD_DOWN_ARROW }
#define Key_LArrow (Key){ KEY_FLAGS, HID_KEYBOARD_LEFT_ARROW  }
#define Key_RArrow (Key){ KEY_FLAGS, HID_KEYBOARD_RIGHT_ARROW}
#define Key_Return (Key){ KEY_FLAGS, KEY_RETURN }
#define Key_Esc (Key){ KEY_FLAGS, KEY_ESC }
#define Key_Backspace (Key){ KEY_FLAGS, KEY_BACKSPACE }
#define Key_Tab (Key){ KEY_FLAGS, KEY_TAB }
#define Key_Insert (Key){ KEY_FLAGS, KEY_INSERT }
#define Key_Delete (Key){ KEY_FLAGS, HID_KEYBOARD_DELETE }
#define Key_PageUp (Key){ KEY_FLAGS, KEY_PGUP }
#define Key_PageDn (Key){ KEY_FLAGS, KEY_PGDN }
#define Key_Home (Key){ KEY_FLAGS, KEY_HOME }
#define Key_End (Key){ KEY_FLAGS, KEY_END }


#define Key_CapsLock (Key){ KEY_FLAGS, KEY_CAPS_LOCK }
#define Key_F1 (Key){ KEY_FLAGS, KEY_F1 }
#define Key_F2 (Key){ KEY_FLAGS, KEY_F2 }
#define Key_F3 (Key){ KEY_FLAGS, KEY_F3 }
#define Key_F4 (Key){ KEY_FLAGS, KEY_F4 }
#define Key_F5 (Key){ KEY_FLAGS, KEY_F5 }
#define Key_F6 (Key){ KEY_FLAGS, KEY_F6 }
#define Key_F7 (Key){ KEY_FLAGS, KEY_F7 }
#define Key_F8 (Key){ KEY_FLAGS, KEY_F8 }
#define Key_F9 (Key){ KEY_FLAGS, KEY_F9 }
#define Key_F10 (Key){ KEY_FLAGS, KEY_F10 }
#define Key_F11 (Key){ KEY_FLAGS, KEY_F11 }
#define Key_F12 (Key){ KEY_FLAGS, KEY_F12 }



#define Key_0 (Key){ KEY_FLAGS, KEY_0 }
#define Key_1 (Key){ KEY_FLAGS, KEY_1 }
#define Key_2 (Key){ KEY_FLAGS, KEY_2 }
#define Key_3 (Key){ KEY_FLAGS, KEY_3 }
#define Key_4 (Key){ KEY_FLAGS, KEY_4 }
#define Key_5 (Key){ KEY_FLAGS, KEY_5 }
#define Key_6 (Key){ KEY_FLAGS, KEY_6 }
#define Key_7 (Key){ KEY_FLAGS, KEY_7 }
#define Key_8 (Key){ KEY_FLAGS, KEY_8 }
#define Key_9 (Key){ KEY_FLAGS, KEY_9 }
#define Key_A (Key){ KEY_FLAGS, KEY_A }
#define Key_B (Key){ KEY_FLAGS, KEY_B }
#define Key_C (Key){ KEY_FLAGS, KEY_C }
#define Key_D (Key){ KEY_FLAGS, KEY_D }
#define Key_E (Key){ KEY_FLAGS, KEY_E }
#define Key_F (Key){ KEY_FLAGS, KEY_F }
#define Key_G (Key){ KEY_FLAGS, KEY_G }
#define Key_H (Key){ KEY_FLAGS, KEY_H }
#define Key_I (Key){ KEY_FLAGS, KEY_I }
#define Key_J (Key){ KEY_FLAGS, KEY_J }
#define Key_K (Key){ KEY_FLAGS, KEY_K }
#define Key_L (Key){ KEY_FLAGS, KEY_L }
#define Key_M (Key){ KEY_FLAGS, KEY_M }
#define Key_N (Key){ KEY_FLAGS, KEY_N }
#define Key_O (Key){ KEY_FLAGS, KEY_O }
#define Key_P (Key){ KEY_FLAGS, KEY_P }
#define Key_Q (Key){ KEY_FLAGS, KEY_Q }
#define Key_R (Key){ KEY_FLAGS, KEY_R }
#define Key_S (Key){ KEY_FLAGS, KEY_S }
#define Key_T (Key){ KEY_FLAGS, KEY_T }
#define Key_U (Key){ KEY_FLAGS, KEY_U }
#define Key_V (Key){ KEY_FLAGS, KEY_V }
#define Key_W (Key){ KEY_FLAGS, KEY_W }
#define Key_X (Key){ KEY_FLAGS, KEY_X }
#define Key_Y (Key){ KEY_FLAGS, KEY_Y }
#define Key_Z (Key){ KEY_FLAGS, KEY_Z }

#define Key_Backtick (Key){ KEY_FLAGS, KEY_GRAVE }
#define Key_Minus (Key){ KEY_FLAGS, KEY_MINUS }
#define Key_Equals (Key){ KEY_FLAGS, KEY_EQUALS }
#define Key_LBracket (Key){ KEY_FLAGS, KEY_LEFT_BRACKET }
#define Key_RBracket (Key){ KEY_FLAGS, KEY_RIGHT_BRACKET }
#define Key_Backslash (Key){ KEY_FLAGS, KEY_BACKSLASH }
#define Key_Pipe (Key){ KEY_FLAGS|SHIFT_HELD, HID_KEYBOARD_BACKSLASH_AND_PIPE }
#define Key_LSquareBracket (Key){ KEY_FLAGS, KEY_L_BRACKET }
#define Key_RSquareBracket (Key){ KEY_FLAGS, KEY_R_BRACKET }
#define Key_LCurlyBracket (Key){ KEY_FLAGS|SHIFT_HELD, KEY_L_BRACKET }
#define Key_RCurlyBracket (Key){ KEY_FLAGS|SHIFT_HELD, KEY_R_BRACKET }
#define Key_Semicolon (Key){ KEY_FLAGS, KEY_SEMICOLON }
#define Key_Quote (Key){ KEY_FLAGS, KEY_QUOTE }
#define Key_Comma (Key){ KEY_FLAGS, KEY_COMMA }
#define Key_Period (Key){ KEY_FLAGS, KEY_PERIOD }
#define Key_Space (Key){ KEY_FLAGS, KEY_SPACE }
#define Key_Slash (Key){ KEY_FLAGS, KEY_SLASH }

#define Key_LEFT_PAREN (Key) { KEY_FLAGS|SHIFT_HELD, KEY_9 }
#define Key_RIGHT_PAREN (Key){ KEY_FLAGS|SHIFT_HELD, KEY_0 }


#define Key_KeypadClear (Key){ KEY_FLAGS, HID_KEYPAD_CLEAR }
#define Key_KeypadSlash (Key){ KEY_FLAGS, HID_KEYPAD_DIVIDE }
#define Key_KeypadMultiply (Key){ KEY_FLAGS, HID_KEYPAD_MULTIPLY }
#define Key_KeypadMinus (Key){ KEY_FLAGS, HID_KEYPAD_SUBTRACT }
#define Key_KeypadPlus (Key){ KEY_FLAGS, HID_KEYPAD_ADD }
#define Key_Enter (Key){ KEY_FLAGS, HID_KEYBOARD_ENTER }
#define Key_Keypad1 (Key){ KEY_FLAGS, HID_KEYPAD_1_AND_END }
#define Key_Keypad2 (Key){ KEY_FLAGS, HID_KEYPAD_2_AND_DOWN_ARROW}
#define Key_Keypad3 (Key){ KEY_FLAGS, HID_KEYPAD_3_AND_PAGE_DOWN}
#define Key_Keypad4 (Key){ KEY_FLAGS, HID_KEYPAD_4_AND_LEFT_ARROW }
#define Key_Keypad5 (Key){ KEY_FLAGS, HID_KEYPAD_5 }
#define Key_Keypad6 (Key){ KEY_FLAGS, HID_KEYPAD_6_AND_RIGHT_ARROW }
#define Key_Keypad7 (Key){ KEY_FLAGS, HID_KEYPAD_7_AND_HOME }
#define Key_Keypad8 (Key){ KEY_FLAGS, HID_KEYPAD_8_AND_UP_ARROW }
#define Key_Keypad9 (Key){ KEY_FLAGS, HID_KEYPAD_9_AND_PAGE_UP }
#define Key_Keypad0 (Key){ KEY_FLAGS, HID_KEYPAD_0_AND_INSERT }
#define Key_KeypadDot (Key){ KEY_FLAGS, HID_KEYPAD_PERIOD_AND_DELETE }
#define Key_DanishDollar (Key){ KEY_FLAGS, KEY_DANISH_DOLLAR }
#define Key_PcApplciation (Key){ KEY_FLAGS, KEY_PC_APPLCIATION }
#define Key_F13 (Key){ KEY_FLAGS, KEY_F13 }
#define Key_F16 (Key){ KEY_FLAGS, KEY_F16 }
#define Key_F17 (Key){ KEY_FLAGS, KEY_F17 }
#define Key_F18 (Key){ KEY_FLAGS, KEY_F18 }
#define Key_F19 (Key){ KEY_FLAGS, KEY_F19 }
#define Key_Help (Key){ KEY_FLAGS, KEY_HELP }
#define KEY_BACKLIGHT_DOWN 0xF1
#define KEY_BACKLIGHT_UP 0xF2
#define Key_BacklightDn (Key){ KEY_FLAGS, KEY_BACKLIGHT_DOWN }
#define Key_BacklightUp (Key){ KEY_FLAGS, KEY_BACKLIGHT_UP }
#define KEY_RIGHT_FN2 0xfe
#define Key_RFN2 (Key){ KEY_FLAGS, KEY_RIGHT_FN2 }
#define KEY_LEFT_FN2 0xff
#define Key_LFN2 (Key){ KEY_FLAGS, KEY_LEFT_FN2 }

#define Key_Undo (Key){ KEY_FLAGS, KEY_UNDO }


#define META_NEXT_KEYMAP_MOMENTARY 0xFF
#define Key_Keymap0 (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP , KEYMAP_0 }
#define Key_Keymap1 (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP , KEYMAP_1 }
#define Key_Keymap2 (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP , KEYMAP_2 }
#define Key_Keymap3 (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP , KEYMAP_3 }
#define Key_Keymap4 (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP , KEYMAP_4 }
#define Key_Keymap5 (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP , KEYMAP_5 }
#define Key_Keymap0_Momentary (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP | MOMENTARY, KEYMAP_0 }
#define Key_Keymap1_Momentary (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP | MOMENTARY, KEYMAP_1 }
#define Key_Keymap2_Momentary (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP | MOMENTARY, KEYMAP_2 }
#define Key_Keymap3_Momentary (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP | MOMENTARY, KEYMAP_3 }
#define Key_Keymap4_Momentary (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP | MOMENTARY, KEYMAP_4 }
#define Key_Keymap5_Momentary (Key){ KEY_FLAGS | SWITCH_TO_KEYMAP | MOMENTARY, KEYMAP_5 }

#define Key_KeymapNext_Momentary (Key) {KEY_FLAGS | SWITCH_TO_KEYMAP | MOMENTARY, KEYMAP_NEXT }
#define Key_KeymapPrevious_Momentary (Key) {KEY_FLAGS | SWITCH_TO_KEYMAP | MOMENTARY, KEYMAP_PREVIOUS }




#define Key_LEDEffectNext (Key) { KEY_FLAGS | SYNTHETIC_KEY | IS_INTERNAL, LED_TOGGLE }


