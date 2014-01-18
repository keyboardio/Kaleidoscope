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





#define Key_powerDown (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_POWER_DOWN }
#define Key_sleep (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_SLEEP }
#define Key_wakeup (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_WAKEUP }
#define Key_coldRestart (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_COLD_RESTART }
#define Key_warmRestart (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_WARM_RESTART }
#define Key_dock (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_DOCK }
#define Key_undock (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_UNDOCK }
#define Key_speakerMute (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_SPEAKER_MUTE }
#define Key_hibernate (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_HIBERNATE }
#define Key_displayInvert (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_DISPLAY_INVERT }
#define Key_displayInternal (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_DISPLAY_INTERNAL }
#define Key_displayExternal (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_DISPLAY_EXTERNAL }
#define Key_displayBoth (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_DISPLAY_BOTH }
#define Key_displayDual (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_DISPLAY_DUAL }
#define Key_displayToggle (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_DISPLAY_TOGGLE_INT_EXT
#define Key_displaySwap (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_SYSCTL,SYSTEM_CONTROL_DISPLAY_SWAP }


#define Key_volumeMute (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_VOLUME_MUTE}
#define Key_volumeUp (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_VOLUME_UP}
#define Key_volumeDown (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_VOLUME_DOWN}
#define Key_playPause (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_PLAY_PAUSE}
#define Key_stop (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_STOP}
#define Key_prevTrack (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_PREV_TRACK}
#define Key_nextTrack (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_NEXT_TRACK}
#define Key_Eject (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_EJECT}

//#define Key_Cut (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_AC_CUT }
//#define Key_Copy (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_AC_COPY}
//#define Key_Paste (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_AC_PASTE}
//#define Key_Undo (Key) {KEY_FLAGS | SYNTHETIC_KEY|IS_CONSUMER, CONSUMER_CONTROL_AC_UNDO }

#define Key_LCtrl (Key){ KEY_FLAGS, KEY_LEFT_CTRL }
#define KEY_LEFT_CTRL 0x80
#define Key_LCtrl (Key){ KEY_FLAGS, KEY_LEFT_CTRL }
#define KEY_LEFT_SHIFT 0x81
#define Key_LShift (Key){ KEY_FLAGS, KEY_LEFT_SHIFT }
#define KEY_LEFT_ALT 0x82
#define Key_LAlt (Key){ KEY_FLAGS, KEY_LEFT_ALT }
#define KEY_LEFT_GUI 0x83
#define Key_LGUI (Key){ KEY_FLAGS, KEY_LEFT_GUI }
#define KEY_RIGHT_CTRL 0x84
#define Key_RCtrl (Key){ KEY_FLAGS, KEY_RIGHT_CTRL }
#define KEY_RIGHT_SHIFT 0x85
#define Key_RShift (Key){ KEY_FLAGS, KEY_RIGHT_SHIFT }
#define KEY_RIGHT_ALT 0x86
#define Key_RAlt (Key){ KEY_FLAGS, KEY_RIGHT_ALT }
#define KEY_RIGHT_GUI 0x87
#define Key_RGUI (Key){ KEY_FLAGS, KEY_RIGHT_GUI }

#define KEY_UP_ARROW 0xDA
#define Key_UpArrow (Key){ KEY_FLAGS, KEY_UP_ARROW }
#define KEY_DOWN_ARROW 0xD9
#define Key_DnArrow (Key){ KEY_FLAGS, KEY_DOWN_ARROW }
#define KEY_LEFT_ARROW 0xD8
#define Key_LArrow (Key){ KEY_FLAGS, KEY_LEFT_ARROW }
#define KEY_RIGHT_ARROW 0xD7
#define Key_RArrow (Key){ KEY_FLAGS, KEY_RIGHT_ARROW }
#define KEY_RETURN 0xB0
#define Key_Return (Key){ KEY_FLAGS, KEY_RETURN }
#define KEY_ESC 0xB1
#define Key_Esc (Key){ KEY_FLAGS, KEY_ESC }
#define KEY_BACKSPACE 0xB2
#define Key_Backspace (Key){ KEY_FLAGS, KEY_BACKSPACE }
#define KEY_TAB 0xB3
#define Key_Tab (Key){ KEY_FLAGS, KEY_TAB }
#define KEY_INSERT 0xD1
#define Key_Insert (Key){ KEY_FLAGS, KEY_INSERT }
#define KEY_DELETE 0xD4
#define Key_Delete (Key){ KEY_FLAGS, KEY_DELETE }
#define KEY_PAGE_UP 0xD3
#define Key_PageUp (Key){ KEY_FLAGS, KEY_PAGE_UP }
#define KEY_PAGE_DOWN 0xD6
#define Key_PageDn (Key){ KEY_FLAGS, KEY_PAGE_DOWN }
#define KEY_HOME 0xD2
#define Key_Home (Key){ KEY_FLAGS, KEY_HOME }
#define KEY_END 0xD5
#define Key_End (Key){ KEY_FLAGS, KEY_END }


