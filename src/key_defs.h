#pragma once


#include "HIDTables.h"
typedef union Key_ {

    struct {
        uint8_t keyCode;
        uint8_t flags;
    };
    uint16_t raw;

    inline bool operator==(uint16_t rhs) { return this->raw == rhs; };
    inline bool operator==(const Key_ rhs) { return this->raw == rhs.raw; };
    inline Key_& operator=(uint16_t raw) { this->raw = raw; return *this; };
    inline bool operator!=(const Key_& rhs) { return !(*this == rhs); };
    inline bool operator>=(uint16_t raw) { return this->raw >= raw; };
    inline bool operator<=(uint16_t raw) { return this->raw <= raw; };
    inline bool operator>(uint16_t raw) { return this->raw > raw; };
    inline bool operator<(uint16_t raw) { return this->raw < raw; };
    inline bool operator>=(const Key_& other) { return this->raw >= other.raw; };
    inline bool operator<=(const Key_& other) { return this->raw <= other.raw; };
    inline bool operator>(const Key_& other) { return this->raw > other.raw; };
    inline bool operator<(const Key_& other) { return this->raw < other.raw; };
} Key;



#define KEY_FLAGS         B00000000
#define CTRL_HELD         B00000001
#define LALT_HELD         B00000010
#define RALT_HELD         B00000100
#define SHIFT_HELD        B00001000
#define GUI_HELD          B00010000
#define SYNTHETIC         B01000000
#define RESERVED          B10000000

#define LCTRL(k)  ((Key) { k.keyCode, k.flags | CTRL_HELD })
#define LALT(k)   ((Key) { k.keyCode, k.flags | LALT_HELD })
#define RALT(k)   ((Key) { k.keyCode, k.flags | RALT_HELD })
#define LSHIFT(k) ((Key) { k.keyCode, k.flags | SHIFT_HELD })
#define LGUI(k)   ((Key) { k.keyCode, k.flags | GUI_HELD })

// we assert that synthetic keys can never have keys held, so we reuse the _HELD bits
#define IS_SYSCTL                  B00000001
#define IS_CONSUMER                B00000010
#define SWITCH_TO_KEYMAP 		       B00000100
#define IS_INTERNAL                B00001000

#define MOMENTARY_OFFSET 42

// IS_INTERNAL key table:

#define LED_TOGGLE   B00000001 // Synthetic, internal


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


#define Key_NoKey (Key) { 0,  KEY_FLAGS }
#define Key_skip (Key) { 0,  KEY_FLAGS }
#define Key_Transparent (Key){ .raw = 0xffff }
#define ___ Key_Transparent
#define XXX Key_NoKey


#define Key_powerDown (Key) { HID_SYSTEM_POWER_DOWN, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_sleep (Key) { HID_SYSTEM_SLEEP, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_wakeup (Key) { HID_SYSTEM_WAKEUP, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_coldRestart (Key) { HID_SYSTEM_COLD_RESTART, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_warmRestart (Key) { HID_SYSTEM_WARM_RESTART, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_dock (Key) { HID_SYSTEM_DOCK, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_undock (Key) { HID_SYSTEM_UNDOCK, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_speakerMute (Key) { HID_SYSTEM_SPEAKER_MUTE, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_hibernate (Key) { HID_SYSTEM_HIBERNATE, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_displayInvert (Key) { HID_SYSTEM_DISPLAY_INVERT, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_displayInternal (Key) { HID_SYSTEM_DISPLAY_INTERNAL, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_displayExternal (Key) { HID_SYSTEM_DISPLAY_EXTERNAL, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_displayBoth (Key) { HID_SYSTEM_DISPLAY_BOTH, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_displayDual (Key) { HID_SYSTEM_DISPLAY_DUAL, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }
#define Key_displayToggle (Key) {KEY_FLAGS | SYNTHETIC|IS_SYSCTL,HID_SYSTEM_DISPLAY_TOGGLE_INT_EXT
#define Key_displaySwap (Key) { HID_SYSTEM_DISPLAY_SWAP, KEY_FLAGS | SYNTHETIC|IS_SYSCTL }


