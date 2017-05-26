#pragma once


#include "HIDTables.h"

#include "key_defs_keyboard.h"
#include "key_defs_sysctl.h"
#include "key_defs_consumerctl.h"

#include "KeyboardioHID.h"


typedef union Key_ {

    struct {
        uint8_t keyCode;
        uint8_t flags;
    };
    uint16_t raw;

    inline bool operator==(uint16_t rhs) {
        return this->raw == rhs;
    };
    inline bool operator==(const Key_ rhs) {
        return this->raw == rhs.raw;
    };
    inline Key_& operator=(uint16_t raw) {
        this->raw = raw;
        return *this;
    };
    inline bool operator!=(const Key_& rhs) {
        return !(*this == rhs);
    };
    inline bool operator>=(uint16_t raw) {
        return this->raw >= raw;
    };
    inline bool operator<=(uint16_t raw) {
        return this->raw <= raw;
    };
    inline bool operator>(uint16_t raw) {
        return this->raw > raw;
    };
    inline bool operator<(uint16_t raw) {
        return this->raw < raw;
    };
    inline bool operator>=(const Key_& other) {
        return this->raw >= other.raw;
    };
    inline bool operator<=(const Key_& other) {
        return this->raw <= other.raw;
    };
    inline bool operator>(const Key_& other) {
        return this->raw > other.raw;
    };
    inline bool operator<(const Key_& other) {
        return this->raw < other.raw;
    };
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
#define SWITCH_TO_KEYMAP           B00000100
#define IS_INTERNAL                B00001000

/* HID types we need to encode in the key flags for system and consumer control hid controls
   Each key can only have one, so we don't need to use a bit vector.
   We need to keep the top two bits clear for defining the keys as synthetic
   We need to keep the bottom two bits clear for defining the keys as sysctl / consumerctl
*/

#define HID_TYPE_CL    B00000000
#define HID_TYPE_LC    B00000100
#define HID_TYPE_NARY  B00001000
#define HID_TYPE_OOC   B00001100
#define HID_TYPE_OSC   B00010000
#define HID_TYPE_RTC   B00010100
#define HID_TYPE_SEL   B00011000

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


#define Key_NoKey (Key) { 0,  KEY_FLAGS }
#define Key_skip (Key) { 0,  KEY_FLAGS }
#define Key_Transparent (Key){ .raw = 0xffff }
#define ___ Key_Transparent
#define XXX Key_NoKey




#define Key_LeftParen LSHIFT(Key_9)
#define Key_RightParen LSHIFT(Key_0)
#define Key_LeftCurlyBracket LSHIFT(Key_LeftBracket)
#define Key_RightCurlyBracket LSHIFT(Key_RightBracket)

#define Key_Pipe LSHIFT(Key_Backslash)



#define KEY_BACKLIGHT_DOWN 0xF1
#define KEY_BACKLIGHT_UP 0xF2
#define Key_BacklightDown (Key) { KEY_BACKLIGHT_DOWN,  KEY_FLAGS }
#define Key_BacklightUp (Key) { KEY_BACKLIGHT_UP,  KEY_FLAGS }
#define KEY_RIGHT_FN2 0xfe
#define Key_RFN2 (Key) { KEY_RIGHT_FN2,  KEY_FLAGS }
#define KEY_LEFT_FN2 0xff
#define Key_LFN2 (Key) { KEY_LEFT_FN2,  KEY_FLAGS }

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