#define KEY_CAPS_LOCK 0xC1
#define Key_CapsLock (Key){ KEY_FLAGS, KEY_CAPS_LOCK }
#define KEY_F1 0xC2
#define Key_F1 (Key){ KEY_FLAGS, KEY_F1 }
#define KEY_F2 0xC3
#define Key_F2 (Key){ KEY_FLAGS, KEY_F2 }
#define KEY_F3 0xC4
#define Key_F3 (Key){ KEY_FLAGS, KEY_F3 }
#define KEY_F4 0xC5
#define Key_F4 (Key){ KEY_FLAGS, KEY_F4 }
#define KEY_F5 0xC6
#define Key_F5 (Key){ KEY_FLAGS, KEY_F5 }
#define KEY_F6 0xC7
#define Key_F6 (Key){ KEY_FLAGS, KEY_F6 }
#define KEY_F7 0xC8
#define Key_F7 (Key){ KEY_FLAGS, KEY_F7 }
#define KEY_F8 0xC9
#define Key_F8 (Key){ KEY_FLAGS, KEY_F8 }
#define KEY_F9 0xCA
#define Key_F9 (Key){ KEY_FLAGS, KEY_F9 }
#define KEY_F10 0xCB
#define Key_F10 (Key){ KEY_FLAGS, KEY_F10 }
#define KEY_F11 0xCC
#define Key_F11 (Key){ KEY_FLAGS, KEY_F11 }
#define KEY_F12 0xCD
#define Key_F12 (Key){ KEY_FLAGS, KEY_F12 }



#define KEY_0 '0'
#define Key_0 (Key){ KEY_FLAGS, KEY_0 }
#define KEY_1 '1'
#define Key_1 (Key){ KEY_FLAGS, KEY_1 }
#define KEY_2 '2'
#define Key_2 (Key){ KEY_FLAGS, KEY_2 }
#define KEY_3 '3'
#define Key_3 (Key){ KEY_FLAGS, KEY_3 }
#define KEY_4 '4'
#define Key_4 (Key){ KEY_FLAGS, KEY_4 }
#define KEY_5 '5'
#define Key_5 (Key){ KEY_FLAGS, KEY_5 }
#define KEY_6 '6'
#define Key_6 (Key){ KEY_FLAGS, KEY_6 }
#define KEY_7 '7'
#define Key_7 (Key){ KEY_FLAGS, KEY_7 }
#define KEY_8 '8'
#define Key_8 (Key){ KEY_FLAGS, KEY_8 }
#define KEY_9 '9'
#define Key_9 (Key){ KEY_FLAGS, KEY_9 }
#define KEY_A 'a'
#define Key_A (Key){ KEY_FLAGS, KEY_A }
#define KEY_B 'b'
#define Key_B (Key){ KEY_FLAGS, KEY_B }
#define KEY_C 'c'
#define Key_C (Key){ KEY_FLAGS, KEY_C }
#define KEY_D 'd'
#define Key_D (Key){ KEY_FLAGS, KEY_D }
#define KEY_E 'e'
#define Key_E (Key){ KEY_FLAGS, KEY_E }
#define KEY_F 'f'
#define Key_F (Key){ KEY_FLAGS, KEY_F }
#define KEY_G 'g'
#define Key_G (Key){ KEY_FLAGS, KEY_G }
#define KEY_H 'h'
#define Key_H (Key){ KEY_FLAGS, KEY_H }
#define KEY_I 'i'
#define Key_I (Key){ KEY_FLAGS, KEY_I }
#define KEY_J 'j'
#define Key_J (Key){ KEY_FLAGS, KEY_J }
#define KEY_K 'k'
#define Key_K (Key){ KEY_FLAGS, KEY_K }
#define KEY_L 'l'
#define Key_L (Key){ KEY_FLAGS, KEY_L }
#define KEY_M 'm'
#define Key_M (Key){ KEY_FLAGS, KEY_M }
#define KEY_N 'n'
#define Key_N (Key){ KEY_FLAGS, KEY_N }
#define KEY_O 'o'
#define Key_O (Key){ KEY_FLAGS, KEY_O }
#define KEY_P 'p'
#define Key_P (Key){ KEY_FLAGS, KEY_P }
#define KEY_Q 'q'
#define Key_Q (Key){ KEY_FLAGS, KEY_Q }
#define KEY_R 'r'
#define Key_R (Key){ KEY_FLAGS, KEY_R }
#define KEY_S 's'
#define Key_S (Key){ KEY_FLAGS, KEY_S }
#define KEY_T 't'
#define Key_T (Key){ KEY_FLAGS, KEY_T }
#define KEY_U 'u'
#define Key_U (Key){ KEY_FLAGS, KEY_U }
#define KEY_V 'v'
#define Key_V (Key){ KEY_FLAGS, KEY_V }
#define KEY_W 'w'
#define Key_W (Key){ KEY_FLAGS, KEY_W }
#define KEY_X 'x'
#define Key_X (Key){ KEY_FLAGS, KEY_X }
#define KEY_Y 'y'
#define Key_Y (Key){ KEY_FLAGS, KEY_Y }
#define KEY_Z 'z'
#define Key_Z (Key){ KEY_FLAGS, KEY_Z }

