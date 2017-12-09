/* -*- mode: c++ -*-
 * Kaleidoscope-MyOldFriend -- Host sleep support plugin.
 * Copyright (C) 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "WakeupKeyboard.h"

static const uint8_t _hidReportDescriptorKeyboard[] PROGMEM = {
  //  Keyboard
  0x05, 0x01,                      /* USAGE_PAGE (Generic Desktop) */
  0x09, 0x06,                      /* USAGE (Keyboard) */
  0xa1, 0x01,                      /* COLLECTION (Application) */
  0x05, 0x07,                      /*   USAGE_PAGE (Keyboard) */

  /* 0 LEDs, to have something the host can work with. */
  0x05, 0x08,						 /*   USAGE_PAGE (LEDs) */
  0x19, 0x00,						 /*   USAGE_MINIMUM (-) */
  0x29, 0x00,						 /*   USAGE_MAXIMUM (-) */
  0x95, 0x00,						 /*   REPORT_COUNT (0)  */
  0x75, 0x00,						 /*   REPORT_SIZE (0)   */
  0x91, 0x02,						 /*   OUTPUT (Data,Var,Abs) */

  /* End */
  0xc0                            /* END_COLLECTION */
};

WakeupKeyboard_::WakeupKeyboard_(void) : PluggableUSBModule(1, 1, epType) {
  epType[0] = EP_TYPE_INTERRUPT_IN;
  PluggableUSB().plug(this);
}

int WakeupKeyboard_::getInterface(uint8_t* interfaceCount) {
  *interfaceCount += 1;
  HIDDescriptor hidInterface = {
    D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_BOOT_INTERFACE, HID_PROTOCOL_KEYBOARD),
    D_HIDREPORT(sizeof(_hidReportDescriptorKeyboard)),
    D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 0x01)
  };
  return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}

int WakeupKeyboard_::getDescriptor(USBSetup& setup) {
  if (setup.bmRequestType != REQUEST_DEVICETOHOST_STANDARD_INTERFACE) {
    return 0;
  }
  if (setup.wValueH != HID_REPORT_DESCRIPTOR_TYPE) {
    return 0;
  }

  if (setup.wIndex != pluggedInterface) {
    return 0;
  }

  return USB_SendControl(TRANSFER_PGM, _hidReportDescriptorKeyboard, sizeof(_hidReportDescriptorKeyboard));
}

bool WakeupKeyboard_::setup(USBSetup& setup) {
  if (pluggedInterface != setup.wIndex) {
    return false;
  }

  uint8_t request = setup.bRequest;
  uint8_t requestType = setup.bmRequestType;

  if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE) {
    if (request == HID_GET_PROTOCOL) {
      UEDATX = HID_BOOT_PROTOCOL;
      return true;
    }
  }

  return false;
}

void WakeupKeyboard_::begin () {
}

WakeupKeyboard_ WakeupKeyboard;
