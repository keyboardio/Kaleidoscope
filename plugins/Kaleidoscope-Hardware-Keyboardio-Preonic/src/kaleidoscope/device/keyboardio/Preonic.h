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
  
  // SYSOFF is connected to a physical switch, not controlled by MCU
  static constexpr uint8_t sysoff_pin = 0;  // Not MCU controlled
  
  // Configuration details (for reference):
  // - ISET resistor: 1.6kΩ (sets charge current to 556mA) 
  // - ILIM resistor: 1.6kΩ (sets input current limit to 968mA)
  // - CE: Connected to ground (charge enabled)
  // - TMR: Floating (safety timer enabled, 5hr)
  // - EN1: Connected to VCC unregulated (High)
  // - EN2: Connected to GND (Low)
  // - EN1 High + EN2 Low = USB500 Mode (500mA max)
  // - OUT: Connected to VCC unregulated
  // - BAT: Connected to battery with 10μF capacitor to GND
  // - TS: Connected to battery NTC
  
  // If ISET monitoring is needed, uncomment and define:
  // static constexpr uint8_t iset_pin = PIN_ISET_MONITOR;
  static constexpr float iset_resistance = 1600.0f; // 1.6kΩ ISET resistor
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
  }

  // Configure all column pins to stop sensing
  static void disableColumnSensing() {
    for (uint8_t i = 0; i < KeyScannerProps::matrix_columns; i++) {
      disablePinSensing(KeyScannerProps::matrix_col_pins[i]);
    }
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
    tone(PIN_SPEAKER, 1500, 100);
    delay(100);
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

    // Dump the entire system state to help identify power leaks
   // Serial.println(F("============= SYSTEM STATE BEFORE SLEEP ============="));
   // Serial.println(F("============= END OF SYSTEM STATE BEFORE SLEEP ============="));
   // Serial.flush();

    // disableTWIForSleep();
     disableRTC();
    // disableTimers();
    
    // Disable FPU state preservation to prevent ~3mA power drain in sleep
    disableFPUForSleep();
    
    sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
   // dumpSystemState();

    while (!input_event_pending_) {
      waitForEvent();
    }
      speaker().playTone(1500, 100);
    // Resume all freertos tasks

    // Wake up sequence:
    // 1. Switch back to constant latency mode
    sd_power_mode_set(NRF_POWER_MODE_CONSTLAT);

    // restoreTWIAfterSleep();
    // restoreTimers();
    restoreRTC();
    restoreMatrixAfterSleep();
    disableColumnSensing();
    // Start processing BLE HID reports
    kaleidoscope::driver::hid::bluefruit::blehid.startReportProcessing();

    // Resume the keyscanner timer immediately
    keyScanner().resumeTimer();

    enableLEDPower();

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
      // return false;
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
    uint8_t new_level = 100; 
   new_level= batteryGauge().getBatteryLevel();
    if (new_level != ble().getBatteryLevel()) {
    //  ble().setBatteryLevel(new_level);
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
          triggerBatteryShutdown();
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
        warning_active_ = true;
        last_warning_time_ = now;
        triggerBatteryWarning(true);
      } else if (warning_active_ && (now - last_warning_time_ >= WARNING_DURATION_MS)) {
        // End the current warning period
        warning_active_ = false;
        triggerBatteryWarning(false);
      }
    }
  }

  /**
   * @brief Dumps the state of all peripherals, pins, and registers over serial to debug power consumption issues
   * 
   * This function outputs a comprehensive report of the current system state including:
   * - GPIO pin configurations and states
   * - All peripheral enable states
   * - Clock configurations
   * - Power management settings
   * - Interrupt states
   * - Timer and RTC configurations
   * - I2C/TWI settings
   * - FPU state information
   * 
   * The output is intended to be compared between different system configurations 
   * to identify power leaks.
   */
  void dumpSystemState() {
    // ===== MCU CORE CONFIGURATION =====
    

    // Begin the dump
    sendKeyStrokes(PSTR("\n\n========= SYSTEM STATE DUMP =========\n"));
    sendKeyStrokes(PSTR("This will help identify power leaks\n"));
    
    // GPIO Pin Configuration
    sendKeyStrokes(PSTR("\n----- GPIO PIN STATES -----\n"));
    sendKeyStrokes(PSTR("Pin\tDir\tInput\tPull\tDrive\tSense\tValue\n"));
    
    // P0 port
    for (int i = 0; i < 32; i++) {
      uint32_t config = NRF_P0->PIN_CNF[i];
      bool dir = (config & GPIO_PIN_CNF_DIR_Msk) >> GPIO_PIN_CNF_DIR_Pos;
      bool input = (config & GPIO_PIN_CNF_INPUT_Msk) >> GPIO_PIN_CNF_INPUT_Pos;
      uint8_t pull = (config & GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos;
      uint8_t drive = (config & GPIO_PIN_CNF_DRIVE_Msk) >> GPIO_PIN_CNF_DRIVE_Pos;
      uint8_t sense = (config & GPIO_PIN_CNF_SENSE_Msk) >> GPIO_PIN_CNF_SENSE_Pos;
      bool value = NRF_P0->IN & (1 << i);
      
      sendKeyStrokes(PSTR("P0."));
      if (i < 10) sendKeyStrokes(PSTR("0"));
      
      char num_str[4];
      snprintf(num_str, sizeof(num_str), "%d", i);
      sendKeyStrokes(num_str);
      sendKeyStrokes(PSTR(":\t"));
      sendKeyStrokes(dir ? PSTR("Out\t") : PSTR("In\t"));
      sendKeyStrokes(input ? PSTR("Connect\t") : PSTR("Disconnect\t"));
      
      // Pull configuration
      switch (pull) {
        case 0: sendKeyStrokes(PSTR("None\t")); break;
        case 1: sendKeyStrokes(PSTR("Down\t")); break;
        case 3: sendKeyStrokes(PSTR("Up\t")); break;
        default: sendKeyStrokes(PSTR("???\t")); break;
      }
      
      // Drive configuration
      switch (drive) {
        case 0: sendKeyStrokes(PSTR("S0S1\t")); break;
        case 1: sendKeyStrokes(PSTR("H0S1\t")); break;
        case 2: sendKeyStrokes(PSTR("S0H1\t")); break;
        case 3: sendKeyStrokes(PSTR("H0H1\t")); break;
        case 4: sendKeyStrokes(PSTR("D0S1\t")); break;
        case 5: sendKeyStrokes(PSTR("D0H1\t")); break;
        case 6: sendKeyStrokes(PSTR("S0D1\t")); break;
        case 7: sendKeyStrokes(PSTR("H0D1\t")); break;
        default: sendKeyStrokes(PSTR("???\t")); break;
      }
      
      // Sense configuration
      switch (sense) {
        case 0: sendKeyStrokes(PSTR("Disabled\t")); break;
        case 2: sendKeyStrokes(PSTR("High\t")); break;
        case 3: sendKeyStrokes(PSTR("Low\t")); break;
        default: sendKeyStrokes(PSTR("???\t")); break;
      }
      
      sendKeyStrokes(value ? PSTR("1\n") : PSTR("0\n"));
    }
    
    // P1 port (for nRF52840)
    #if defined(NRF52840_XXAA)
    for (int i = 0; i < 16; i++) {
      uint32_t config = NRF_P1->PIN_CNF[i];
      bool dir = (config & GPIO_PIN_CNF_DIR_Msk) >> GPIO_PIN_CNF_DIR_Pos;
      bool input = (config & GPIO_PIN_CNF_INPUT_Msk) >> GPIO_PIN_CNF_INPUT_Pos;
      uint8_t pull = (config & GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos;
      uint8_t drive = (config & GPIO_PIN_CNF_DRIVE_Msk) >> GPIO_PIN_CNF_DRIVE_Pos;
      uint8_t sense = (config & GPIO_PIN_CNF_SENSE_Msk) >> GPIO_PIN_CNF_SENSE_Pos;
      bool value = NRF_P1->IN & (1 << i);
      
      sendKeyStrokes(PSTR("P1."));
      if (i < 10) sendKeyStrokes(PSTR("0"));
      
      char num_str[4];
      snprintf(num_str, sizeof(num_str), "%d", i);
      sendKeyStrokes(num_str);
      sendKeyStrokes(PSTR(":\t"));
      sendKeyStrokes(dir ? PSTR("Out\t") : PSTR("In\t"));
      sendKeyStrokes(input ? PSTR("Connect\t") : PSTR("Disconnect\t"));
      
      // Pull configuration
      switch (pull) {
        case 0: sendKeyStrokes(PSTR("None\t")); break;
        case 1: sendKeyStrokes(PSTR("Down\t")); break;
        case 3: sendKeyStrokes(PSTR("Up\t")); break;
        default: sendKeyStrokes(PSTR("???\t")); break;
      }
      
      // Drive configuration
      switch (drive) {
        case 0: sendKeyStrokes(PSTR("S0S1\t")); break;
        case 1: sendKeyStrokes(PSTR("H0S1\t")); break;
        case 2: sendKeyStrokes(PSTR("S0H1\t")); break;
        case 3: sendKeyStrokes(PSTR("H0H1\t")); break;
        case 4: sendKeyStrokes(PSTR("D0S1\t")); break;
        case 5: sendKeyStrokes(PSTR("D0H1\t")); break;
        case 6: sendKeyStrokes(PSTR("S0D1\t")); break;
        case 7: sendKeyStrokes(PSTR("H0D1\t")); break;
        default: sendKeyStrokes(PSTR("???\t")); break;
      }
      
      // Sense configuration
      switch (sense) {
        case 0: sendKeyStrokes(PSTR("Disabled\t")); break;
        case 2: sendKeyStrokes(PSTR("High\t")); break;
        case 3: sendKeyStrokes(PSTR("Low\t")); break;
        default: sendKeyStrokes(PSTR("???\t")); break;
      }
      
      sendKeyStrokes(value ? PSTR("1\n") : PSTR("0\n"));
    }
    #endif
    
    sendKeyStrokes(PSTR("\n----- PERIPHERAL CONFIGURATION -----\n"));
    
    // NVIC Configuration
    sendKeyStrokes(PSTR("NVIC Configuration:\n"));
    sendKeyStrokes(PSTR("  Active IRQs: "));
    
    // Count active IRQs
    uint32_t active_irq_count = 0;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 32; j++) {
        if (NVIC->ISER[i] & (1 << j)) {
          active_irq_count++;
        }
      }
    }
    
    char active_irqs_str[11];
    snprintf(active_irqs_str, sizeof(active_irqs_str), "%lu", active_irq_count);
    sendKeyStrokes(active_irqs_str);
    sendKeyStrokes(PSTR("\n"));
    

    
    // Radio Configuration
    sendKeyStrokes(PSTR("\n----- RADIO CONFIGURATION -----\n"));
    sendKeyStrokes(PSTR("RADIO Enabled: "));
    sendKeyStrokes((NRF_RADIO->POWER & RADIO_POWER_POWER_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    
    sendKeyStrokes(PSTR("RADIO Mode: 0x"));
    char radio_mode_str[9];
    snprintf(radio_mode_str, sizeof(radio_mode_str), "%02lX", NRF_RADIO->MODE);
    sendKeyStrokes(radio_mode_str);
    sendKeyStrokes(PSTR("\n"));
    
    sendKeyStrokes(PSTR("RADIO Frequency: "));
    char radio_freq_str[11];
    snprintf(radio_freq_str, sizeof(radio_freq_str), "%lu", NRF_RADIO->FREQUENCY);
    sendKeyStrokes(radio_freq_str);
    sendKeyStrokes(PSTR("\n"));
    
    sendKeyStrokes(PSTR("RADIO TX Power: 0x"));
    char radio_tx_str[9];
    snprintf(radio_tx_str, sizeof(radio_tx_str), "%02lX", NRF_RADIO->TXPOWER);
    sendKeyStrokes(radio_tx_str);
    sendKeyStrokes(PSTR("\n"));
    
    // Watchdog Configuration
    sendKeyStrokes(PSTR("\n----- WATCHDOG CONFIGURATION -----\n"));
    sendKeyStrokes(PSTR("Watchdog Running: "));
    sendKeyStrokes((NRF_WDT->RUNSTATUS & WDT_RUNSTATUS_RUNSTATUS_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    
    sendKeyStrokes(PSTR("Watchdog Reload Value: "));
    char wdt_crv_str[11];
    snprintf(wdt_crv_str, sizeof(wdt_crv_str), "%lu", NRF_WDT->CRV);
    sendKeyStrokes(wdt_crv_str);
    sendKeyStrokes(PSTR("\n"));
    
    sendKeyStrokes(PSTR("Watchdog Reload Requests: 0x"));
    char wdt_rren_str[9];
    snprintf(wdt_rren_str, sizeof(wdt_rren_str), "%08lX", NRF_WDT->RREN);
    sendKeyStrokes(wdt_rren_str);
    sendKeyStrokes(PSTR("\n"));
    
    // EasyDMA Status
    sendKeyStrokes(PSTR("\n----- EasyDMA STATUS -----\n"));
    
    // Check SPIM0
    sendKeyStrokes(PSTR("SPIM0 EasyDMA:\n"));
    sendKeyStrokes(PSTR("  Enabled: "));
    sendKeyStrokes((NRF_SPIM0->ENABLE & SPIM_ENABLE_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    if (NRF_SPIM0->ENABLE) {
      sendKeyStrokes(PSTR("  TX Buffer: 0x"));
      char spim0_tx_ptr_str[9];
      snprintf(spim0_tx_ptr_str, sizeof(spim0_tx_ptr_str), "%08lX", NRF_SPIM0->TXD.PTR);
      sendKeyStrokes(spim0_tx_ptr_str);
      sendKeyStrokes(PSTR("\n"));
      sendKeyStrokes(PSTR("  TX Count: "));
      char spim0_tx_maxcnt_str[5];
      snprintf(spim0_tx_maxcnt_str, sizeof(spim0_tx_maxcnt_str), "%lu", NRF_SPIM0->TXD.MAXCNT);
      sendKeyStrokes(spim0_tx_maxcnt_str);
      sendKeyStrokes(PSTR("\n"));
    }
    
    // Check SPIM1
    sendKeyStrokes(PSTR("SPIM1 EasyDMA:\n"));
    sendKeyStrokes(PSTR("  Enabled: "));
    sendKeyStrokes((NRF_SPIM1->ENABLE & SPIM_ENABLE_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    
    // QSPI Configuration
    sendKeyStrokes(PSTR("\n----- QSPI CONFIGURATION -----\n"));
    sendKeyStrokes(PSTR("QSPI Enabled: "));
    sendKeyStrokes((NRF_QSPI->ENABLE & QSPI_ENABLE_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    if (NRF_QSPI->ENABLE) {
      sendKeyStrokes(PSTR("  QSPI IFCONFIG0: 0x"));
      char qspi_ifconfig0_str[11];
      snprintf(qspi_ifconfig0_str, sizeof(qspi_ifconfig0_str), "%lX", NRF_QSPI->IFCONFIG0);
      sendKeyStrokes(qspi_ifconfig0_str);
      sendKeyStrokes(PSTR("\n"));
      sendKeyStrokes(PSTR("  QSPI IFCONFIG1: 0x"));
      char qspi_ifconfig1_str[11];
      snprintf(qspi_ifconfig1_str, sizeof(qspi_ifconfig1_str), "%lX", NRF_QSPI->IFCONFIG1);
      sendKeyStrokes(qspi_ifconfig1_str);
      sendKeyStrokes(PSTR("\n"));
    }
    
    // USB Configuration
    sendKeyStrokes(PSTR("\n----- USB CONFIGURATION -----\n"));
    sendKeyStrokes(PSTR("USB Enabled: "));
    sendKeyStrokes((NRF_USBD->ENABLE & USBD_ENABLE_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    
    sendKeyStrokes(PSTR("USB Pullup: "));
    sendKeyStrokes((NRF_USBD->USBPULLUP & USBD_USBPULLUP_CONNECT_Msk) ? PSTR("Connected\n") : PSTR("Disconnected\n"));
    
    // Power System
    sendKeyStrokes(PSTR("\n----- ADDITIONAL POWER SYSTEM INFO -----\n"));
    
    sendKeyStrokes(PSTR("System OFF Register: 0x"));
    char systemoff_str[11];
    snprintf(systemoff_str, sizeof(systemoff_str), "%lX", NRF_POWER->SYSTEMOFF);
    sendKeyStrokes(systemoff_str);
    sendKeyStrokes(PSTR("\n"));
    
    sendKeyStrokes(PSTR("RAM Power Config: "));
    for (int i = 0; i < 8; i++) {
      sendKeyStrokes(PSTR("Section "));
      char section_str[4];
      snprintf(section_str, sizeof(section_str), "%d", i);
      sendKeyStrokes(section_str);
      sendKeyStrokes(PSTR(": 0x"));
      char ram_power_str[9];
      snprintf(ram_power_str, sizeof(ram_power_str), "%lX", NRF_POWER->RAM[i].POWER);
      sendKeyStrokes(ram_power_str);
      sendKeyStrokes(PSTR(" "));
    }
    sendKeyStrokes(PSTR("\n"));
    
    sendKeyStrokes(PSTR("POFCON (Power failure comparator): 0x"));
    char pofcon_str[11];
    snprintf(pofcon_str, sizeof(pofcon_str), "%lX", NRF_POWER->POFCON);
    sendKeyStrokes(pofcon_str);
    sendKeyStrokes(PSTR("\n"));
    
    sendKeyStrokes(PSTR("RESETREAS (Reset reason): 0x"));
    char resetreas_str[11];
    snprintf(resetreas_str, sizeof(resetreas_str), "%lX", NRF_POWER->RESETREAS);
    sendKeyStrokes(resetreas_str);
    sendKeyStrokes(PSTR("\n"));
    
    // Additional peripheral status checks
    sendKeyStrokes(PSTR("\n----- ADDITIONAL PERIPHERAL STATUS -----\n"));
    
    // Check ACL (Access Control Lists)
    sendKeyStrokes(PSTR("ACL Count: "));
    #ifdef ACL_CONFIG_SIZE_Msk
    char acl_count_str[11];
    snprintf(acl_count_str, sizeof(acl_count_str), "%ld", (NRF_ACL->CONFIG & ACL_CONFIG_SIZE_Msk) >> ACL_CONFIG_SIZE_Pos);
    sendKeyStrokes(acl_count_str);
    sendKeyStrokes(PSTR("\n"));
    #else
    sendKeyStrokes(PSTR("ACL size info not available\n"));
    #endif
  
    
    // Check CCM (AES CCM mode encryption)
    sendKeyStrokes(PSTR("CCM Running: "));
    sendKeyStrokes((NRF_CCM->ENABLE & CCM_ENABLE_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    
    // Check AAR (Accelerated Address Resolver)
    sendKeyStrokes(PSTR("AAR Enabled: "));
    sendKeyStrokes((NRF_AAR->ENABLE & AAR_ENABLE_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    
    // Check ECB (AES Electronic Codebook mode encryption)
    sendKeyStrokes(PSTR("ECB Running: "));
    sendKeyStrokes(NRF_ECB->TASKS_STARTECB ? PSTR("Yes\n") : PSTR("No\n"));
    
    // TEMP (Temperature sensor)
    sendKeyStrokes(PSTR("TEMP Enabled: "));
    sendKeyStrokes((NRF_TEMP->TASKS_START) ? PSTR("Yes\n") : PSTR("No\n"));
    
    // RNG (Random Number Generator)
    sendKeyStrokes(PSTR("RNG Enabled: "));
    sendKeyStrokes((NRF_RNG->TASKS_START) ? PSTR("Yes\n") : PSTR("No\n"));
    
    // ===== PERIPHERAL STATES =====
    sendKeyStrokes(PSTR("\n----- PERIPHERAL STATES -----\n"));
    
    // TWIM0 (I2C Master)
    sendKeyStrokes(PSTR("TWIM0 (I2C Master):\n"));
    sendKeyStrokes(PSTR("  Enabled: "));
    sendKeyStrokes((NRF_TWIM0->ENABLE & TWIM_ENABLE_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    sendKeyStrokes(PSTR("  SCL Pin: 0x"));
    char twim0_scl_str[11];
    snprintf(twim0_scl_str, sizeof(twim0_scl_str), "%lX", NRF_TWIM0->PSEL.SCL);
    sendKeyStrokes(twim0_scl_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  SDA Pin: 0x"));
    char twim0_sda_str[11];
    snprintf(twim0_sda_str, sizeof(twim0_sda_str), "%lX", NRF_TWIM0->PSEL.SDA);
    sendKeyStrokes(twim0_sda_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Frequency: 0x"));
    char twim0_freq_str[11];
    snprintf(twim0_freq_str, sizeof(twim0_freq_str), "%lX", NRF_TWIM0->FREQUENCY);
    sendKeyStrokes(twim0_freq_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Interrupts Enabled: 0x"));
    char twim0_int_str[11];
    snprintf(twim0_int_str, sizeof(twim0_int_str), "%lX", NRF_TWIM0->INTENSET);
    sendKeyStrokes(twim0_int_str);
    sendKeyStrokes(PSTR("\n"));
    
    // TWIS0 (I2C Slave)
    sendKeyStrokes(PSTR("TWIS0 (I2C Slave):\n"));
    sendKeyStrokes(PSTR("  Enabled: "));
    sendKeyStrokes((NRF_TWIS0->ENABLE & TWIS_ENABLE_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    sendKeyStrokes(PSTR("  SCL Pin: 0x"));
    char twis0_scl_str[11];
    snprintf(twis0_scl_str, sizeof(twis0_scl_str), "%lX", NRF_TWIS0->PSEL.SCL);
    sendKeyStrokes(twis0_scl_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  SDA Pin: 0x"));
    char twis0_sda_str[11];
    snprintf(twis0_sda_str, sizeof(twis0_sda_str), "%lX", NRF_TWIS0->PSEL.SDA);
    sendKeyStrokes(twis0_sda_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Interrupts Enabled: 0x"));
    char twis0_int_str[11];
    snprintf(twis0_int_str, sizeof(twis0_int_str), "%lX", NRF_TWIS0->INTENSET);
    sendKeyStrokes(twis0_int_str);
    sendKeyStrokes(PSTR("\n"));
    
    // Detailed I2C pin configuration
    sendKeyStrokes(PSTR("I2C Pin Configuration:\n"));
    uint32_t scl_pin = (NRF_TWIM0->PSEL.SCL & 0x3F);  // Pin number is in bits 0-5
    uint32_t sda_pin = (NRF_TWIM0->PSEL.SDA & 0x3F);  // Pin number is in bits 0-5
    
    if (scl_pin < 32) {
      sendKeyStrokes(PSTR("  SCL Pin Config (P0."));
      char scl_pin_str[3];
      snprintf(scl_pin_str, sizeof(scl_pin_str), "%ld", scl_pin);
      sendKeyStrokes(scl_pin_str);
      sendKeyStrokes(PSTR("): 0x"));
      char scl_config_str[11];
      snprintf(scl_config_str, sizeof(scl_config_str), "%lX", NRF_P0->PIN_CNF[scl_pin]);
      sendKeyStrokes(scl_config_str);
      sendKeyStrokes(PSTR("\n"));
      
      sendKeyStrokes(PSTR("    Direction: "));
      sendKeyStrokes((NRF_P0->PIN_CNF[scl_pin] & GPIO_PIN_CNF_DIR_Msk) ? PSTR("Output\n") : PSTR("Input\n"));
      
      sendKeyStrokes(PSTR("    Input Buffer: "));
      sendKeyStrokes((NRF_P0->PIN_CNF[scl_pin] & GPIO_PIN_CNF_INPUT_Msk) ? PSTR("Connected\n") : PSTR("Disconnected\n"));
      
      sendKeyStrokes(PSTR("    Pull Configuration: "));
      uint32_t pull = (NRF_P0->PIN_CNF[scl_pin] & GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos;
      switch (pull) {
        case 0: sendKeyStrokes(PSTR("Disabled\n")); break;
        case 1: sendKeyStrokes(PSTR("Pulldown\n")); break;
        case 3: sendKeyStrokes(PSTR("Pullup\n")); break;
        default: sendKeyStrokes(PSTR("Unknown\n")); break;
      }
    }
    
    if (sda_pin < 32) {
      sendKeyStrokes(PSTR("  SDA Pin Config (P0."));
      char sda_pin_str[3];
      snprintf(sda_pin_str, sizeof(sda_pin_str), "%ld", sda_pin);
      sendKeyStrokes(sda_pin_str);
      sendKeyStrokes(PSTR("): 0x"));
      char sda_config_str[11];
      snprintf(sda_config_str, sizeof(sda_config_str), "%lX", NRF_P0->PIN_CNF[sda_pin]);
      sendKeyStrokes(sda_config_str);
      sendKeyStrokes(PSTR("\n"));
      
      sendKeyStrokes(PSTR("    Direction: "));
      sendKeyStrokes((NRF_P0->PIN_CNF[sda_pin] & GPIO_PIN_CNF_DIR_Msk) ? PSTR("Output\n") : PSTR("Input\n"));
      
      sendKeyStrokes(PSTR("    Input Buffer: "));
      sendKeyStrokes((NRF_P0->PIN_CNF[sda_pin] & GPIO_PIN_CNF_INPUT_Msk) ? PSTR("Connected\n") : PSTR("Disconnected\n"));
      
      sendKeyStrokes(PSTR("    Pull Configuration: "));
      uint32_t pull = (NRF_P0->PIN_CNF[sda_pin] & GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos;
      switch (pull) {
        case 0: sendKeyStrokes(PSTR("Disabled\n")); break;
        case 1: sendKeyStrokes(PSTR("Pulldown\n")); break;
        case 3: sendKeyStrokes(PSTR("Pullup\n")); break;
        default: sendKeyStrokes(PSTR("Unknown\n")); break;
      }
    }
    
    // UARTE0 (Serial)
    sendKeyStrokes(PSTR("UARTE0 (Serial):\n"));
    sendKeyStrokes(PSTR("  Enabled: "));
    sendKeyStrokes((NRF_UARTE0->ENABLE & UARTE_ENABLE_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    sendKeyStrokes(PSTR("  TX Pin: 0x"));
    char uarte0_tx_str[11];
    snprintf(uarte0_tx_str, sizeof(uarte0_tx_str), "%lX", NRF_UARTE0->PSEL.TXD);
    sendKeyStrokes(uarte0_tx_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  RX Pin: 0x"));
    char uarte0_rx_str[11];
    snprintf(uarte0_rx_str, sizeof(uarte0_rx_str), "%lX", NRF_UARTE0->PSEL.RXD);
    sendKeyStrokes(uarte0_rx_str);
    sendKeyStrokes(PSTR("\n"));
    
    // SAADC (Analog-to-Digital Converter)
    sendKeyStrokes(PSTR("SAADC (ADC):\n"));
    sendKeyStrokes(PSTR("  Enabled: "));
    sendKeyStrokes((NRF_SAADC->ENABLE & SAADC_ENABLE_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    if (NRF_SAADC->ENABLE) {
      sendKeyStrokes(PSTR("  Channel Configuration:\n"));
      for (int i = 0; i < 8; i++) {
        sendKeyStrokes(PSTR("    Channel "));
        char ch_num_str[3];
        snprintf(ch_num_str, sizeof(ch_num_str), "%d", i);
        sendKeyStrokes(ch_num_str);
        sendKeyStrokes(PSTR(" Config: 0x"));
        char ch_config_str[11];
        snprintf(ch_config_str, sizeof(ch_config_str), "%lX", NRF_SAADC->CH[i].CONFIG);
        sendKeyStrokes(ch_config_str);
        sendKeyStrokes(PSTR("\n"));
      }
    }
    
    // TIMERS
    sendKeyStrokes(PSTR("TIMER0:\n"));
    sendKeyStrokes(PSTR("  Mode: "));
    char timer0_mode_str[11];
    snprintf(timer0_mode_str, sizeof(timer0_mode_str), "%lX", NRF_TIMER0->MODE);
    sendKeyStrokes(timer0_mode_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Enabled Interrupts: 0x"));
    char timer0_int_str[11];
    snprintf(timer0_int_str, sizeof(timer0_int_str), "%lX", NRF_TIMER0->INTENSET);
    sendKeyStrokes(timer0_int_str);
    sendKeyStrokes(PSTR("\n"));
    
    sendKeyStrokes(PSTR("TIMER1:\n"));
    sendKeyStrokes(PSTR("  Mode: "));
    char timer1_mode_str[11];
    snprintf(timer1_mode_str, sizeof(timer1_mode_str), "%lX", NRF_TIMER1->MODE);
    sendKeyStrokes(timer1_mode_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Enabled Interrupts: 0x"));
    char timer1_int_str[11];
    snprintf(timer1_int_str, sizeof(timer1_int_str), "%lX", NRF_TIMER1->INTENSET);
    sendKeyStrokes(timer1_int_str);
    sendKeyStrokes(PSTR("\n"));
    
    sendKeyStrokes(PSTR("TIMER2:\n"));
    sendKeyStrokes(PSTR("  Mode: "));
    char timer2_mode_str[11];
    snprintf(timer2_mode_str, sizeof(timer2_mode_str), "%lX", NRF_TIMER2->MODE);
    sendKeyStrokes(timer2_mode_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Enabled Interrupts: 0x"));
    char timer2_int_str[11];
    snprintf(timer2_int_str, sizeof(timer2_int_str), "%lX", NRF_TIMER2->INTENSET);
    sendKeyStrokes(timer2_int_str);
    sendKeyStrokes(PSTR("\n"));
    
    // RTC
    sendKeyStrokes(PSTR("RTC0:\n"));
    sendKeyStrokes(PSTR("  Enabled Interrupts: 0x"));
    char rtc0_int_str[11];
    snprintf(rtc0_int_str, sizeof(rtc0_int_str), "%lX", NRF_RTC0->INTENSET);
    sendKeyStrokes(rtc0_int_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Enabled Events: 0x"));
    char rtc0_evt_str[11];
    snprintf(rtc0_evt_str, sizeof(rtc0_evt_str), "%lX", NRF_RTC0->EVTEN);
    sendKeyStrokes(rtc0_evt_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Counter: "));
    char rtc0_cnt_str[11];
    snprintf(rtc0_cnt_str, sizeof(rtc0_cnt_str), "%ld", NRF_RTC0->COUNTER);
    sendKeyStrokes(rtc0_cnt_str);
    sendKeyStrokes(PSTR("\n"));
    
    sendKeyStrokes(PSTR("RTC1:\n"));
    sendKeyStrokes(PSTR("  Enabled Interrupts: 0x"));
    char rtc1_int_str[11];
    snprintf(rtc1_int_str, sizeof(rtc1_int_str), "%lX", NRF_RTC1->INTENSET);
    sendKeyStrokes(rtc1_int_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Enabled Events: 0x"));
    char rtc1_evt_str[11];
    snprintf(rtc1_evt_str, sizeof(rtc1_evt_str), "%lX", NRF_RTC1->EVTEN);
    sendKeyStrokes(rtc1_evt_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Counter: "));
    char rtc1_cnt_str[11];
    snprintf(rtc1_cnt_str, sizeof(rtc1_cnt_str), "%ld", NRF_RTC1->COUNTER);
    sendKeyStrokes(rtc1_cnt_str);
    sendKeyStrokes(PSTR("\n"));
    
    // GPIOTE
    sendKeyStrokes(PSTR("GPIOTE:\n"));
    sendKeyStrokes(PSTR("  Enabled Interrupts: 0x"));
    char gpiote_int_str[11];
    snprintf(gpiote_int_str, sizeof(gpiote_int_str), "%lX", NRF_GPIOTE->INTENSET);
    sendKeyStrokes(gpiote_int_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Channel Configuration:\n"));
    for (int i = 0; i < 8; i++) {
      sendKeyStrokes(PSTR("    Channel "));
      char gpiote_ch_str[3];
      snprintf(gpiote_ch_str, sizeof(gpiote_ch_str), "%d", i);
      sendKeyStrokes(gpiote_ch_str);
      sendKeyStrokes(PSTR(": 0x"));
      char gpiote_cfg_str[11];
      snprintf(gpiote_cfg_str, sizeof(gpiote_cfg_str), "%lX", NRF_GPIOTE->CONFIG[i]);
      sendKeyStrokes(gpiote_cfg_str);
      sendKeyStrokes(PSTR("\n"));
    }
    
    // PPI (Programmable Peripheral Interconnect)
    sendKeyStrokes(PSTR("PPI:\n"));
    sendKeyStrokes(PSTR("  Enabled Channels: 0x"));
    char ppi_chen_str[11];
    snprintf(ppi_chen_str, sizeof(ppi_chen_str), "%lX", NRF_PPI->CHEN);
    sendKeyStrokes(ppi_chen_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Channel Configuration:\n"));
    for (int i = 0; i < 16; i++) {
      if (NRF_PPI->CHEN & (1 << i)) {
        sendKeyStrokes(PSTR("    Channel "));
        char ppi_ch_str[3];
        snprintf(ppi_ch_str, sizeof(ppi_ch_str), "%d", i);
        sendKeyStrokes(ppi_ch_str);
        sendKeyStrokes(PSTR(" EEP: 0x"));
        char ppi_eep_str[11];
        snprintf(ppi_eep_str, sizeof(ppi_eep_str), "%lX", NRF_PPI->CH[i].EEP);
        sendKeyStrokes(ppi_eep_str);
        sendKeyStrokes(PSTR(", TEP: 0x"));
        char ppi_tep_str[11];
        snprintf(ppi_tep_str, sizeof(ppi_tep_str), "%lX", NRF_PPI->CH[i].TEP);
        sendKeyStrokes(ppi_tep_str);
        sendKeyStrokes(PSTR("\n"));
      }
    }
    
    
    // ===== EXTENDED CLOCK INFORMATION =====
    sendKeyStrokes(PSTR("\n----- EXTENDED CLOCK INFORMATION -----\n"));
    
    sendKeyStrokes(PSTR("\n----- CLOCK CONFIGURATION -----\n"));
    
   
    // Low-frequency clock
    sendKeyStrokes(PSTR("LFCLK Source: "));
    uint32_t lfclk_stat_src = (NRF_CLOCK->LFCLKSTAT & CLOCK_LFCLKSTAT_SRC_Msk) >> CLOCK_LFCLKSTAT_SRC_Pos;
    switch (lfclk_stat_src) {
      case CLOCK_LFCLKSTAT_SRC_RC:
        sendKeyStrokes(PSTR("RC Oscillator (higher power, less accurate)\n"));
        break;
      case CLOCK_LFCLKSTAT_SRC_Xtal:
        sendKeyStrokes(PSTR("Crystal Oscillator (more accurate, uses slightly more power)\n"));
        break;
      case CLOCK_LFCLKSTAT_SRC_Synth:
        sendKeyStrokes(PSTR("Synthesized from HFCLK (high power!)\n"));
        break;
      default:
        sendKeyStrokes(PSTR("Unknown\n"));
        break;
    }
    sendKeyStrokes(PSTR("LFCLK Running: "));
    sendKeyStrokes((NRF_CLOCK->LFCLKSTAT & CLOCK_LFCLKSTAT_STATE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    

    

    // Clock source details
    sendKeyStrokes(PSTR("LFCLKSRC: 0x"));
    char lfclksrc_str[11];
    snprintf(lfclksrc_str, sizeof(lfclksrc_str), "%lX", NRF_CLOCK->LFCLKSRC);
    sendKeyStrokes(lfclksrc_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("  Source: "));
    uint32_t lfclk_stat_src_value = (NRF_CLOCK->LFCLKSTAT & CLOCK_LFCLKSTAT_SRC_Msk) >> CLOCK_LFCLKSTAT_SRC_Pos;
    uint32_t lfclk_src = (NRF_CLOCK->LFCLKSRC & CLOCK_LFCLKSRC_SRC_Msk) >> CLOCK_LFCLKSRC_SRC_Pos;
    switch (lfclk_src) {
      case 0: sendKeyStrokes(PSTR("RC Oscillator\n")); break;
      case 1: sendKeyStrokes(PSTR("XTAL Oscillator\n")); break;
      case 2: sendKeyStrokes(PSTR("Synthesized from HFCLK\n")); break;
      default: sendKeyStrokes(PSTR("Unknown\n")); break;
    }
     // High-frequency clock
    sendKeyStrokes(PSTR("HFCLK Source: "));
    switch (NRF_CLOCK->HFCLKSTAT & CLOCK_HFCLKSTAT_SRC_Msk) {
      case CLOCK_HFCLKSTAT_SRC_RC:
        sendKeyStrokes(PSTR("RC Oscillator (good for power saving)\n"));
        break;
      case CLOCK_HFCLKSTAT_SRC_Xtal:
        sendKeyStrokes(PSTR("Crystal Oscillator (accurate but uses more power)\n"));
        break;
      default:
        sendKeyStrokes(PSTR("Unknown\n"));
        break;
    }
    sendKeyStrokes(PSTR("HFCLK Running: "));
    sendKeyStrokes((NRF_CLOCK->HFCLKSTAT & CLOCK_HFCLKSTAT_STATE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    

    sendKeyStrokes(PSTR("HFXODEBOUNCE: 0x"));
    char hfxodebounce_str[11];
    snprintf(hfxodebounce_str, sizeof(hfxodebounce_str), "%lX", NRF_CLOCK->HFXODEBOUNCE);
    sendKeyStrokes(hfxodebounce_str);
    sendKeyStrokes(PSTR("\n"));
    
    // Clock calibration status
    sendKeyStrokes(PSTR("CTIV (Calibration timer interval): "));
    char ctiv_str[11];
    snprintf(ctiv_str, sizeof(ctiv_str), "%ld", (NRF_CLOCK->CTIV & CLOCK_CTIV_CTIV_Msk) >> CLOCK_CTIV_CTIV_Pos);
    sendKeyStrokes(ctiv_str);
    sendKeyStrokes(PSTR("\n"));
    
    sendKeyStrokes(PSTR("Clock Events: 0x"));
    char clock_evt_str[11];
    snprintf(clock_evt_str, sizeof(clock_evt_str), "%lX", NRF_CLOCK->EVENTS_DONE);
    sendKeyStrokes(clock_evt_str);
    sendKeyStrokes(PSTR("\n"));
    
    sendKeyStrokes(PSTR("Clock Interrupts Enabled: 0x"));
    char clock_int_str[11];
    snprintf(clock_int_str, sizeof(clock_int_str), "%lX", NRF_CLOCK->INTENSET);
    sendKeyStrokes(clock_int_str);
    sendKeyStrokes(PSTR("\n"));
    
    // ===== EXTENDED FREERTOS INFORMATION =====
    sendKeyStrokes(PSTR("\n----- EXTENDED FREERTOS INFORMATION -----\n"));
    
    // Get detailed task information using FreeRTOS API
    sendKeyStrokes(PSTR("Scheduler State: "));
    UBaseType_t schedulerState = xTaskGetSchedulerState();
    switch (schedulerState) {
      case 0: sendKeyStrokes(PSTR("Not Started\n")); break;
      case 1: sendKeyStrokes(PSTR("Suspended\n")); break;
      case 2: sendKeyStrokes(PSTR("Running\n")); break;
      default: sendKeyStrokes(PSTR("Unknown\n")); break;
    }
    
    // Get detailed task list
    const UBaseType_t MAX_TASKS = 10;
    TaskStatus_t taskStatusArray[MAX_TASKS];
    uint32_t totalRunTime = 0;
    
    UBaseType_t actualTaskCount = uxTaskGetSystemState(taskStatusArray, MAX_TASKS, &totalRunTime);
    
    sendKeyStrokes(PSTR("Total tasks: "));
    char task_count_str[4];
    snprintf(task_count_str, sizeof(task_count_str), "%ld", actualTaskCount);
    sendKeyStrokes(task_count_str);
    sendKeyStrokes(PSTR("\n"));
    
    if (actualTaskCount > 0) {
      sendKeyStrokes(PSTR("Task Details:\n"));
      sendKeyStrokes(PSTR("  Name\t\tState\tPriority\tStack (words)\tTask #\n"));
      
      for (UBaseType_t i = 0; i < actualTaskCount; i++) {
        sendKeyStrokes(PSTR("  "));
        sendKeyStrokes(taskStatusArray[i].pcTaskName);
        sendKeyStrokes(PSTR("\t"));
        
        // Task state
        switch (taskStatusArray[i].eCurrentState) {
          case eRunning: sendKeyStrokes(PSTR("Running\t")); break;
          case eReady: sendKeyStrokes(PSTR("Ready\t")); break;
          case eBlocked: sendKeyStrokes(PSTR("Blocked\t")); break;
          case eSuspended: sendKeyStrokes(PSTR("Suspended")); break;
          case eDeleted: sendKeyStrokes(PSTR("Deleted\t")); break;
          default: sendKeyStrokes(PSTR("Unknown\t")); break;
        }
        
        // Print priority, stack high water mark, and task number
        char task_priority_str[4];
        snprintf(task_priority_str, sizeof(task_priority_str), "%d", taskStatusArray[i].uxCurrentPriority);
        sendKeyStrokes(task_priority_str);
        sendKeyStrokes(PSTR("\t\t"));
        
        char task_stack_str[6];
        snprintf(task_stack_str, sizeof(task_stack_str), "%d", taskStatusArray[i].usStackHighWaterMark);
        sendKeyStrokes(task_stack_str);
        sendKeyStrokes(PSTR("\t\t"));
        
        char task_num_str[4];
        snprintf(task_num_str, sizeof(task_num_str), "%d", taskStatusArray[i].xTaskNumber);
        sendKeyStrokes(task_num_str);
        sendKeyStrokes(PSTR("\n"));
      }
    }
  
    
    // ===== EXTENDED PERIPHERAL INFORMATION =====
    sendKeyStrokes(PSTR("\n----- EXTENDED PERIPHERAL INFORMATION -----\n"));
    
    // SPI status
    sendKeyStrokes(PSTR("SPI0:\n"));
    sendKeyStrokes(PSTR("  Enabled: "));
    sendKeyStrokes(NRF_SPIM0->ENABLE & SPIM_ENABLE_ENABLE_Msk ? PSTR("Yes\n") : PSTR("No\n"));
    if (NRF_SPIM0->ENABLE & SPIM_ENABLE_ENABLE_Msk) {
      sendKeyStrokes(PSTR("  Frequency: 0x"));
      char spi0_freq_str[11];
      snprintf(spi0_freq_str, sizeof(spi0_freq_str), "%lX", NRF_SPIM0->FREQUENCY);
      sendKeyStrokes(spi0_freq_str);
      sendKeyStrokes(PSTR("\n"));
      sendKeyStrokes(PSTR("  SCK Pin: 0x"));
      char spi0_sck_str[11];
      snprintf(spi0_sck_str, sizeof(spi0_sck_str), "%lX", NRF_SPIM0->PSEL.SCK);
      sendKeyStrokes(spi0_sck_str);
      sendKeyStrokes(PSTR("\n"));
      sendKeyStrokes(PSTR("  MOSI Pin: 0x"));
      char spi0_mosi_str[11];
      snprintf(spi0_mosi_str, sizeof(spi0_mosi_str), "%lX", NRF_SPIM0->PSEL.MOSI);
      sendKeyStrokes(spi0_mosi_str);
      sendKeyStrokes(PSTR("\n"));
      sendKeyStrokes(PSTR("  MISO Pin: 0x"));
      char spi0_miso_str[11];
      snprintf(spi0_miso_str, sizeof(spi0_miso_str), "%lX", NRF_SPIM0->PSEL.MISO);
      sendKeyStrokes(spi0_miso_str);
      sendKeyStrokes(PSTR("\n"));
    }
    
    // Flash Cache configuration
    sendKeyStrokes(PSTR("NVMC (Flash Controller):\n"));
    sendKeyStrokes(PSTR("  Ready: "));
    sendKeyStrokes(NRF_NVMC->READY & NVMC_READY_READY_Msk ? PSTR("Yes\n") : PSTR("No\n"));
    sendKeyStrokes(PSTR("  Config: 0x"));
    char nvmc_config[9];
    snprintf(nvmc_config, sizeof(nvmc_config), "%X\n", NRF_NVMC->CONFIG);
    sendKeyStrokes(nvmc_config);
    sendKeyStrokes(PSTR("  ICACHE Enabled: "));
    sendKeyStrokes(NRF_NVMC->ICACHECNF & NVMC_ICACHECNF_CACHEEN_Msk ? PSTR("Yes\n") : PSTR("No\n"));
    
    // MPU (Memory Protection Unit) configuration
    sendKeyStrokes(PSTR("MPU:\n"));
    sendKeyStrokes(PSTR("  MPU Control Register: "));
    sendKeyStrokes(SCB->SHCSR & SCB_SHCSR_MEMFAULTENA_Msk ? PSTR("Memory Fault Enabled\n") : PSTR("Memory Fault Disabled\n"));
    
    // ===== BLE CONFIGURATION =====
    sendKeyStrokes(PSTR("\n----- BLE CONFIGURATION -----\n"));
    
    // BLE connection configuration
    sendKeyStrokes(PSTR("BLE Connections: "));
    #ifdef BLE_CONN_STATE_MAX_CONNECTIONS
    uint8_t conn_count = 0;
    // We don't have direct access to BLE connection handles, so we'll show basic info
    char conn_count_str[4];
    snprintf(conn_count_str, sizeof(conn_count_str), "%d\n", conn_count);
    sendKeyStrokes(conn_count_str);
    
    // BLE connection details
    if (conn_count > 0) {
      sendKeyStrokes(PSTR("Connected devices:\n"));
      // We can't enumerate connections without proper API access
      sendKeyStrokes(PSTR("  [Connection details not available]\n"));
    }
    #else
    sendKeyStrokes(PSTR("BLE connection info not available\n"));
    #endif
    
    // Check CONTROL register for FPCA bit (FP Context Active)
    uint32_t control = __get_CONTROL();
    sendKeyStrokes(PSTR("CONTROL Register: 0x"));
    char control_str[9];
    snprintf(control_str, sizeof(control_str), "%X\n", control);
    sendKeyStrokes(control_str);
    sendKeyStrokes(PSTR("  FPCA Bit (bit 2): "));
    sendKeyStrokes((control & (1 << 2)) ? PSTR("Set (FP Context Active - 3mA DRAIN!)\n") : PSTR("Clear (No Active FP Context)\n"));
   
   
   
    // ===== MPU CONFIGURATION =====
    sendKeyStrokes(PSTR("\n----- MPU CONFIGURATION -----\n"));
    
    sendKeyStrokes(PSTR("MPU CTRL: 0x"));
    char mpu_ctrl[9];
    snprintf(mpu_ctrl, sizeof(mpu_ctrl), "%X\n", MPU->CTRL);
    sendKeyStrokes(mpu_ctrl);
    
    sendKeyStrokes(PSTR("MPU RBAR: 0x"));
    char mpu_rbar[9];
    snprintf(mpu_rbar, sizeof(mpu_rbar), "%X\n", MPU->RBAR);
    sendKeyStrokes(mpu_rbar);
    
    sendKeyStrokes(PSTR("MPU RASR: 0x"));
    char mpu_rasr[9];
    snprintf(mpu_rasr, sizeof(mpu_rasr), "%X\n", MPU->RASR);
    sendKeyStrokes(mpu_rasr);
    
    // Check if MPU is enabled
    sendKeyStrokes(PSTR("MPU Enabled: "));
    sendKeyStrokes((MPU->CTRL & MPU_CTRL_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    
    // Check if MPU uses default memory map as background region
    sendKeyStrokes(PSTR("Default Map Enabled: "));
    sendKeyStrokes((MPU->CTRL & MPU_CTRL_PRIVDEFENA_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    // If possible, check FPCCR register
    #ifdef SCB_FPCCR_ASPEN_Msk
    sendKeyStrokes(PSTR("FPCCR Register: 0x"));
    char fpccr_str[9];
    snprintf(fpccr_str, sizeof(fpccr_str), "%X\n", SCB->FPCCR);
    sendKeyStrokes(fpccr_str);
    sendKeyStrokes(PSTR("  ASPEN (Automatic State Preservation): "));
    sendKeyStrokes((SCB->FPCCR & SCB_FPCCR_ASPEN_Msk) ? PSTR("Enabled\n") : PSTR("Disabled\n"));
    sendKeyStrokes(PSTR("  LSPEN (Lazy State Preservation): "));
    sendKeyStrokes((SCB->FPCCR & SCB_FPCCR_LSPEN_Msk) ? PSTR("Enabled\n") : PSTR("Disabled\n"));
    #endif
    
    // ===== MEMORY PROTECTION =====
    sendKeyStrokes(PSTR("\n----- MEMORY PROTECTION -----\n"));
    
    sendKeyStrokes(PSTR("ACL Count: "));
    // NRF_ACL might not have a SIZE member; using direct register definition if available
    #ifdef ACL_CONFIG_SIZE_Msk
    char acl_size[4];
    snprintf(acl_size, sizeof(acl_size), "%d\n", (NRF_ACL->CONFIG & ACL_CONFIG_SIZE_Msk) >> ACL_CONFIG_SIZE_Pos);
    sendKeyStrokes(acl_size);
   
    
    // Check if ACL is enabled
    sendKeyStrokes(PSTR("ACL Enabled: "));
    #ifdef ACL_CONFIG_ENABLE_Msk
    sendKeyStrokes((NRF_ACL->CONFIG & ACL_CONFIG_ENABLE_Msk) ? PSTR("Yes\n") : PSTR("No\n"));
    #else
    sendKeyStrokes(PSTR("ACL enable info not available\n"));
    #endif
    #else
    sendKeyStrokes(PSTR("Memory protection information not available on this platform\n"));
    #endif
    
    // ===== POWER CRITICAL SETTINGS =====
    sendKeyStrokes(PSTR("\n----- POWER CRITICAL SETTINGS -----\n"));
    

    
    // Check DCDC converter settings - LDO uses more power
    sendKeyStrokes(PSTR("DCDC Converter: "));
    sendKeyStrokes(NRF_POWER->DCDCEN ? PSTR("Enabled (good)\n") : PSTR("Disabled (BAD - uses more power)\n"));
    
    // RAM retention settings - unnecessary retention wastes power
    sendKeyStrokes(PSTR("RAM Retention:\n"));
    #if defined(NRF52840_XXAA)
    // Check each RAM block's retention state
    for (int i = 0; i < 8; i++) {
      uint32_t ram_powerset = 0;
      sd_power_ram_power_get(i, &ram_powerset);
      sendKeyStrokes(PSTR("  Section "));
      char section_num[2];
      snprintf(section_num, sizeof(section_num), "%d", i);
      sendKeyStrokes(section_num);
      sendKeyStrokes(PSTR(": "));
      sendKeyStrokes(ram_powerset ? PSTR("Retained (uses power)\n") : PSTR("Off (good for unused RAM)\n"));
    }
    #endif
    
    // Radio settings
    sendKeyStrokes(PSTR("Radio Status:\n"));
    sendKeyStrokes(PSTR("  Radio activity: "));
    if ((NRF_RADIO->STATE & RADIO_STATE_STATE_Msk) != RADIO_STATE_STATE_Disabled) {
      sendKeyStrokes(PSTR("ACTIVE (power drain!)\n"));
    } else {
      sendKeyStrokes(PSTR("Disabled (good)\n"));
    }
    sendKeyStrokes(PSTR("  TX Power: "));
    char tx_power[5];
    snprintf(tx_power, sizeof(tx_power), "%d", NRF_RADIO->TXPOWER);
    sendKeyStrokes(tx_power);
    sendKeyStrokes(PSTR(" dBm\n"));
    
    // Check BLE advertising state
    sendKeyStrokes(PSTR("BLE Status:\n"));
    bool advertising = false;
    if (ble().connected()) {
      advertising = false;
    } else {
      advertising = true; // Assume advertising if not connected
    }
    sendKeyStrokes(PSTR("  Advertising: "));
    sendKeyStrokes(advertising ? PSTR("Yes\n") : PSTR("No\n"));

    // Replace ble().connCount() with appropriate connection check
    sendKeyStrokes(PSTR("  Connected devices: "));
    sendKeyStrokes(ble().connected() ? PSTR("1\n") : PSTR("0\n"));
    
    // Check system ON reason
    sendKeyStrokes(PSTR("System ON reason: "));
    if (NRF_POWER->RESETREAS == 0) {
      sendKeyStrokes(PSTR("Power on or external reset\n"));
    } else {
      if (NRF_POWER->RESETREAS & POWER_RESETREAS_RESETPIN_Msk)
        sendKeyStrokes(PSTR("Reset pin\n"));
      if (NRF_POWER->RESETREAS & POWER_RESETREAS_DOG_Msk)
        sendKeyStrokes(PSTR("Watchdog\n"));
      if (NRF_POWER->RESETREAS & POWER_RESETREAS_SREQ_Msk)
        sendKeyStrokes(PSTR("Software reset\n"));
      if (NRF_POWER->RESETREAS & POWER_RESETREAS_LOCKUP_Msk)
        sendKeyStrokes(PSTR("CPU lock-up\n"));
      if (NRF_POWER->RESETREAS & POWER_RESETREAS_OFF_Msk)
        sendKeyStrokes(PSTR("GPIO wakeup from OFF\n"));
    }
    
    // Power management configuration
    sendKeyStrokes(PSTR("Power Management:\n"));
    uint8_t sd_enabled = 0;
    sd_softdevice_is_enabled(&sd_enabled);
    sendKeyStrokes(PSTR("  SoftDevice enabled: "));
    sendKeyStrokes(sd_enabled ? PSTR("Yes\n") : PSTR("No\n"));
    
    uint8_t power_mode = 0;
    if (sd_enabled) {
      // We can't directly get the power mode through the API, so we'll report what we know
      sendKeyStrokes(PSTR("  Power mode: "));
      // For now, just report the presumed mode
      sendKeyStrokes(PSTR("Unknown (check power consumption to determine)\n"));
      
      // Add general power management information
      sendKeyStrokes(PSTR("  Power Optimization:\n"));
      sendKeyStrokes(PSTR("    - Low Power Mode: "));
      sendKeyStrokes(PSTR("Can be enabled using sd_power_mode_set(NRF_POWER_MODE_LOWPWR)\n"));
    }
    
    // Add RAM retention info
    sendKeyStrokes(PSTR("\nRAM Power Status:\n"));
    for (uint8_t i = 0; i < 8; i++) {
      uint32_t ram_powerset = 0;
      if (sd_power_ram_power_get(i, &ram_powerset) == NRF_SUCCESS) {
        sendKeyStrokes(PSTR("  RAM block "));
        char ram_block[2];
        snprintf(ram_block, sizeof(ram_block), "%d", i);
        sendKeyStrokes(ram_block);
        sendKeyStrokes(PSTR(": "));
        sendKeyStrokes(ram_powerset ? PSTR("Retained in System OFF\n") : PSTR("Not retained\n"));
      }
    }
    
    // Check USB regulator (major power drain when active)
    sendKeyStrokes(PSTR("\nUSB Regulator: "));
    sendKeyStrokes(NRF_USBD->ENABLE ? PSTR("Enabled (high power drain if not used)\n") : PSTR("Disabled\n"));
    
    // Add USB power status info if available
    uint32_t usbregstatus = 0;
    if (sd_power_usbregstatus_get(&usbregstatus) == NRF_SUCCESS) {
      sendKeyStrokes(PSTR("  USB Power Status: "));
      if (usbregstatus & POWER_USBREGSTATUS_VBUSDETECT_Msk) 
        sendKeyStrokes(PSTR("VBUS detected\n"));
      else if (usbregstatus & POWER_USBREGSTATUS_OUTPUTRDY_Msk)
        sendKeyStrokes(PSTR("USB power ready\n"));
      else
        sendKeyStrokes(PSTR("No USB power\n"));
    }
    
    // Check instruction cache (small power impact)
    sendKeyStrokes(PSTR("\nInstruction Cache: "));
    sendKeyStrokes((NRF_NVMC->ICACHECNF & NVMC_ICACHECNF_CACHEEN_Msk) ? 
                  PSTR("Enabled (better performance)\n") : 
                  PSTR("Disabled (slightly lower power)\n"));
    
    // Check floating GPIO inputs (common power drain source)
    sendKeyStrokes(PSTR("\nPotentially Floating Inputs (can cause power drain):\n"));
    int floating_count = 0;
    
    // P0 port
    for (int i = 0; i < 32; i++) {
      uint32_t config = NRF_P0->PIN_CNF[i];
      bool is_input = !((config & GPIO_PIN_CNF_DIR_Msk) >> GPIO_PIN_CNF_DIR_Pos);
      bool input_connected = ((config & GPIO_PIN_CNF_INPUT_Msk) >> GPIO_PIN_CNF_INPUT_Pos);
      bool pull_enabled = ((config & GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos) > 0;
      
      if (is_input && input_connected && !pull_enabled) {
        if (floating_count == 0) {
          sendKeyStrokes(PSTR("  Potentially floating inputs:\n"));
        }
        sendKeyStrokes(PSTR("    P0."));
        if (i < 10) sendKeyStrokes(PSTR("0"));
        sendKeyStrokes(i);
        sendKeyStrokes(PSTR("\n"));
        floating_count++;
      }
    }
    
    // P1 port (for nRF52840)
    #if defined(NRF52840_XXAA)
    for (int i = 0; i < 16; i++) {
      uint32_t config = NRF_P1->PIN_CNF[i];
      bool is_input = !((config & GPIO_PIN_CNF_DIR_Msk) >> GPIO_PIN_CNF_DIR_Pos);
      bool input_connected = ((config & GPIO_PIN_CNF_INPUT_Msk) >> GPIO_PIN_CNF_INPUT_Pos);
      bool pull_enabled = ((config & GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos) > 0;
      
      if (is_input && input_connected && !pull_enabled) {
        if (floating_count == 0) {
          sendKeyStrokes(PSTR("  Potentially floating inputs:\n"));
        }
        sendKeyStrokes(PSTR("    P1."));
        if (i < 10) sendKeyStrokes(PSTR("0"));
        sendKeyStrokes(i);
        sendKeyStrokes(PSTR("\n"));
        floating_count++;
      }
    }
    #endif
    
    if (floating_count == 0) {
      sendKeyStrokes(PSTR("  None detected (good)\n"));
    } else {
      sendKeyStrokes(PSTR("  Total floating inputs: "));
      sendKeyStrokes(floating_count);
      sendKeyStrokes(PSTR(" (FIX THESE!)\n"));
    }
    
    // Check for active peripheral interrupts that might prevent sleep
    sendKeyStrokes(PSTR("\nActive Interrupts (prevent deep sleep):\n"));
    bool active_interrupts = false;
    
    for (int i = 0; i < 8; i++) {
      if (NVIC->ISER[i] != 0) {
        active_interrupts = true;
        sendKeyStrokes(PSTR("  NVIC->ISER["));
        sendKeyStrokes(i);
        sendKeyStrokes(PSTR("]: 0x"));
        sendKeyStrokes(NVIC->ISER[i], HEX);
        sendKeyStrokes(PSTR("\n"));
      }
    }
    
    if (!active_interrupts) {
      sendKeyStrokes(PSTR("  None (good for power saving)\n"));
    }
    
    // Check for active sense configurations on pins (prevent ultra-low power)
    sendKeyStrokes(PSTR("\nPins with SENSE enabled (needed for wakeup but use power):\n"));
    int sense_count = 0;
    
    // P0 port
    for (int i = 0; i < 32; i++) {
      uint32_t config = NRF_P0->PIN_CNF[i];
      uint8_t sense = (config & GPIO_PIN_CNF_SENSE_Msk) >> GPIO_PIN_CNF_SENSE_Pos;
      
      if (sense != GPIO_PIN_CNF_SENSE_Disabled) {
        if (sense_count == 0) {
          sendKeyStrokes(PSTR("  Pins with SENSE enabled:\n"));
        }
        sendKeyStrokes(PSTR("    P0."));
        if (i < 10) sendKeyStrokes(PSTR("0"));
        sendKeyStrokes(i);
        sendKeyStrokes(PSTR(": "));
        sendKeyStrokes(sense == GPIO_PIN_CNF_SENSE_High ? PSTR("High\n") : PSTR("Low\n"));
        sense_count++;
      }
    }
    
    // P1 port (for nRF52840)
    #if defined(NRF52840_XXAA)
    for (int i = 0; i < 16; i++) {
      uint32_t config = NRF_P1->PIN_CNF[i];
      uint8_t sense = (config & GPIO_PIN_CNF_SENSE_Msk) >> GPIO_PIN_CNF_SENSE_Pos;
      
      if (sense != GPIO_PIN_CNF_SENSE_Disabled) {
        if (sense_count == 0) {
          sendKeyStrokes(PSTR("  Pins with SENSE enabled:\n"));
        }
        sendKeyStrokes(PSTR("    P1."));
        if (i < 10) sendKeyStrokes(PSTR("0"));
        sendKeyStrokes(i);
        sendKeyStrokes(PSTR(": "));
        sendKeyStrokes(sense == GPIO_PIN_CNF_SENSE_High ? PSTR("High\n") : PSTR("Low\n"));
        sense_count++;
      }
    }
    #endif
    
    if (sense_count == 0) {
      sendKeyStrokes(PSTR("  None (only needed for specific wakeup sources)\n"));
    }
    
    // Summary of power optimization status
    sendKeyStrokes(PSTR("\n===== POWER OPTIMIZATION SUMMARY =====\n"));
    
    int critical_issues = 0;
    int warnings = 0;
    
    // Check for critical issues (high power drain)
    if (control & (1 << 2)) {
      sendKeyStrokes(PSTR("CRITICAL: FPU context active (3mA drain)\n"));
      critical_issues++;
    }
    
    if (NRF_USBD->ENABLE && !mcu_.USBConfigured()) {
      sendKeyStrokes(PSTR("CRITICAL: USB regulator enabled but USB not connected\n"));
      critical_issues++;
    }
    
    if (floating_count > 0) {
      sendKeyStrokes(PSTR("CRITICAL: Floating inputs detected (fix with pull-up/down)\n"));
      critical_issues++;
    }
    
    if ((NRF_RADIO->STATE & RADIO_STATE_STATE_Msk) != RADIO_STATE_STATE_Disabled) {
      sendKeyStrokes(PSTR("CRITICAL: Radio active\n"));
      critical_issues++;
    }
    
    // Check for warnings (moderate power impact)
    if (!NRF_POWER->DCDCEN) {
      sendKeyStrokes(PSTR("WARNING: DCDC converter disabled (higher power consumption)\n"));
      warnings++;
    }
    
    if (power_mode == NRF_POWER_MODE_CONSTLAT) {
      sendKeyStrokes(PSTR("WARNING: Constant latency mode active (prevents deep sleep)\n"));
      warnings++;
    }
    
    if (active_interrupts) {
      sendKeyStrokes(PSTR("WARNING: Active interrupts may prevent deep sleep\n"));
      warnings++;
    }
    
    if (lfclk_stat_src != 1) {  // Not using external crystal
      sendKeyStrokes(PSTR("NOTE: Not using XTAL for LFCLK (less accurate timing)\n"));
    }
    
    sendKeyStrokes(PSTR("\nTotal critical issues: "));
    sendKeyStrokes(critical_issues);
    sendKeyStrokes(PSTR("Total warnings: "));
    sendKeyStrokes(warnings);
    
    if (critical_issues == 0 && warnings == 0) {
      sendKeyStrokes(PSTR("Power configuration looks good!\n"));
    }

    // ===== ADDITIONAL POWER-CRITICAL PERIPHERALS =====
    sendKeyStrokes(PSTR("\n----- ADDITIONAL POWER-CRITICAL PERIPHERALS -----\n"));
    
    // Check ADC/SAADC state (can consume significant power if enabled)
    sendKeyStrokes(PSTR("ADC/SAADC: "));
    sendKeyStrokes(NRF_SAADC->ENABLE ? PSTR("Enabled (power drain if not actively used)\n") : PSTR("Disabled (good)\n"));
    if (NRF_SAADC->ENABLE) {
      // Check if a conversion is in progress
      sendKeyStrokes(PSTR("  Conversion active: "));
      sendKeyStrokes(NRF_SAADC->EVENTS_STARTED && !NRF_SAADC->EVENTS_END ? PSTR("Yes (high power)\n") : PSTR("No\n"));
    }
    
    // Check watchdog state
    sendKeyStrokes(PSTR("Watchdog: "));
    sendKeyStrokes(NRF_WDT->RUNSTATUS ? PSTR("Running (periodic wake-ups)\n") : PSTR("Disabled\n"));
    if (NRF_WDT->RUNSTATUS) {
      sendKeyStrokes(PSTR("  Reload value: "));
      char wdt_crv_str[11];
      snprintf(wdt_crv_str, sizeof(wdt_crv_str), "%ld", NRF_WDT->CRV);
      sendKeyStrokes(wdt_crv_str);
      sendKeyStrokes(PSTR(" (lower values = more frequent wake-ups = more power)\n"));
    }
    
    // Check debug interface
    sendKeyStrokes(PSTR("Debug interface: "));
    #ifdef ENABLE_DEBUG
    sendKeyStrokes(PSTR("Enabled (uses power)\n"));
    #else
    sendKeyStrokes(PSTR("Disabled (good for power)\n"));
    #endif
    
    // Check SWO trace interface
    sendKeyStrokes(PSTR("SWO trace: "));
    sendKeyStrokes((NRF_CLOCK->TRACECONFIG & CLOCK_TRACECONFIG_TRACEMUX_Msk) ? 
               PSTR("Enabled (consumes power)\n") : 
               PSTR("Disabled (good)\n"));
    
    // Check TWI/I2C peripherals
    sendKeyStrokes(PSTR("\nI2C/TWI peripherals:\n"));
    sendKeyStrokes(PSTR("  TWIM0: "));
    sendKeyStrokes(NRF_TWIM0->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    sendKeyStrokes(PSTR("  TWIM1: "));
    sendKeyStrokes(NRF_TWIM1->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    sendKeyStrokes(PSTR("  TWIS0: "));
    sendKeyStrokes(NRF_TWIS0->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    sendKeyStrokes(PSTR("  TWIS1: "));
    sendKeyStrokes(NRF_TWIS1->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    
    // Check SPIM (SPI Master) peripherals
    sendKeyStrokes(PSTR("\nSPI peripherals:\n"));
    sendKeyStrokes(PSTR("  SPIM0: "));
    sendKeyStrokes(NRF_SPIM0->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    sendKeyStrokes(PSTR("  SPIM1: "));
    sendKeyStrokes(NRF_SPIM1->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    sendKeyStrokes(PSTR("  SPIM2: "));
    sendKeyStrokes(NRF_SPIM2->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    #if defined(NRF52840_XXAA)
    sendKeyStrokes(PSTR("  SPIM3: "));
    sendKeyStrokes(NRF_SPIM3->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    #endif
    
    // Check UARTE (UART with EasyDMA) peripherals
    sendKeyStrokes(PSTR("\nUART peripherals:\n"));
    sendKeyStrokes(PSTR("  UARTE0: "));
    sendKeyStrokes(NRF_UARTE0->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    sendKeyStrokes(PSTR("  UARTE1: "));
    sendKeyStrokes(NRF_UARTE1->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    
    // Check PWM peripherals
    sendKeyStrokes(PSTR("\nPWM peripherals:\n"));
    sendKeyStrokes(PSTR("  PWM0: "));
    sendKeyStrokes(NRF_PWM0->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    sendKeyStrokes(PSTR("  PWM1: "));
    sendKeyStrokes(NRF_PWM1->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    sendKeyStrokes(PSTR("  PWM2: "));
    sendKeyStrokes(NRF_PWM2->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    sendKeyStrokes(PSTR("  PWM3: "));
    sendKeyStrokes(NRF_PWM3->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    
    // Check active timers
    sendKeyStrokes(PSTR("\nTimer peripherals:\n"));
    sendKeyStrokes(PSTR("  TIMER0: "));
    sendKeyStrokes(NRF_TIMER0->TASKS_START ? PSTR("Running\n") : PSTR("Stopped (good)\n"));
    sendKeyStrokes(PSTR("  TIMER1: "));
    sendKeyStrokes(NRF_TIMER1->TASKS_START ? PSTR("Running\n") : PSTR("Stopped (good)\n"));
    sendKeyStrokes(PSTR("  TIMER2: "));
    sendKeyStrokes(NRF_TIMER2->TASKS_START ? PSTR("Running\n") : PSTR("Stopped (good)\n"));
    sendKeyStrokes(PSTR("  TIMER3: "));
    sendKeyStrokes(NRF_TIMER3->TASKS_START ? PSTR("Running\n") : PSTR("Stopped (good)\n"));
    sendKeyStrokes(PSTR("  TIMER4: "));
    sendKeyStrokes(NRF_TIMER4->TASKS_START ? PSTR("Running\n") : PSTR("Stopped (good)\n"));
    
    // Check RTC peripherals
    sendKeyStrokes(PSTR("\nRTC peripherals:\n"));
    sendKeyStrokes(PSTR("  RTC0: "));
    sendKeyStrokes(NRF_RTC0->TASKS_START ? PSTR("Running\n") : PSTR("Stopped (good)\n"));
    sendKeyStrokes(PSTR("  RTC1: "));
    sendKeyStrokes(NRF_RTC1->TASKS_START ? PSTR("Running\n") : PSTR("Stopped (good)\n"));
    sendKeyStrokes(PSTR("  RTC2: "));
    sendKeyStrokes(NRF_RTC2->TASKS_START ? PSTR("Running\n") : PSTR("Stopped (good)\n"));
    
    // Check battery gauge active state
    sendKeyStrokes(PSTR("\nBattery gauge:\n"));
    sendKeyStrokes(PSTR("  MAX17048 active: "));
    bool gauge_active = batteryGauge().begin();
    sendKeyStrokes(gauge_active ? PSTR("Yes\n") : PSTR("No\n"));
    
    if (gauge_active) {
      // Get voltage and battery level
      uint16_t voltage = batteryGauge().getVoltage();
      uint8_t level = batteryGauge().getBatteryLevel();
      
      sendKeyStrokes(PSTR("  Battery voltage: "));
      char voltage_str[7];
      snprintf(voltage_str, sizeof(voltage_str), "%d", voltage);
      sendKeyStrokes(voltage_str);
      sendKeyStrokes(PSTR(" mV\n"));
      
      sendKeyStrokes(PSTR("  Battery level: "));
      char level_str[4];
      snprintf(level_str, sizeof(level_str), "%d", level);
      sendKeyStrokes(level_str);
      sendKeyStrokes(PSTR("%\n"));
      
      // Try to detect if battery gauge is in low power mode
      sendKeyStrokes(PSTR("  Gauge low power mode: "));
      // This is an indirect detection since we can't directly read the sleep status
      // Instead, check if charge rate can be read
      int16_t charge_rate = batteryGauge().getChargeRate();
      sendKeyStrokes(charge_rate == 0 ? PSTR("Possibly in sleep mode (good)\n") : PSTR("Active mode (uses more power)\n"));
    }
    
    // Identify additional potential power drains
    sendKeyStrokes(PSTR("\nAdditional potential power drains:\n"));
    
    // Check for active PDM (microphone interface)
    sendKeyStrokes(PSTR("  PDM: "));
    sendKeyStrokes(NRF_PDM->ENABLE ? PSTR("Enabled (high power drain)\n") : PSTR("Disabled (good)\n"));
    
    // Check for active I2S (audio interface)
    sendKeyStrokes(PSTR("  I2S: "));
    sendKeyStrokes(NRF_I2S->ENABLE ? PSTR("Enabled (high power drain)\n") : PSTR("Disabled (good)\n"));
    
    // Check for active QDEC (quadrature decoder for rotary encoders)
    sendKeyStrokes(PSTR("  QDEC: "));
    sendKeyStrokes(NRF_QDEC->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    
    // Check for active COMP (analog comparator)
    sendKeyStrokes(PSTR("  COMP: "));
    sendKeyStrokes(NRF_COMP->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    
    // Check for active LPCOMP (low-power comparator)
    sendKeyStrokes(PSTR("  LPCOMP: "));
    sendKeyStrokes(NRF_LPCOMP->ENABLE ? PSTR("Enabled\n") : PSTR("Disabled (good)\n"));
    
    // Check for active NFC
    #if defined(NRF52840_XXAA)
    sendKeyStrokes(PSTR("  NFCT: "));
    sendKeyStrokes(NRF_NFCT->TASKS_DISABLE ? PSTR("Disabled (good)\n") : PSTR("Possibly active (high power drain)\n"));
    #endif
    
    // Update summary based on new peripherals checked
    if (NRF_SAADC->ENABLE) {
      sendKeyStrokes(PSTR("CRITICAL: SAADC enabled but not in use (power drain)\n"));
      critical_issues++;
    }
    
    if (NRF_PDM->ENABLE) {
      sendKeyStrokes(PSTR("CRITICAL: PDM (microphone) interface enabled (high power drain)\n"));
      critical_issues++;
    }
    
    if (NRF_I2S->ENABLE) {
      sendKeyStrokes(PSTR("CRITICAL: I2S (audio) interface enabled (high power drain)\n"));
      critical_issues++;
    }
    
    #if defined(NRF52840_XXAA)
    if (!NRF_NFCT->TASKS_DISABLE) {
      sendKeyStrokes(PSTR("CRITICAL: NFCT (NFC) possibly active (high power drain)\n"));
      critical_issues++;
    }
    #endif
    
    // Update critical issues and warnings count
    sendKeyStrokes(PSTR("\nUpdated total critical issues: "));
    char updated_critical_str[4];
    snprintf(updated_critical_str, sizeof(updated_critical_str), "%d", critical_issues);
    sendKeyStrokes(updated_critical_str);
    sendKeyStrokes(PSTR("\n"));
    sendKeyStrokes(PSTR("Updated total warnings: "));
    char updated_warnings_str[4];
    snprintf(updated_warnings_str, sizeof(updated_warnings_str), "%d", warnings);
    sendKeyStrokes(updated_warnings_str);
    sendKeyStrokes(PSTR("\n"));

    // End the dump
    sendKeyStrokes(PSTR("\n========= END OF SYSTEM STATE DUMP =========\n"));
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
    }

    // Update battery monitoring - this handles both warning and shutdown thresholds
    // Use the MAX17048 battery gauge to provide the voltage reading
    uint16_t battery_voltage = 4200; //batteryGauge().getVoltage();
    updateBatteryStatus(battery_voltage);

    // Check if we should update battery level for BLE (every 5m or on alert)
    // In theory, we shouldn't need to do this every 5m. But I don't totally trust the alert updates yet.
    if (0) { // batteryGauge().hasAlert() || (now - last_battery_update_ >= BATTERY_UPDATE_INTERVAL)) {
      updateBatteryLevel();
      if (batteryGauge().hasAlert()) {
        batteryGauge().clearAlert();
      }
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
    digitalWrite(PIN_LED_ENABLE, HIGH);
  }

  void disableLEDPower() {
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
        pinMode(PIN_LED_ENABLE, OUTPUT);
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
    // Only needed if FPU is present and used
    #ifdef __FPU_PRESENT
      #ifdef __FPU_USED
        // Clear FPCA bit in CONTROL register to indicate no active FP context
        // This is the most reliable way to prevent FPU power drain
        __set_CONTROL(__get_CONTROL() & ~(1U << 2));
        
        // Memory barriers to ensure completion
        __DSB();
        __ISB();
      #endif
    #endif
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