#define KEY_BACKTICK '`'
#define Key_Backtick (Key){ KEY_FLAGS, KEY_BACKTICK }
#define KEY_MINUS '-'
#define Key_Minus (Key){ KEY_FLAGS, KEY_MINUS }
#define KEY_EQUALS '='
#define Key_Equals (Key){ KEY_FLAGS, KEY_EQUALS }
#define KEY_LEFT_BRACKET '['
#define Key_LBracket (Key){ KEY_FLAGS, KEY_LEFT_BRACKET }
#define KEY_RIGHT_BRACKET ']'
#define Key_RBracket (Key){ KEY_FLAGS, KEY_RIGHT_BRACKET }
#define KEY_BACKSLASH '\\'
#define Key_Backslash (Key){ KEY_FLAGS, KEY_BACKSLASH }
#define KEY_PIPE '|'
#define Key_Pipe (Key){ KEY_FLAGS, KEY_PIPE }

#define KEY_LEFT_SQUARE_BRACKET '['
#define Key_LSquareBracket (Key){ KEY_FLAGS, KEY_LEFT_SQUARE_BRACKET }
#define KEY_RIGHT_SQUARE_BRACKET ']'
#define Key_RSquareBracket (Key){ KEY_FLAGS, KEY_RIGHT_SQUARE_BRACKET }
#define KEY_LEFT_CURLY_BRACKET '{'
#define Key_LCurlyBracket (Key){ KEY_FLAGS, KEY_LEFT_CURLY_BRACKET }
#define KEY_RIGHT_CURLY_BRACKET '}'
#define Key_RCurlyBracket (Key){ KEY_FLAGS, KEY_RIGHT_CURLY_BRACKET }

#define KEY_SEMICOLON ';'
#define Key_Semicolon (Key){ KEY_FLAGS, KEY_SEMICOLON }
#define KEY_QUOTE '\''
#define Key_Quote (Key){ KEY_FLAGS, KEY_QUOTE }
#define KEY_COMMA ','
#define Key_Comma (Key){ KEY_FLAGS, KEY_COMMA }
#define KEY_PERIOD '.'
#define Key_Period (Key){ KEY_FLAGS, KEY_PERIOD }
#define KEY_SPACE ' '
#define Key_Space (Key){ KEY_FLAGS, KEY_SPACE }
#define KEY_SLASH '/'
#define Key_Slash (Key){ KEY_FLAGS, KEY_SLASH }