#define Key_volumeMute (Key) { HID_CONSUMER_MUTE, KEY_FLAGS | SYNTHETIC|IS_CONSUMER }
#define Key_volumeUp (Key) { HID_CONSUMER_VOLUME_INCREMENT, KEY_FLAGS | SYNTHETIC|IS_CONSUMER }
#define Key_volumeDown (Key) { HID_CONSUMER_VOLUME_DECREMENT, KEY_FLAGS | SYNTHETIC|IS_CONSUMER }
#define Key_playPause (Key) { HID_CONSUMER_PLAY_SLASH_PAUSE, KEY_FLAGS | SYNTHETIC|IS_CONSUMER }
#define Key_stop (Key) { HID_CONSUMER_STOP, KEY_FLAGS | SYNTHETIC|IS_CONSUMER }
#define Key_prevTrack (Key) { HID_CONSUMER_SCAN_PREVIOUS_TRACK, KEY_FLAGS | SYNTHETIC|IS_CONSUMER }
#define Key_nextTrack (Key) { HID_CONSUMER_SCAN_NEXT_TRACK, KEY_FLAGS | SYNTHETIC|IS_CONSUMER }
#define Key_Eject (Key) { HID_CONSUMER_EJECT, KEY_FLAGS | SYNTHETIC|IS_CONSUMER }


#define Key_LCtrl (Key) { HID_KEYBOARD_LEFT_CONTROL,  KEY_FLAGS }
#define Key_LShift (Key) { HID_KEYBOARD_LEFT_SHIFT,  KEY_FLAGS }
#define Key_LAlt (Key) { HID_KEYBOARD_LEFT_ALT,  KEY_FLAGS }
#define Key_LGUI (Key) { HID_KEYBOARD_LEFT_GUI,  KEY_FLAGS }
#define Key_RCtrl (Key) { HID_KEYBOARD_RIGHT_CONTROL,  KEY_FLAGS }
#define Key_RShift (Key) { HID_KEYBOARD_RIGHT_SHIFT,  KEY_FLAGS }
#define Key_RAlt (Key) { HID_KEYBOARD_RIGHT_ALT,  KEY_FLAGS }
#define Key_RGUI (Key) { HID_KEYBOARD_RIGHT_GUI,  KEY_FLAGS }

#define Key_UpArrow (Key) { HID_KEYBOARD_UP_ARROW,  KEY_FLAGS }
#define Key_DnArrow (Key) { HID_KEYBOARD_DOWN_ARROW,  KEY_FLAGS }
#define Key_LArrow (Key) { HID_KEYBOARD_LEFT_ARROW,  KEY_FLAGS }
#define Key_RArrow (Key) { HID_KEYBOARD_RIGHT_ARROW,  KEY_FLAGS }
#define Key_Return (Key) { HID_KEYBOARD_RETURN,  KEY_FLAGS }
#define Key_Esc (Key) { HID_KEYBOARD_ESCAPE,  KEY_FLAGS }
#define Key_Backspace (Key) { HID_KEYBOARD_DELETE,  KEY_FLAGS }
#define Key_Tab (Key) { HID_KEYBOARD_TAB,  KEY_FLAGS }
#define Key_Insert (Key) { HID_KEYBOARD_INSERT,  KEY_FLAGS }
#define Key_Delete (Key) { HID_KEYBOARD_DELETE_FORWARD,  KEY_FLAGS }
#define Key_PageUp (Key) { HID_KEYBOARD_PAGE_UP,  KEY_FLAGS }
#define Key_PageDn (Key) { HID_KEYBOARD_PAGE_DOWN,  KEY_FLAGS }
#define Key_Home (Key) { HID_KEYBOARD_HOME,  KEY_FLAGS }
#define Key_End (Key) { HID_KEYBOARD_END,  KEY_FLAGS }


#define Key_CapsLock (Key) { HID_KEYBOARD_CAPS_LOCK,  KEY_FLAGS }
#define Key_NumLock (Key) { HID_KEYPAD_NUM_LOCK_AND_CLEAR, KEY_FLAGS }

