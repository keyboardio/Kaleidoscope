/*
Copyright (c) 2014-2015 NicoHood
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

// Include guard
#pragma once

#include <Arduino.h>
#include "../HID.h"
#include "../HID-Settings.h"
#include "../MouseButtons.h"
#include "../DescriptorPrimitives.h"


#define DESCRIPTOR_ABS_MOUSE_BUTTONS  					 \
  /* 8 Buttons */							 \
  D_USAGE_PAGE, D_PAGE_BUTTON,        /*     USAGE_PAGE (Button) */	 \
  D_USAGE_MINIMUM, 0x01,              /*     USAGE_MINIMUM (Button 1) */ \
  D_USAGE_MAXIMUM, 0x08,              /*     USAGE_MAXIMUM (Button 8) */ \
  D_LOGICAL_MINIMUM, 0x00,            /*     LOGICAL_MINIMUM (0) */	 \
  D_LOGICAL_MAXIMUM, 0x01,            /*     LOGICAL_MAXIMUM (1) */	 \
  D_REPORT_COUNT, 0x08,               /*     REPORT_COUNT (8) */	 \
  D_REPORT_SIZE, 0x01,                /*     REPORT_SIZE (1) */	         \
  D_INPUT, (D_DATA|D_VARIABLE|D_ABSOLUTE),

# define DESCRIPTOR_ABS_MOUSE_XY \
  /* X, Y */ 									 \
  D_USAGE_PAGE, D_PAGE_GENERIC_DESKTOP,       /* USAGE_PAGE (Generic Desktop) */ \
  D_USAGE, 0x30,                      	      /* USAGE (X) */			 \
  D_USAGE, 0x31,                              /* USAGE (Y) */			 \
  D_MULTIBYTE(D_LOGICAL_MINIMUM), 0x00, 0x00, /* Logical Minimum (0) */ 	 \
  D_MULTIBYTE(D_LOGICAL_MAXIMUM), 0xFF, 0x7f, /* Logical Maximum (32767) */	 \
  D_REPORT_SIZE, 0x10,			      /* Report Size (16), */		 \
  D_REPORT_COUNT, 0x02,		 	      /* Report Count (2), */		 \
  D_INPUT, (D_DATA|D_VARIABLE|D_ABSOLUTE),    /* Input (Data, Variable, Absolute) */

#define DESCRIPTOR_ABS_MOUSE_WHEEL \
  /* Wheel */									\
  D_USAGE, 0x38,                      	    /*     USAGE (Wheel) */		\
  D_LOGICAL_MINIMUM, 0x81,                  /*     LOGICAL_MINIMUM (-127) */	\
  D_LOGICAL_MAXIMUM, 0x7f,                  /*     LOGICAL_MAXIMUM (127) */	\
  D_REPORT_SIZE, 0x08,                      /*     REPORT_SIZE (8) */		\
  D_REPORT_COUNT, 0x01,                     /*     REPORT_COUNT (1) */		\
  D_INPUT, (D_DATA|D_VARIABLE|D_RELATIVE),

typedef union {
    // Absolute mouse report: 8 buttons, 2 absolute axis, wheel
    struct {
        uint8_t buttons;
        uint16_t xAxis;
        uint16_t yAxis;
        int8_t wheel;
    };
} HID_MouseAbsoluteReport_Data_t;

class AbsoluteMouseAPI {
  public:
    inline AbsoluteMouseAPI(void);
    inline void begin(void);
    inline void end(void);

    inline void click(uint8_t b = MOUSE_LEFT);
    inline void moveTo(uint16_t x, uint16_t y, signed char wheel = 0);
    inline void move(int x, int y, signed char wheel = 0);
    inline void press(uint8_t b = MOUSE_LEFT);
    inline void release(uint8_t b = MOUSE_LEFT);
    inline bool isPressed(uint8_t b = MOUSE_LEFT);

    // Sending is public in the base class for advanced users.
    virtual void sendReport(void* data, int length) {}

  protected:
    uint16_t xAxis;
    uint16_t yAxis;
    uint8_t _buttons;

    inline void buttons(uint8_t b);
    inline int16_t qadd16(int16_t base, int16_t increment);
};

#include "AbsoluteMouseAPI.hpp"
