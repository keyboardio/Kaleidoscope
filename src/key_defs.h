#pragma once


#include "HIDTables.h"

#include "key_defs_keyboard.h"
#include "key_defs_sysctl.h"
#include "key_defs_consumerctl.h"
#include "key_defs_keymaps.h"

#include "key_defs_aliases.h"

#ifdef ARDUINO_VIRTUAL
#include "VirtualHID/VirtualHID.h"
#else
#include "KeyboardioHID.h"
#endif


typedef union Key_ {

  struct {
    uint8_t keyCode;
    uint8_t flags;
  };
  uint16_t raw;

  inline bool operator==(uint16_t rhs) {
    return this->raw == rhs;
  }
  inline bool operator==(const Key_ rhs) {
    return this->raw == rhs.raw;
  }
  inline Key_& operator=(uint16_t raw) {
    this->raw = raw;
    return *this;
  }
  inline bool operator!=(const Key_& rhs) {
    return !(*this == rhs);
  }
  inline bool operator>=(uint16_t raw) {
    return this->raw >= raw;
  }
  inline bool operator<=(uint16_t raw) {
    return this->raw <= raw;
  }
  inline bool operator>(uint16_t raw) {
    return this->raw > raw;
  }
  inline bool operator<(uint16_t raw) {
    return this->raw < raw;
  }
  inline bool operator>=(const Key_& other) {
    return this->raw >= other.raw;
  }
  inline bool operator<=(const Key_& other) {
    return this->raw <= other.raw;
  }
  inline bool operator>(const Key_& other) {
    return this->raw > other.raw;
  }
  inline bool operator<(const Key_& other) {
    return this->raw < other.raw;
  }
} Key;



#define KEY_FLAGS         uint8_t(B00000000)
#define CTRL_HELD         uint8_t(B00000001)
#define LALT_HELD         uint8_t(B00000010)
#define RALT_HELD         uint8_t(B00000100)
#define SHIFT_HELD        uint8_t(B00001000)
#define GUI_HELD          uint8_t(B00010000)
#define SYNTHETIC         uint8_t(B01000000)
#define RESERVED          uint8_t(B10000000)

#define LCTRL(k)  ((Key) { k.keyCode, uint8_t(k.flags | CTRL_HELD) })
#define LALT(k)   ((Key) { k.keyCode, uint8_t(k.flags | LALT_HELD) })
#define RALT(k)   ((Key) { k.keyCode, uint8_t(k.flags | RALT_HELD) })
#define LSHIFT(k) ((Key) { k.keyCode, uint8_t(k.flags | SHIFT_HELD) })
#define LGUI(k)   ((Key) { k.keyCode, uint8_t(k.flags | GUI_HELD) })

// we assert that synthetic keys can never have keys held, so we reuse the _HELD bits
#define IS_SYSCTL                  uint8_t(B00000001)
#define IS_CONSUMER                uint8_t(B00000010)
#define SWITCH_TO_KEYMAP           uint8_t(B00000100)
#define IS_INTERNAL                uint8_t(B00001000)

/* HID types we need to encode in the key flags for system and consumer control hid controls
   Each key can only have one, so we don't need to use a bit vector.
   We need to keep the top two bits clear for defining the keys as synthetic
   We need to keep the bottom two bits clear for defining the keys as sysctl / consumerctl
*/

#define HID_TYPE_CL    uint8_t(B00000000)
#define HID_TYPE_LC    uint8_t(B00000100)
#define HID_TYPE_NARY  uint8_t(B00001000)
#define HID_TYPE_OOC   uint8_t(B00001100)
#define HID_TYPE_OSC   uint8_t(B00010000)
#define HID_TYPE_RTC   uint8_t(B00010100)
#define HID_TYPE_SEL   uint8_t(B00011000)


#define Key_NoKey (Key) { 0,  KEY_FLAGS }
#define Key_skip (Key) { 0,  KEY_FLAGS }
#define Key_Transparent (Key){ .raw = uint16_t(0xffff) }
#define ___ Key_Transparent
#define XXX Key_NoKey






#define KEY_BACKLIGHT_DOWN uint8_t(0xF1)
#define KEY_BACKLIGHT_UP uint8_t(0xF2)
#define Key_BacklightDown (Key) { KEY_BACKLIGHT_DOWN,  KEY_FLAGS }
#define Key_BacklightUp (Key) { KEY_BACKLIGHT_UP,  KEY_FLAGS }
#define KEY_RIGHT_FN2 uint8_t(0xfe)
#define Key_RFN2 (Key) { KEY_RIGHT_FN2,  KEY_FLAGS }
#define KEY_LEFT_FN2 uint8_t(0xff)
#define Key_LFN2 (Key) { KEY_LEFT_FN2,  KEY_FLAGS }


/* Most Consumer keys are more then 8bit, the highest Consumer hid code
   uses 10bit. By using the 11bit as flag to indicate a consumer keys was activate we can
   use the 10 lsb as the HID Consumer code. If you need to get the keycode of a Consumer key
   use the CONSUMER(key) macro this will return the 10bit keycode.
*/
#define CONSUMER(key) (key.raw & 0x03FF)
#define CONSUMER_KEY(code, flags) (Key) { .raw = (code) | ((flags | SYNTHETIC|IS_CONSUMER) << 8) }
