#pragma once

#define _MULTIBYTE(n) (n + 0x01)

#define _USAGE_PAGE 0x05
#define _USAGE 0x09


#define _REPORT_ID 0x85

#define _USAGE_MINIMUM 0x19
#define _USAGE_MAXIMUM 0x29

#define _LOGICAL_MINIMUM 0x15
#define _LOGICAL_MAXIMUM 0x25

#define _REPORT_SIZE 0x75
#define _REPORT_COUNT 0x95


// USB HID DCD 1.11 section 6.2.2.4 -  Main items
//
// each of these are the type description + 0x01 for a single byte item
#define _INPUT 0x81
#define _OUTPUT 0x91
#define _FEATURE 0xb1
#define _COLLECTION 0xa1
#define _END_COLLECTION 0xc0

// The bits that make up inputs, outputs and features

// Bit 0
#define _DATA         0b00000000
#define _CONSTANT     0b00000001
// Bit 1
#define _ARRAY        0b00000000
#define _VARIABLE     0b00000010
// Bit 2
#define _ABSOLUTE     0b00000000
#define _RELATIVE     0b00000100
// Bit 3
#define _NO_WRAP      0b00000000
#define _WRAP         0b00001000
// Bit 4
#define _LINEAR       0b00000000
#define _NON_LINEAR   0b00010000
// Bit 5
#define _PREFERRED_STATE 0b00000000
#define _NO_PREFERRED    0b00100000
// Bit 6
#define _NO_NULL_POSITION      0b00000000
#define _NULL_STATE            0b01000000
// Bit 7
#define _NON_VOLATILE    0b00000000
#define _VOLATILE        0b01000000

// Bit 8
#define _BIT_FIELD        0b00000000
#define _BUFFERED_BYTES   0b10000000



// Collection types


#define _PHYSICAL    0x00  // (group of axes) 
#define _APPLICATION  0x01  // (mouse, keyboard) 
#define _LOGICAL      0x02// (interrelated data) 
#define _REPORT       0x03
#define _NAMED_ARRAY  0x04
#define _USAGE_SWITCH 0x05
#define _USAGE_MODIFIER  0x06
// 0x07-0x7f - Reserved 
// 0x80-0xff - Vendor define


#define _PAGE_GENERIC_DESKTOP 0x01
#define _PAGE_SIMULATION 0x02
#define _PAGE_VR 0x03
#define _PAGE_SPORT 0x04
#define _PAGE_GAME 0x05
#define _PAGE_GENERIC_DEVICE 0x06
#define _PAGE_KEYBOARD 0x07
#define _PAGE_LEDS 0x08
#define _PAGE_BUTTON 0x09
#define _PAGE_ORDINAL 0x0A
#define _PAGE_TELEPHONY 0x0B
#define _PAGE_CONSUMER 0x0C
#define _PAGE_DIGITIZER 0x0D
#define _PAGE_RESERVED 0x0E
#define _PAGE_PID 0x0F
#define _PAGE_UNICODE 0x10
// 0x11-13 RESERVED
#define _PAGE_ALPHANUMERIC_DISPLAY 0x14
#define _PAGE_MEDICAL_INSTRUMENTS 0x40
// 0x80-83 MONITOR
// 0x84-87 POWER
#define _PAGE_BAR_CODE_SCANNER 0x8C
#define _PAGE_SCALE 0x8D
#define _PAGE_MSR 0x8E
// 0x8F RESERVED POINT OF SALE
#define _PAGE_CAMERA_CONTROL 0x90
#define _PAGE_ARCADE 0x91


// Generic Desktop Usages HUT Section 4 p27


#define _USAGE_POINTER 0x01
#define _USAGE_MOUSE 0x02
// 0x03 is reserved
#define _USAGE_JOYSTICK 0x04
#define _USAGE_GAMEPAD 0x05
#define _USAGE_KEYBOARD 0x06
#define _USAGE_KEYPAD 0x07
#define _USAGE_MULITAXIS 0x08