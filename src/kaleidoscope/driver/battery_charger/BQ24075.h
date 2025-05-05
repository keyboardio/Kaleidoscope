/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2024-2025 Keyboard.io, inc.
 *
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

#include "kaleidoscope/driver/battery_charger/Base.h"
#include "kaleidoscope/trace.h"

namespace kaleidoscope {
namespace driver {
namespace battery_charger {

/** @brief Properties for the BQ24075 battery charger driver
 *
 * This struct defines properties for the BQ24075 battery charger IC.
 * It extends BaseProps with additional pins and capabilities specific to the BQ24075.
 */
struct BQ24075Props : public BaseProps {
  /**
   * @brief Indicates the device has battery charger capabilities
   */
  static constexpr bool has_battery_charger = true;

  /**
   * @brief Pin connected to the BQ24075 PGOOD output
   * 
   * This pin is LOW when input power supply is good, HIGH otherwise.
   * Note: PGOOD is an open-drain output that requires a pull-up resistor.
   */
  static constexpr uint8_t power_good_pin = 0;  // Default value, should be overridden

  /**
   * @brief Pin connected to the BQ24075 CHG output
   * 
   * This pin is LOW when charging, HIGH when charge complete or not charging.
   * Note: CHG is an open-drain output that requires a pull-up resistor.
   */
  static constexpr uint8_t charge_status_pin = 0;  // Default value, should be overridden

  /**
   * @brief Pin connected to the BQ24075 SYSOFF input
   * 
   * This pin allows disconnecting the battery from the system.
   * HIGH disconnects the battery, LOW connects it (normal operation).
   * Optional - set to 0 if not used.
   */
  static constexpr uint8_t sysoff_pin = 0;  // Default value, set to 0 if not used

  /**
   * @brief Analog pin to monitor charge current via ISET
   * 
   * Optional - set to 0 if not used.
   */
  static constexpr uint8_t iset_pin = 0;  // Default value, set to 0 if not used

  /**
   * @brief Resistance of ISET resistor in milliohms
   * 
   * Used for charge current calculation. The charge current is 400 times
   * the current flowing through this resistor.
   */
  static constexpr uint32_t iset_resistance = 1130000;  // Default 1.13kΩ in milliohms, should be overridden
};

/** @brief BQ24075 battery charger driver
 *
 * This class implements a driver for the BQ24075 battery charger IC.
 * It provides status monitoring for power source and charge status,
 * along with battery disconnect control (SYSOFF) and charge current
 * monitoring capabilities.
 *
 * @tparam _Props Properties class defining the pins and capabilities of the charger
 */
template<typename _Props>
class BQ24075 : public Base<_Props> {
 private:
  bool initialized_{false};  // Track initialization state

  // Variables for charge fault detection
  uint32_t last_chg_toggle_time_{0};
  bool last_chg_state_{false};
  uint8_t chg_toggle_count_{0};
  bool fault_detected_{false};

  /** @brief Check if the CHG pin is flashing (indicating a fault)
   * 
   * The BQ24075 flashes the CHG pin at ~2Hz when a timer fault occurs.
   * This method detects that pattern by counting transitions.
   * 
   * @return true if a flashing pattern is detected
   */
  bool detectChgFlashing() {
    if (!initialized_ || !_Props::charge_status_pin)
      return false;

    bool current_state    = digitalRead(_Props::charge_status_pin) == LOW;
    uint32_t current_time = millis();

    // Reset detection if no toggles for 2 seconds
    if (current_time - last_chg_toggle_time_ > 2000) {
      chg_toggle_count_ = 0;
      fault_detected_   = false;
    }

    // Detect state change
    if (current_state != last_chg_state_) {
      // Calculate time between toggles
      uint32_t toggle_interval = current_time - last_chg_toggle_time_;

      // If toggle interval is around 500ms (2Hz), increment counter
      if (toggle_interval > 300 && toggle_interval < 700) {
        chg_toggle_count_++;

        // If we see 4 toggles with the right timing, it's a fault
        if (chg_toggle_count_ >= 4) {
          fault_detected_ = true;
        }
      } else {
        // Reset count if timing is wrong
        chg_toggle_count_ = 0;
      }

      last_chg_toggle_time_ = current_time;
      last_chg_state_       = current_state;
    }

    return fault_detected_;
  }