#define KEY_KEYPAD_CLEAR 0xDB
#define Key_KeypadClear (Key){ KEY_FLAGS, KEY_KEYPAD_CLEAR }
#define KEY_KEYPAD_SLASH 0xDC
#define Key_KeypadSlash (Key){ KEY_FLAGS, KEY_KEYPAD_SLASH }
#define KEY_KEYPAD_MULTIPLY 0xDD
#define Key_KeypadMultiply (Key){ KEY_FLAGS, KEY_KEYPAD_MULTIPLY }
#define KEY_KEYPAD_MINUS 0xDE
#define Key_KeypadMinus (Key){ KEY_FLAGS, KEY_KEYPAD_MINUS }
#define KEY_KEYPAD_PLUS 0xDF
#define Key_KeypadPlus (Key){ KEY_FLAGS, KEY_KEYPAD_PLUS }
#define KEY_ENTER 0xe0
#define Key_Enter (Key){ KEY_FLAGS, KEY_ENTER }
#define KEY_KEYPAD_1 0xe1
#define Key_Keypad1 (Key){ KEY_FLAGS, KEY_KEYPAD_1 }
#define KEY_KEYPAD_2 0xe2
#define Key_Keypad2 (Key){ KEY_FLAGS, KEY_KEYPAD_2 }
#define KEY_KEYPAD_3 0xe3
#define Key_Keypad3 (Key){ KEY_FLAGS, KEY_KEYPAD_3 }
#define KEY_KEYPAD_4 0xe4
#define Key_Keypad4 (Key){ KEY_FLAGS, KEY_KEYPAD_4 }
#define KEY_KEYPAD_5 0xe5
#define Key_Keypad5 (Key){ KEY_FLAGS, KEY_KEYPAD_5 }
#define KEY_KEYPAD_6 0xe6
#define Key_Keypad6 (Key){ KEY_FLAGS, KEY_KEYPAD_6 }
#define KEY_KEYPAD_7 0xe7
#define Key_Keypad7 (Key){ KEY_FLAGS, KEY_KEYPAD_7 }
#define KEY_KEYPAD_8 0xe8
#define Key_Keypad8 (Key){ KEY_FLAGS, KEY_KEYPAD_8 }
#define KEY_KEYPAD_9 0xe9
#define Key_Keypad9 (Key){ KEY_FLAGS, KEY_KEYPAD_9 }
#define KEY_KEYPAD_0 0xea
#define Key_Keypad0 (Key){ KEY_FLAGS, KEY_KEYPAD_0 }
#define KEY_KEYPAD_DOT 0xeb
#define Key_KeypadDot (Key){ KEY_FLAGS, KEY_KEYPAD_DOT }
#define KEY_DANISH_DOLLAR 0xec
#define Key_DanishDollar (Key){ KEY_FLAGS, KEY_DANISH_DOLLAR }
#define KEY_PC_APPLCIATION 0xed
#define Key_PcApplciation (Key){ KEY_FLAGS, KEY_PC_APPLCIATION }
#define KEY_F13 0xF0
#define Key_F13 (Key){ KEY_FLAGS, KEY_F13 }
#define KEY_F16 0xF3
#define Key_F16 (Key){ KEY_FLAGS, KEY_F16 }
#define KEY_F17 0xF4
#define Key_F17 (Key){ KEY_FLAGS, KEY_F17 }
#define KEY_F18 0xF5
#define Key_F18 (Key){ KEY_FLAGS, KEY_F18 }
#define KEY_F19 0xF6
#define Key_F19 (Key){ KEY_FLAGS, KEY_F19 }

#define KEY_HELP 0xfb
#define Key_Help (Key){ KEY_FLAGS, KEY_HELP }
#define KEY_BACKLIGHT_DOWN 0xF1
#define Key_BacklightDn (Key){ KEY_FLAGS, KEY_BACKLIGHT_DOWN }
#define KEY_BACKLIGHT_UP 0xF2
#define Key_BacklightUp (Key){ KEY_FLAGS, KEY_BACKLIGHT_UP }
#define NO_KEY false
#define KEY_RIGHT_FN2 0xfe
#define Key_RFN2 (Key){ KEY_FLAGS, KEY_RIGHT_FN2 }
#define KEY_LEFT_FN2 0xff
#define Key_LFN2 (Key){ KEY_FLAGS, KEY_LEFT_FN2 }

#define KEY_UNDO    0x7a
#define Key_Undo (Key){ KEY_FLAGS, KEY_UNDO }

/*

 These keycodes don't seem to map to unique keypresses. we can use them for meta-keys
#define KEY_WTF 0xF7
#define Key_Wtf (Key){ KEY_FLAGS, KEY_WTF }

#define Key_Xxxx (Key){ KEY_FLAGS, xxxx }
#define xxxxx 0xFE
#define Key_Xxxxx (Key){ KEY_FLAGS, xxxxx }
#define KEY_ 0xB4
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_ 0xB5
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_ 0xB6
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_ 0xB7
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_ 0xB8
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_ 0xB9
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_ 0xBA
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_ 0xBB
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_ 0xBC
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_ 0xBD
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_ 0xBE
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_ 0xBF
#define Key_ (Key){ KEY_FLAGS, KEY_ }
#define KEY_F13_ALSO 0xC0
#define Key_F13_also (Key){ KEY_FLAGS, KEY_F13_ALSO }
#define KEY_SLASH_ALSO 0xCE
#define Key_Slash_also (Key){ KEY_FLAGS, KEY_SLASH_ALSO }
#define KEY_BACKLIGHT_DOWN_ALSO 0xCF
#define Key_Backlight_down_also (Key){ KEY_FLAGS, KEY_BACKLIGHT_DOWN_ALSO }
#define KEY_BACKLIGHT_UP_ALSO 0xD0
#define Key_Backlight_up_also (Key){ KEY_FLAGS, KEY_BACKLIGHT_UP_ALSO }
 */


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



