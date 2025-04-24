/* Kaleidoscope-Hardware-Keyboardio-Preonic -- Keyboardio Preonic hardware support for Kaleidoscope
 * Copyright 2017-2025 Keyboard.io, inc.
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

#ifdef ARDUINO_PREONIC

#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>

// Include the CMSIS header through nrf.h which brings in all necessary definitions
#include "nrf.h"
// Remove this line as it might be causing issues
// #include "core_cmFunc.h"

#define CRGB(r, g, b) \
  (cRGB) {            \
    b, g, r           \
  }

struct cRGB {
  uint8_t b;
  uint8_t g;
  uint8_t r;
};

#include "RotaryEncoder.h"

#include "kaleidoscope/device/Base.h"
#include "kaleidoscope/driver/bootloader/nrf52/uf2.h"

#include "kaleidoscope/driver/hid/Base.h"
#include "kaleidoscope/driver/hid/Bluefruit.h"
#include "kaleidoscope/driver/hid/Hybrid.h"
#include "kaleidoscope/driver/hid/TinyUSB.h"
#include "kaleidoscope/driver/keyscanner/Base.h"
#include "kaleidoscope/driver/keyscanner/NRF52KeyScanner.h"
#include "kaleidoscope/driver/led/WS2812.h"
#include "kaleidoscope/driver/mcu/TinyUSB.h"
#include "kaleidoscope/driver/storage/NRF52Flash.h"
#include "kaleidoscope/driver/ble/Bluefruit.h"
#include "kaleidoscope/driver/speaker/Piezo.h"
#include "nrfx_gpiote.h"
#include "kaleidoscope/driver/battery_gauge/MAX17048.h"
#include "kaleidoscope/driver/battery_charger/BQ24075.h"
#include "kaleidoscope/power_event.h"
#include "nrf_power.h"
#include "nrf_gpio.h"
#include "nrf_clock.h"
#include "nrf_twim.h"
#include "nrf_twis.h"
#include "nrf_uarte.h"
#include "nrf_saadc.h"
#include "nrf_timer.h"
#include "nrf_rtc.h"
#include "nrf_pwm.h"
#include "nrf_nfct.h"
#include "nrf_usbd.h"
#include "nrf_ppi.h"
#include "nrf_gpiote.h"

namespace kaleidoscope {
namespace device {
namespace keyboardio {

using Color = kaleidoscope::driver::led::color::GRB;
// Forward declaration of a global function to send keystrokes
// This will be implemented in a plugin-accessible location
void sendKeyStrokes(const char* str);
// Overload to handle integer values
void sendKeyStrokes(int value);
// Overload to handle integer values with format specifier (HEX, DEC, etc.)
void sendKeyStrokes(uint32_t value, int format);

// Structure to define rotary encoder configuration
struct EncoderConfig {
  uint8_t pinA;
  uint8_t pinB;
  struct {
    uint8_t row;
    uint8_t col;
  } ccw;  // Counter-clockwise key address
  struct {
    uint8_t row;
    uint8_t col;
  } cw;  // Clockwise key address
};

// Move encoder definitions to namespace scope for shared access
static constexpr size_t NUM_ENCODERS                     = 3;
static const EncoderConfig ENCODER_CONFIGS[NUM_ENCODERS] = {
  {PIN_ENC1_A, PIN_ENC1_B, {0, 0}, {0, 1}},  // Encoder 1
  {PIN_ENC2_A, PIN_ENC2_B, {0, 2}, {0, 3}},  // Encoder 2
  {PIN_ENC3_A, PIN_ENC3_B, {0, 4}, {0, 5}}   // Encoder 3
};

// Custom keyscanner for Preonic that adds rotary encoder support
template<typename _KeyScannerProps>
class PreonicKeyScanner : public kaleidoscope::driver::keyscanner::NRF52KeyScanner<_KeyScannerProps> {
 private:
  typedef kaleidoscope::driver::keyscanner::NRF52KeyScanner<_KeyScannerProps> Parent;
  static int last_encoder_values_[NUM_ENCODERS];
  SwRotaryEncoder encoders_[NUM_ENCODERS];
  static PreonicKeyScanner *active_scanner_;  // Static pointer to active scanner instance

  // Static callbacks for each encoder
  static void handleEncoder0(int step) {
    if (!active_scanner_) return;
    if (step < 0) {
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[0].ccw.row, ENCODER_CONFIGS[0].ccw.col, true);
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[0].ccw.row, ENCODER_CONFIGS[0].ccw.col, false);
    } else {
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[0].cw.row, ENCODER_CONFIGS[0].cw.col, true);
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[0].cw.row, ENCODER_CONFIGS[0].cw.col, false);
    }
  }

  static void handleEncoder1(int step) {
    if (!active_scanner_) return;
    if (step < 0) {
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[1].ccw.row, ENCODER_CONFIGS[1].ccw.col, true);
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[1].ccw.row, ENCODER_CONFIGS[1].ccw.col, false);
    } else {
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[1].cw.row, ENCODER_CONFIGS[1].cw.col, true);
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[1].cw.row, ENCODER_CONFIGS[1].cw.col, false);
    }
  }

  static void handleEncoder2(int step) {
    if (!active_scanner_) return;
    if (step < 0) {
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[2].ccw.row, ENCODER_CONFIGS[2].ccw.col, true);
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[2].ccw.row, ENCODER_CONFIGS[2].ccw.col, false);
    } else {
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[2].cw.row, ENCODER_CONFIGS[2].cw.col, true);
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[2].cw.row, ENCODER_CONFIGS[2].cw.col, false);
    }
  }

  static const SwRotaryEncoder::callback_t encoder_callbacks_[NUM_ENCODERS];

 public:
  PreonicKeyScanner() {
    active_scanner_ = this;  // Store pointer to this instance
  }

  void setup() {
    Parent::setup();
    initializeEncoders();
  }


  void scanMatrix() {
    Parent::scanMatrix();
  }

 private:
  void initializeEncoders() {
    for (size_t i = 0; i < NUM_ENCODERS; i++) {
      pinMode(ENCODER_CONFIGS[i].pinA, INPUT_PULLUP);
      pinMode(ENCODER_CONFIGS[i].pinB, INPUT_PULLUP);
      encoders_[i].begin(ENCODER_CONFIGS[i].pinA, ENCODER_CONFIGS[i].pinB);
      encoders_[i].setCallback(encoder_callbacks_[i]);
      last_encoder_values_[i] = 0;
    }
  }
};

template<typename _KeyScannerProps>
int PreonicKeyScanner<_KeyScannerProps>::last_encoder_values_[kaleidoscope::device::keyboardio::NUM_ENCODERS];

template<typename _KeyScannerProps>
PreonicKeyScanner<_KeyScannerProps> *PreonicKeyScanner<_KeyScannerProps>::active_scanner_ = nullptr;

template<typename _KeyScannerProps>
const SwRotaryEncoder::callback_t PreonicKeyScanner<_KeyScannerProps>::encoder_callbacks_[NUM_ENCODERS] = {
  PreonicKeyScanner::handleEncoder0,
  PreonicKeyScanner::handleEncoder1,
  PreonicKeyScanner::handleEncoder2};

struct PreonicStorageProps : public kaleidoscope::driver::storage::NRF52FlashProps {
  static constexpr uint16_t length = 16384;
};

struct PreonicKeyScannerProps : public kaleidoscope::driver::keyscanner::NRF52KeyScannerProps {
  static constexpr uint32_t keyscan_interval_micros = 750;
  static constexpr uint8_t matrix_rows              = 6;
  static constexpr uint8_t matrix_columns           = 12;
  typedef MatrixAddr<matrix_rows, matrix_columns> KeyAddr;
  typedef uint16_t RowState;
  static constexpr uint8_t matrix_row_pins[matrix_rows]    = {0, 1, 2, 3, 4, 5};
  static constexpr uint8_t matrix_col_pins[matrix_columns] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
};

// If we need to override HID props:
struct PreonicHIDProps : public kaleidoscope::driver::hid::HybridProps {
  //typedef kaleidoscope::driver::hid::base::AbsoluteMouseProps AbsoluteMouseProps;
  //typedef kaleidoscope::driver::hid::base::AbsoluteMouse<AbsoluteMouseProps> AbsoluteMouse;
};

struct PreonicLEDProps : public kaleidoscope::driver::led::WS2812Props {
  static constexpr uint8_t led_count     = 4;
  static constexpr uint8_t pin           = 19;
  static constexpr uint8_t key_led_map[] = {
    2, 3, 1, 0  // The logical order of the LEDS should be clockwise, starting in the top left corner
  };
};

struct PreonicSpeakerProps : public kaleidoscope::driver::speaker::PiezoProps {
  static constexpr uint8_t pin = PIN_SPEAKER;
};

struct PreonicBatteryGaugeProps : public kaleidoscope::driver::battery_gauge::MAX17048Props {
  static constexpr uint8_t alert_pin = PIN_BATT_ALERT;  // Pin connected to MAX17048 ALERT output
  // Battery configuration for Preonic's 3.7V LiPo
  static constexpr uint16_t battery_voltage_min    = 3100;  // 2.9V cutoff
  static constexpr uint16_t battery_voltage_max    = 4200;  // 4.2V max
  static constexpr uint8_t battery_alert_threshold = 15;    // Alert at 15%
};

struct PreonicBatteryChargerProps : public kaleidoscope::driver::battery_charger::BQ24075Props {
  static constexpr uint8_t power_good_pin = PIN_POWER_GOOD;       // Pin connected to BQ24075 PGOOD output
  static constexpr uint8_t charge_status_pin = PIN_CHARGE_STATUS; // Pin connected to BQ24075 CHG output

};

struct PreonicProps : public kaleidoscope::device::BaseProps {
  typedef PreonicHIDProps HIDProps;
  typedef kaleidoscope::driver::hid::Hybrid<HIDProps> HID;

  typedef PreonicLEDProps LEDDriverProps;
  typedef kaleidoscope::driver::led::WS2812<LEDDriverProps> LEDDriver;

  typedef PreonicKeyScannerProps KeyScannerProps;
  typedef PreonicKeyScanner<KeyScannerProps> KeyScanner;

  typedef PreonicStorageProps StorageProps;
  typedef kaleidoscope::driver::storage::NRF52Flash<StorageProps> Storage;

  typedef kaleidoscope::driver::bootloader::nrf52::UF2 Bootloader;
  static constexpr const char *short_name = "preonic";

  typedef kaleidoscope::driver::mcu::TinyUSBProps MCUProps;
  typedef kaleidoscope::driver::mcu::TinyUSB<MCUProps> MCU;

  typedef kaleidoscope::driver::ble::BLEBluefruit BLE;

  static constexpr const bool isHybridHostConnection = true;

  typedef PreonicSpeakerProps SpeakerProps;
  typedef kaleidoscope::driver::speaker::Piezo<SpeakerProps> Speaker;

  typedef PreonicBatteryGaugeProps BatteryGaugeProps;
  typedef kaleidoscope::driver::battery_gauge::MAX17048<BatteryGaugeProps> BatteryGauge;
  
  typedef PreonicBatteryChargerProps BatteryChargerProps;
  typedef kaleidoscope::driver::battery_charger::BQ24075<BatteryChargerProps> BatteryCharger;
};

class Preonic : public kaleidoscope::device::Base<PreonicProps> {
 private:
  // Deep sleep timers
  static uint32_t last_activity_time_;                      // Used for deep sleep
  static constexpr uint32_t DEEP_SLEEP_TIMEOUT_MS = 2000;  // Enter deep sleep after 10s
  static volatile bool input_event_pending_;
  static uint32_t last_battery_update_;                     // Last battery level update time
  static constexpr uint32_t BATTERY_UPDATE_INTERVAL = 300000;  // 5 minutes in milliseconds

  // Battery monitoring variables and thresholds
  static uint16_t last_battery_voltage_mv_;
  static uint32_t last_battery_check_time_;
  static uint32_t last_warning_time_;
  static bool warning_active_;
  static bool shutdown_active_;
  static constexpr uint32_t BATTERY_CHECK_INTERVAL_MS = 10000;    // Check battery every 10 seconds
  static constexpr uint32_t WARNING_INTERVAL_MS = 10000;          // Warning flash interval (10 seconds)
  static constexpr uint32_t WARNING_DURATION_MS = 1000;           // Warning flash duration (1 second)
  static constexpr uint32_t SHUTDOWN_DURATION_MS = 10000;         // Shutdown indication duration (10 seconds)
  static constexpr uint16_t BATTERY_WARNING_THRESHOLD_MV = 3300;  // 3.3V
  static constexpr uint16_t BATTERY_SHUTDOWN_THRESHOLD_MV = 3100; // 3.1V
  
  // Battery status enum
  enum class BatteryStatus {
    Normal,     // Battery level is good
    Warning,    // Battery is low (warning threshold)
    Critical,   // Battery is critically low (shutdown threshold)
    Shutdown    // Battery is too low, system will shut down
  };
  static BatteryStatus battery_status_;

  /**
   * @brief Structure to track timer and RTC states for sleep/wake
   */
  struct TimerState {
    bool timer0_enabled;
    bool timer1_enabled;
    bool timer2_enabled;
    bool timer3_enabled;
    bool timer4_enabled;

    // RTC state
    bool rtc0_enabled;
    bool rtc1_enabled;
    bool rtc2_enabled;

    // RTC register backups
    uint32_t rtc0_intenset;
    uint32_t rtc0_evten;
    uint32_t rtc1_intenset;
    uint32_t rtc1_evten;
    uint32_t rtc2_intenset;
    uint32_t rtc2_evten;
  };
  static TimerState timer_state_;

  // Battery state tracking
  static uint8_t last_battery_level_;

  /// @brief Configure a pin for wake-on-low sensing
  /// @param arduino_pin The Arduino pin number to configure
  static void enablePinSensing(uint8_t arduino_pin) {
    uint32_t nrf_pin = g_ADigitalPinMap[arduino_pin];
    if (nrf_pin >= 32) {
      NRF_P1->PIN_CNF[nrf_pin - 32] |= (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos);
    } else {
      NRF_GPIO->PIN_CNF[nrf_pin] |= (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos);
    }
  }

  /// @brief Disable wake-on-low sensing for a pin
  /// @param arduino_pin The Arduino pin number to configure
  static void disablePinSensing(uint8_t arduino_pin) {
    uint32_t nrf_pin = g_ADigitalPinMap[arduino_pin];
    if (nrf_pin >= 32) {
      NRF_P1->PIN_CNF[nrf_pin - 32] &= ~(GPIO_PIN_CNF_SENSE_Msk);
    } else {
      NRF_GPIO->PIN_CNF[nrf_pin] &= ~(GPIO_PIN_CNF_SENSE_Msk);
    }
  }

  // Configure all column pins for PORT event detection
  static void configureColumnsForSensing() {
    for (uint8_t i = 0; i < KeyScannerProps::matrix_columns; i++) {
      enablePinSensing(KeyScannerProps::matrix_col_pins[i]);
    }
    // Let's see if we can wake up from the battery alert pin
    enablePinSensing(BatteryGaugeProps::alert_pin);
  }

  // Configure all column pins to stop sensing
  static void disableColumnSensing() {
    for (uint8_t i = 0; i < KeyScannerProps::matrix_columns; i++) {
      disablePinSensing(KeyScannerProps::matrix_col_pins[i]);
    }
    disablePinSensing(BatteryGaugeProps::alert_pin);
  }


  // Drive all rows LOW to enable key detection across the entire matrix
  static void prepareMatrixForSleep() {
    // Drive all rows LOW so we can detect any keypress
    for (uint8_t i = 0; i < KeyScannerProps::matrix_rows; i++) {
      digitalWrite(KeyScannerProps::matrix_row_pins[i], LOW);
    }
  }

  // Restore matrix pins to their normal scanning state
  static void restoreMatrixAfterSleep() {
    // Reset all rows to HIGH
    for (uint8_t i = 0; i < KeyScannerProps::matrix_rows; i++) {
      digitalWrite(KeyScannerProps::matrix_row_pins[i], HIGH);
    }
  }

 private:
  /**
   * Sets up the Programmable Peripheral Interconnect (PPI) system to handle GPIO PORT events
   * 
   * This implementation is based on the solution by jgartrel:
   * https://github.com/adafruit/Adafruit_nRF52_Arduino/issues/754#issuecomment-1437329605
   * 
   * The nRF52 PPI system allows events from one peripheral to trigger actions in another
   * peripheral without CPU intervention. Here we use it to:
   * 
   * 1. Route GPIOTE PORT events to a Software Interrupt (SWI3) via EGU3
   * 2. Avoid conflicts with the Arduino core's GPIOTE interrupt handler
   * 
   * The setup process:
   * a) Configure SWI3_EGU3 interrupt with priority 6 (same as SDK drivers)
   * b) Enable EGU3 interrupt for channel 0
   * c) Connect GPIOTE PORT event to EGU3 trigger via PPI channel 0
   * d) Enable the PPI channel
   */
  static void setupPPIInterrupt() {
    // Set up SWI3_EGU3 interrupt with priority 6 (same as SDK drivers)
    NVIC_SetPriority(SWI3_EGU3_IRQn, 6);
    NVIC_ClearPendingIRQ(SWI3_EGU3_IRQn);
    NVIC_EnableIRQ(SWI3_EGU3_IRQn);

    // Enable EGU3 (Event Generator Unit) interrupt for channel 0
    // EGU is used to trigger the software interrupt
    NRF_EGU3->INTENSET = EGU_INTEN_TRIGGERED0_Msk;

    // Configure PPI channel 0:
    // - Event End Point (EEP): GPIOTE PORT event
    // - Task End Point (TEP): EGU3 trigger task
    NRF_PPI->CH[0].EEP = (uint32_t)&NRF_GPIOTE->EVENTS_PORT;
    NRF_PPI->CH[0].TEP = (uint32_t)&NRF_EGU3->TASKS_TRIGGER[0];

    // Enable PPI channel 0
    NRF_PPI->CHENSET = PPI_CHENSET_CH0_Msk;
  }

  /**
   * Sets up GPIO and GPIOTE for matrix column sensing and wake-on-keypress
   * 
   * This function:
   * 1. Configures all matrix columns as inputs with pull-ups
   * 2. Enables sense detection (LOW) on all columns for wake-on-keypress
   * 3. Sets up PPI to route PORT events to our interrupt handler
   * 
   * The nRF52's PORT event is triggered when any pin's sense condition is met,
   * making it perfect for wake-on-keypress from any key. However, we can't use
   * the standard GPIOTE interrupt handler as it's already used by the Arduino
   * core. Instead, we use PPI to route PORT events to a separate interrupt.
   */
  static void setupGPIOTE() {
    // Configure each column pin for sense detection
    for (uint8_t i = 0; i < KeyScannerProps::matrix_columns; i++) {
      uint8_t arduino_pin = KeyScannerProps::matrix_col_pins[i];
      uint32_t nrf_pin    = g_ADigitalPinMap[arduino_pin];

      // Configure pin as input with pull-up and LOW sense detection
      // When the pin goes LOW (key pressed), it will trigger a PORT event
      nrf_gpio_cfg_sense_input(nrf_pin, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
    }

    // Configure all rotary encoder pins for sense detection
    for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
      // Configure both A and B pins for each encoder
      uint32_t encoder_a = g_ADigitalPinMap[ENCODER_CONFIGS[i].pinA];
      uint32_t encoder_b = g_ADigitalPinMap[ENCODER_CONFIGS[i].pinB];

      // Configure encoder pins with pull-ups and LOW sense detection
      // This won't interfere with SwEncoder because we're just adding sense detection
      // to the existing pin configuration
      nrf_gpio_cfg_sense_input(encoder_a, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
      nrf_gpio_cfg_sense_input(encoder_b, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
    }

    // Set up PPI to route PORT events to our interrupt handler
    setupPPIInterrupt();

    input_event_pending_ = false;
  }

  /**
   * Enter deep sleep mode. Will only wake on GPIO activity.
   * In this mode:
   * - Main loop is suspended
   * - CPU enters low power mode
   * - GPIO sense remains active to detect key presses
   * 
   * @returns true if woken by GPIO activity
   */
  /**
   * @brief Safely disable non-critical timers
   * @details Only disables timers that are safe to stop, preserving system-critical ones
   */
  static void disableTimers() {
    // Save current timer states
    timer_state_.timer2_enabled = (NRF_TIMER2->INTENSET != 0);
    timer_state_.timer3_enabled = (NRF_TIMER3->INTENSET != 0);
    timer_state_.timer4_enabled = (NRF_TIMER4->INTENSET != 0);

    // TIMER0: System timer - don't touch
    // TIMER1: Keyscanner - managed by NRF52KeyScanner

    // TIMER2: Application timer
    if (timer_state_.timer2_enabled) {
      // Stop the timer
      NRF_TIMER2->TASKS_STOP = 1;
      // Disable timer interrupt
      NVIC_DisableIRQ(TIMER2_IRQn);
      // Clear any pending interrupts
      NVIC_ClearPendingIRQ(TIMER2_IRQn);
    }

    // TIMER3: Application timer
    if (timer_state_.timer3_enabled) {
      NRF_TIMER3->TASKS_STOP = 1;
      NVIC_DisableIRQ(TIMER3_IRQn);
      NVIC_ClearPendingIRQ(TIMER3_IRQn);
    }

    // TIMER4: Application timer
    if (timer_state_.timer4_enabled) {
      NRF_TIMER4->TASKS_STOP = 1;
      NVIC_DisableIRQ(TIMER4_IRQn);
      NVIC_ClearPendingIRQ(TIMER4_IRQn);
    }
  }

  /**
   * @brief Helper function to disable a specific RTC
   * @param rtc_num RTC number (0, 1, or 2)
   * @returns true if RTC was disabled, false if invalid RTC number
   */
  static bool disableRTCHelper(uint8_t rtc_num) {
    NRF_RTC_Type *rtc;
    IRQn_Type irq;
    bool *enabled_state;
    uint32_t *intenset_backup;
    uint32_t *evten_backup;

    switch (rtc_num) {
    case 0:
      rtc             = NRF_RTC0;
      irq             = RTC0_IRQn;
      enabled_state   = &timer_state_.rtc0_enabled;
      intenset_backup = &timer_state_.rtc0_intenset;
      evten_backup    = &timer_state_.rtc0_evten;
      break;
    case 1:
      rtc             = NRF_RTC1;
      irq             = RTC1_IRQn;
      enabled_state   = &timer_state_.rtc1_enabled;
      intenset_backup = &timer_state_.rtc1_intenset;
      evten_backup    = &timer_state_.rtc1_evten;
      break;
    case 2:
      rtc             = NRF_RTC2;
      irq             = RTC2_IRQn;
      enabled_state   = &timer_state_.rtc2_enabled;
      intenset_backup = &timer_state_.rtc2_intenset;
      evten_backup    = &timer_state_.rtc2_evten;
      break;
    default:
      return false;
    }

    // Save current state
    *enabled_state   = (rtc->INTENSET != 0);
    *intenset_backup = rtc->INTENSET;
    *evten_backup    = rtc->EVTEN;

    // Clear all interrupts and events
    rtc->INTENCLR = 0xFFFFFFFF;
    rtc->EVTENCLR = 0xFFFFFFFF;

    // Clear all event flags
    rtc->EVENTS_TICK       = 0;
    rtc->EVENTS_OVRFLW     = 0;
    rtc->EVENTS_COMPARE[0] = 0;
    rtc->EVENTS_COMPARE[1] = 0;
    rtc->EVENTS_COMPARE[2] = 0;
    rtc->EVENTS_COMPARE[3] = 0;

    // Stop the RTC
    rtc->TASKS_STOP = 1;

    // Disable RTC interrupt
    NVIC_DisableIRQ(irq);
    // Clear any pending interrupts
    NVIC_ClearPendingIRQ(irq);

    return true;
  }

  /**
   * @brief Helper function to restore a specific RTC
   * @param rtc_num RTC number (0, 1, or 2)
   * @returns true if RTC was restored, false if invalid RTC number
   */
  static bool restoreRTCHelper(uint8_t rtc_num) {
    NRF_RTC_Type *rtc;
    IRQn_Type irq;
    bool enabled_state;
    uint32_t intenset_backup;
    uint32_t evten_backup;

    switch (rtc_num) {
    case 0:
      rtc             = NRF_RTC0;
      irq             = RTC0_IRQn;
      enabled_state   = timer_state_.rtc0_enabled;
      intenset_backup = timer_state_.rtc0_intenset;
      evten_backup    = timer_state_.rtc0_evten;
      break;
    case 1:
      rtc             = NRF_RTC1;
      irq             = RTC1_IRQn;
      enabled_state   = timer_state_.rtc1_enabled;
      intenset_backup = timer_state_.rtc1_intenset;
      evten_backup    = timer_state_.rtc1_evten;
      break;
    case 2:
      rtc             = NRF_RTC2;
      irq             = RTC2_IRQn;
      enabled_state   = timer_state_.rtc2_enabled;
      intenset_backup = timer_state_.rtc2_intenset;
      evten_backup    = timer_state_.rtc2_evten;
      break;
    default:
      return false;
    }

    // Restore interrupt and event configuration
    rtc->INTENSET = intenset_backup;
    rtc->EVTEN    = evten_backup;

    if (enabled_state) {
      // Clear any pending interrupts
      NVIC_ClearPendingIRQ(irq);
      // Re-enable RTC interrupt
      NVIC_EnableIRQ(irq);
      // Start the RTC
      rtc->TASKS_START = 1;
    }

    return true;
  }

  /**
   * @brief Disable all RTC functions
   */
  static void disableRTC() {
    // disableRTCHelper(0);  // RTC0
    disableRTCHelper(1);  // RTC1
    disableRTCHelper(2);  // RTC2
  }


  /**
   * @brief Restore all previously disabled RTC functions
   */
  static void restoreRTC() {
    // restoreRTCHelper(0);  // RTC0
    restoreRTCHelper(1);  // RTC1
    restoreRTCHelper(2);  // RTC2
  }

  /**
   * @brief Restore previously enabled timers and RTCs
   * @details Restores timer states saved by disableTimersAndRTC()
   */
  /** 
   * @brief Restore previously disabled timers
   * @details Only restores TIMER3 and TIMER4 that we explicitly disabled
   */
  static void restoreTimers() {
    // TIMER0: System timer - don't touch
    // TIMER1: Keyscanner - managed by NRF52KeyScanner

    // TIMER2: Application timer
    if (timer_state_.timer2_enabled) {
      // Clear any pending interrupts
      NVIC_ClearPendingIRQ(TIMER2_IRQn);
      // Re-enable timer interrupt
      NVIC_EnableIRQ(TIMER2_IRQn);
      // Start the timer
      NRF_TIMER2->TASKS_START = 1;
    }

    // TIMER3: Application timer
    if (timer_state_.timer3_enabled) {
      NVIC_ClearPendingIRQ(TIMER3_IRQn);
      NVIC_EnableIRQ(TIMER3_IRQn);
      NRF_TIMER3->TASKS_START = 1;
    }

    // TIMER4: Application timer
    if (timer_state_.timer4_enabled) {
      NVIC_ClearPendingIRQ(TIMER4_IRQn);
      NVIC_EnableIRQ(TIMER4_IRQn);
      NRF_TIMER4->TASKS_START = 1;
    }
  }


  /**
   * @brief Structure to store TWI state during sleep
   */
  struct TWIState {
    uint32_t frequency;
    uint32_t pin_scl;
    uint32_t pin_sda;
    bool enabled;
  };
  static TWIState twi_state_;

  /**
   * @brief Disable TWIM for sleep
   * @details Directly forces power off for the TWIM0 peripheral using multiple methods
   */
  static void disableTWIForSleep() {
    // End the Wire library connection
    Wire.end();
    
    // Save TWIM0 state for restoration
    twi_state_.frequency = NRF_TWIM0->FREQUENCY;
    twi_state_.pin_scl   = NRF_TWIM0->PSEL.SCL;
    twi_state_.pin_sda   = NRF_TWIM0->PSEL.SDA;
    twi_state_.enabled   = true;
  }

  /**
   * @brief Restore TWIM after sleep
   * @details Restores TWIM configuration if it was previously enabled
   */
  static void restoreTWIAfterSleep() {
    if (!twi_state_.enabled) return;
    
    // Restore TWIM0 configuration
    NRF_TWIM0->FREQUENCY = twi_state_.frequency;
    NRF_TWIM0->PSEL.SCL  = twi_state_.pin_scl;
    NRF_TWIM0->PSEL.SDA  = twi_state_.pin_sda;
    
    // Re-enable IRQ
    NVIC_ClearPendingIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);
    NVIC_EnableIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);

    // Enable TWIM0
    NRF_TWIM0->ENABLE = TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos;

    twi_state_.enabled = false;
    
    // Reinitialize Wire library
    Wire.begin();
  }

  /**
   * @brief Check if recovery mode keys are being held
   * @return true if recovery mode should be entered
   */
  bool checkRecoveryMode() {
    // Configure GPIO pins directly for key detection
    // This avoids initializing the full key scanner
    for (uint8_t i = 0; i < KeyScannerProps::matrix_rows; i++) {
      pinMode(KeyScannerProps::matrix_row_pins[i], OUTPUT);
      digitalWrite(KeyScannerProps::matrix_row_pins[i], HIGH);
    }
    for (uint8_t i = 0; i < KeyScannerProps::matrix_columns; i++) {
      pinMode(KeyScannerProps::matrix_col_pins[i], INPUT_PULLUP);
    }
    
    uint32_t start_time = millis();
    bool recovery_keys_held = false;
    
    while (millis() - start_time < 5000) {

      // Simple matrix scan without debouncing
      uint8_t pressed_count = 0;
      bool r1c0_pressed = false;
      bool r1c11_pressed = false;
      bool r5c11_pressed = false;
      
      for (uint8_t row = 0; row < KeyScannerProps::matrix_rows; row++) {
        digitalWrite(KeyScannerProps::matrix_row_pins[row], LOW);
        delayMicroseconds(10);
        
        for (uint8_t col = 0; col < KeyScannerProps::matrix_columns; col++) {
          if (!digitalRead(KeyScannerProps::matrix_col_pins[col])) {
            pressed_count++;
            
            // Check for recovery mode keys
            if (row == 1 && col == 0) r1c0_pressed = true;
            if (row == 1 && col == 11) r1c11_pressed = true;
            if (row == 5 && col == 11) r5c11_pressed = true;
          }
        }
        digitalWrite(KeyScannerProps::matrix_row_pins[row], HIGH);
      }

      // Check if any other keys are pressed
      if (pressed_count > 3) {
        return false;
      }
      
      // Check if required keys are held
      if (!r1c0_pressed || !r1c11_pressed || !r5c11_pressed) {
        return false;
      }

    if (recovery_keys_held == false) {
          ledDriver().setup();
      ledDriver().setBrightness(100);
      // Set all the LEDs to yellow
      for (uint8_t i = 0; i < PreonicProps::LEDDriverProps::led_count; i++) {
        setCrgbAt(i, CRGB(255, 255, 0));
      }
      syncLeds();
    }
      recovery_keys_held = true;
      delay(10); // Small delay to prevent busy-waiting
    }
    return recovery_keys_held;
  }

  /**
   * @brief Handle recovery mode operations
   */
  void handleRecoveryMode() {

    // Set all LEDs to red
    for (uint8_t i = 0; i < PreonicProps::LEDDriverProps::led_count; i++) {
      setCrgbAt(i, CRGB(255, 0, 0));
    }
    syncLeds();
    delay(1000);
    storage().setup();
    storage().erase();
    // Set the leds to green and then pause for 2 seconds
    for (uint8_t i = 0; i < PreonicProps::LEDDriverProps::led_count; i++) {
      setCrgbAt(i, CRGB(0, 255, 0));
    }
    syncLeds();
    delay(2000);
    
    NVIC_SystemReset();
  }

  bool enterDeepSleep() {
   // prepareBLEForSleep();
    disableLEDPower();
    keyScanner().suspendTimer();
    prepareMatrixForSleep();
    configureColumnsForSensing();
    setupGPIOTE();
    speaker().prepareForSleep();
    // Bluefruit hid - process all queue reports, then shut down processing
    while (kaleidoscope::driver::hid::bluefruit::blehid.hasQueuedReports()) {
      delay(1);
    }
    kaleidoscope::driver::hid::bluefruit::blehid.stopReportProcessing();

    disableTWIForSleep();
    disableRTC();
  //disableTimers(); // Disabling timers seems to make the keyscanner a little sad

    // Disable FPU state preservation to prevent ~3mA power drain in sleep
    disableFPUForSleep();

    sd_power_mode_set(NRF_POWER_MODE_LOWPWR);

    while (!input_event_pending_) {
      waitForEvent();
    }

    sd_power_mode_set(NRF_POWER_MODE_CONSTLAT);
    restoreMatrixAfterSleep();
    disableColumnSensing();
    //restoreTimers();
    restoreRTC();
    restoreTWIAfterSleep();
  
    // Start processing BLE HID reports
    kaleidoscope::driver::hid::bluefruit::blehid.startReportProcessing();

    // Resume the keyscanner timer immediately
    keyScanner().resumeTimer();

    enableLEDPower();
   // restoreBLEAfterSleep();

    last_activity_time_ = millis();

    return true;
  }

  /**
   * Check if we should enter deep sleep based on inactivity time
   * Never enter deep sleep if USB is connected
   */
  bool shouldEnterDeepSleep() {
    uint32_t now = millis();
    // Never sleep if USB is connected
    if (mcu_.USBConfigured()) {
      return false;
    }
    if (keyScanner().pressedKeyswitchCount()) {
      last_activity_time_ = now;
      return false;
    }

    // Don't sleep if the keyscanner has events in queue
    if (keyScanner().hasQueuedEvents()) {
      last_activity_time_ = now;
      return false;
    }

    // Don't sleep if there are pending BLE HID reports
    if (kaleidoscope::driver::hid::bluefruit::blehid.hasQueuedReports()) {
      last_activity_time_ = now;
      return false;
    }

    // Don't sleep if LEDs were recently active
    if ((now - ledDriver().LEDsLastOn()) < 1000) {
      return false;
    }

    if (now - last_activity_time_ >= DEEP_SLEEP_TIMEOUT_MS) {
      return true;
    }
    return false;
  }

  // Battery monitoring methods
  void updateBatteryLevel() {
  uint8_t  new_level= batteryGauge().getBatteryLevel();
    if (new_level != ble().getBatteryLevel()) {
     ble().setBatteryLevel(new_level);
    }
  }
  
  /**
   * @brief Check if battery voltage is below warning threshold
   * @return true if battery is below warning threshold
   */
  bool isBatteryBelowWarningThreshold(uint16_t voltage_mv) {
    return voltage_mv <= BATTERY_WARNING_THRESHOLD_MV;
  }
  
  /**
   * @brief Check if battery voltage is below shutdown threshold
   * @return true if battery is below shutdown threshold
   */
  bool isBatteryBelowShutdownThreshold(uint16_t voltage_mv) {
    return voltage_mv <= BATTERY_SHUTDOWN_THRESHOLD_MV;
  }
  
  /**
   * @brief Trigger low battery warning
   * @param active true to activate warning, false to deactivate
   */
  void triggerBatteryWarning(bool active);
  
  /**
   * @brief Trigger battery shutdown sequence
   */
  void triggerBatteryShutdown();
  
  /**
   * @brief Put the system into deep sleep or system off state
   */
  void systemOff() {
    // Disable interrupts during shutdown
    noInterrupts();
    
    // Disable all peripherals
    NRF_SAADC->ENABLE = 0;
    
    // Disable FPU state preservation to prevent power drain
    disableFPUForSleep();
    
    // Use system off functionality to fully power down
    sd_power_system_off();
  }
  
  /**
   * @brief Update battery status based on current voltage
   * @param voltage_mv Current battery voltage in millivolts
   */
  void updateBatteryStatus(uint16_t voltage_mv) {
    // Get current time
    uint32_t now = millis();

    // Check if it's time to check the battery
    if (now - last_battery_check_time_ >= BATTERY_CHECK_INTERVAL_MS) {
      // Update the battery voltage
      last_battery_voltage_mv_ = voltage_mv;
      last_battery_check_time_ = now;

      // Only check thresholds if running on battery
      if (!mcu_.USBConfigured()) {
        // Check for shutdown threshold
        if (isBatteryBelowShutdownThreshold(voltage_mv)) {
          battery_status_ = BatteryStatus::Shutdown;
          // Right now, the best thing we can do is to turn off Bluetooth and the LED and the keyscanner.

          prepareBLEForSleep();
          disableLEDPower();
          keyScanner().suspendTimer();
          prepareMatrixForSleep();
          configureColumnsForSensing();
          setupGPIOTE();
          speaker().prepareForSleep();

          kaleidoscope::driver::hid::bluefruit::blehid.stopReportProcessing();

          Bluefruit.Advertising.stop();

          disableTWIForSleep();
          disableRTC();
          // disableTimers();  // Disabling timers seems to make the keyscanner a little sad

          // Disable FPU state preservation to prevent ~3mA power drain in sleep
          disableFPUForSleep();

          sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
         // vTaskSuspendAll();
          while (1) {
            waitForEvent();
          }


        }
        // Check for warning threshold
        else if (isBatteryBelowWarningThreshold(voltage_mv)) {
          if (battery_status_ != BatteryStatus::Warning) {
            battery_status_ = BatteryStatus::Warning;
            // Reset warning timing to ensure immediate warning
            last_warning_time_ = now - WARNING_INTERVAL_MS;
          }
        }
        // Normal operation
        else {
          battery_status_ = BatteryStatus::Normal;
          warning_active_ = false;
        }
      } else {
        // On USB power, battery status is normal
        battery_status_ = BatteryStatus::Normal;
        warning_active_ = false;
      }
    }

    // Handle warning indication timing
    if (battery_status_ == BatteryStatus::Warning) {
      if (!warning_active_ && (now - last_warning_time_ >= WARNING_INTERVAL_MS)) {
        // Start a new warning period
        warning_active_    = true;
        last_warning_time_ = now;
        triggerBatteryWarning(true);
      } else if (warning_active_ && (now - last_warning_time_ >= WARNING_DURATION_MS)) {
        // End the current warning period
        warning_active_ = false;
        triggerBatteryWarning(false);
      }
    }
  }

 void prepareBLEForSleep() {
    Bluefruit.Advertising.restartOnDisconnect(false);

 }
 void restoreBLEAfterSleep() {
    Bluefruit.Advertising.restartOnDisconnect(true);
 }


 public:
  Preonic() {
  }


  void betweenCycles() {
    uint32_t now = millis();
    // Handle speaker updates
    // TODO(jesse): move this into a hook
    updateSpeaker();

    // Manage LED power based on LED activity
    if (ledDriver().areAnyLEDsOn() || ((now - ledDriver().LEDsLastOn()) < 1000)) {
      enableLEDPower();
    } else {
      disableLEDPower();
      // Set the LED pin to disconnected to prevent current leakage
      pinMode(PreonicProps::LEDDriverProps::pin, OUTPUT_D0S1);

    }


    // Check if we should update battery level for BLE (every 5m or on alert)
    // In theory, we shouldn't need to do this every 5m. But I don't totally trust the alert updates yet.
    if (batteryGauge().hasAlert() || (now - last_battery_update_ >= BATTERY_UPDATE_INTERVAL)) {
      updateBatteryLevel();
      if (batteryGauge().hasAlert()) {
        batteryGauge().clearAlert();
      }
      // Update battery monitoring - this handles both warning and shutdown thresholds
      // Use the MAX17048 battery gauge to provide the voltage reading
      uint16_t battery_voltage = batteryGauge().getVoltage();
      updateBatteryStatus(battery_voltage);
      last_battery_update_ = now;
    }

    // Handle any pending GPIOTE events
    if (input_event_pending_) {
      input_event_pending_ = false;
      last_activity_time_  = now;  // Update activity time on GPIOTE event

    } else if (shouldEnterDeepSleep()) {
      // Check if we should enter deep sleep
      // Only enter deep sleep if no keys are pressed and we're not connected via USB
      enterDeepSleep();
    }

    // In the future, this should run in response to a USB connect/disconnect event rather than on every cycle
    // Enabling this method will cause the device to automatically switch to USB mode if it's connected to
    // a USB host, even if the user is explicitlytrying to connect to a bluetooth host
    // autoHostConnectionMode();
  }


  void enableLEDPower() {
    pinMode(PIN_LED_ENABLE, OUTPUT);

    digitalWrite(PIN_LED_ENABLE, HIGH);
  }

  void disableLEDPower() {
    pinMode(PIN_LED_ENABLE, OUTPUT);

    digitalWrite(PIN_LED_ENABLE, LOW);
  }


  /**
   * @brief Disable unused peripherals at startup
   * @details Disables and properly cleans up UART, ADC, TWI Slave, SPI, and NFC
   */
  static void disableUnusedPeripherals() {
    // Disable UART
    NRF_UARTE0->TASKS_STOPTX  = 1;
    NRF_UARTE0->TASKS_STOPRX  = 1;
    NRF_UARTE0->EVENTS_TXDRDY = 0;
    NRF_UARTE0->EVENTS_RXDRDY = 0;
    NRF_UARTE0->ENABLE        = 0;

    // Disable ADC
    NRF_SAADC->TASKS_STOP  = 1;
    NRF_SAADC->EVENTS_DONE = 0;
    NRF_SAADC->ENABLE      = 0;

    // Disable TWI Slave
    NRF_TWIS0->TASKS_STOP   = 1;
    NRF_TWIS0->EVENTS_READ  = 0;
    NRF_TWIS0->EVENTS_WRITE = 0;
    NRF_TWIS0->ENABLE       = 0;

    // Disable SPI
    NRF_SPI0->EVENTS_READY = 0;
    NRF_SPI0->ENABLE       = 0;

    // Disable NFC
    NRF_NFCT->TASKS_DISABLE        = 1;
    NRF_NFCT->EVENTS_FIELDDETECTED = 0;
    NRF_NFCT->EVENTS_FIELDLOST     = 0;
    NRF_NFCT->EVENTS_READY         = 0;
  }


  void setup() {
  
    enableLEDPower();
    // Check for recovery mode before full initialization
    if (checkRecoveryMode()) {
      // Handle recovery mode - this will reboot when done
      handleRecoveryMode();
      // We never get here due to the reboot
    }

    // Work around some BSP problems with TinyUSB and Serial1 logging
#if CFG_LOGGER == 1
    Serial1.begin(115200);
#endif

    // Disable debug interface if not actively debugging
    NRF_CLOCK->TRACECONFIG = 0;

      disableUnusedPeripherals(); // As of this writing, disableUnusedPeripherals() does not provide a measurable power efficiency improvement
    // Turn on the LED power


    device::Base<PreonicProps>::setup();
    last_activity_time_ = millis();
    last_battery_check_time_ = millis();
    last_warning_time_ = millis();
    warning_active_ = false;
    shutdown_active_ = false;
    battery_status_ = BatteryStatus::Normal;
    updateBatteryLevel();
  }

  Stream &serialPort() {
    return Serial;  // For now, we *always* use USB Serial
    if (getHostConnectionMode() == MODE_USB && mcu_.USBConfigured()) {
      return Serial;
    } else {
      return ble().serialPort();
    }
  }

  void initSerial() {
    Serial.begin(9600);
  }