 public:
  /** @brief Actively wait and monitor for CHG pin flashing
   * 
   * This method actively monitors the CHG pin for a specified duration
   * to detect the flashing pattern that indicates a fault.
   * 
   * @param timeout_ms Maximum time to wait for flashing detection (default: 5000ms)
   * @param check_interval_ms How often to check the pin state (default: 5ms)
   * @return true if flashing is detected within the timeout period
   */
  bool waitForChgFlashing(uint32_t timeout_ms = 5000, uint32_t check_interval_ms = 5) {
    if (!initialized_ || !_Props::charge_status_pin)
      return false;

    // Reset detection state
    chg_toggle_count_     = 0;
    fault_detected_       = false;
    last_chg_state_       = digitalRead(_Props::charge_status_pin) == LOW;
    last_chg_toggle_time_ = millis();

    uint32_t start_time      = millis();
    uint32_t last_check_time = start_time;

    // Monitor until timeout or fault detected
    while (millis() - start_time < timeout_ms) {
      uint32_t current_time = millis();

      // Only check at the specified interval to avoid busy-waiting
      if (current_time - last_check_time >= check_interval_ms) {
        bool current_state = digitalRead(_Props::charge_status_pin) == LOW;

        // Detect state change
        if (current_state != last_chg_state_) {
          // Calculate time between toggles
          uint32_t toggle_interval = current_time - last_chg_toggle_time_;

          // If toggle interval is around 500ms (2Hz), increment counter
          if (toggle_interval > 300 && toggle_interval < 700) {
            chg_toggle_count_++;

            // If we see 4 toggles with the right timing, it's a fault
            if (chg_toggle_count_ >= 4) {
              fault_detected_ = true;
              return true;
            }
          } else {
            // Reset count if timing is wrong
            chg_toggle_count_ = 0;
          }

          last_chg_toggle_time_ = current_time;
          last_chg_state_       = current_state;
        }

        last_check_time = current_time;
      }

      // Small delay to prevent busy-waiting
      delay(1);
    }

    return false;
  }

  /** @brief Charging state enumeration
   * 
   * Detailed states of the battery charger
   */
  enum ChargeState {
    NOT_CHARGING         = 0,  // No power source or charge complete
    CHARGING             = 1,  // Actively charging
    CHARGE_COMPLETE      = 2,  // Charging completed
    CHARGE_FAULT         = 3,  // Timer or other fault detected
    NO_BATTERY           = 4,  // No battery detected
    BATTERY_DISCONNECTED = 5   // Battery manually disconnected via SYSOFF
  };

  /** @brief Constructor
   */
  BQ24075() {}

  /** @brief Initialize the BQ24075 hardware
   *
   * Configures the GPIO pins for monitoring the charger's status.
   */
  void setup() override {
    // Configure status pins as inputs with pull-ups (for open-drain outputs)
    if (_Props::power_good_pin) {
      pinMode(_Props::power_good_pin, INPUT_PULLUP);
      DEBUG_TRACE("BQ24075", "PGOOD pin configured: %d", _Props::power_good_pin);
    }

    if (_Props::charge_status_pin) {
      pinMode(_Props::charge_status_pin, INPUT_PULLUP);
      DEBUG_TRACE("BQ24075", "CHG pin configured: %d", _Props::charge_status_pin);
    }

    // Configure SYSOFF pin as output if used
    if (_Props::sysoff_pin) {
      pinMode(_Props::sysoff_pin, OUTPUT);
      digitalWrite(_Props::sysoff_pin, LOW);  // Normal operation by default
      DEBUG_TRACE("BQ24075", "SYSOFF pin configured: %d", _Props::sysoff_pin);
    }

    // Configure ISET pin as analog input if used
    if (_Props::iset_pin) {
      pinMode(_Props::iset_pin, INPUT);
      DEBUG_TRACE("BQ24075", "ISET pin configured: %d", _Props::iset_pin);
    }

    initialized_ = true;
    DEBUG_TRACE("BQ24075", "Initialization complete");
  }

  /** @brief Begin battery charger monitoring
   *
   * @return true if the charger was initialized successfully
   */
  bool begin() override {
    return initialized_;
  }

