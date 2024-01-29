/*
  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  Permission to use, copy, modify, and/or distribute this software for
  any purpose with or without fee is hereby granted, provided that the
  above copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
  WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
  BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
  OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
  SOFTWARE.
 */

#pragma once

#include "tusb_hid.h"

// HID 'Driver'
// ------------
#define HID_GET_REPORT               0x01
#define HID_GET_IDLE                 0x02
#define HID_GET_PROTOCOL             0x03
#define HID_SET_REPORT               0x09
#define HID_SET_IDLE                 0x0A
#define HID_SET_PROTOCOL             0x0B

#define HID_HID_DESCRIPTOR_TYPE      0x21
#define HID_REPORT_DESCRIPTOR_TYPE   0x22
#define HID_PHYSICAL_DESCRIPTOR_TYPE 0x23

// HID Keyboard/Mouse bios compatible protocols HID1.11 Page 9 4.3 Protocols
#define HID_PROTOCOL_NONE     0
#define HID_PROTOCOL_KEYBOARD 1
#define HID_PROTOCOL_MOUSE    2

// Normal or bios protocol (Keyboard/Mouse) HID1.11 Page 54 7.2.5 Get_Protocol Request
// "protocol" variable is used for this purpose.
#define HID_BOOT_PROTOCOL   0
#define HID_REPORT_PROTOCOL 1
