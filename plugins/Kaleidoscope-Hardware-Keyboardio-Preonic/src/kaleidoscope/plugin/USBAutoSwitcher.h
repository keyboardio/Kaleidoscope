/* Kaleidoscope-USBAutoSwitcher -- Automatic USB/BLE device switching for Preonic
 * Copyright 2025 Keyboard.io, inc.
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

#include <stdint.h>
#include "kaleidoscope/plugin.h"
#include "kaleidoscope/host_connection_status.h"
#include "kaleidoscope/Runtime.h"

namespace kaleidoscope {
namespace plugin {

/**
 * @brief Automatic USB/BLE device switching for Keyboardio Preonic
 * 
 * This plugin automatically switches between USB and Bluetooth connections:
 * - On startup without USB data: switch to last-used BLE device (default: device 1)
 * - USB data connected while on BLE: switch to USB
 * - USB data disconnected while on USB: switch to last-used BLE device
 * 
 * Manual device switching naturally overrides auto-switching since this plugin
 * responds to connection status events regardless of their cause.
 */
class USBAutoSwitcher : public kaleidoscope::Plugin {
 public:
  USBAutoSwitcher() {}

  /**
   * @brief Configure startup delay before auto-switching begins
   * @param delay_ms Milliseconds to wait after startup (default: 1000ms)
   */
  static void setStartupDelay(uint16_t delay_ms) {
    startup_delay_ms_ = delay_ms;
  }

  /**
   * @brief Get the current fallback BLE device number
   * @return BLE device number (1-4) used for auto-switching
   */
  static uint8_t getFallbackBLEDevice() {
    return fallback_ble_device_;
  }

  // Plugin hooks
  EventHandlerResult onSetup();
  EventHandlerResult afterEachCycle();
  EventHandlerResult onHostConnectionStatusChanged(uint8_t device_id, HostConnectionStatus status);

 private:
  static uint8_t fallback_ble_device_;      // Last manually selected BLE device (1-4)
  static uint32_t startup_time_;            // When the plugin was initialized
  static uint16_t startup_delay_ms_;        // Delay before auto-switching starts
  static bool startup_switch_done_;         // Prevent multiple startup switches
  static uint8_t current_host_mode_;        // Track current connection mode

  /**
   * @brief Check if we're still within the startup delay period
   * @return true if startup delay is still active
   */
  bool isStartupPeriod();

  /**
   * @brief Switch to the remembered BLE device
   */
  void switchToFallbackBLE();

  /**
   * @brief Switch to USB mode
   */
  void switchToUSB();

  /**
   * @brief Update fallback BLE device based on current selection
   * Called when BLE device connection status changes
   */
  void updateFallbackBLEDevice(uint8_t device_id);

  /**
   * @brief Get current host connection mode from device
   * @return MODE_USB or MODE_BLE
   */
  uint8_t getCurrentHostMode();
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::USBAutoSwitcher USBAutoSwitcher;