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

/* Portability definitions for different PluggableUSB back ends */

// Controls whether to pack messages or not. When set, any sends will be delayed
// until packing is toggled off, and sent then. This is a no-op on AVR, but is
// required for SAMD. We place a forward-declaration here to be able to avoid
// architecture-specific ifdefs elsewhere in the code.
void USB_PackMessages(bool pack);

#if defined(ARDUINO_ARCH_AVR)

#include <PluggableUSB.h>

#define EPTYPE_DESCRIPTOR_SIZE uint8_t
#define USB_Configured         USBDevice.configured

#elif defined(ARDUINO_ARCH_SAM)

#include <PluggableUSB.h>

#define EPTYPE_DESCRIPTOR_SIZE uint32_t
#define EP_TYPE_INTERRUPT_IN   (UOTGHS_DEVEPTCFG_EPSIZE_512_BYTE | \
                              UOTGHS_DEVEPTCFG_EPDIR_IN |          \
                              UOTGHS_DEVEPTCFG_EPTYPE_BLK |        \
                              UOTGHS_DEVEPTCFG_EPBK_1_BANK |       \
                              UOTGHS_DEVEPTCFG_NBTRANS_1_TRANS |   \
                              UOTGHS_DEVEPTCFG_ALLOC)
#define EP_TYPE_INTERRUPT_OUT (UOTGHS_DEVEPTCFG_EPSIZE_512_BYTE | \
                               UOTGHS_DEVEPTCFG_EPTYPE_BLK |      \
                               UOTGHS_DEVEPTCFG_EPBK_1_BANK |     \
                               UOTGHS_DEVEPTCFG_NBTRANS_1_TRANS | \
                               UOTGHS_DEVEPTCFG_ALLOC)
#define USB_EP_SIZE     EPX_SIZE
#define USB_SendControl USBD_SendControl
#define USB_Available   USBD_Available
#define USB_Recv        USBD_Recv
#define USB_Send        USBD_Send
#define USB_Flush       USBD_Flush
#define USB_Configured  USBDevice.configured

#elif defined(ARDUINO_ARCH_SAMD)

#include <PluggableUSB.h>

#define EPTYPE_DESCRIPTOR_SIZE uint32_t
#define EP_TYPE_INTERRUPT_IN   USB_ENDPOINT_TYPE_INTERRUPT | USB_ENDPOINT_IN(0);
#define EP_TYPE_INTERRUPT_OUT  USB_ENDPOINT_TYPE_INTERRUPT | USB_ENDPOINT_OUT(0);
#define USB_EP_SIZE            EPX_SIZE
//#define USB_SendControl           USBDevice.sendControl -> real C++ functions to take care of PGM overloading
#define USB_Available          USBDevice.available
#define USB_Recv               USBDevice.recv
#define USB_RecvControl        USBDevice.recvControl
#define USB_Send               USBDevice.send
#define USB_Flush              USBDevice.flush
#define USB_Configured         USBDevice.configured

int USB_SendControl(void *y, uint8_t z);
int USB_SendControl(uint8_t x, const void *y, uint8_t z);

#define TRANSFER_PGM           0
#define TRANSFER_RELEASE       0

#elif defined(ARDUINO_ARCH_GD32)

#include "USBCore.h"

#define EPTYPE_DESCRIPTOR_SIZE unsigned int


// Should eventually get defined upstream
#ifndef USB_DEVICE_CLASS_HUMAN_INTERFACE
#define USB_DEVICE_CLASS_HUMAN_INTERFACE 0x03
#endif

#define ARCH_HAS_CONFIGURABLE_EP_SIZES

constexpr uint16_t EP_TYPE_INTERRUPT_IN(uint8_t buffer_size) {
  return EPDesc(USB_TRX_IN, USB_EP_ATTR_INT, buffer_size).val;
}
constexpr uint16_t EP_TYPE_INTERRUPT_OUT(uint8_t buffer_size) {
  return EPDesc(USB_TRX_OUT, USB_EP_ATTR_INT, buffer_size).val;
}

#define USB_Configured USBCore().configured

#else

#error "Unsupported architecture"

#endif