#define Key_F1 (Key) { HID_KEYBOARD_F1,  KEY_FLAGS }
#define Key_F2 (Key) { HID_KEYBOARD_F2,  KEY_FLAGS }
#define Key_F3 (Key) { HID_KEYBOARD_F3,  KEY_FLAGS }
#define Key_F4 (Key) { HID_KEYBOARD_F4,  KEY_FLAGS }
#define Key_F5 (Key) { HID_KEYBOARD_F5,  KEY_FLAGS }
#define Key_F6 (Key) { HID_KEYBOARD_F6,  KEY_FLAGS }
#define Key_F7 (Key) { HID_KEYBOARD_F7,  KEY_FLAGS }
#define Key_F8 (Key) { HID_KEYBOARD_F8,  KEY_FLAGS }
#define Key_F9 (Key) { HID_KEYBOARD_F9,  KEY_FLAGS }
#define Key_F10 (Key) { HID_KEYBOARD_F10,  KEY_FLAGS }
#define Key_F11 (Key) { HID_KEYBOARD_F11,  KEY_FLAGS }
#define Key_F12 (Key) { HID_KEYBOARD_F12,  KEY_FLAGS }
#define Key_F13 (Key) { HID_KEYBOARD_F13,  KEY_FLAGS }
#define Key_F14 (Key) { HID_KEYBOARD_F14,  KEY_FLAGS }
#define Key_F15 (Key) { HID_KEYBOARD_F15,  KEY_FLAGS }
#define Key_F16 (Key) { HID_KEYBOARD_F16,  KEY_FLAGS }
#define Key_F17 (Key) { HID_KEYBOARD_F17,  KEY_FLAGS }
#define Key_F18 (Key) { HID_KEYBOARD_F18,  KEY_FLAGS }
#define Key_F19 (Key) { HID_KEYBOARD_F19,  KEY_FLAGS }
#define Key_F20 (Key) { HID_KEYBOARD_F20,  KEY_FLAGS }
#define Key_F21 (Key) { HID_KEYBOARD_F21,  KEY_FLAGS }
#define Key_F22 (Key) { HID_KEYBOARD_F22,  KEY_FLAGS }
#define Key_F23 (Key) { HID_KEYBOARD_F23,  KEY_FLAGS }
#define Key_F24 (Key) { HID_KEYBOARD_F24,  KEY_FLAGS }




#define Key_1 (Key) { HID_KEYBOARD_1_AND_EXCLAMATION_POINT,  KEY_FLAGS }
#define Key_2 (Key) { HID_KEYBOARD_2_AND_AT,  KEY_FLAGS }
#define Key_3 (Key) { HID_KEYBOARD_3_AND_POUND,  KEY_FLAGS }
#define Key_4 (Key) { HID_KEYBOARD_4_AND_DOLLAR,  KEY_FLAGS }
#define Key_5 (Key) { HID_KEYBOARD_5_AND_PERCENT,  KEY_FLAGS }
#define Key_6 (Key) { HID_KEYBOARD_6_AND_CARAT,  KEY_FLAGS }
#define Key_7 (Key) { HID_KEYBOARD_7_AND_AMPERSAND,  KEY_FLAGS }
#define Key_8 (Key) { HID_KEYBOARD_8_AND_ASTERISK,  KEY_FLAGS }
#define Key_9 (Key) { HID_KEYBOARD_9_AND_LEFT_PAREN,  KEY_FLAGS }
#define Key_0 (Key) { HID_KEYBOARD_0_AND_RIGHT_PAREN,  KEY_FLAGS }
#define Key_A (Key) { HID_KEYBOARD_A_AND_A,  KEY_FLAGS }
#define Key_B (Key) { HID_KEYBOARD_B_AND_B,  KEY_FLAGS }
#define Key_C (Key) { HID_KEYBOARD_C_AND_C,  KEY_FLAGS }
#define Key_D (Key) { HID_KEYBOARD_D_AND_D,  KEY_FLAGS }
#define Key_E (Key) { HID_KEYBOARD_E_AND_E,  KEY_FLAGS }
#define Key_F (Key) { HID_KEYBOARD_F_AND_F,  KEY_FLAGS }
#define Key_G (Key) { HID_KEYBOARD_G_AND_G,  KEY_FLAGS }
#define Key_H (Key) { HID_KEYBOARD_H_AND_H,  KEY_FLAGS }
#define Key_I (Key) { HID_KEYBOARD_I_AND_I,  KEY_FLAGS }
#define Key_J (Key) { HID_KEYBOARD_J_AND_J,  KEY_FLAGS }
#define Key_K (Key) { HID_KEYBOARD_K_AND_K,  KEY_FLAGS }
#define Key_L (Key) { HID_KEYBOARD_L_AND_L,  KEY_FLAGS }
#define Key_M (Key) { HID_KEYBOARD_M_AND_M,  KEY_FLAGS }
#define Key_N (Key) { HID_KEYBOARD_N_AND_N,  KEY_FLAGS }
#define Key_O (Key) { HID_KEYBOARD_O_AND_O,  KEY_FLAGS }
#define Key_P (Key) { HID_KEYBOARD_P_AND_P,  KEY_FLAGS }
#define Key_Q (Key) { HID_KEYBOARD_Q_AND_Q,  KEY_FLAGS }
#define Key_R (Key) { HID_KEYBOARD_R_AND_R,  KEY_FLAGS }
#define Key_S (Key) { HID_KEYBOARD_S_AND_S,  KEY_FLAGS }
#define Key_T (Key) { HID_KEYBOARD_T_AND_T,  KEY_FLAGS }
#define Key_U (Key) { HID_KEYBOARD_U_AND_U,  KEY_FLAGS }
#define Key_V (Key) { HID_KEYBOARD_V_AND_V,  KEY_FLAGS }
#define Key_W (Key) { HID_KEYBOARD_W_AND_W,  KEY_FLAGS }
#define Key_X (Key) { HID_KEYBOARD_X_AND_X,  KEY_FLAGS }
#define Key_Y (Key) { HID_KEYBOARD_Y_AND_Y,  KEY_FLAGS }
#define Key_Z (Key) { HID_KEYBOARD_Z_AND_Z,  KEY_FLAGS }

