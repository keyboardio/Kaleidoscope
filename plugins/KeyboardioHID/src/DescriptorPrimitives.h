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

#pragma once

#define D_MULTIBYTE(n) (n + 0x01)

#define D_USAGE_PAGE 0x05
#define D_USAGE 0x09


#define D_REPORT_ID 0x85

#define D_USAGE_MINIMUM 0x19
#define D_USAGE_MAXIMUM 0x29

#define D_LOGICAL_MINIMUM 0x15
#define D_LOGICAL_MAXIMUM 0x25

#define D_PHYSICAL_MINIMUM 0x35
#define D_PHYSICAL_MAXIMUM 0x45

#define D_REPORT_SIZE 0x75
#define D_REPORT_COUNT 0x95

#define D_PUSH 0xa4
#define D_POP 0xb4

// USB HID DCD 1.11 section 6.2.2.4 -  Main items
//
// each of these are the type description + 0x01 for a single byte item
#define D_INPUT 0x81
#define D_OUTPUT 0x91
#define D_FEATURE 0xb1
#define D_COLLECTION 0xa1
#define D_END_COLLECTION 0xc0

// The bits that make up inputs, outputs and features

// Bit 0
#define D_DATA         0b00000000
#define D_CONSTANT     0b00000001
// Bit 1
#define D_ARRAY        0b00000000
#define D_VARIABLE     0b00000010
// Bit 2
#define D_ABSOLUTE     0b00000000
#define D_RELATIVE     0b00000100
// Bit 3
#define D_NO_WRAP      0b00000000
#define D_WRAP         0b00001000
// Bit 4
#define D_LINEAR       0b00000000
#define D_NON_LINEAR   0b00010000
// Bit 5
#define D_PREFERRED_STATE 0b00000000
#define D_NO_PREFERRED    0b00100000
// Bit 6
#define D_NO_NULL_POSITION      0b00000000
#define D_NULL_STATE            0b01000000
// Bit 7
#define D_NON_VOLATILE    0b00000000
#define D_VOLATILE        0b01000000

// Bit 8
#define D_BIT_FIELD        0b00000000
#define D_BUFFERED_BYTES   0b10000000



// Collection types


#define D_PHYSICAL    0x00  // (group of axes)
#define D_APPLICATION  0x01  // (mouse, keyboard)
#define D_LOGICAL      0x02// (interrelated data)
#define D_REPORT       0x03
#define D_NAMED_ARRAY  0x04
#define D_USAGE_SWITCH 0x05
#define D_USAGE_MODIFIER  0x06
// 0x07-0x7f - Reserved
// 0x80-0xff - Vendor define


#define D_PAGE_GENERIC_DESKTOP 0x01
#define D_PAGE_SIMULATION 0x02
#define D_PAGE_VR 0x03
#define D_PAGE_SPORT 0x04
#define D_PAGE_GAME 0x05
#define D_PAGE_GENERIC_DEVICE 0x06
#define D_PAGE_KEYBOARD 0x07
#define D_PAGE_LEDS 0x08
#define D_PAGE_BUTTON 0x09
#define D_PAGE_ORDINAL 0x0A
#define D_PAGE_TELEPHONY 0x0B
#define D_PAGE_CONSUMER 0x0C
#define D_PAGE_DIGITIZER 0x0D
#define D_PAGE_RESERVED 0x0E
#define D_PAGE_PID 0x0F
#define D_PAGE_UNICODE 0x10
// 0x11-13 RESERVED
#define D_PAGE_ALPHANUMERIC_DISPLAY 0x14
#define D_PAGE_MEDICAL_INSTRUMENTS 0x40
// 0x80-83 MONITOR
// 0x84-87 POWER
#define D_PAGE_BAR_CODE_SCANNER 0x8C
#define D_PAGE_SCALE 0x8D
#define D_PAGE_MSR 0x8E
// 0x8F RESERVED POINT OF SALE
#define D_PAGE_CAMERA_CONTROL 0x90
#define D_PAGE_ARCADE 0x91


// Generic Desktop Usages HUT Section 4 p27


#define D_USAGE_POINTER 0x01
#define D_USAGE_MOUSE 0x02
// 0x03 is reserved
#define D_USAGE_JOYSTICK 0x04
#define D_USAGE_GAMEPAD 0x05
#define D_USAGE_KEYBOARD 0x06
#define D_USAGE_KEYPAD 0x07
#define D_USAGE_MULITAXIS 0x08
