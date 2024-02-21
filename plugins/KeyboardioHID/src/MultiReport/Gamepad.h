/*
Copyright (c) 2014-2015 NicoHood
Copyright (c) 2015-2024 Keyboard.io, Inc

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

// Include guard
#pragma once

#include <Arduino.h>
#include "HID.h"
#include "HID-Settings.h"

// Dpad directions
#define GAMEPAD_DPAD_CENTERED   0
#define GAMEPAD_DPAD_UP         1
#define GAMEPAD_DPAD_UP_RIGHT   2
#define GAMEPAD_DPAD_RIGHT      3
#define GAMEPAD_DPAD_DOWN_RIGHT 4
#define GAMEPAD_DPAD_DOWN       5
#define GAMEPAD_DPAD_DOWN_LEFT  6
#define GAMEPAD_DPAD_LEFT       7
#define GAMEPAD_DPAD_UP_LEFT    8

#define DESCRIPTOR_GAMEPAD(...)                        \
  /* Gamepad with 32 buttons and 6 axis*/              \
                                                       \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),              \
    HID_USAGE(HID_USAGE_DESKTOP_JOYSTICK),             \
    HID_COLLECTION(HID_COLLECTION_APPLICATION),        \
                                                       \
    /* Report ID, if any */                            \
    __VA_ARGS__                                        \
                                                       \
    HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),             \
    HID_USAGE_MIN(1),                                  \
    HID_USAGE_MAX(32),                                 \
    HID_LOGICAL_MIN(0),                                \
    HID_LOGICAL_MAX(1),                                \
    HID_REPORT_SIZE(1),                                \
    HID_REPORT_COUNT(32),                              \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
                                                       \
    /* 4 16bit Axis [-32768, 32767] */                 \
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),            \
    HID_COLLECTION(HID_COLLECTION_PHYSICAL),           \
    HID_USAGE(HID_USAGE_DESKTOP_X),                    \
    HID_USAGE(HID_USAGE_DESKTOP_Y),                    \
    HID_USAGE(HID_USAGE_DESKTOP_RX),                   \
    HID_USAGE(HID_USAGE_DESKTOP_RY),                   \
    HID_LOGICAL_MIN_N(0x8000, 2),                      \
    HID_LOGICAL_MAX_N(0x7fff, 2),                      \
    HID_REPORT_SIZE(16),                               \
    HID_REPORT_COUNT(4),                               \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
                                                       \
    /* 2 8bit Axis [-128, 127] */                      \
    HID_USAGE(HID_USAGE_DESKTOP_Z),                    \
    HID_USAGE(HID_USAGE_DESKTOP_RZ),                   \
    HID_LOGICAL_MIN(0x80),                             \
    HID_LOGICAL_MAX(0x7f),                             \
    HID_REPORT_SIZE(8),                                \
    HID_REPORT_COUNT(2),                               \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
                                                       \
    HID_COLLECTION_END,                                \
                                                       \
    /* 2 Hat Switches */                               \
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),            \
    HID_USAGE(HID_USAGE_DESKTOP_HAT_SWITCH),           \
    HID_USAGE(HID_USAGE_DESKTOP_HAT_SWITCH),           \
    HID_LOGICAL_MIN(1),                                \
    HID_LOGICAL_MAX(8),                                \
    HID_REPORT_COUNT(2),                               \
    HID_REPORT_SIZE(4),                                \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
                                                       \
    HID_COLLECTION_END

typedef union {
  // 32 Buttons, 6 Axis, 2 D-Pads
  uint32_t buttons;

  struct {
    uint8_t button1 : 1;
    uint8_t button2 : 1;
    uint8_t button3 : 1;
    uint8_t button4 : 1;
    uint8_t button5 : 1;
    uint8_t button6 : 1;
    uint8_t button7 : 1;
    uint8_t button8 : 1;

    uint8_t button9 : 1;
    uint8_t button10 : 1;
    uint8_t button11 : 1;
    uint8_t button12 : 1;
    uint8_t button13 : 1;
    uint8_t button14 : 1;
    uint8_t button15 : 1;
    uint8_t button16 : 1;

    uint8_t button17 : 1;
    uint8_t button18 : 1;
    uint8_t button19 : 1;
    uint8_t button20 : 1;
    uint8_t button21 : 1;
    uint8_t button22 : 1;
    uint8_t button23 : 1;
    uint8_t button24 : 1;

    uint8_t button25 : 1;
    uint8_t button26 : 1;
    uint8_t button27 : 1;
    uint8_t button28 : 1;
    uint8_t button29 : 1;
    uint8_t button30 : 1;
    uint8_t button31 : 1;
    uint8_t button32 : 1;

    int16_t xAxis;
    int16_t yAxis;

    int16_t rxAxis;
    int16_t ryAxis;

    int8_t zAxis;
    int8_t rzAxis;

    uint8_t dPad1 : 4;
    uint8_t dPad2 : 4;
  };
} HID_GamepadReport_Data_t;

class Gamepad_ {
 public:
  Gamepad_();

  void begin();
  void end();
  void write();
  void press(uint8_t b);
  void release(uint8_t b);
  void releaseAll();

  void buttons(uint32_t b);
  void xAxis(int16_t a);
  void yAxis(int16_t a);
  void zAxis(int8_t a);
  void rxAxis(int16_t a);
  void ryAxis(int16_t a);
  void rzAxis(int8_t a);
  void dPad1(int8_t d);
  void dPad2(int8_t d);

  void sendReport(void *data, int length);

 protected:
  HID_GamepadReport_Data_t report_;
};
extern Gamepad_ Gamepad;
