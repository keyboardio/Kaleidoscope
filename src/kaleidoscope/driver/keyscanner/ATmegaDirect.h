/* -*- mode: c++ -*-
 * Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2022  Keyboard.io, Inc
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

#pragma once

#include <stdint.h>  // for uint16_t, uint8_t

#include "kaleidoscope/device/avr/pins_and_ports.h"  // IWYU pragma: keep
#include "kaleidoscope/driver/keyscanner/Base.h"     // for BaseProps
#include "kaleidoscope/driver/keyscanner/None.h"     // for None

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
#include <avr/wdt.h>
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

namespace kaleidoscope {
namespace driver {
namespace keyscanner {

struct ATmegaDirectProps : public kaleidoscope::driver::keyscanner::BaseProps {
  static const uint16_t keyscan_interval = 1500;
  typedef uint16_t PinState;

  /*
   * The following lines declares empty arrays, and both must be shadowed by the
   * descendant keyscanner description class.
   */
  static constexpr uint8_t direct_pins[] = {};
  static constexpr uint8_t unused_pins[] = {};
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
template<typename _KeyScannerProps>
class ATmegaDirect : kaleidoscope::driver::keyscanner::Base<_KeyScannerProps> {
 private:
  typedef ATmegaDirect<_KeyScannerProps> ThisType;

 public:
  void setup() {
    static_assert(
        sizeof(_KeyScannerProps::direct_pins) > 0,
        "The key scanner description has an empty array of direct pins.");
    static_assert(
        _KeyScannerProps::matrix_rows == 1,
        "ATmegaDirect requires matrix_rows to be 1.");

    for (uint8_t i = 0; i < sizeof(_KeyScannerProps::unused_pins); i++) {
      DDR_OUTPUT(_KeyScannerProps::unused_pins[i]);
      DISABLE_PULLUP(_KeyScannerProps::unused_pins[i]);
    }

    for (uint8_t i = 0; i < sizeof(_KeyScannerProps::direct_pins); i++) {
      DDR_INPUT(_KeyScannerProps::direct_pins[i]);
      ENABLE_PULLUP(_KeyScannerProps::direct_pins[i]);
    }

    setScanCycleTime(_KeyScannerProps::keyscan_interval);
  }

  /* setScanCycleTime takes a value of between 0 and 8192. This corresponds
   * (roughly) to the number of microseconds to wait between scanning the key
   * matrix. Our debouncing algorithm does four checks before deciding that a
   * result is valid. Most normal mechanical switches specify a 5ms debounce
   * period. On an ATMega32U4, 1700 gets you about 5ms of debouncing.
   *
   * Because keycanning is triggered by an interrupt but not run in that
   * interrupt, the actual amount of time between scans is prone to a little bit
   * of jitter.
   */
  void setScanCycleTime(uint16_t c) {
    TCCR1B = _BV(WGM13);
    TCCR1A = 0;

    const uint32_t cycles = (F_CPU / 2000000) * c;

    ICR1   = cycles;
    TCCR1B = _BV(WGM13) | _BV(CS10);
    TIMSK1 = _BV(TOIE1);
  }

  void readMatrix() {
    typename _KeyScannerProps::PinState hot_pins = readPins();
    typename _KeyScannerProps::PinState debounced_changes =
        debounce(hot_pins, &(pin_state_.debouncer));

    if (debounced_changes) {
      pin_state_.current = pin_state_.debouncer.debounced_state;
    }
  }

  void scanMatrix() {
    if (do_scan_) {
      do_scan_ = false;
      readMatrix();
    }
    actOnMatrixScan();
  }

  void actOnMatrixScan() {
    for (uint8_t pin = 0; pin < sizeof(_KeyScannerProps::direct_pins); pin++) {
      uint8_t keyState = (bitRead(pin_state_.previous, pin) << 0) |
                         (bitRead(pin_state_.current, pin) << 1);
      if (keyState) {
        ThisType::handleKeyswitchEvent(Key_NoKey, typename _KeyScannerProps::KeyAddr(0, pin), keyState);
      }
    }
    pin_state_.previous = pin_state_.current;
  }

  uint8_t pressedKeyswitchCount() {
    return __builtin_popcount(pin_state_.current);
  }
  bool isKeyswitchPressed(typename _KeyScannerProps::KeyAddr key_addr) {
    return (bitRead(pin_state_.current, key_addr.col()) != 0);
  }

  uint8_t previousPressedKeyswitchCount() {
    return __builtin_popcount(pin_state_.previous);
  }
  bool wasKeyswitchPressed(typename _KeyScannerProps::KeyAddr key_addr) {
    return (bitRead(pin_state_.previous, key_addr.col()) != 0);
  }

  bool do_scan_;

 protected:
  /*
    each of these variables are storing the state for a row of keys

    so for key 0, the counter is represented by db0[0] and db1[0]
    and the state in debounced_state[0].
  */
  struct debounce_t {
    typename _KeyScannerProps::PinState db0;              // counter bit 0
    typename _KeyScannerProps::PinState db1;              // counter bit 1
    typename _KeyScannerProps::PinState debounced_state;  // debounced state
  };

  struct pin_state_t {
    typename _KeyScannerProps::PinState previous;
    typename _KeyScannerProps::PinState current;
    debounce_t debouncer;
  };

 private:
  typedef _KeyScannerProps KeyScannerProps_;
  static pin_state_t pin_state_;

  typename _KeyScannerProps::PinState
  readPins() {
    typename _KeyScannerProps::PinState hot_pins = 0;
    for (uint8_t i = 0; i < sizeof(_KeyScannerProps::direct_pins); i++) {
      hot_pins |= (!READ_PIN(_KeyScannerProps::direct_pins[i]) << i);
    }

    return hot_pins;
  }

  static inline typename _KeyScannerProps::PinState debounce(
      typename _KeyScannerProps::PinState sample, debounce_t *debouncer) {
    typename _KeyScannerProps::PinState delta, changes;

    // Use xor to detect changes from last stable state:
    // if a key has changed, it's bit will be 1, otherwise 0
    delta = sample ^ debouncer->debounced_state;

    // Increment counters and reset any unchanged bits:
    // increment bit 1 for all changed keys
    debouncer->db1 = ((debouncer->db1) ^ (debouncer->db0)) & delta;
    // increment bit 0 for all changed keys
    debouncer->db0 = ~(debouncer->db0) & delta;

    // Calculate returned change set: if delta is still true
    // and the counter has wrapped back to 0, the key is changed.

    changes = ~(~delta | (debouncer->db0) | (debouncer->db1));
    // Update state: in this case use xor to flip any bit that is true in changes.
    debouncer->debounced_state ^= changes;

    return changes;
  }
};
#else   // ifndef KALEIDOSCOPE_VIRTUAL_BUILD
template<typename _KeyScannerProps>
class ATmegaDirect : public keyscanner::None {};
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

}  // namespace keyscanner
}  // namespace driver
}  // namespace kaleidoscope
