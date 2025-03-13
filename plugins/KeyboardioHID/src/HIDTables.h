/*
Copyright (c) 2015-2018 Keyboard.io, Inc

See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)


// These mappings were extracted and transcribed from
// https://www.usb.org/sites/default/files/hut1_2.pdf
//
// In most cases, I've preserved the "official" USB Implementers forum
// "Usage Name", though I've standardized some abbreviations and spacing
// that were inconsistent in the original specification. Non alpha-numeric
// characters in symbol names were converted into those characters' names.
//
// To match Arduino code style, all hid usage names are fully upper case.
//
// Not every HID usage listed in this file is currently supported by Arduino
// In particular, any System Control or Consumer Control entry that doesn't
// have a comment indicating that it's "HID type OSC" may require additional
// code in the Arduino core to work, although // some keycodes with usage
// type RTC (Re-Trigger Control) and OOC (On/Off Control) are also functional.
//
// Non-working usages are listed here in the interest of not having to manually
// convert more usage names each and every time our HID stack gets a little bit
// better.
//
//
//              -- Jesse Vincent <jesse@keyboard.io>, September 2020


// ==============================================================================
// System Control USB HID keycodes
// [Section 4: Generic Desktop Page (0x01)]

// ----|-------------------------------------------------------|-------|---------
//	Name							Value	Usage Type
// ----|-------------------------------------------------------|-------|---------
#define HID_SYSTEM_POWER_DOWN                    0x81  // OSC
#define HID_SYSTEM_SLEEP                         0x82  // OSC
#define HID_SYSTEM_WAKE_UP                       0x83  // OSC
#define HID_SYSTEM_CONTEXT_MENU                  0x84  // OSC
#define HID_SYSTEM_MAIN_MENU                     0x85  // OSC
#define HID_SYSTEM_APP_MENU                      0x86  // OSC
#define HID_SYSTEM_MENU_HELP                     0x87  // OSC
#define HID_SYSTEM_MENU_EXIT                     0x88  // OSC
#define HID_SYSTEM_MENU_SELECT                   0x89  // OSC
#define HID_SYSTEM_MENU_RIGHT                    0x8A  // RTC
#define HID_SYSTEM_MENU_LEFT                     0x8B  // RTC
#define HID_SYSTEM_MENU_UP                       0x8C  // RTC
#define HID_SYSTEM_MENU_DOWN                     0x8D  // RTC
#define HID_SYSTEM_COLD_RESTART                  0x8E  // OSC
#define HID_SYSTEM_WARM_RESTART                  0x8F  // OSC
#define HID_D_PAD_UP                             0x90  // OOC
#define HID_D_PAD_DOWN                           0x91  // OOC
#define HID_D_PAD_RIGHT                          0x92  // OOC
#define HID_D_PAD_LEFT                           0x93  // OOC
#define HID_INDEX_TRIGGER                        0x94  // MC/DV
#define HID_PALM_TRIGGER                         0x95  // MC/DV
#define HID_THUMBSTICK                           0x96  // CP
#define HID_SYSTEM_FUNCTION_SHIFT                0x97  // MC
#define HID_SYSTEM_FUNCTION_SHIFT_LOCK           0x98  // OOC
#define HID_SYSTEM_FUNCTION_SHIFT_LOCK_INDICATOR 0x99  // DV
#define HID_DISMISS_NOTIFICATION                 0x9A  // OSC
#define HID_DO_NOT_DISTURB                       0x9B  // OOC
// Reserved							0x9C-9F
#define HID_SYSTEM_DOCK                0xA0  // OSC
#define HID_SYSTEM_UNDOCK              0xA1  // OSC
#define HID_SYSTEM_SETUP               0xA2  // OSC
#define HID_SYSTEM_BREAK               0xA3  // OSC
#define HID_SYSTEM_DEBUGGER_BREAK      0xA4  // OSC
#define HID_APPLICATION_BREAK          0xA5  // OSC
#define HID_APPLICATION_DEBUGGER_BREAK 0xA6  // OSC
#define HID_SYSTEM_SPEAKER_MUTE        0xA7  // OSC
#define HID_SYSTEM_HIBERNATE           0xA8  // OSC
// Reserved							0xA9-AF
#define HID_SYSTEM_DISPLAY_INVERT                       0xB0  // OSC
#define HID_SYSTEM_DISPLAY_INTERNAL                     0xB1  // OSC
#define HID_SYSTEM_DISPLAY_EXTERNAL                     0xB2  // OSC
#define HID_SYSTEM_DISPLAY_BOTH                         0xB3  // OSC
#define HID_SYSTEM_DISPLAY_DUAL                         0xB4  // OSC
#define HID_SYSTEM_DISPLAY_TOGGLE_INT_SLASH_EXT         0xB5  // OSC
#define HID_SYSTEM_DISPLAY_SWAP_PRIMARY_SLASH_SECONDARY 0xB6  // OSC
#define HID_SYSTEM_DISPLAY_LCD_AUTOSCALE                0xB7  // OSC
// Reserved							0xB8-BF


// ==============================================================================
// Keyboard USB HID keycodes
// [Section 10: Keyboard/Keypad Page (0x07)]

// ----|-------------------------------------------------------|-------|---------
//	Name							Value	Usage Type
// ----|-------------------------------------------------------|-------|---------
#define HID_KEYBOARD_NO_EVENT                0x00  // Sel
#define HID_KEYBOARD_ERROR_ROLLOVER          0x01  // Sel
#define HID_KEYBOARD_POST_FAIL               0x02  // Sel
#define HID_KEYBOARD_ERROR_UNDEFINED         0x03  // Sel
#define HID_KEYBOARD_A_AND_A                 0x04  // Sel
#define HID_KEYBOARD_B_AND_B                 0x05  // Sel
#define HID_KEYBOARD_C_AND_C                 0x06  // Sel
#define HID_KEYBOARD_D_AND_D                 0x07  // Sel
#define HID_KEYBOARD_E_AND_E                 0x08  // Sel
#define HID_KEYBOARD_F_AND_F                 0x09  // Sel
#define HID_KEYBOARD_G_AND_G                 0x0A  // Sel
#define HID_KEYBOARD_H_AND_H                 0x0B  // Sel
#define HID_KEYBOARD_I_AND_I                 0x0C  // Sel
#define HID_KEYBOARD_J_AND_J                 0x0D  // Sel
#define HID_KEYBOARD_K_AND_K                 0x0E  // Sel
#define HID_KEYBOARD_L_AND_L                 0x0F  // Sel
#define HID_KEYBOARD_M_AND_M                 0x10  // Sel
#define HID_KEYBOARD_N_AND_N                 0x11  // Sel
#define HID_KEYBOARD_O_AND_O                 0x12  // Sel
#define HID_KEYBOARD_P_AND_P                 0x13  // Sel
#define HID_KEYBOARD_Q_AND_Q                 0x14  // Sel
#define HID_KEYBOARD_R_AND_R                 0x15  // Sel
#define HID_KEYBOARD_S_AND_S                 0x16  // Sel
#define HID_KEYBOARD_T_AND_T                 0x17  // Sel
#define HID_KEYBOARD_U_AND_U                 0x18  // Sel
#define HID_KEYBOARD_V_AND_V                 0x19  // Sel
#define HID_KEYBOARD_W_AND_W                 0x1A  // Sel
#define HID_KEYBOARD_X_AND_X                 0x1B  // Sel
#define HID_KEYBOARD_Y_AND_Y                 0x1C  // Sel
#define HID_KEYBOARD_Z_AND_Z                 0x1D  // Sel
#define HID_KEYBOARD_1_AND_EXCLAMATION_POINT 0x1E  // Sel
#define HID_KEYBOARD_2_AND_AT                0x1F  // Sel
#define HID_KEYBOARD_3_AND_POUND             0x20  // Sel
#define HID_KEYBOARD_4_AND_DOLLAR            0x21  // Sel
#define HID_KEYBOARD_5_AND_PERCENT           0x22  // Sel
#define HID_KEYBOARD_6_AND_CARAT             0x23  // Sel
#define HID_KEYBOARD_7_AND_AMPERSAND         0x24  // Sel
#define HID_KEYBOARD_8_AND_ASTERISK          0x25  // Sel
#define HID_KEYBOARD_9_AND_LEFT_PAREN        0x26  // Sel
#define HID_KEYBOARD_0_AND_RIGHT_PAREN       0x27  // Sel
#define HID_KEYBOARD_ENTER                   0x28  // Sel
//#define HID_KEYBOARD_ENTER  HID_KEYBOARD_ENTER_SLASH_RETURN
#define HID_KEYBOARD_ESCAPE 0x29  // Sel
#define HID_KEYBOARD_DELETE 0x2A  // Sel
//#define HID_KEYBOARD_BACKSPACE	HID_KEYBOARD_DELETE
#define HID_KEYBOARD_TAB                                 0x2B  // Sel
#define HID_KEYBOARD_SPACEBAR                            0x2C  // Sel
#define HID_KEYBOARD_MINUS_AND_UNDERSCORE                0x2D  // Sel
#define HID_KEYBOARD_EQUALS_AND_PLUS                     0x2E  // Sel
#define HID_KEYBOARD_LEFT_BRACKET_AND_LEFT_CURLY_BRACE   0x2F  // Sel
#define HID_KEYBOARD_RIGHT_BRACKET_AND_RIGHT_CURLY_BRACE 0x30  // Sel
#define HID_KEYBOARD_BACKSLASH_AND_PIPE                  0x31  // Sel
#define HID_KEYBOARD_NON_US_POUND_AND_TILDE              0x32  // Sel
#define HID_KEYBOARD_SEMICOLON_AND_COLON                 0x33  // Sel
#define HID_KEYBOARD_QUOTE_AND_DOUBLEQUOTE               0x34  // Sel
#define HID_KEYBOARD_GRAVE_ACCENT_AND_TILDE              0x35  // Sel
#define HID_KEYBOARD_COMMA_AND_LESS_THAN                 0x36  // Sel
#define HID_KEYBOARD_PERIOD_AND_GREATER_THAN             0x37  // Sel
#define HID_KEYBOARD_SLASH_AND_QUESTION_MARK             0x38  // Sel
#define HID_KEYBOARD_CAPS_LOCK                           0x39  // Sel
#define HID_KEYBOARD_F1                                  0x3A  // Sel
#define HID_KEYBOARD_F2                                  0x3B  // Sel
#define HID_KEYBOARD_F3                                  0x3C  // Sel
#define HID_KEYBOARD_F4                                  0x3D  // Sel
#define HID_KEYBOARD_F5                                  0x3E  // Sel
#define HID_KEYBOARD_F6                                  0x3F  // Sel
#define HID_KEYBOARD_F7                                  0x40  // Sel
#define HID_KEYBOARD_F8                                  0x41  // Sel
#define HID_KEYBOARD_F9                                  0x42  // Sel
#define HID_KEYBOARD_F10                                 0x43  // Sel
#define HID_KEYBOARD_F11                                 0x44  // Sel
#define HID_KEYBOARD_F12                                 0x45  // Sel
#define HID_KEYBOARD_PRINTSCREEN                         0x46  // Sel
#define HID_KEYBOARD_SCROLL_LOCK                         0x47  // Sel
#define HID_KEYBOARD_PAUSE                               0x48  // Sel
#define HID_KEYBOARD_INSERT                              0x49  // Sel
#define HID_KEYBOARD_HOME                                0x4A  // Sel
#define HID_KEYBOARD_PAGE_UP                             0x4B  // Sel
#define HID_KEYBOARD_DELETE_FORWARD                      0x4C  // Sel
#define HID_KEYBOARD_END                                 0x4D  // Sel
#define HID_KEYBOARD_PAGE_DOWN                           0x4E  // Sel
#define HID_KEYBOARD_RIGHT_ARROW                         0x4F  // Sel
#define HID_KEYBOARD_LEFT_ARROW                          0x50  // Sel
#define HID_KEYBOARD_DOWN_ARROW                          0x51  // Sel
#define HID_KEYBOARD_UP_ARROW                            0x52  // Sel
#define HID_KEYPAD_NUM_LOCK_AND_CLEAR                    0x53  // Sel
#define HID_KEYPAD_DIVIDE                                0x54  // Sel
#define HID_KEYPAD_MULTIPLY                              0x55  // Sel
#define HID_KEYPAD_SUBTRACT                              0x56  // Sel
#define HID_KEYPAD_ADD                                   0x57  // Sel
#define HID_KEYPAD_ENTER                                 0x58  // Sel
#define HID_KEYPAD_1_AND_END                             0x59  // Sel
#define HID_KEYPAD_2_AND_DOWN_ARROW                      0x5A  // Sel
#define HID_KEYPAD_3_AND_PAGE_DOWN                       0x5B  // Sel
#define HID_KEYPAD_4_AND_LEFT_ARROW                      0x5C  // Sel
#define HID_KEYPAD_5                                     0x5D  // Sel
#define HID_KEYPAD_6_AND_RIGHT_ARROW                     0x5E  // Sel
#define HID_KEYPAD_7_AND_HOME                            0x5F  // Sel
#define HID_KEYPAD_8_AND_UP_ARROW                        0x60  // Sel
#define HID_KEYPAD_9_AND_PAGE_UP                         0x61  // Sel
#define HID_KEYPAD_0_AND_INSERT                          0x62  // Sel
#define HID_KEYPAD_PERIOD_AND_DELETE                     0x63  // Sel
#define HID_KEYBOARD_NON_US_BACKSLASH_AND_PIPE           0x64  // Sel
#define HID_KEYBOARD_APPLICATION                         0x65  // Sel
#define HID_KEYBOARD_POWER                               0x66  // Sel
#define HID_KEYPAD_EQUALS                                0x67  // Sel
#define HID_KEYBOARD_F13                                 0x68  // Sel
#define HID_KEYBOARD_F14                                 0x69  // Sel
#define HID_KEYBOARD_F15                                 0x6A  // Sel
#define HID_KEYBOARD_F16                                 0x6B  // Sel
#define HID_KEYBOARD_F17                                 0x6C  // Sel
#define HID_KEYBOARD_F18                                 0x6D  // Sel
#define HID_KEYBOARD_F19                                 0x6E  // Sel
#define HID_KEYBOARD_F20                                 0x6F  // Sel
#define HID_KEYBOARD_F21                                 0x70  // Sel
#define HID_KEYBOARD_F22                                 0x71  // Sel
#define HID_KEYBOARD_F23                                 0x72  // Sel
#define HID_KEYBOARD_F24                                 0x73  // Sel
#define HID_KEYBOARD_EXECUTE                             0x74  // Sel
#define HID_KEYBOARD_HELP                                0x75  // Sel
#define HID_KEYBOARD_MENU                                0x76  // Sel
#define HID_KEYBOARD_SELECT                              0x77  // Sel
#define HID_KEYBOARD_STOP                                0x78  // Sel
#define HID_KEYBOARD_AGAIN                               0x79  // Sel
#define HID_KEYBOARD_UNDO                                0x7A  // Sel
#define HID_KEYBOARD_CUT                                 0x7B  // Sel
#define HID_KEYBOARD_COPY                                0x7C  // Sel
#define HID_KEYBOARD_PASTE                               0x7D  // Sel
#define HID_KEYBOARD_FIND                                0x7E  // Sel
#define HID_KEYBOARD_MUTE                                0x7F  // Sel
#define HID_KEYBOARD_VOLUME_UP                           0x80  // Sel
#define HID_KEYBOARD_VOLUME_DOWN                         0x81  // Sel
#define HID_KEYBOARD_LOCKING_CAPS_LOCK                   0x82  // Sel
#define HID_KEYBOARD_LOCKING_NUM_LOCK                    0x83  // Sel
#define HID_KEYBOARD_LOCKING_SCROLL_LOCK                 0x84  // Sel
#define HID_KEYPAD_COMMA                                 0x85  // Sel
#define HID_KEYPAD_EQUAL_SIGN                            0x86  // Sel
#define HID_KEYBOARD_INTERNATIONAL1                      0x87  // Sel
#define HID_KEYBOARD_INTERNATIONAL2                      0x88  // Sel
#define HID_KEYBOARD_INTERNATIONAL3                      0x89  // Sel
#define HID_KEYBOARD_INTERNATIONAL4                      0x8A  // Sel
#define HID_KEYBOARD_INTERNATIONAL5                      0x8B  // Sel
#define HID_KEYBOARD_INTERNATIONAL6                      0x8C  // Sel
#define HID_KEYBOARD_INTERNATIONAL7                      0x8D  // Sel
#define HID_KEYBOARD_INTERNATIONAL8                      0x8E  // Sel
#define HID_KEYBOARD_INTERNATIONAL9                      0x8F  // Sel
#define HID_KEYBOARD_LANG1                               0x90  // Sel
#define HID_KEYBOARD_LANG2                               0x91  // Sel
#define HID_KEYBOARD_LANG3                               0x92  // Sel
#define HID_KEYBOARD_LANG4                               0x93  // Sel
#define HID_KEYBOARD_LANG5                               0x94  // Sel
#define HID_KEYBOARD_LANG6                               0x95  // Sel
#define HID_KEYBOARD_LANG7                               0x96  // Sel
#define HID_KEYBOARD_LANG8                               0x97  // Sel
#define HID_KEYBOARD_LANG9                               0x98  // Sel
#define HID_KEYBOARD_ALTERNATE_ERASE                     0x99  // Sel
#define HID_KEYBOARD_SYSREQ_SLASH_ATTENTION              0x9A  // Sel
#define HID_KEYBOARD_CANCEL                              0x9B  // Sel
#define HID_KEYBOARD_CLEAR                               0x9C  // Sel
#define HID_KEYBOARD_PRIOR                               0x9D  // Sel
#define HID_KEYBOARD_RETURN                              0x9E  // Sel
#define HID_KEYBOARD_SEPARATOR                           0x9F  // Sel
#define HID_KEYBOARD_OUT                                 0xA0  // Sel
#define HID_KEYBOARD_OPER                                0xA1  // Sel
#define HID_KEYBOARD_CLEAR_SLASH_AGAIN                   0xA2  // Sel
#define HID_KEYBOARD_CRSEL_SLASH_PROPS                   0xA3  // Sel
#define HID_KEYBOARD_EXSEL                               0xA4  // Sel
// Reserved							0xA5-AF
#define HID_KEYPAD_00                0xB0  // Sel
#define HID_KEYPAD_000               0xB1  // Sel
#define HID_THOUSANDS_SEPARATOR      0xB2  // Sel
#define HID_DECIMAL_SEPARATOR        0xB3  // Sel
#define HID_CURRENCY_UNIT            0xB4  // Sel
#define HID_CURRENCY_SUBUNIT         0xB5  // Sel
#define HID_KEYPAD_LEFT_PAREN        0xB6  // Sel
#define HID_KEYPAD_RIGHT_PAREN       0xB7  // Sel
#define HID_KEYPAD_LEFT_CURLY_BRACE  0xB8  // Sel
#define HID_KEYPAD_RIGHT_CURLY_BRACE 0xB9  // Sel
#define HID_KEYPAD_TAB               0xBA  // Sel
#define HID_KEYPAD_BACKSPACE         0xBB  // Sel
#define HID_KEYPAD_A                 0xBC  // Sel
#define HID_KEYPAD_B                 0xBD  // Sel
#define HID_KEYPAD_C                 0xBE  // Sel
#define HID_KEYPAD_D                 0xBF  // Sel
#define HID_KEYPAD_E                 0xC0  // Sel
#define HID_KEYPAD_F                 0xC1  // Sel
#define HID_KEYPAD_XOR               0xC2  // Sel
#define HID_KEYPAD_CARAT             0xC3  // Sel
#define HID_KEYPAD_PERCENT           0xC4  // Sel
#define HID_KEYPAD_LESS_THAN         0xC5  // Sel
#define HID_KEYPAD_GREATER_THAN      0xC6  // Sel
#define HID_KEYPAD_AMPERSAND         0xC7  // Sel
#define HID_KEYPAD_DOUBLEAMPERSAND   0xC8  // Sel
#define HID_KEYPAD_PIPE              0xC9  // Sel
#define HID_KEYPAD_DOUBLEPIPE        0xCA  // Sel
#define HID_KEYPAD_COLON             0xCB  // Sel
#define HID_KEYPAD_POUND_SIGN        0xCC  // Sel
#define HID_KEYPAD_SPACE             0xCD  // Sel
#define HID_KEYPAD_AT_SIGN           0xCE  // Sel
#define HID_KEYPAD_EXCLAMATION_POINT 0xCF  // Sel
#define HID_KEYPAD_MEMORY_STORE      0xD0  // Sel
#define HID_KEYPAD_MEMORY_RECALL     0xD1  // Sel
#define HID_KEYPAD_MEMORY_CLEAR      0xD2  // Sel
#define HID_KEYPAD_MEMORY_ADD        0xD3  // Sel
#define HID_KEYPAD_MEMORY_SUBTRACT   0xD4  // Sel
#define HID_KEYPAD_MEMORY_MULTIPLY   0xD5  // Sel
#define HID_KEYPAD_MEMORY_DIVIDE     0xD6  // Sel
#define HID_KEYPAD_PLUS_SLASH_MINUS  0xD7  // Sel
#define HID_KEYPAD_CLEAR             0xD8  // Sel
#define HID_KEYPAD_CLEAR_ENTRY       0xD9  // Sel
#define HID_KEYPAD_BINARY            0xDA  // Sel
#define HID_KEYPAD_OCTAL             0xDB  // Sel
#define HID_KEYPAD_DECIMAL           0xDC  // Sel
#define HID_KEYPAD_HEXADECIMAL       0xDD  // Sel
// Reserved							0xDE-DF
#define HID_KEYBOARD_LEFT_CONTROL  0xE0  // DV
#define HID_KEYBOARD_LEFT_SHIFT    0xE1  // DV
#define HID_KEYBOARD_LEFT_ALT      0xE2  // DV
#define HID_KEYBOARD_LEFT_GUI      0xE3  // DV
#define HID_KEYBOARD_RIGHT_CONTROL 0xE4  // DV
#define HID_KEYBOARD_RIGHT_SHIFT   0xE5  // DV
#define HID_KEYBOARD_RIGHT_ALT     0xE6  // DV
#define HID_KEYBOARD_RIGHT_GUI     0xE7  // DV
// Reserved							0xE8-FFFF


// ==============================================================================
// Consumer Control USB HID keycodes
// [Section 15: Consumer Page (0x0C)]

// ----|-------------------------------------------------------|-------|---------
//	Name							Value	Usage Type
// ----|-------------------------------------------------------|-------|---------
#define HID_CONSUMER_CONSUMER_CONTROL_CA  0x01  // CA
#define HID_CONSUMER_NUMERIC_KEY_PAD      0x02  // NAry
#define HID_CONSUMER_PROGRAMMABLE_BUTTONS 0x03  // NAry
#define HID_CONSUMER_MICROPHONE_CA        0x04
#define HID_CONSUMER_HEADPHONE_CA         0x05
#define HID_CONSUMER_GRAPHIC_EQUALIZER_CA 0x06
// Reserved							0x07-1F
#define HID_CONSUMER_PLUS_10     0x20  // OSC
#define HID_CONSUMER_PLUS_100    0x21  // OSC
#define HID_CONSUMER_AM_SLASH_PM 0x22  // OSC
// Reserved							0x23-3F
#define HID_CONSUMER_POWER            0x30  // OOC
#define HID_CONSUMER_RESET            0x31  // OSC
#define HID_CONSUMER_SLEEP            0x32  // OSC
#define HID_CONSUMER_SLEEP_AFTER      0x33  // OSC
#define HID_CONSUMER_SLEEP_MODE       0x34  // RTC
#define HID_CONSUMER_ILLUMINATION     0x35  // OOC
#define HID_CONSUMER_FUNCTION_BUTTONS 0x36  // NAry
// Reserved							0x37-3F
#define HID_CONSUMER_MENU                0x40  // OOC
#define HID_CONSUMER_MENU_PICK           0x41  // OSC
#define HID_CONSUMER_MENU_UP             0x42  // OSC
#define HID_CONSUMER_MENU_DOWN           0x43  // OSC
#define HID_CONSUMER_MENU_LEFT           0x44  // OSC
#define HID_CONSUMER_MENU_RIGHT          0x45  // OSC
#define HID_CONSUMER_MENU_ESCAPE         0x46  // OSC
#define HID_CONSUMER_MENU_VALUE_INCREASE 0x47  // OSC
#define HID_CONSUMER_MENU_VALUE_DECREASE 0x48  // OSC
// Reserved							0x49-5F
#define HID_CONSUMER_DATA_ON_SCREEN                    0x60  // OOC
#define HID_CONSUMER_CLOSED_CAPTION                    0x61  // OOC
#define HID_CONSUMER_CLOSED_CAPTION_SELECT             0x62  // OSC
#define HID_CONSUMER_VCR_SLASH_TV                      0x63  // OOC
#define HID_CONSUMER_BROADCAST_MODE                    0x64  // OSC
#define HID_CONSUMER_SNAPSHOT                          0x65  // OSC
#define HID_CONSUMER_STILL                             0x66  // OSC
#define HID_CONSUMER_PICTURE_IN_PICTURE_TOGGLE         0x67  // OSC
#define HID_CONSUMER_PICTURE_IN_PICTURE_SWAP           0x68  // OSC
#define HID_CONSUMER_RED_MENU_BUTTON                   0x69  // MC
#define HID_CONSUMER_GREEN_MENU_BUTTON                 0x6A  // MC
#define HID_CONSUMER_BLUE_MENU_BUTTON                  0x6B  // MC
#define HID_CONSUMER_YELLOW_MENU_BUTTON                0x6C  // MC
#define HID_CONSUMER_ASPECT                            0x6D  // OSC
#define HID_CONSUMER_3D_MODE_SELECT                    0x6E  // OSC
#define HID_CONSUMER_DISPLAY_BRIGHTNESS_INCREMENT      0x6F  // RTC
#define HID_CONSUMER_DISPLAY_BRIGHTNESS_DECREMENT      0x70  // RTC
#define HID_CONSUMER_DISPLAY_BRIGHTNESS                0x71  // LC
#define HID_CONSUMER_DISPLAY_BACKLIGHT_TOGGLE          0x72  // OOC
#define HID_CONSUMER_DISPLAY_SET_BRIGHTNESS_TO_MINIMUM 0x73  // OSC
#define HID_CONSUMER_DISPLAY_SET_BRIGHTNESS_TO_MAXIMUM 0x74  // OSC
#define HID_CONSUMER_DISPLAY_SET_AUTO_BRIGHTNESS       0x75  // OOC
#define HID_CONSUMER_CAMERA_ACCESS_ENABLED             0x76  // OOC
#define HID_CONSUMER_CAMERA_ACCESS_DISABLED            0x77  // OOC
#define HID_CONSUMER_CAMERA_ACCESS_TOGGLE              0x78  // OOC
#define HID_CONSUMER_KEYBOARD_BRIGHTNESS_INCREMENT     0x79  // OSC
#define HID_CONSUMER_KEYBOARD_BRIGHTNESS_DECREMENT     0x7A  // OSC
#define HID_CONSUMER_KEYBOARD_BACKLIGHT_SET_LEVEL      0x7B  // OC
#define HID_CONSUMER_KEYBOARD_BACKLIGHT_OOC            0x7C  // OOC
#define HID_CONSUMER_KEYBOARD_BACKLIGHT_SET_MINIMUM    0x7D  // OSC
#define HID_CONSUMER_KEYBOARD_BACKLIGHT_SET_MAXIMUM    0x7E  // OSC
#define HID_CONSUMER_KEYBOARD_BACKLIGHT_AUTO           0x7F  // OOC
#define HID_CONSUMER_SELECTION                         0x80  // NAry
#define HID_CONSUMER_ASSIGN_SELECTION                  0x81  // OSC
#define HID_CONSUMER_MODE_STEP                         0x82  // OSC
#define HID_CONSUMER_RECALL_LAST                       0x83  // OSC
#define HID_CONSUMER_ENTER_CHANNEL                     0x84  // OSC
#define HID_CONSUMER_ORDER_MOVIE                       0x85  // OSC
#define HID_CONSUMER_CHANNEL                           0x86  // LC
#define HID_CONSUMER_MEDIA_SELECTION                   0x87  // NAry
#define HID_CONSUMER_MEDIA_SELECT_COMPUTER             0x88  // Sel
#define HID_CONSUMER_MEDIA_SELECT_TV                   0x89  // Sel
#define HID_CONSUMER_MEDIA_SELECT_WWW                  0x8A  // Sel
#define HID_CONSUMER_MEDIA_SELECT_DVD                  0x8B  // Sel
#define HID_CONSUMER_MEDIA_SELECT_TELEPHONE            0x8C  // Sel
#define HID_CONSUMER_MEDIA_SELECT_PROGRAM_GUIDE        0x8D  // Sel
#define HID_CONSUMER_MEDIA_SELECT_VIDEO_PHONE          0x8E  // Sel
#define HID_CONSUMER_MEDIA_SELECT_GAMES                0x8F  // Sel
#define HID_CONSUMER_MEDIA_SELECT_MESSAGES             0x90  // Sel
#define HID_CONSUMER_MEDIA_SELECT_CD                   0x91  // Sel
#define HID_CONSUMER_MEDIA_SELECT_VCR                  0x92  // Sel
#define HID_CONSUMER_MEDIA_SELECT_TUNER                0x93  // Sel
#define HID_CONSUMER_QUIT                              0x94  // OSC
#define HID_CONSUMER_HELP                              0x95  // OOC
#define HID_CONSUMER_MEDIA_SELECT_TAPE                 0x96  // Sel
#define HID_CONSUMER_MEDIA_SELECT_CABLE                0x97  // Sel
#define HID_CONSUMER_MEDIA_SELECT_SATELLITE            0x98  // Sel
#define HID_CONSUMER_MEDIA_SELECT_SECURITY             0x99  // Sel
#define HID_CONSUMER_MEDIA_SELECT_HOME                 0x9A  // Sel
#define HID_CONSUMER_MEDIA_SELECT_CALL                 0x9B  // Sel
#define HID_CONSUMER_CHANNEL_INCREMENT                 0x9C  // OSC
#define HID_CONSUMER_CHANNEL_DECREMENT                 0x9D  // OSC
#define HID_CONSUMER_MEDIA_SELECT_SAP                  0x9E  // Sel
// Reserved							0x9F
#define HID_CONSUMER_VCR_PLUS 0xA0  // OSC
#define HID_CONSUMER_ONCE     0xA1  // OSC
#define HID_CONSUMER_DAILY    0xA2  // OSC
#define HID_CONSUMER_WEEKLY   0xA3  // OSC
#define HID_CONSUMER_MONTHLY  0xA4  // OSC
// Reserved							0xA5-AF
#define HID_CONSUMER_PLAY                             0xB0  // OOC
#define HID_CONSUMER_PAUSE                            0xB1  // OOC
#define HID_CONSUMER_RECORD                           0xB2  // OOC
#define HID_CONSUMER_FAST_FORWARD                     0xB3  // OOC
#define HID_CONSUMER_REWIND                           0xB4  // OOC
#define HID_CONSUMER_SCAN_NEXT_TRACK                  0xB5  // OSC
#define HID_CONSUMER_SCAN_PREVIOUS_TRACK              0xB6  // OSC
#define HID_CONSUMER_STOP                             0xB7  // OSC
#define HID_CONSUMER_EJECT                            0xB8  // OSC
#define HID_CONSUMER_RANDOM_PLAY                      0xB9  // OOC
#define HID_CONSUMER_SELECT_DISC                      0xBA  // NAry
#define HID_CONSUMER_ENTER_DISC_MC                    0xBB
#define HID_CONSUMER_REPEAT                           0xBC  // OSC
#define HID_CONSUMER_TRACKING                         0xBD  // LC
#define HID_CONSUMER_TRACK_NORMAL                     0xBE  // OSC
#define HID_CONSUMER_SLOW_TRACKING                    0xBF  // LC
#define HID_CONSUMER_FRAME_FORWARD                    0xC0  // RTC
#define HID_CONSUMER_FRAME_BACK                       0xC1  // RTC
#define HID_CONSUMER_MARK                             0xC2  // OSC
#define HID_CONSUMER_CLEAR_MARK                       0xC3  // OSC
#define HID_CONSUMER_REPEAT_FROM_MARK                 0xC4  // OOC
#define HID_CONSUMER_RETURN_TO_MARK                   0xC5  // OSC
#define HID_CONSUMER_SEARCH_MARK_FORWARD              0xC6  // OSC
#define HID_CONSUMER_SEARCH_MARK_BACKWARDS            0xC7  // OSC
#define HID_CONSUMER_COUNTER_RESET                    0xC8  // OSC
#define HID_CONSUMER_SHOW_COUNTER                     0xC9  // OSC
#define HID_CONSUMER_TRACKING_INCREMENT               0xCA  // RTC
#define HID_CONSUMER_TRACKING_DECREMENT               0xCB  // RTC
#define HID_CONSUMER_STOP_SLASH_EJECT                 0xCC  // OSC
#define HID_CONSUMER_PLAY_SLASH_PAUSE                 0xCD  // OSC
#define HID_CONSUMER_PLAY_SLASH_SKIP                  0xCE  // OSC
#define HID_CONSUMER_VOICE_COMMAND                    0xCF  // Sel
#define HID_CONSUMER_INVOKE_CAPTURE_INTERFACE         0xD0  // Sel
#define HID_CONSUMER_START_OR_STOP_GAME_RECORDING     0xD1  // Sel
#define HID_CONSUMER_HISTORICAL_GAME_CAPTURE          0xD2  // Sel
#define HID_CONSUMER_CAPTURE_GAME_SCREENSHOT          0xD3  // Sel
#define HID_CONSUMER_SHOW_OR_HIDE_RECORDING_INDICATOR 0xD4  // Sel
#define HID_CONSUMER_START_OR_STOP_MICROPHONE_CAPTURE 0xD5  // Sel
#define HID_CONSUMER_START_OR_STOP_CAMERA_CAPTURE     0xD6  // Sel
#define HID_CONSUMER_START_OR_STOP_GAME_BROADCAST     0xD7  // Sel
// Reserved							0xD8-DF
#define HID_CONSUMER_VOLUME           0xE0  // LC
#define HID_CONSUMER_BALANCE          0xE1  // LC
#define HID_CONSUMER_MUTE             0xE2  // OOC
#define HID_CONSUMER_BASS             0xE3  // LC
#define HID_CONSUMER_TREBLE           0xE4  // LC
#define HID_CONSUMER_BASS_BOOST       0xE5  // OOC
#define HID_CONSUMER_SURROUND_MODE    0xE6  // OSC
#define HID_CONSUMER_LOUDNESS         0xE7  // OOC
#define HID_CONSUMER_MPX              0xE8  // OOC
#define HID_CONSUMER_VOLUME_INCREMENT 0xE9  // RTC
#define HID_CONSUMER_VOLUME_DECREMENT 0xEA  // RTC
// Reserved							0xEB-EF
#define HID_CONSUMER_SPEED_SELECT   0xF0  // OSC
#define HID_CONSUMER_PLAYBACK_SPEED 0xF1  // NAry
#define HID_CONSUMER_STANDARD_PLAY  0xF2  // Sel
#define HID_CONSUMER_LONG_PLAY      0xF3  // Sel
#define HID_CONSUMER_EXTENDED_PLAY  0xF4  // Sel
#define HID_CONSUMER_SLOW           0xF5  // OSC
// Reserved							0xF6-FF
#define HID_CONSUMER_FAN_ENABLE               0x100  // OOC
#define HID_CONSUMER_FAN_SPEED                0x101  // LC
#define HID_CONSUMER_LIGHT_ENABLE             0x102  // OOC
#define HID_CONSUMER_LIGHT_ILLUMINATION_LEVEL 0x103  // LC
#define HID_CONSUMER_CLIMATE_CONTROL_ENABLE   0x104  // OOC
#define HID_CONSUMER_ROOM_TEMPERATURE         0x105  // LC
#define HID_CONSUMER_SECURITY_ENABLE          0x106  // OOC
#define HID_CONSUMER_FIRE_ALARM               0x107  // OSC
#define HID_CONSUMER_POLICE_ALARM             0x108  // OSC
#define HID_CONSUMER_PROXIMITY                0x109  // LC
#define HID_CONSUMER_MOTION                   0x10A  // OSC
#define HID_CONSUMER_DURESS_ALARM             0x10B  // OSC
#define HID_CONSUMER_HOLDUP_ALARM             0x10C  // OSC
#define HID_CONSUMER_MEDICAL_ALARM            0x10D  // OSC
// Reserved							0x10E-14F
#define HID_CONSUMER_BALANCE_RIGHT    0x150  // RTC
#define HID_CONSUMER_BALANCE_LEFT     0x151  // RTC
#define HID_CONSUMER_BASS_INCREMENT   0x152  // RTC
#define HID_CONSUMER_BASS_DECREMENT   0x153  // RTC
#define HID_CONSUMER_TREBLE_INCREMENT 0x154  // RTC
#define HID_CONSUMER_TREBLE_DECREMENT 0x155  // RTC
// Reserved							0x156-15F
#define HID_CONSUMER_SPEAKER_SYSTEM                    0x160  // CL
#define HID_CONSUMER_CHANNEL_LEFT                      0x161  // CL
#define HID_CONSUMER_CHANNEL_RIGHT                     0x162  // CL
#define HID_CONSUMER_CHANNEL_CENTER                    0x163  // CL
#define HID_CONSUMER_CHANNEL_FRONT                     0x164  // CL
#define HID_CONSUMER_CHANNEL_CENTER_FRONT              0x165  // CL
#define HID_CONSUMER_CHANNEL_SIDE                      0x166  // CL
#define HID_CONSUMER_CHANNEL_SURROUND                  0x167  // CL
#define HID_CONSUMER_CHANNEL_LOW_FREQUENCY_ENHANCEMENT 0x168  // CL
#define HID_CONSUMER_CHANNEL_TOP                       0x169  // CL
#define HID_CONSUMER_CHANNEL_UNKNOWN                   0x16A  // CL
// Reserved							0x16B-16F
#define HID_CONSUMER_SUB_CHANNEL               0x170  // LC
#define HID_CONSUMER_SUB_CHANNEL_INCREMENT     0x171  // OSC
#define HID_CONSUMER_SUB_CHANNEL_DECREMENT     0x172  // OSC
#define HID_CONSUMER_ALTERNATE_AUDIO_INCREMENT 0x173  // OSC
#define HID_CONSUMER_ALTERNATE_AUDIO_DECREMENT 0x174  // OSC
// Reserved							0x175-17F
#define HID_CONSUMER_APPLICATION_LAUNCH_BUTTONS                0x180  // NAry
#define HID_CONSUMER_AL_LAUNCH_BUTTON_CONFIGURATION_TOOL       0x181  // Sel
#define HID_CONSUMER_AL_PROGRAMMABLE_BUTTON_CONFIGURATION      0x182  // Sel
#define HID_CONSUMER_AL_CONSUMER_CONTROL_CONFIGURATION         0x183  // Sel
#define HID_CONSUMER_AL_WORD_PROCESSOR                         0x184  // Sel
#define HID_CONSUMER_AL_TEXT_EDITOR                            0x185  // Sel
#define HID_CONSUMER_AL_SPREADSHEET                            0x186  // Sel
#define HID_CONSUMER_AL_GRAPHICS_EDITOR                        0x187  // Sel
#define HID_CONSUMER_AL_PRESENTATION_APP                       0x188  // Sel
#define HID_CONSUMER_AL_DATABASE_APP                           0x189  // Sel
#define HID_CONSUMER_AL_EMAIL_READER                           0x18A  // Sel
#define HID_CONSUMER_AL_NEWSREADER                             0x18B  // Sel
#define HID_CONSUMER_AL_VOICEMAIL                              0x18C  // Sel
#define HID_CONSUMER_AL_CONTACTS_SLASH_ADDRESS_BOOK            0x18D  // Sel
#define HID_CONSUMER_AL_CALENDAR_SLASH_SCHEDULE                0x18E  // Sel
#define HID_CONSUMER_AL_TASK_SLASH_PROJECT_MANAGER             0x18F  // Sel
#define HID_CONSUMER_AL_LOG_SLASH_JOURNAL_SLASH_TIMECARD       0x190  // Sel
#define HID_CONSUMER_AL_CHECKBOOK_SLASH_FINANCE                0x191  // Sel
#define HID_CONSUMER_AL_CALCULATOR                             0x192  // Sel
#define HID_CONSUMER_AL_A_SLASH_V_CAPTURE_SLASH_PLAYBACK       0x193  // Sel
#define HID_CONSUMER_AL_LOCAL_MACHINE_BROWSER                  0x194  // Sel
#define HID_CONSUMER_AL_LAN_SLASH_WAN_BROWSER                  0x195  // Sel
#define HID_CONSUMER_AL_INTERNET_BROWSER                       0x196  // Sel
#define HID_CONSUMER_AL_REMOTE_NETWORKING_SLASH_ISP_CONNECT    0x197  // Sel
#define HID_CONSUMER_AL_NETWORK_CONFERENCE                     0x198  // Sel
#define HID_CONSUMER_AL_NETWORK_CHAT                           0x199  // Sel
#define HID_CONSUMER_AL_TELEPHONY_SLASH_DIALER                 0x19A  // Sel
#define HID_CONSUMER_AL_LOGON                                  0x19B  // Sel
#define HID_CONSUMER_AL_LOGOFF                                 0x19C  // Sel
#define HID_CONSUMER_AL_LOGON_SLASH_LOGOFF                     0x19D  // Sel
#define HID_CONSUMER_AL_TERMINAL_LOCK_SLASH_SCREENSAVER        0x19E  // Sel
#define HID_CONSUMER_AL_CONTROL_PANEL                          0x19F  // Sel
#define HID_CONSUMER_AL_COMMAND_LINE_PROCESSOR_SLASH_RUN       0x1A0  // Sel
#define HID_CONSUMER_AL_PROCESS_SLASH_TASK_MANAGER             0x1A1  // Sel
#define HID_CONSUMER_AL_SELECT_TASK_SLASH_APPLICATION          0x1A2  // Sel
#define HID_CONSUMER_AL_NEXT_TASK_SLASH_APPLICATION            0x1A3  // Sel
#define HID_CONSUMER_AL_PREVIOUS_TASK_SLASH_APPLICATION        0x1A4  // Sel
#define HID_CONSUMER_AL_PREEMPTIVE_HALT_TASK_SLASH_APPLICATION 0x1A5  // Sel
#define HID_CONSUMER_AL_INTEGRATED_HELP_CENTER                 0x1A6  // Sel
#define HID_CONSUMER_AL_DOCUMENTS                              0x1A7  // Sel
#define HID_CONSUMER_AL_THESAURUS                              0x1A8  // Sel
#define HID_CONSUMER_AL_DICTIONARY                             0x1A9  // Sel
#define HID_CONSUMER_AL_DESKTOP                                0x1AA  // Sel
#define HID_CONSUMER_AL_SPELL_CHECK                            0x1AB  // Sel
#define HID_CONSUMER_AL_GRAMMAR_CHECK                          0x1AC  // Sel
#define HID_CONSUMER_AL_WIRELESS_STATUS                        0x1AD  // Sel
#define HID_CONSUMER_AL_KEYBOARD_LAYOUT                        0x1AE  // Sel
#define HID_CONSUMER_AL_VIRUS_PROTECTION                       0x1AF  // Sel
#define HID_CONSUMER_AL_ENCRYPTION                             0x1B0  // Sel
#define HID_CONSUMER_AL_SCREEN_SAVER                           0x1B1  // Sel
#define HID_CONSUMER_AL_ALARMS                                 0x1B2  // Sel
#define HID_CONSUMER_AL_CLOCK                                  0x1B3  // Sel
#define HID_CONSUMER_AL_FILE_BROWSER                           0x1B4  // Sel
#define HID_CONSUMER_AL_POWER_STATUS                           0x1B5  // Sel
#define HID_CONSUMER_AL_IMAGE_BROWSER                          0x1B6  // Sel
#define HID_CONSUMER_AL_AUDIO_BROWSER                          0x1B7  // Sel
#define HID_CONSUMER_AL_MOVIE_BROWSER                          0x1B8  // Sel
#define HID_CONSUMER_AL_DIGITAL_RIGHTS_MANAGER                 0x1B9  // Sel
#define HID_CONSUMER_AL_DIGITAL_WALLET                         0x1BA  // Sel
// _Reserved							0x1BB
#define HID_CONSUMER_AL_INSTANT_MESSAGING 0x1BC  // Sel
#define HID_CONSUMER_AL_OEM_FEATURES_SLASH_TIPS_SLASH_TUTORIAL_BROWSER \
  0x1BD                                                             // Sel
#define HID_CONSUMER_AL_OEM_HELP                             0x1BE  // Sel
#define HID_CONSUMER_AL_ONLINE_COMMUNITY                     0x1BF  // Sel
#define HID_CONSUMER_AL_ENTERTAINMENT_CONTENT_BROWSER        0x1C0  // Sel
#define HID_CONSUMER_AL_ONLINE_SHOPPING_BROWSER              0x1C1  // Sel
#define HID_CONSUMER_AL_SMARTCARD_INFORMATION_SLASH_HELP     0x1C2  // Sel
#define HID_CONSUMER_AL_MARKET_MONITOR_SLASH_FINANCE_BROWSER 0x1C3  // Sel
#define HID_CONSUMER_AL_CUSTOMIZED_CORPORATE_NEWS_BROWSER    0x1C4  // Sel
#define HID_CONSUMER_AL_ONLINE_ACTIVITY_BROWSER              0x1C5  // Sel
#define HID_CONSUMER_AL_RESEARCH_SLASH_SEARCH_BROWSER        0x1C6  // Sel
#define HID_CONSUMER_AL_AUDIO_PLAYER                         0x1C7  // Sel
#define HID_CONSUMER_AL_MESSAGE_STATUS                       0x1C8  // Sel
#define HID_CONSUMER_AL_CONTACT_SYNC                         0x1C9  // Sel
#define HID_CONSUMER_AL_NAVIGATION                           0x1CA  // Sel
#define HID_CONSUMER_AL_CONTEXT_AWARE_DESKTOP_ASSISTANT      0x1CB  // Sel
// Reserved							0x1CC-1FF
#define HID_CONSUMER_GENERIC_GUI_APPLICATION_CONTROLS 0x200  // NAry
#define HID_CONSUMER_AC_NEW                           0x201  // Sel
#define HID_CONSUMER_AC_OPEN                          0x202  // Sel
#define HID_CONSUMER_AC_CLOSE                         0x203  // Sel
#define HID_CONSUMER_AC_EXIT                          0x204  // Sel
#define HID_CONSUMER_AC_MAXIMIZE                      0x205  // Sel
#define HID_CONSUMER_AC_MINIMIZE                      0x206  // Sel
#define HID_CONSUMER_AC_SAVE                          0x207  // Sel
#define HID_CONSUMER_AC_PRINT                         0x208  // Sel
#define HID_CONSUMER_AC_PROPERTIES                    0x209  // Sel
// Reserved							0x20A-219
#define HID_CONSUMER_AC_UNDO                          0x21A  // Sel
#define HID_CONSUMER_AC_COPY                          0x21B  // Sel
#define HID_CONSUMER_AC_CUT                           0x21C  // Sel
#define HID_CONSUMER_AC_PASTE                         0x21D  // Sel
#define HID_CONSUMER_AC_SELECT_ALL                    0x21E  // Sel
#define HID_CONSUMER_AC_FIND                          0x21F  // Sel
#define HID_CONSUMER_AC_FIND_AND_REPLACE              0x220  // Sel
#define HID_CONSUMER_AC_SEARCH                        0x221  // Sel
#define HID_CONSUMER_AC_GO_TO                         0x222  // Sel
#define HID_CONSUMER_AC_HOME                          0x223  // Sel
#define HID_CONSUMER_AC_BACK                          0x224  // Sel
#define HID_CONSUMER_AC_FORWARD                       0x225  // Sel
#define HID_CONSUMER_AC_STOP                          0x226  // Sel
#define HID_CONSUMER_AC_REFRESH                       0x227  // Sel
#define HID_CONSUMER_AC_PREVIOUS_LINK                 0x228  // Sel
#define HID_CONSUMER_AC_NEXT_LINK                     0x229  // Sel
#define HID_CONSUMER_AC_BOOKMARKS                     0x22A  // Sel
#define HID_CONSUMER_AC_HISTORY                       0x22B  // Sel
#define HID_CONSUMER_AC_SUBSCRIPTIONS                 0x22C  // Sel
#define HID_CONSUMER_AC_ZOOM_IN                       0x22D  // Sel
#define HID_CONSUMER_AC_ZOOM_OUT                      0x22E  // Sel
#define HID_CONSUMER_AC_ZOOM                          0x22F  // LC
#define HID_CONSUMER_AC_FULL_SCREEN_VIEW              0x230  // Sel
#define HID_CONSUMER_AC_NORMAL_VIEW                   0x231  // Sel
#define HID_CONSUMER_AC_VIEW_TOGGLE                   0x232  // Sel
#define HID_CONSUMER_AC_SCROLL_UP                     0x233  // Sel
#define HID_CONSUMER_AC_SCROLL_DOWN                   0x234  // Sel
#define HID_CONSUMER_AC_SCROLL                        0x235  // LC
#define HID_CONSUMER_AC_PAN_LEFT                      0x236  // Sel
#define HID_CONSUMER_AC_PAN_RIGHT                     0x237  // Sel
#define HID_CONSUMER_AC_PAN                           0x238  // LC
#define HID_CONSUMER_AC_NEW_WINDOW                    0x239  // Sel
#define HID_CONSUMER_AC_TILE_HORIZONTALLY             0x23A  // Sel
#define HID_CONSUMER_AC_TILE_VERTICALLY               0x23B  // Sel
#define HID_CONSUMER_AC_FORMAT                        0x23C  // Sel
#define HID_CONSUMER_AC_EDIT                          0x23D  // Sel
#define HID_CONSUMER_AC_BOLD                          0x23E  // Sel
#define HID_CONSUMER_AC_ITALICS                       0x23F  // Sel
#define HID_CONSUMER_AC_UNDERLINE                     0x240  // Sel
#define HID_CONSUMER_AC_STRIKETHROUGH                 0x241  // Sel
#define HID_CONSUMER_AC_SUBSCRIPT                     0x242  // Sel
#define HID_CONSUMER_AC_SUPERSCRIPT                   0x243  // Sel
#define HID_CONSUMER_AC_ALL_CAPS                      0x244  // Sel
#define HID_CONSUMER_AC_ROTATE                        0x245  // Sel
#define HID_CONSUMER_AC_RESIZE                        0x246  // Sel
#define HID_CONSUMER_AC_FLIP_HORIZONTAL               0x247  // Sel
#define HID_CONSUMER_AC_FLIP_VERTICAL                 0x248  // Sel
#define HID_CONSUMER_AC_MIRROR_HORIZONTAL             0x249  // Sel
#define HID_CONSUMER_AC_MIRROR_VERTICAL               0x24A  // Sel
#define HID_CONSUMER_AC_FONT_SELECT                   0x24B  // Sel
#define HID_CONSUMER_AC_FONT_COLOR                    0x24C  // Sel
#define HID_CONSUMER_AC_FONT_SIZE                     0x24D  // Sel
#define HID_CONSUMER_AC_JUSTIFY_LEFT                  0x24E  // Sel
#define HID_CONSUMER_AC_JUSTIFY_CENTER_H              0x24F  // Sel
#define HID_CONSUMER_AC_JUSTIFY_RIGHT                 0x250  // Sel
#define HID_CONSUMER_AC_JUSTIFY_BLOCK_H               0x251  // Sel
#define HID_CONSUMER_AC_JUSTIFY_TOP                   0x252  // Sel
#define HID_CONSUMER_AC_JUSTIFY_CENTER_V              0x253  // Sel
#define HID_CONSUMER_AC_JUSTIFY_BOTTOM                0x254  // Sel
#define HID_CONSUMER_AC_JUSTIFY_BLOCK_V               0x255  // Sel
#define HID_CONSUMER_AC_INDENT_DECREASE               0x256  // Sel
#define HID_CONSUMER_AC_INDENT_INCREASE               0x257  // Sel
#define HID_CONSUMER_AC_NUMBERED_LIST                 0x258  // Sel
#define HID_CONSUMER_AC_RESTART_NUMBERING             0x259  // Sel
#define HID_CONSUMER_AC_BULLETED_LIST                 0x25A  // Sel
#define HID_CONSUMER_AC_PROMOTE                       0x25B  // Sel
#define HID_CONSUMER_AC_DEMOTE                        0x25C  // Sel
#define HID_CONSUMER_AC_YES                           0x25D  // Sel
#define HID_CONSUMER_AC_NO                            0x25E  // Sel
#define HID_CONSUMER_AC_CANCEL                        0x25F  // Sel
#define HID_CONSUMER_AC_CATALOG                       0x260  // Sel
#define HID_CONSUMER_AC_BUY_SLASH_CHECKOUT            0x261  // Sel
#define HID_CONSUMER_AC_ADD_TO_CART                   0x262  // Sel
#define HID_CONSUMER_AC_EXPAND                        0x263  // Sel
#define HID_CONSUMER_AC_EXPAND_ALL                    0x264  // Sel
#define HID_CONSUMER_AC_COLLAPSE                      0x265  // Sel
#define HID_CONSUMER_AC_COLLAPSE_ALL                  0x266  // Sel
#define HID_CONSUMER_AC_PRINT_PREVIEW                 0x267  // Sel
#define HID_CONSUMER_AC_PASTE_SPECIAL                 0x268  // Sel
#define HID_CONSUMER_AC_INSERT_MODE                   0x269  // Sel
#define HID_CONSUMER_AC_DELETE                        0x26A  // Sel
#define HID_CONSUMER_AC_LOCK                          0x26B  // Sel
#define HID_CONSUMER_AC_UNLOCK                        0x26C  // Sel
#define HID_CONSUMER_AC_PROTECT                       0x26D  // Sel
#define HID_CONSUMER_AC_UNPROTECT                     0x26E  // Sel
#define HID_CONSUMER_AC_ATTACH_COMMENT                0x26F  // Sel
#define HID_CONSUMER_AC_DELETE_COMMENT                0x270  // Sel
#define HID_CONSUMER_AC_VIEW_COMMENT                  0x271  // Sel
#define HID_CONSUMER_AC_SELECT_WORD                   0x272  // Sel
#define HID_CONSUMER_AC_SELECT_SENTENCE               0x273  // Sel
#define HID_CONSUMER_AC_SELECT_PARAGRAPH              0x274  // Sel
#define HID_CONSUMER_AC_SELECT_COLUMN                 0x275  // Sel
#define HID_CONSUMER_AC_SELECT_ROW                    0x276  // Sel
#define HID_CONSUMER_AC_SELECT_TABLE                  0x277  // Sel
#define HID_CONSUMER_AC_SELECT_OBJECT                 0x278  // Sel
#define HID_CONSUMER_AC_REDO_SLASH_REPEAT             0x279  // Sel
#define HID_CONSUMER_AC_SORT                          0x27A  // Sel
#define HID_CONSUMER_AC_SORT_ASCENDING                0x27B  // Sel
#define HID_CONSUMER_AC_SORT_DESCENDING               0x27C  // Sel
#define HID_CONSUMER_AC_FILTER                        0x27D  // Sel
#define HID_CONSUMER_AC_SET_CLOCK                     0x27E  // Sel
#define HID_CONSUMER_AC_VIEW_CLOCK                    0x27F  // Sel
#define HID_CONSUMER_AC_SELECT_TIME_ZONE              0x280  // Sel
#define HID_CONSUMER_AC_EDIT_TIME_ZONES               0x281  // Sel
#define HID_CONSUMER_AC_SET_ALARM                     0x282  // Sel
#define HID_CONSUMER_AC_CLEAR_ALARM                   0x283  // Sel
#define HID_CONSUMER_AC_SNOOZE_ALARM                  0x284  // Sel
#define HID_CONSUMER_AC_RESET_ALARM                   0x285  // Sel
#define HID_CONSUMER_AC_SYNCHRONIZE                   0x286  // Sel
#define HID_CONSUMER_AC_SEND_SLASH_RECEIVE            0x287  // Sel
#define HID_CONSUMER_AC_SEND_TO                       0x288  // Sel
#define HID_CONSUMER_AC_REPLY                         0x289  // Sel
#define HID_CONSUMER_AC_REPLY_ALL                     0x28A  // Sel
#define HID_CONSUMER_AC_FORWARD_MSG                   0x28B  // Sel
#define HID_CONSUMER_AC_SEND                          0x28C  // Sel
#define HID_CONSUMER_AC_ATTACH_FILE                   0x28D  // Sel
#define HID_CONSUMER_AC_UPLOAD                        0x28E  // Sel
#define HID_CONSUMER_AC_DOWNLOAD                      0x28F  // Sel
#define HID_CONSUMER_AC_SET_BORDERS                   0x290  // Sel
#define HID_CONSUMER_AC_INSERT_ROW                    0x291  // Sel
#define HID_CONSUMER_AC_INSERT_COLUMN                 0x292  // Sel
#define HID_CONSUMER_AC_INSERT_FILE                   0x293  // Sel
#define HID_CONSUMER_AC_INSERT_PICTURE                0x294  // Sel
#define HID_CONSUMER_AC_INSERT_OBJECT                 0x295  // Sel
#define HID_CONSUMER_AC_INSERT_SYMBOL                 0x296  // Sel
#define HID_CONSUMER_AC_SAVE_AND_CLOSE                0x297  // Sel
#define HID_CONSUMER_AC_RENAME                        0x298  // Sel
#define HID_CONSUMER_AC_MERGE                         0x299  // Sel
#define HID_CONSUMER_AC_SPLIT                         0x29A  // Sel
#define HID_CONSUMER_AC_DISTRIBUTE_HORIZONTALLY       0x29B  // Sel
#define HID_CONSUMER_AC_DISTRIBUTE_VERTICALLY         0x29C  // Sel
#define HID_CONSUMER_AC_NEXT_KEYBOARD_LAYOUT_SELECT   0x29D  // Sel
#define HID_CONSUMER_AC_NAVIGATION_GUIDANCE           0x29E  // Sel
#define HID_CONSUMER_AC_DESKTOP_SHOW_ALL_WINDOWS      0x29F  // Sel
#define HID_CONSUMER_AC_SOFT_KEY_LEFT                 0x2A0  // Sel
#define HID_CONSUMER_AC_SOFT_KEY_RIGHT                0x2A1  // Sel
#define HID_CONSUMER_AC_DESKTOP_SHOW_ALL_APPLICATIONS 0x2A2  // Sel
// Reserved							0x2A3-2AF
#define HID_CONSUMER_AC_IDLE_KEEP_ALIVE 0x2B0  // Sel
// Reserved							0x2B1-2BF
#define HID_CONSUMER_EXTENDED_KEYBOARD_ATTRIBUTES_COLLECTION    0x2C0  // CL
#define HID_CONSUMER_KEYBOARD_FORM_FACTOR                       0x2C1  // SV
#define HID_CONSUMER_KEYBOARD_KEY_TYPE                          0x2C2  // SV
#define HID_CONSUMER_KEYBOARD_PHYSICAL_LAYOUT                   0x2C3  // SV
#define HID_CONSUMER_VENDOR_SPECIFIC_KEYBOARD_PHYSICAL_LAYOUT   0x2C4  // SV
#define HID_CONSUMER_KEYBOARD_IETF_LANGUAGE_TAG_INDEX           0x2C5  // SV
#define HID_CONSUMER_IMPLEMENTED_KEYBOARD_INPUT_ASSIST_CONTROLS 0x2C6  // SV
#define HID_CONSUMER_KEYBOARD_INPUT_ASSIST_PREVIOUS             0x2C7  // Sel
#define HID_CONSUMER_KEYBOARD_INPUT_ASSIST_NEXT                 0x2C8  // Sel
#define HID_CONSUMER_KEYBOARD_INPUT_ASSIST_PREVIOUS_GROUP       0x2C9  // Sel
#define HID_CONSUMER_KEYBOARD_INPUT_ASSIST_NEXT_GROUP           0x2CA  // Sel
#define HID_CONSUMER_KEYBOARD_INPUT_ASSIST_ACCEPT               0x2CB  // Sel
#define HID_CONSUMER_KEYBOARD_INPUT_ASSIST_CANCEL               0x2CC  // Sel
// Reserved							0x2CD-2CF
#define HID_CONSUMER_PRIVACY_SCREEN_TOGGLE          0x2D0  // OOC
#define HID_CONSUMER_PRIVACY_SCREEN_LEVEL_DECREMENT 0x2D1
#define HID_CONSUMER_PRIVACY_SCREEN_LEVEL_INCREMENT 0x2D2
#define HID_CONSUMER_PRIVACY_SCREEN_LEVEL_MINIMUM   0x2D3  // OSC
#define HID_CONSUMER_PRIVACY_SCREEN_LEVEL_MAXIMUM   0x2D4  // OSC
// Reserved							0x2D5-4FF
//	HID_CONTACT_* (not included here)			0x500-514
// Reserved							0x515-FFFF
//
#endif DOXYGEN_SHOULD_SKIP_THIS