  /** @brief Check if external power is connected
   *
   * Reads the PGOOD pin to determine if external power is available.
   * PGOOD is an open-drain output that is LOW when input power supply is good.
   * With a pull-up resistor, it reads HIGH when no power or power is out of valid range.
   *
   * @return true if external power is connected and good, false otherwise
   */
  bool hasPower() const {
    if (!initialized_ || !_Props::power_good_pin)
      return false;

    return digitalRead(_Props::power_good_pin) == LOW;
  }

  /** @brief Check if the battery is currently charging
   *
   * Reads the CHG pin to determine if the battery is being charged.
   * CHG is an open-drain output that is LOW when charging.
   * With a pull-up resistor, it reads HIGH when charge complete or not charging.
   *
   * @return true if the battery is charging, false otherwise
   */
  bool isCharging() const {
    if (!initialized_ || !_Props::charge_status_pin)
      return false;

    return digitalRead(_Props::charge_status_pin) == LOW;
  }

  /** @brief Get the current charging state
   * 
   * Provides detailed information about the current charging state.
   * 
   * @return One of the following states:
   *   - 0: Not charging (no power source)
   *   - 1: Charging
   *   - 2: Charge complete
   *   - 3: Charge fault (timer fault)
   *   - 4: No battery
   *   - 5: Battery disconnected via SYSOFF
   */
  uint8_t getChargingState() const {
    if (!initialized_)
      return NOT_CHARGING;

    // Check if battery is manually disconnected
    if (_Props::sysoff_pin && digitalRead(_Props::sysoff_pin) == HIGH) {
      return BATTERY_DISCONNECTED;
    }

    bool has_power   = hasPower();
    bool is_charging = isCharging();

    if (!has_power) {
      return NOT_CHARGING;  // Not charging, no power source
    }

    // Check for timer fault (requires non-const method)
    if (const_cast<BQ24075 *>(this)->detectChgFlashing()) {
      return CHARGE_FAULT;
    }

    if (is_charging) {
      return CHARGING;  // Charging
    } else {
      // Could be either charge complete or no battery
      // Without additional signals, we can't definitively tell
      return CHARGE_COMPLETE;
    }
  }

  /** @brief Disconnect or reconnect the battery from the system
   * 
   * Controls the SYSOFF pin to disconnect/reconnect the battery.
   * 
   * @param disconnect true to disconnect battery, false to reconnect
   * @return true if operation successful, false if not supported
   */
  bool disconnectBattery(bool disconnect) {
    if (!initialized_ || !_Props::sysoff_pin)
      return false;

    digitalWrite(_Props::sysoff_pin, disconnect ? HIGH : LOW);
    return true;
  }

  /** @brief Check if battery is disconnected from the system
   * 
   * @return true if disconnected, false if connected or feature not supported
   */
  bool isBatteryDisconnected() const {
    if (!initialized_ || !_Props::sysoff_pin)
      return false;

    return digitalRead(_Props::sysoff_pin) == HIGH;
  }

  /** @brief Get the current charge current in mA
   * 
   * Uses the ISET pin to calculate current charge current.
   * Charge current = (VISET × 400) / RISET
   * 
   * @return Charge current in mA, or 0 if not supported
   */
  uint32_t getChargeCurrent() const {
    if (!initialized_ || !_Props::iset_pin)
      return 0;

    // Read analog value
    uint32_t adc_value = analogRead(_Props::iset_pin);

    // Convert to voltage in microvolts (depends on ADC reference voltage)
    // For 3.3V reference and 10-bit ADC: voltage_uv = adc_value * 3300000 / 1023
    uint32_t voltage_uv = (adc_value * 3300000) / 1023;

    // Calculate current in microamps: I = V/R
    // iset_current_ua = voltage_uv / iset_resistance_milliohms * 1000
    uint32_t iset_current_ua = (voltage_uv * 1000) / _Props::iset_resistance;

    // Charge current is 400x the current through ISET (in mA)
    uint32_t charge_current = (iset_current_ua * 400) / 1000;

    return charge_current;
  }

  /** @brief Check if a charge fault has been detected
   * 
   * @return true if a fault is detected, false otherwise
   */
  bool hasFault() const {
    return fault_detected_;
  }

  /** @brief Clear any fault detection state
   */
  void clearFault() {
    fault_detected_   = false;
    chg_toggle_count_ = 0;
  }
};

}  // namespace battery_charger
}  // namespace driver
}  // namespace kaleidoscope