#define Key_Backtick (Key) { HID_KEYBOARD_GRAVE_ACCENT_AND_TILDE,  KEY_FLAGS }
#define Key_Minus (Key) { HID_KEYBOARD_MINUS_AND_UNDERSCORE,  KEY_FLAGS }
#define Key_Equals (Key) { HID_KEYBOARD_EQUALS_AND_PLUS,  KEY_FLAGS }
#define Key_LBracket (Key) { HID_KEYBOARD_LEFT_BRACKET_AND_LEFT_CURLY_BRACE,  KEY_FLAGS }
#define Key_RBracket (Key) { HID_KEYBOARD_RIGHT_BRACKET_AND_RIGHT_CURLY_BRACE,  KEY_FLAGS }
#define Key_Backslash (Key) { HID_KEYBOARD_BACKSLASH_AND_PIPE,  KEY_FLAGS }
#define Key_Pipe LSHIFT(Key_Backslash)
#define Key_LSquareBracket (Key) { HID_KEYBOARD_LEFT_BRACKET_AND_LEFT_CURLY_BRACE,  KEY_FLAGS }
#define Key_RSquareBracket (Key) { HID_KEYBOARD_RIGHT_BRACKET_AND_RIGHT_CURLY_BRACE,  KEY_FLAGS }
#define Key_LCurlyBracket LSHIFT(Key_LSquareBracket)
#define Key_RCurlyBracket LSHIFT(Key_RSquareBracket)
#define Key_Semicolon (Key) { HID_KEYBOARD_SEMICOLON_AND_COLON,  KEY_FLAGS }
#define Key_Quote (Key) { HID_KEYBOARD_QUOTE_AND_DOUBLEQUOTE,  KEY_FLAGS }
#define Key_Comma (Key) { HID_KEYBOARD_COMMA_AND_LESS_THAN,  KEY_FLAGS }
#define Key_Period (Key) { HID_KEYBOARD_PERIOD_AND_GREATER_THAN,  KEY_FLAGS }
#define Key_Space (Key) { HID_KEYBOARD_SPACEBAR,  KEY_FLAGS }
#define Key_Slash (Key) { HID_KEYBOARD_SLASH_AND_QUESTION_MARK,  KEY_FLAGS }

#define Key_LEFT_PAREN LSHIFT(Key_9)
#define Key_RIGHT_PAREN LSHIFT(Key_0)


