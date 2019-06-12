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
#include "HID.h"
#include "HID-Settings.h"
#include "HIDTables.h"
#include "HIDAliases.h"

typedef union {
    // Low level key report: up to 6 keys and shift, ctrl etc at once
    struct {
        uint8_t modifiers;
        uint8_t reserved;
        uint8_t keycodes[6];
    };
    uint8_t bytes[8];
} HID_BootKeyboardReport_Data_t;


class BootKeyboard_ : public PluggableUSBModule {
  public:
    BootKeyboard_(void);
    size_t press(uint8_t);
    void begin(void);
    void end(void);
    size_t release(uint8_t);
    void releaseAll(void);

    int sendReport(void);

    boolean isModifierActive(uint8_t k);
    boolean wasModifierActive(uint8_t k);
    boolean isAnyModifierActive();
    boolean wasAnyModifierActive();
    boolean isKeyPressed(uint8_t k);
    boolean wasKeyPressed(uint8_t k);

    uint8_t getLeds(void);
    uint8_t getProtocol(void);
    void setProtocol(uint8_t protocol);

    uint8_t default_protocol = HID_REPORT_PROTOCOL;

  protected:
    HID_BootKeyboardReport_Data_t _keyReport, _lastKeyReport;

    // Implementation of the PUSBListNode
    int getInterface(uint8_t* interfaceCount);
    int getDescriptor(USBSetup& setup);
    bool setup(USBSetup& setup);

    EPTYPE_DESCRIPTOR_SIZE epType[1];
    uint8_t protocol;
    uint8_t idle;

    uint8_t leds;
};
extern BootKeyboard_ BootKeyboard;
