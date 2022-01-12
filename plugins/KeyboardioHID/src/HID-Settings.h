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

#define HID_REPORTID_NONE 0

#ifndef HID_REPORTID_MOUSE
#define HID_REPORTID_MOUSE 1
#endif

#ifndef HID_REPORTID_KEYBOARD
#define HID_REPORTID_KEYBOARD 2
#endif

#ifndef HID_REPORTID_RAWHID
// This will not work properly in most cases.
// The number is just kept from the old number counting.
//#define HID_REPORTID_RAWHID 3
#endif

#ifndef HID_REPORTID_CONSUMERCONTROL
#define HID_REPORTID_CONSUMERCONTROL 4
#endif

#ifndef HID_REPORTID_SYSTEMCONTROL
#define HID_REPORTID_SYSTEMCONTROL 5
#endif

#ifndef HID_REPORTID_GAMEPAD
#define HID_REPORTID_GAMEPAD 6
#endif

#ifndef HID_REPORTID_MOUSE_ABSOLUTE
#define HID_REPORTID_MOUSE_ABSOLUTE 7
#endif

#ifndef HID_REPORTID_NKRO_KEYBOARD
#define HID_REPORTID_NKRO_KEYBOARD 8
#endif


// Nico has submitted these definitions upstream, but they're not merged yet
// HID Request Type HID1.11 Page 51 7.2.1 Get_Report Request
#define HID_REPORT_TYPE_INPUT   1
#define HID_REPORT_TYPE_OUTPUT  2
#define HID_REPORT_TYPE_FEATURE 3

// Controls whether to pack messages or not. When set, any sends will be delayed
// until packing is toggled off, and sent then. This is a no-op on AVR, but is
// required for SAMD. We place a forward-declaration here to be able to avoid
// architecture-specific ifdefs elsewhere in the code.
void USB_PackMessages(bool pack);

#if defined(ARDUINO_ARCH_AVR)

#include "PluggableUSB.h"

#define EPTYPE_DESCRIPTOR_SIZE      uint8_t

#elif defined(ARDUINO_ARCH_SAM)

#include "USB/PluggableUSB.h"

#define EPTYPE_DESCRIPTOR_SIZE      uint32_t
#define EP_TYPE_INTERRUPT_IN        (UOTGHS_DEVEPTCFG_EPSIZE_512_BYTE | \
                                    UOTGHS_DEVEPTCFG_EPDIR_IN |         \
                                    UOTGHS_DEVEPTCFG_EPTYPE_BLK |       \
                                    UOTGHS_DEVEPTCFG_EPBK_1_BANK |      \
                                    UOTGHS_DEVEPTCFG_NBTRANS_1_TRANS |  \
                                    UOTGHS_DEVEPTCFG_ALLOC)
#define EP_TYPE_INTERRUPT_OUT       (UOTGHS_DEVEPTCFG_EPSIZE_512_BYTE | \
                                    UOTGHS_DEVEPTCFG_EPTYPE_BLK |       \
                                    UOTGHS_DEVEPTCFG_EPBK_1_BANK |      \
                                    UOTGHS_DEVEPTCFG_NBTRANS_1_TRANS |  \
                                    UOTGHS_DEVEPTCFG_ALLOC)
#define USB_EP_SIZE                 EPX_SIZE
#define USB_SendControl             USBD_SendControl
#define USB_Available               USBD_Available
#define USB_Recv                    USBD_Recv
#define USB_Send                    USBD_Send
#define USB_Flush                   USBD_Flush

#elif defined(ARDUINO_ARCH_SAMD)

#include "USB/PluggableUSB.h"

