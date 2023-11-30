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
#include "HIDTables.h"
#include "HIDAliases.h"

// Declare the hybrid boot keyboard feature so Kaleidoscope can depend on it
#define BOOTKB_HYBRID   1

#define BOOT_KEY_BYTES  6
#define BOOT_REPORT_LEN 8

#define NKRO_KEY_BITS   (4 + HID_LAST_KEY - HID_KEYBOARD_A_AND_A + 1)
#define NKRO_KEY_BYTES  ((NKRO_KEY_BITS + 7) / 8)

/*
 * Keep the current key states in a NKRO bitmap. We'll convert it to Boot
 * Protocol as needed.
 */
typedef union {
  // Modifiers + keymap
  struct {
    uint8_t modifiers;
    uint8_t keys[NKRO_KEY_BYTES];
  };
  uint8_t allkeys[1 + NKRO_KEY_BYTES];
} HID_NKRO_KeyboardReport_Data_t;

/*
 * Hybrid boot report that contains the Boot Protocol report as a prefix to
 * a bitmap NKRO report. The keycodes array of the Boot Report is marked as
 * padding in the Report Descriptor, so HID-aware hosts will ignore it, but
 * hosts that require Boot Protocol without requesting it have a chance of
 * working, assuming they can deal with the extended report.
 *
 * We do send only the Boot Report if the host has requested Boot Protocol.
 */
typedef union {
  // Hybrid report: boot keyboard report + NKRO report
  struct {
    // Boot/NKRO keyboard modifiers
    uint8_t modifiers;
    uint8_t reserved;
    // Boot keyboard non-modifier keys array
    uint8_t boot_keycodes[BOOT_KEY_BYTES];
    // NKRO keyboard non-modifiers keys bitmap
    uint8_t nkro_keys[NKRO_KEY_BYTES];
  };
  uint8_t bytes[BOOT_REPORT_LEN + NKRO_KEY_BYTES];
} HID_BootKeyboardReport_Data_t;


class BootKeyboard_ : public PluggableUSBModule {
 public:
  /*
   * Change to `bootkb_only_ = 1` if you need to default to only sending
   * Boot Protocol, even if in Report Protocol.
   */
  explicit BootKeyboard_(uint8_t bootkb_only_ = 0);
  size_t press(uint8_t k);
  void begin();
  void end();
  size_t release(uint8_t k);
  void releaseAll();

  int sendReport();

  bool isModifierActive(uint8_t k);
  bool wasModifierActive(uint8_t k);
  bool isAnyModifierActive();
  bool wasAnyModifierActive();
  bool isKeyPressed(uint8_t k);
  bool wasKeyPressed(uint8_t k);

  uint8_t getLeds();
  uint8_t getProtocol();

  uint8_t getBootOnly();
  void setBootOnly(uint8_t bootonly);

  void onUSBReset();

 protected:
  HID_NKRO_KeyboardReport_Data_t report_, last_report_;

  // Implementation of the PUSBListNode
  int getInterface(uint8_t *interfaceCount);
  int getDescriptor(USBSetup &setup);
  bool setup(USBSetup &setup);

  EPTYPE_DESCRIPTOR_SIZE epType[1];
  uint8_t protocol;
  uint8_t idle;

  uint8_t leds;

  uint8_t bootkb_only;

 private:
  void convertReport(uint8_t *boot, const uint8_t *nkro);
  int sendReportUnchecked();
};
extern BootKeyboard_ &BootKeyboard();