void complete_system_shutdown(void);
 public:
  static void setInputEventPending() {
    input_event_pending_ = true;
  }

  /**
   * @brief Disable the FPU (Floating Point Unit) state preservation before sleep
   *
   * This prevents the CPU from keeping the FPU powered during sleep mode.
   * When floating point operations occur, the Cortex-M4F processor activates 
   * the FPU hardware, which can remain partially powered during sleep unless
   * explicitly disabled, causing a power drain of approximately 3mA.
   */
  static void disableFPUForSleep() {
    

        // Clear FPCA bit in CONTROL register to indicate no active FP context
        // This is the most reliable way to prevent FPU power drain
        __set_CONTROL(__get_CONTROL() & ~(1U << 2));
        
        // Memory barriers to ensure completion
        __DSB();
        __ISB();
   
  }
};

}  // namespace keyboardio
}  // namespace device


EXPORT_DEVICE(kaleidoscope::device::keyboardio::Preonic)

}  // namespace kaleidoscope

// clang-format off

#define PER_LED_DATA(dft, r0c0, r0c1, r0c2, r0c3, ...)\
		          r0c0, r0c1, r0c2, RESTRICT_ARGS_COUNT((r0c3), 4, LEDMAP, ##__VA_ARGS__)

#define PER_KEY_DATA(dflt,                                                                                 \
                                r0c4, r0c5,                  r0c9, r0c10, r0c11,  \
        r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11,  \
        r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11,  \
        r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, \
        r4c0, r4c1, r4c2, r4c3, r4c4, r4c5, r4c6, r4c7, r4c8, r4c9, r4c10, r4c11,  \
        r5c0, r5c1, r5c2, r5c3, r5c4, r5c5, r5c6, r5c7, r5c8, r5c9, r5c10, r5c11, ...)\
         XXX,  XXX,  XXX,  XXX, r0c4, r0c5, XXX,   XXX,  XXX, r0c9, r0c10, r0c11, \
        r1c0, r1c1, r1c2, r1c3, r1c4, r1c5, r1c6, r1c7, r1c8, r1c9, r1c10, r1c11, \
        r2c0, r2c1, r2c2, r2c3, r2c4, r2c5, r2c6, r2c7, r2c8, r2c9, r2c10, r2c11, \
        r3c0, r3c1, r3c2, r3c3, r3c4, r3c5, r3c6, r3c7, r3c8, r3c9, r3c10, r3c11, \
        r4c0, r4c1, r4c2, r4c3, r4c4, r4c5, r4c6, r4c7, r4c8, r4c9, r4c10, r4c11, \
        r5c0, r5c1, r5c2, r5c3, r5c4, r5c5,  r5c6, r5c7, r5c8, r5c9, r5c10, RESTRICT_ARGS_COUNT((r5c11), 63, KEYMAP, ##__VA_ARGS__)

#endif /* ARDUINO_ARCH_NRF52 */