#define EPTYPE_DESCRIPTOR_SIZE      uint32_t
#define EP_TYPE_INTERRUPT_IN        USB_ENDPOINT_TYPE_INTERRUPT | USB_ENDPOINT_IN(0);
#define EP_TYPE_INTERRUPT_OUT       USB_ENDPOINT_TYPE_INTERRUPT | USB_ENDPOINT_OUT(0);
#define USB_EP_SIZE                 EPX_SIZE
//#define USB_SendControl           USBDevice.sendControl -> real C++ functions to take care of PGM overloading
#define USB_Available               USBDevice.available
#define USB_Recv                    USBDevice.recv
#define USB_RecvControl             USBDevice.recvControl
#define USB_Send                    USBDevice.send
#define USB_Flush                   USBDevice.flush

int USB_SendControl(void* y, uint8_t z);
int USB_SendControl(uint8_t x, const void* y, uint8_t z);

#define TRANSFER_PGM                0
#define TRANSFER_RELEASE            0

#define HID_REPORT_TYPE_INPUT       1
#define HID_REPORT_TYPE_OUTPUT      2
#define HID_REPORT_TYPE_FEATURE     3

#elif defined(ARDUINO_ARCH_GD32)

#include "api/PluggableUSB.h"
#include "USBCore.h"

#define EPTYPE_DESCRIPTOR_SIZE      unsigned int
#define USB_DEVICE_CLASS_HUMAN_INTERFACE       0x03

#define EP_TYPE_INTERRUPT_IN EPTYPE(USB_TRX_IN, USB_EP_ATTR_INT);
#define EP_TYPE_INTERRUPT_OUT EPTYPE(USB_TRX_OUT, USB_EP_ATTR_INT);

// TODO(@algernon): Most of these would belong to the core, into USBCore.h and
// USBAPI.h. Move them there, eventually.

#define USB_ENDPOINT_TYPE_INTERRUPT USB_EP_ATTR_INT

#define D_INTERFACE(_n,_numEndpoints,_class,_subClass,_protocol) \
{ 9, 4, _n, 0, _numEndpoints, _class,_subClass, _protocol, 0 }

#define D_ENDPOINT(_addr,_attr,_packetSize, _interval) \
{ 7, 5, _addr,_attr,_packetSize, _interval }

#define USB_ENDPOINT_IN(addr)                  (lowByte((addr) | 0x80))

#define USB_EP_SIZE 64

// bmRequestType
#define REQUEST_HOSTTODEVICE    0x00
#define REQUEST_DEVICETOHOST    0x80
#define REQUEST_DIRECTION               0x80

#define REQUEST_STANDARD                0x00
#define REQUEST_CLASS                   0x20
#define REQUEST_VENDOR                  0x40
#define REQUEST_TYPE                    0x60

#define REQUEST_DEVICE                  0x00
#define REQUEST_INTERFACE               0x01
#define REQUEST_ENDPOINT                0x02
#define REQUEST_OTHER                   0x03
#define REQUEST_RECIPIENT               0x03

#define REQUEST_DEVICETOHOST_CLASS_INTERFACE    (REQUEST_DEVICETOHOST | REQUEST_CLASS | REQUEST_INTERFACE)
#define REQUEST_HOSTTODEVICE_CLASS_INTERFACE    (REQUEST_HOSTTODEVICE | REQUEST_CLASS | REQUEST_INTERFACE)
#define REQUEST_DEVICETOHOST_STANDARD_INTERFACE (REQUEST_DEVICETOHOST | REQUEST_STANDARD | REQUEST_INTERFACE)

#pragma pack(push, 1)
//      Interface
typedef struct
{
        uint8_t len;         // 9
        uint8_t dtype;       // 4
        uint8_t number;
        uint8_t alternate;
        uint8_t numEndpoints;
        uint8_t interfaceClass;
        uint8_t interfaceSubClass;
        uint8_t protocol;
        uint8_t iInterface;
} InterfaceDescriptor;

//      Endpoint
typedef struct
{
        uint8_t len;         // 7
        uint8_t dtype;       // 5
        uint8_t addr;
        uint8_t attr;
        uint16_t packetSize;
        uint8_t interval;
} EndpointDescriptor;
#pragma pack(pop)

#else

#error "Unsupported architecture"

#endif
