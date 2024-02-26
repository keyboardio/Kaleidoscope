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
#include "kaleidoscope/driver/hid/HIDDefs.h"
#include "kaleidoscope/HIDTables.h"
#include "kaleidoscope/HIDAliases.h"

// Declare the hybrid boot keyboard feature so Kaleidoscope can depend on it
#define BOOTKB_HYBRID   1

#define BOOT_KEY_BYTES  6
#define BOOT_REPORT_LEN 8

#define NKRO_KEY_BITS   (4 + HID_LAST_KEY - HID_KEYBOARD_A_AND_A + 1)
#define NKRO_KEY_BYTES  ((NKRO_KEY_BITS + 7) / 8)

// See Appendix B of USB HID spec
#define DESCRIPTOR_BOOT_KEYBOARD(...)                   \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),               \
    HID_USAGE(HID_USAGE_DESKTOP_KEYBOARD),              \
    HID_COLLECTION(HID_COLLECTION_APPLICATION),         \
                                                        \
    /* Report ID, if any */                             \
    __VA_ARGS__                                         \
                                                        \
      /* LEDs */                                        \
      HID_REPORT_COUNT(8),                              \
    HID_REPORT_SIZE(1),                                 \
    HID_USAGE_PAGE(HID_USAGE_PAGE_LED),                 \
    HID_USAGE_MIN(1),                                   \
    HID_USAGE_MAX(8),                                   \
    HID_LOGICAL_MIN(0),                                 \
    HID_LOGICAL_MAX(1),                                 \
    HID_OUTPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
                                                        \
    /* Modifiers */                                     \
    HID_USAGE_PAGE(HID_USAGE_PAGE_KEYBOARD),            \
    HID_USAGE_MIN(HID_KEYBOARD_FIRST_MODIFIER),         \
    HID_USAGE_MAX(HID_KEYBOARD_LAST_MODIFIER),          \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),  \
                                                        \
    /* Reserved byte */                                 \
    HID_REPORT_COUNT(1),                                \
    HID_REPORT_SIZE(8),                                 \
    HID_INPUT(HID_CONSTANT),                            \
                                                        \
    /* Non-modifiers */                                 \
    HID_REPORT_COUNT(BOOT_KEY_BYTES),                   \
    HID_LOGICAL_MAX_N(HID_LAST_KEY, 2),                 \
    HID_USAGE_MIN(HID_FIRST_KEY),                       \
    HID_USAGE_MAX(HID_LAST_KEY),                        \
    HID_INPUT(HID_DATA | HID_ARRAY | HID_ABSOLUTE),     \
                                                        \
    HID_COLLECTION_END

// Padding to round up the report to byte boundary.
#if (NKRO_KEY_BITS % 8)
#define DESCRIPTOR_HYBRID_KEYBOARD_PADDING   \
  HID_REPORT_COUNT(8 - (NKRO_KEY_BITS % 8)), \
    HID_INPUT(HID_CONSTANT),
#else
#define DESCRIPTOR_HYBRID_KEYBOARD_PADDING /* empty */
#endif

#define DESCRIPTOR_HYBRID_KEYBOARD(...)                                       \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),                                     \
    HID_USAGE(HID_USAGE_DESKTOP_KEYBOARD),                                    \
    HID_COLLECTION(HID_COLLECTION_APPLICATION),                               \
                                                                              \
    /* Report ID, if any */                                                   \
    __VA_ARGS__                                                               \
                                                                              \
    /* 5 LEDs for num lock etc, 3 left for advanced, custom usage */          \
    HID_USAGE_PAGE(HID_USAGE_PAGE_LED),                                       \
    HID_USAGE_MIN(1),                                                         \
    HID_USAGE_MAX(8),                                                         \
    HID_LOGICAL_MIN(0),                                                       \
    HID_LOGICAL_MAX(1),                                                       \
    HID_REPORT_SIZE(1),                                                       \
    HID_REPORT_COUNT(8),                                                      \
    HID_OUTPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),                       \
                                                                              \
    /* Key modifier byte for both boot and NKRO */                            \
    HID_USAGE_PAGE(HID_USAGE_PAGE_KEYBOARD),                                  \
    HID_USAGE_MIN(HID_KEYBOARD_FIRST_MODIFIER),                               \
    HID_USAGE_MAX(HID_KEYBOARD_LAST_MODIFIER),                                \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),                        \
                                                                              \
    /* Send rest of boot report as padding, so HID-aware hosts will ignore */ \
    HID_REPORT_SIZE(8),                                                       \
    HID_REPORT_COUNT(7),                                                      \
    HID_INPUT(HID_CONSTANT),                                                  \
                                                                              \
    /* NKRO key bitmap */                                                     \
                                                                              \
    /* Padding 4 bits, to skip NO_EVENT & 3 error states. */                  \
    HID_REPORT_SIZE(1),                                                       \
    HID_REPORT_COUNT(4),                                                      \
    HID_INPUT(HID_CONSTANT),                                                  \
                                                                              \
    /* Actual non-modifier keys */                                            \
    HID_USAGE_MIN(HID_KEYBOARD_A_AND_A),                                      \
    HID_USAGE_MAX(HID_LAST_KEY),                                              \
    HID_REPORT_COUNT(NKRO_KEY_BITS - 4),                                      \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),                        \
                                                                              \
    /* Padding (if needed )*/                                                 \
    DESCRIPTOR_HYBRID_KEYBOARD_PADDING                                        \
                                                                              \
      HID_COLLECTION_END

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


class BootKeyboardAPI {
 public:
  /*
   * Change to `bootkb_only_ = 1` if you need to default to only sending
   * Boot Protocol, even if in Report Protocol.
   */
  inline explicit BootKeyboardAPI(uint8_t bootkb_only_ = 0);
  inline size_t press(uint8_t k);
  inline void begin();
  inline void end();
  inline size_t release(uint8_t k);
  inline void releaseAll();

  inline int sendReport();

  inline bool isModifierActive(uint8_t k);
  inline bool wasModifierActive(uint8_t k);
  inline bool isAnyModifierActive();
  inline bool wasAnyModifierActive();
  inline bool isKeyPressed(uint8_t k);
  inline bool wasKeyPressed(uint8_t k);

  virtual uint8_t getLeds() = 0;

  inline uint8_t getBootOnly();
  inline void setBootOnly(uint8_t bootonly);

  virtual void onUSBReset() = 0;

 protected:
  virtual int SendHIDReport(const void *data, int len)  = 0;
  virtual void setReportDescriptor(uint8_t bootkb_only) = 0;
  virtual uint8_t getProtocol()                         = 0;

  HID_NKRO_KeyboardReport_Data_t report_, last_report_;

  uint8_t bootkb_only;

 private:
  inline void convertReport(uint8_t *boot, const uint8_t *nkro);
  inline int sendReportUnchecked();
};

#include "BootKeyboardAPI.hpp"