#define Key_KeypadClear (Key) { HID_KEYPAD_CLEAR,  KEY_FLAGS }
#define Key_KeypadSlash (Key) { HID_KEYPAD_DIVIDE,  KEY_FLAGS }
#define Key_KeypadMultiply (Key) { HID_KEYPAD_MULTIPLY,  KEY_FLAGS }
#define Key_KeypadMinus (Key) { HID_KEYPAD_SUBTRACT,  KEY_FLAGS }
#define Key_KeypadPlus (Key) { HID_KEYPAD_ADD,  KEY_FLAGS }
#define Key_Enter (Key) { HID_KEYBOARD_ENTER,  KEY_FLAGS }
#define Key_Keypad1 (Key) { HID_KEYPAD_1_AND_END,  KEY_FLAGS }
#define Key_Keypad2 (Key) { HID_KEYPAD_2_AND_DOWN_ARROW,  KEY_FLAGS }
#define Key_Keypad3 (Key) { HID_KEYPAD_3_AND_PAGE_DOWN,  KEY_FLAGS }
#define Key_Keypad4 (Key) { HID_KEYPAD_4_AND_LEFT_ARROW,  KEY_FLAGS }
#define Key_Keypad5 (Key) { HID_KEYPAD_5,  KEY_FLAGS }
#define Key_Keypad6 (Key) { HID_KEYPAD_6_AND_RIGHT_ARROW,  KEY_FLAGS }
#define Key_Keypad7 (Key) { HID_KEYPAD_7_AND_HOME,  KEY_FLAGS }
#define Key_Keypad8 (Key) { HID_KEYPAD_8_AND_UP_ARROW,  KEY_FLAGS }
#define Key_Keypad9 (Key) { HID_KEYPAD_9_AND_PAGE_UP,  KEY_FLAGS }
#define Key_Keypad0 (Key) { HID_KEYPAD_0_AND_INSERT,  KEY_FLAGS }
#define Key_KeypadDot (Key) { HID_KEYPAD_PERIOD_AND_DELETE,  KEY_FLAGS }
#define Key_PcApplciation (Key) { HID_KEYBOARD_APPLICATION,  KEY_FLAGS }
#define Key_Help (Key) { HID_KEYBOARD_HELP,  KEY_FLAGS }
#define KEY_BACKLIGHT_DOWN 0xF1
#define KEY_BACKLIGHT_UP 0xF2
#define Key_BacklightDn (Key) { KEY_BACKLIGHT_DOWN,  KEY_FLAGS }
#define Key_BacklightUp (Key) { KEY_BACKLIGHT_UP,  KEY_FLAGS }
#define KEY_RIGHT_FN2 0xfe
#define Key_RFN2 (Key) { KEY_RIGHT_FN2,  KEY_FLAGS }
#define KEY_LEFT_FN2 0xff
#define Key_LFN2 (Key) { KEY_LEFT_FN2,  KEY_FLAGS }

#define Key_Undo (Key) { HID_KEYBOARD_UNDO,  KEY_FLAGS }
#define Key_Cut (Key) { HID_KEYBOARD_CUT,  KEY_FLAGS }
#define Key_Copy (Key) { HID_KEYBOARD_COPY,  KEY_FLAGS }
#define Key_Paste (Key) { HID_KEYBOARD_PASTE,  KEY_FLAGS }


#define Key_Keymap0 (Key) { KEYMAP_0,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap1 (Key) { KEYMAP_1,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap2 (Key) { KEYMAP_2,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap3 (Key) { KEYMAP_3,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap4 (Key) { KEYMAP_4,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap5 (Key) { KEYMAP_5,  KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP  }
#define Key_Keymap0_Momentary (Key){ KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP, KEYMAP_0 + MOMENTARY_OFFSET}
#define Key_Keymap1_Momentary (Key){ KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP, KEYMAP_1 + MOMENTARY_OFFSET}
#define Key_Keymap2_Momentary (Key){ KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP, KEYMAP_2 + MOMENTARY_OFFSET }
#define Key_Keymap3_Momentary (Key){ KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP, KEYMAP_3 + MOMENTARY_OFFSET }
#define Key_Keymap4_Momentary (Key){ KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP, KEYMAP_4 + MOMENTARY_OFFSET }
#define Key_Keymap5_Momentary (Key){ KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP, KEYMAP_5 + MOMENTARY_OFFSET }

#define Key_KeymapNext_Momentary (Key) {KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP, KEYMAP_NEXT + MOMENTARY_OFFSET }
#define Key_KeymapPrevious_Momentary (Key) {KEY_FLAGS | SYNTHETIC | SWITCH_TO_KEYMAP, KEYMAP_PREVIOUS + MOMENTARY_OFFSET }




#define Key_LEDEffectNext (Key) { 0,  KEY_FLAGS | SYNTHETIC | IS_INTERNAL | LED_TOGGLE }
