/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2025 Keyboard.io, inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef ARDUINO_ARCH_NRF52

// Include debug macros first
#include "kaleidoscope/driver/ble/Bluefruit.h"  // for DEBUG_BLE_MSG

#include <bluefruit.h>

#include "kaleidoscope/driver/hid/apis/AbsoluteMouseAPI.h"
#include "kaleidoscope/driver/hid/apis/MouseAPI.h"  // for HID_MouseReport_Data_t
#include "kaleidoscope/driver/hid/bluefruit/HIDD.h"
#include "kaleidoscope/trace.h"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace bluefruit {

// Forward declaration of BLE-specific absolute mouse report structure
struct BLEAbsoluteMouseReport {
  uint16_t xAxis;
  uint16_t yAxis;
  int8_t wheel;
} __attribute__((packed));

class AbsoluteMouse_ : public AbsoluteMouseAPI {
 public:
  /*
   * BUTTON FORWARDING TO REGULAR MOUSE
   * 
   * PROBLEM: The BLE HID descriptor originally included button declarations in both the
   * regular mouse (DESCRIPTOR_MOUSE) and absolute mouse (DESCRIPTOR_ABSOLUTE_MOUSE) 
   * descriptors. This caused macOS to see two mouse devices both claiming to handle
   * mouse buttons, leading to device classification confusion where macOS would ignore
   * button clicks while still processing mouse movement.
   * 
   * SOLUTION: Remove button declarations from the absolute mouse descriptor and forward
   * all button events from absolute mouse API calls to regular mouse reports (RID_MOUSE).
   * This creates clean separation:
   * - Regular mouse (RID_MOUSE): handles ALL button events + relative movement  
   * - Absolute mouse (RID_ABS_MOUSE): handles ONLY absolute positioning + wheel
   * 
   * WHY THIS WORKS: macOS now sees only one device claiming mouse button functionality,
   * eliminating the classification conflict. The absolute mouse can still handle buttons
   * through the API, but they're sent as regular mouse reports internally.
   * 
   * ARCHITECTURE: This forwarding happens only in the BLE-specific implementation.
   * USB implementations (TinyUSB, KeyboardioHID) are unaffected since they either
   * don't include absolute mouse in the composite descriptor or use separate HID devices.
   */

  void click(uint8_t b) {
    // Forward button click to regular mouse report (RID_MOUSE)
    // This maintains API compatibility while avoiding HID descriptor conflicts
    HID_MouseReport_Data_t mouse_report = {0};

    // Send button press
    mouse_report.buttons = b;
    blehid.sendInputReport(RID_MOUSE, &mouse_report, sizeof(mouse_report));

    // Send button release (complete click action)
    mouse_report.buttons = 0;
    blehid.sendInputReport(RID_MOUSE, &mouse_report, sizeof(mouse_report));
  }

  void press(uint8_t b) {
    // Forward button press to regular mouse report (RID_MOUSE)
    HID_MouseReport_Data_t mouse_report = {0};
    mouse_report.buttons                = b;
    blehid.sendInputReport(RID_MOUSE, &mouse_report, sizeof(mouse_report));
  }

  void release(uint8_t b) {
    // Forward button release to regular mouse report (RID_MOUSE)
    // Note: For simplicity, we release all buttons rather than tracking individual
    // button state. This is acceptable since absolute mouse buttons are rarely used
    // and this maintains the core functionality while fixing the BLE compatibility issue.
    HID_MouseReport_Data_t mouse_report = {0};
    mouse_report.buttons                = 0;  // Release all buttons
    blehid.sendInputReport(RID_MOUSE, &mouse_report, sizeof(mouse_report));
  }

  void sendReport(void *data, int length) override {
    DEBUG_TRACE("HID", "Sending BLE Absolute Mouse report");

    if (blehid.isBootMode()) {
      return;
    }

    // Convert global AbsoluteMouse report to BLE-specific format (no buttons)
    const HID_MouseAbsoluteReport_Data_t *full_report =
      static_cast<const HID_MouseAbsoluteReport_Data_t *>(data);

    BLEAbsoluteMouseReport ble_report;
    ble_report.xAxis = full_report->xAxis;
    ble_report.yAxis = full_report->yAxis;
    ble_report.wheel = full_report->wheel;
    // Note: buttons field intentionally omitted for BLE

    blehid.sendInputReport(RID_ABS_MOUSE, &ble_report, sizeof(ble_report));
  }
};

struct AbsoluteMouseProps {
  typedef AbsoluteMouse_ AbsoluteMouse;
};

template<typename _Props>
class AbsoluteMouse : public base::AbsoluteMouse<_Props> {};

}  // namespace bluefruit
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope
#endif /* ARDUINO_ARCH_NRF52 */
