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

    // Bluefruit hid - process all queue reports, then shut down processing
    while (kaleidoscope::driver::hid::bluefruit::blehid.hasQueuedReports()) {
      delay(1);
    }
    kaleidoscope::driver::hid::bluefruit::blehid.stopReportProcessing();

    // Dump the entire system state to help identify power leaks
    Serial.println(F("============= SYSTEM STATE BEFORE SLEEP ============="));
    dumpSystemState();
    Serial.println(F("============= END OF SYSTEM STATE BEFORE SLEEP ============="));
    Serial.flush();

    // disableTWIForSleep();
    // disableRTC();
    // disableTimers();
    
    // Disable FPU state preservation to prevent ~3mA power drain in sleep
    disableFPUForSleep();
    
    sd_power_mode_set(NRF_POWER_MODE_LOWPWR);

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
    // restoreRTC();
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
    Serial.println(F("\n\n========= SYSTEM STATE DUMP ========="));
    Serial.println(F("This will help identify power leaks"));
    
    // ===== GPIO PIN STATES =====
    Serial.println(F("\n----- GPIO PIN STATES -----"));
    Serial.println(F("Pin\tDir\tInput\tPull\tDrive\tSense\tValue"));
    
    // P0 pins (0-31)
    for (int i = 0; i < 32; i++) {
      uint32_t config = NRF_P0->PIN_CNF[i];
      bool dir = (config & GPIO_PIN_CNF_DIR_Msk) >> GPIO_PIN_CNF_DIR_Pos;
      bool input = (config & GPIO_PIN_CNF_INPUT_Msk) >> GPIO_PIN_CNF_INPUT_Pos;
      uint8_t pull = (config & GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos;
      uint8_t drive = (config & GPIO_PIN_CNF_DRIVE_Msk) >> GPIO_PIN_CNF_DRIVE_Pos;
      uint8_t sense = (config & GPIO_PIN_CNF_SENSE_Msk) >> GPIO_PIN_CNF_SENSE_Pos;
      bool value = (dir == 1) ? 
                  ((NRF_P0->OUT & (1UL << i)) != 0) : 
                  ((NRF_P0->IN & (1UL << i)) != 0);
      
      // Print in format: "P0.00: DIR=Out INPUT=Disconnect PULL=None DRIVE=S0S1 SENSE=Disabled VALUE=1"
      Serial.print(F("P0."));
      if (i < 10) Serial.print('0');
      Serial.print(i);
      Serial.print(F(":\t"));
      Serial.print(dir ? F("Out\t") : F("In\t"));
      Serial.print(input ? F("Connect\t") : F("Disconnect\t"));
      
      // Pull configuration
      switch (pull) {
        case 0: Serial.print(F("None\t")); break;
        case 1: Serial.print(F("Down\t")); break;
        case 3: Serial.print(F("Up\t")); break;
        default: Serial.print(F("???\t")); break;
      }
      
      // Drive configuration
      switch (drive) {
        case 0: Serial.print(F("S0S1\t")); break;
        case 1: Serial.print(F("H0S1\t")); break;
        case 2: Serial.print(F("S0H1\t")); break;
        case 3: Serial.print(F("H0H1\t")); break;
        case 4: Serial.print(F("D0S1\t")); break;
        case 5: Serial.print(F("D0H1\t")); break;
        case 6: Serial.print(F("S0D1\t")); break;
        case 7: Serial.print(F("H0D1\t")); break;
        default: Serial.print(F("???\t")); break;
      }
      
      // Sense configuration
      switch (sense) {
        case 0: Serial.print(F("Disabled\t")); break;
        case 2: Serial.print(F("High\t")); break;
        case 3: Serial.print(F("Low\t")); break;
        default: Serial.print(F("???\t")); break;
      }
      
      Serial.println(value ? F("1") : F("0"));
    }
    
    // P1 pins (32-47)
    for (int i = 0; i < 16; i++) {
      uint32_t config = NRF_P1->PIN_CNF[i];
      bool dir = (config & GPIO_PIN_CNF_DIR_Msk) >> GPIO_PIN_CNF_DIR_Pos;
      bool input = (config & GPIO_PIN_CNF_INPUT_Msk) >> GPIO_PIN_CNF_INPUT_Pos;
      uint8_t pull = (config & GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos;
      uint8_t drive = (config & GPIO_PIN_CNF_DRIVE_Msk) >> GPIO_PIN_CNF_DRIVE_Pos;
      uint8_t sense = (config & GPIO_PIN_CNF_SENSE_Msk) >> GPIO_PIN_CNF_SENSE_Pos;
      bool value = (dir == 1) ? 
                  ((NRF_P1->OUT & (1UL << i)) != 0) : 
                  ((NRF_P1->IN & (1UL << i)) != 0);
      
      Serial.print(F("P1."));
      if (i < 10) Serial.print('0');
      Serial.print(i);
      Serial.print(F(":\t"));
      Serial.print(dir ? F("Out\t") : F("In\t"));
      Serial.print(input ? F("Connect\t") : F("Disconnect\t"));
      
      // Pull configuration
      switch (pull) {
        case 0: Serial.print(F("None\t")); break;
        case 1: Serial.print(F("Down\t")); break;
        case 3: Serial.print(F("Up\t")); break;
        default: Serial.print(F("???\t")); break;
      }
      
      // Drive configuration
      switch (drive) {
        case 0: Serial.print(F("S0S1\t")); break;
        case 1: Serial.print(F("H0S1\t")); break;
        case 2: Serial.print(F("S0H1\t")); break;
        case 3: Serial.print(F("H0H1\t")); break;
        case 4: Serial.print(F("D0S1\t")); break;
        case 5: Serial.print(F("D0H1\t")); break;
        case 6: Serial.print(F("S0D1\t")); break;
        case 7: Serial.print(F("H0D1\t")); break;
        default: Serial.print(F("???\t")); break;
      }
      
      // Sense configuration
      switch (sense) {
        case 0: Serial.print(F("Disabled\t")); break;
        case 2: Serial.print(F("High\t")); break;
        case 3: Serial.print(F("Low\t")); break;
        default: Serial.print(F("???\t")); break;
      }
      
      Serial.println(value ? F("1") : F("0"));
    }
    
    // ===== CLOCK CONFIGURATION =====
  
    // ===== PERIPHERAL CONFIGURATION =====
    Serial.println(F("\n----- PERIPHERAL CONFIGURATION -----"));
    
    // NVIC (Nested Vector Interrupt Controller)
    Serial.println(F("NVIC Configuration:"));
    Serial.print(F("  Active IRQs: "));
    int activeIRQs = 0;
    #ifdef NVIC_NUM_VECTORS
    for (int i = 0; i < NVIC_NUM_VECTORS; i++) {
      if (NVIC->ISER[i / 32] & (1 << (i % 32))) {
        activeIRQs++;
      }
    }
    #else
    // For nRF52840, there are 8 ISER registers with 32 bits each
    for (int i = 0; i < 256; i++) {
      if (NVIC->ISER[i / 32] & (1 << (i % 32))) {
        activeIRQs++;
      }
    }
    #endif
    Serial.println(activeIRQs);
    
    // PPI (Programmable Peripheral Interconnect)
    Serial.println(F("PPI Configuration:"));
    Serial.print(F("  Enabled Channels: "));
    #ifdef NRF_PPI_BASE
    Serial.println(NRF_PPI->CHEN, HEX);
    #else
    Serial.println(F("PPI info not available"));
    #endif
 
    
    // ===== RADIO CONFIGURATION =====
    Serial.println(F("\n----- RADIO CONFIGURATION -----"));
    Serial.print(F("RADIO Enabled: "));
    Serial.println((NRF_RADIO->POWER & RADIO_POWER_POWER_Msk) ? F("Yes") : F("No"));
    
    Serial.print(F("RADIO Mode: 0x"));
    Serial.println(NRF_RADIO->MODE, HEX);
    
    Serial.print(F("RADIO Frequency: "));
    Serial.println(NRF_RADIO->FREQUENCY);
    
    Serial.print(F("RADIO TX Power: 0x"));
    Serial.println(NRF_RADIO->TXPOWER, HEX);
    
    // ===== WATCHDOG CONFIGURATION =====
    Serial.println(F("\n----- WATCHDOG CONFIGURATION -----"));
    Serial.print(F("Watchdog Running: "));
    Serial.println((NRF_WDT->RUNSTATUS & WDT_RUNSTATUS_RUNSTATUS_Msk) ? F("Yes") : F("No"));
    
    Serial.print(F("Watchdog Reload Value: "));
    Serial.println(NRF_WDT->CRV);
    
    Serial.print(F("Watchdog Reload Requests: 0x"));
    Serial.println(NRF_WDT->RREN, HEX);
    
    // ===== DMA/EasyDMA STATUS =====
    Serial.println(F("\n----- EasyDMA STATUS -----"));
    
    // SPIM0 EasyDMA
    Serial.println(F("SPIM0 EasyDMA:"));
    Serial.print(F("  Enabled: "));
    Serial.println((NRF_SPIM0->ENABLE & SPIM_ENABLE_ENABLE_Msk) ? F("Yes") : F("No"));
    if (NRF_SPIM0->ENABLE & SPIM_ENABLE_ENABLE_Msk) {
      Serial.print(F("  TX Buffer: 0x"));
      Serial.println(NRF_SPIM0->TXD.PTR, HEX);
      Serial.print(F("  TX Count: "));
      Serial.println(NRF_SPIM0->TXD.MAXCNT);
    }
    
    // SPIM1 EasyDMA
    Serial.println(F("SPIM1 EasyDMA:"));
    Serial.print(F("  Enabled: "));
    Serial.println((NRF_SPIM1->ENABLE & SPIM_ENABLE_ENABLE_Msk) ? F("Yes") : F("No"));
    
    // ===== QSPI CONFIGURATION (if available on nRF52840) =====
    Serial.println(F("\n----- QSPI CONFIGURATION -----"));
    Serial.print(F("QSPI Enabled: "));
    Serial.println((NRF_QSPI->ENABLE & QSPI_ENABLE_ENABLE_Msk) ? F("Yes") : F("No"));
    
    if (NRF_QSPI->ENABLE & QSPI_ENABLE_ENABLE_Msk) {
      Serial.print(F("  QSPI IFCONFIG0: 0x"));
      Serial.println(NRF_QSPI->IFCONFIG0, HEX);
      Serial.print(F("  QSPI IFCONFIG1: 0x"));
      Serial.println(NRF_QSPI->IFCONFIG1, HEX);
    }
    
    // ===== USB CONFIGURATION =====
    Serial.println(F("\n----- USB CONFIGURATION -----"));
    Serial.print(F("USB Enabled: "));
    Serial.println((NRF_USBD->ENABLE & USBD_ENABLE_ENABLE_Msk) ? F("Yes") : F("No"));
    
    Serial.print(F("USB Pullup: "));
    Serial.println((NRF_USBD->USBPULLUP & USBD_USBPULLUP_CONNECT_Msk) ? F("Connected") : F("Disconnected"));
    
    // ===== POWER SYSTEM =====
    Serial.println(F("\n----- ADDITIONAL POWER SYSTEM INFO -----"));
    
    Serial.print(F("System OFF Register: 0x"));
    Serial.println(NRF_POWER->SYSTEMOFF, HEX);
    
    Serial.print(F("RAM Power Config: "));
    for (int i = 0; i < 8; i++) {
      Serial.print(F("Section "));
      Serial.print(i);
      Serial.print(F(": 0x"));
      Serial.print(NRF_POWER->RAM[i].POWER, HEX);
      Serial.print(F(" "));
    }
    Serial.println();
    
    Serial.print(F("POFCON (Power failure comparator): 0x"));
    Serial.println(NRF_POWER->POFCON, HEX);
    
    Serial.print(F("RESETREAS (Reset reason): 0x"));
    Serial.println(NRF_POWER->RESETREAS, HEX);
    
    // Additional peripheral status checks
    Serial.println(F("\n----- ADDITIONAL PERIPHERAL STATUS -----"));
    
    // Check ACL (Access Control Lists)
    Serial.print(F("ACL Count: "));
    #ifdef ACL_CONFIG_SIZE_Msk
    Serial.println((NRF_ACL->CONFIG & ACL_CONFIG_SIZE_Msk) >> ACL_CONFIG_SIZE_Pos);
    #else
    Serial.println(F("ACL size info not available"));
    #endif
  
    
    // Check CCM (AES CCM mode encryption)
    Serial.print(F("CCM Running: "));
    Serial.println((NRF_CCM->ENABLE & CCM_ENABLE_ENABLE_Msk) ? F("Yes") : F("No"));
    
    // Check AAR (Accelerated Address Resolver)
    Serial.print(F("AAR Enabled: "));
    Serial.println((NRF_AAR->ENABLE & AAR_ENABLE_ENABLE_Msk) ? F("Yes") : F("No"));
    
    // Check ECB (AES Electronic Codebook mode encryption)
    Serial.print(F("ECB Running: "));
    Serial.println(NRF_ECB->TASKS_STARTECB ? F("Yes") : F("No"));
    
    // TEMP (Temperature sensor)
    Serial.print(F("TEMP Enabled: "));
    Serial.println((NRF_TEMP->TASKS_START) ? F("Yes") : F("No"));
    
    // RNG (Random Number Generator)
    Serial.print(F("RNG Enabled: "));
    Serial.println((NRF_RNG->TASKS_START) ? F("Yes") : F("No"));
    
    // ===== PERIPHERAL STATES =====
    Serial.println(F("\n----- PERIPHERAL STATES -----"));
    
    // TWIM0 (I2C Master)
    Serial.println(F("TWIM0 (I2C Master):"));
    Serial.print(F("  Enabled: "));
    Serial.println((NRF_TWIM0->ENABLE & TWIM_ENABLE_ENABLE_Msk) ? F("Yes") : F("No"));
    Serial.print(F("  SCL Pin: 0x"));
    Serial.println(NRF_TWIM0->PSEL.SCL, HEX);
    Serial.print(F("  SDA Pin: 0x"));
    Serial.println(NRF_TWIM0->PSEL.SDA, HEX);
    Serial.print(F("  Frequency: 0x"));
    Serial.println(NRF_TWIM0->FREQUENCY, HEX);
    Serial.print(F("  Interrupts Enabled: 0x"));
    Serial.println(NRF_TWIM0->INTENSET, HEX);
    
    // TWIS0 (I2C Slave)
    Serial.println(F("TWIS0 (I2C Slave):"));
    Serial.print(F("  Enabled: "));
    Serial.println((NRF_TWIS0->ENABLE & TWIS_ENABLE_ENABLE_Msk) ? F("Yes") : F("No"));
    Serial.print(F("  SCL Pin: 0x"));
    Serial.println(NRF_TWIS0->PSEL.SCL, HEX);
    Serial.print(F("  SDA Pin: 0x"));
    Serial.println(NRF_TWIS0->PSEL.SDA, HEX);
    Serial.print(F("  Interrupts Enabled: 0x"));
    Serial.println(NRF_TWIS0->INTENSET, HEX);
    
    // Detailed I2C pin configuration
    Serial.println(F("I2C Pin Configuration:"));
    uint32_t scl_pin = (NRF_TWIM0->PSEL.SCL & 0x3F);  // Pin number is in bits 0-5
    uint32_t sda_pin = (NRF_TWIM0->PSEL.SDA & 0x3F);  // Pin number is in bits 0-5
    
    if (scl_pin < 32) {
      Serial.print(F("  SCL Pin Config (P0."));
      Serial.print(scl_pin);
      Serial.print(F("): 0x"));
      Serial.println(NRF_P0->PIN_CNF[scl_pin], HEX);
      
      Serial.print(F("    Direction: "));
      Serial.println((NRF_P0->PIN_CNF[scl_pin] & GPIO_PIN_CNF_DIR_Msk) ? F("Output") : F("Input"));
      
      Serial.print(F("    Input Buffer: "));
      Serial.println((NRF_P0->PIN_CNF[scl_pin] & GPIO_PIN_CNF_INPUT_Msk) ? F("Connected") : F("Disconnected"));
      
      Serial.print(F("    Pull Configuration: "));
      uint32_t pull = (NRF_P0->PIN_CNF[scl_pin] & GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos;
      switch (pull) {
        case 0: Serial.println(F("Disabled")); break;
        case 1: Serial.println(F("Pulldown")); break;
        case 3: Serial.println(F("Pullup")); break;
        default: Serial.println(F("Unknown")); break;
      }
    }
    
    if (sda_pin < 32) {
      Serial.print(F("  SDA Pin Config (P0."));
      Serial.print(sda_pin);
      Serial.print(F("): 0x"));
      Serial.println(NRF_P0->PIN_CNF[sda_pin], HEX);
      
      Serial.print(F("    Direction: "));
      Serial.println((NRF_P0->PIN_CNF[sda_pin] & GPIO_PIN_CNF_DIR_Msk) ? F("Output") : F("Input"));
      
      Serial.print(F("    Input Buffer: "));
      Serial.println((NRF_P0->PIN_CNF[sda_pin] & GPIO_PIN_CNF_INPUT_Msk) ? F("Connected") : F("Disconnected"));
      
      Serial.print(F("    Pull Configuration: "));
      uint32_t pull = (NRF_P0->PIN_CNF[sda_pin] & GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos;
      switch (pull) {
        case 0: Serial.println(F("Disabled")); break;
        case 1: Serial.println(F("Pulldown")); break;
        case 3: Serial.println(F("Pullup")); break;
        default: Serial.println(F("Unknown")); break;
      }
    }
    
    // UARTE0 (Serial)
    Serial.println(F("UARTE0 (Serial):"));
    Serial.print(F("  Enabled: "));
    Serial.println((NRF_UARTE0->ENABLE & UARTE_ENABLE_ENABLE_Msk) ? F("Yes") : F("No"));
    Serial.print(F("  TX Pin: 0x"));
    Serial.println(NRF_UARTE0->PSEL.TXD, HEX);
    Serial.print(F("  RX Pin: 0x"));
    Serial.println(NRF_UARTE0->PSEL.RXD, HEX);
    
    // SAADC (Analog-to-Digital Converter)
    Serial.println(F("SAADC (ADC):"));
    Serial.print(F("  Enabled: "));
    Serial.println((NRF_SAADC->ENABLE & SAADC_ENABLE_ENABLE_Msk) ? F("Yes") : F("No"));
    if (NRF_SAADC->ENABLE & SAADC_ENABLE_ENABLE_Msk) {
      Serial.println(F("  Channel Configuration:"));
      for (int i = 0; i < 8; i++) {
        Serial.print(F("    Channel "));
        Serial.print(i);
        Serial.print(F(" Config: 0x"));
        Serial.println(NRF_SAADC->CH[i].CONFIG, HEX);
      }
    }
    
    // TIMERS
    Serial.println(F("TIMER0:"));
    Serial.print(F("  Mode: "));
    Serial.println(NRF_TIMER0->MODE, HEX);
    Serial.print(F("  Enabled Interrupts: 0x"));
    Serial.println(NRF_TIMER0->INTENSET, HEX);
    
    Serial.println(F("TIMER1:"));
    Serial.print(F("  Mode: "));
    Serial.println(NRF_TIMER1->MODE, HEX);
    Serial.print(F("  Enabled Interrupts: 0x"));
    Serial.println(NRF_TIMER1->INTENSET, HEX);
    
    Serial.println(F("TIMER2:"));
    Serial.print(F("  Mode: "));
    Serial.println(NRF_TIMER2->MODE, HEX);
    Serial.print(F("  Enabled Interrupts: 0x"));
    Serial.println(NRF_TIMER2->INTENSET, HEX);
    
    // RTC
    Serial.println(F("RTC0:"));
    Serial.print(F("  Enabled Interrupts: 0x"));
    Serial.println(NRF_RTC0->INTENSET, HEX);
    Serial.print(F("  Enabled Events: 0x"));
    Serial.println(NRF_RTC0->EVTEN, HEX);
    Serial.print(F("  Counter: "));
    Serial.println(NRF_RTC0->COUNTER);
    
    Serial.println(F("RTC1:"));
    Serial.print(F("  Enabled Interrupts: 0x"));
    Serial.println(NRF_RTC1->INTENSET, HEX);
    Serial.print(F("  Enabled Events: 0x"));
    Serial.println(NRF_RTC1->EVTEN, HEX);
    Serial.print(F("  Counter: "));
    Serial.println(NRF_RTC1->COUNTER);
    
    // GPIOTE
    Serial.println(F("GPIOTE:"));
    Serial.print(F("  Enabled Interrupts: 0x"));
    Serial.println(NRF_GPIOTE->INTENSET, HEX);
    Serial.println(F("  Channel Configuration:"));
    for (int i = 0; i < 8; i++) {
      Serial.print(F("    Channel "));
      Serial.print(i);
      Serial.print(F(": 0x"));
      Serial.println(NRF_GPIOTE->CONFIG[i], HEX);
    }
    
    // PPI (Programmable Peripheral Interconnect)
    Serial.println(F("PPI:"));
    Serial.print(F("  Enabled Channels: 0x"));
    Serial.println(NRF_PPI->CHEN, HEX);
    Serial.println(F("  Channel Configuration:"));
    for (int i = 0; i < 16; i++) {
      if (NRF_PPI->CHEN & (1 << i)) {
        Serial.print(F("    Channel "));
        Serial.print(i);
        Serial.print(F(" EEP: 0x"));
        Serial.print(NRF_PPI->CH[i].EEP, HEX);
        Serial.print(F(", TEP: 0x"));
        Serial.println(NRF_PPI->CH[i].TEP, HEX);
      }
    }
    
    
    // ===== EXTENDED CLOCK INFORMATION =====
    Serial.println(F("\n----- EXTENDED CLOCK INFORMATION -----"));
    
  Serial.println(F("\n----- CLOCK CONFIGURATION -----"));
    
   
    // Low-frequency clock
    Serial.print(F("LFCLK Source: "));
    uint32_t lfclk_stat_src = (NRF_CLOCK->LFCLKSTAT & CLOCK_LFCLKSTAT_SRC_Msk) >> CLOCK_LFCLKSTAT_SRC_Pos;
    switch (lfclk_stat_src) {
      case CLOCK_LFCLKSTAT_SRC_RC:
        Serial.println(F("RC Oscillator (higher power, less accurate)"));
        break;
      case CLOCK_LFCLKSTAT_SRC_Xtal:
        Serial.println(F("Crystal Oscillator (more accurate, uses slightly more power)"));
        break;
      case CLOCK_LFCLKSTAT_SRC_Synth:
        Serial.println(F("Synthesized from HFCLK (high power!)"));
        break;
      default:
        Serial.println(F("Unknown"));
        break;
    }
    Serial.print(F("LFCLK Running: "));
    Serial.println((NRF_CLOCK->LFCLKSTAT & CLOCK_LFCLKSTAT_STATE_Msk) ? F("Yes") : F("No"));
    

    

    // Clock source details
    Serial.print(F("LFCLKSRC: 0x"));
    Serial.println(NRF_CLOCK->LFCLKSRC, HEX);
    Serial.print(F("  Source: "));
    uint32_t lfclk_stat_src_value = (NRF_CLOCK->LFCLKSTAT & CLOCK_LFCLKSTAT_SRC_Msk) >> CLOCK_LFCLKSTAT_SRC_Pos;
    uint32_t lfclk_src = (NRF_CLOCK->LFCLKSRC & CLOCK_LFCLKSRC_SRC_Msk) >> CLOCK_LFCLKSRC_SRC_Pos;
    switch (lfclk_src) {
      case 0: Serial.println(F("RC Oscillator")); break;
      case 1: Serial.println(F("XTAL Oscillator")); break;
      case 2: Serial.println(F("Synthesized from HFCLK")); break;
      default: Serial.println(F("Unknown")); break;
    }
     // High-frequency clock
    Serial.print(F("HFCLK Source: "));
    switch (NRF_CLOCK->HFCLKSTAT & CLOCK_HFCLKSTAT_SRC_Msk) {
      case CLOCK_HFCLKSTAT_SRC_RC:
        Serial.println(F("RC Oscillator (good for power saving)"));
        break;
      case CLOCK_HFCLKSTAT_SRC_Xtal:
        Serial.println(F("Crystal Oscillator (accurate but uses more power)"));
        break;
      default:
        Serial.println(F("Unknown"));
        break;
    }
    Serial.print(F("HFCLK Running: "));
    Serial.println((NRF_CLOCK->HFCLKSTAT & CLOCK_HFCLKSTAT_STATE_Msk) ? F("Yes") : F("No"));
    


    Serial.print(F("HFXODEBOUNCE: 0x"));
    Serial.println(NRF_CLOCK->HFXODEBOUNCE, HEX);
    
    // Clock calibration status
    Serial.print(F("CTIV (Calibration timer interval): "));
    Serial.println((NRF_CLOCK->CTIV & CLOCK_CTIV_CTIV_Msk) >> CLOCK_CTIV_CTIV_Pos);
    
    Serial.print(F("Clock Events: 0x"));
    Serial.println(NRF_CLOCK->EVENTS_DONE, HEX);
    
    Serial.print(F("Clock Interrupts Enabled: 0x"));
    Serial.println(NRF_CLOCK->INTENSET, HEX);
    
    // ===== EXTENDED FREERTOS INFORMATION =====
    Serial.println(F("\n----- EXTENDED FREERTOS INFORMATION -----"));
    
    // Get detailed task information using FreeRTOS API
    Serial.print(F("Scheduler State: "));
    UBaseType_t schedulerState = xTaskGetSchedulerState();
    switch (schedulerState) {
      case 0: Serial.println(F("Not Started")); break;
      case 1: Serial.println(F("Suspended")); break;
      case 2: Serial.println(F("Running")); break;
      default: Serial.println(F("Unknown")); break;
    }
    
    // Get detailed task list
    const UBaseType_t MAX_TASKS = 10;
    TaskStatus_t taskStatusArray[MAX_TASKS];
    uint32_t totalRunTime = 0;
    
    UBaseType_t actualTaskCount = uxTaskGetSystemState(taskStatusArray, MAX_TASKS, &totalRunTime);
    
    Serial.print(F("Total tasks: "));
    Serial.println(actualTaskCount);
    
    if (actualTaskCount > 0) {
      Serial.println(F("Task Details:"));
      Serial.println(F("  Name\t\tState\tPriority\tStack (words)\tTask #"));
      
      for (UBaseType_t i = 0; i < actualTaskCount; i++) {
        Serial.print(F("  "));
        Serial.print(taskStatusArray[i].pcTaskName);
        Serial.print(F("\t"));
        
        // Task state
        switch (taskStatusArray[i].eCurrentState) {
          case eRunning: Serial.print(F("Running\t")); break;
          case eReady: Serial.print(F("Ready\t")); break;
          case eBlocked: Serial.print(F("Blocked\t")); break;
          case eSuspended: Serial.print(F("Suspended")); break;
          case eDeleted: Serial.print(F("Deleted\t")); break;
          default: Serial.print(F("Unknown\t")); break;
        }
        
        // Print priority, stack high water mark, and task number
        Serial.print(taskStatusArray[i].uxCurrentPriority);
        Serial.print(F("\t\t"));
        Serial.print(taskStatusArray[i].usStackHighWaterMark);
        Serial.print(F("\t\t"));
        Serial.println(taskStatusArray[i].xTaskNumber);
      }
    }
  
    
    // ===== EXTENDED PERIPHERAL INFORMATION =====
    Serial.println(F("\n----- EXTENDED PERIPHERAL INFORMATION -----"));
    
    // SPI status
    Serial.println(F("SPI0:"));
    Serial.print(F("  Enabled: "));
    Serial.println(NRF_SPIM0->ENABLE & SPIM_ENABLE_ENABLE_Msk ? F("Yes") : F("No"));
    if (NRF_SPIM0->ENABLE & SPIM_ENABLE_ENABLE_Msk) {
      Serial.print(F("  Frequency: 0x"));
      Serial.println(NRF_SPIM0->FREQUENCY, HEX);
      Serial.print(F("  SCK Pin: 0x"));
      Serial.println(NRF_SPIM0->PSEL.SCK, HEX);
      Serial.print(F("  MOSI Pin: 0x"));
      Serial.println(NRF_SPIM0->PSEL.MOSI, HEX);
      Serial.print(F("  MISO Pin: 0x"));
      Serial.println(NRF_SPIM0->PSEL.MISO, HEX);
    }
    
    // Flash Cache configuration
    Serial.println(F("NVMC (Flash Controller):"));
    Serial.print(F("  Ready: "));
    Serial.println(NRF_NVMC->READY & NVMC_READY_READY_Msk ? F("Yes") : F("No"));
    Serial.print(F("  Config: 0x"));
    Serial.println(NRF_NVMC->CONFIG, HEX);
    Serial.print(F("  ICACHE Enabled: "));
    Serial.println(NRF_NVMC->ICACHECNF & NVMC_ICACHECNF_CACHEEN_Msk ? F("Yes") : F("No"));
    
    // MPU (Memory Protection Unit) configuration
    Serial.println(F("MPU:"));
    Serial.print(F("  MPU Control Register: 0x"));
    Serial.println(SCB->SHCSR & SCB_SHCSR_MEMFAULTENA_Msk ? F("Memory Fault Enabled") : F("Memory Fault Disabled"));
    
    // ===== BLE CONFIGURATION =====
    Serial.println(F("\n----- BLE CONFIGURATION -----"));
    
    // BLE connection configuration
    Serial.print(F("BLE Connections: "));
    #ifdef BLE_CONN_STATE_MAX_CONNECTIONS
    uint8_t conn_count = 0;
    // We don't have direct access to BLE connection handles, so we'll show basic info
    Serial.println(conn_count);
    
    // BLE connection details
    if (conn_count > 0) {
      Serial.println(F("Connected devices:"));
      // We can't enumerate connections without proper API access
      Serial.println(F("  [Connection details not available]"));
    }
    #else
    Serial.println(F("BLE connection info not available"));
    #endif
    
    // Check CONTROL register for FPCA bit (FP Context Active)
    uint32_t control = __get_CONTROL();
    Serial.print(F("CONTROL Register: 0x"));
    Serial.println(control, HEX);
    Serial.print(F("  FPCA Bit (bit 2): "));
    Serial.println((control & (1 << 2)) ? F("Set (FP Context Active - 3mA DRAIN!)") : F("Clear (No Active FP Context)"));
   
   
   
    // ===== MPU CONFIGURATION =====
    Serial.println(F("\n----- MPU CONFIGURATION -----"));
    
    Serial.print(F("MPU CTRL: 0x"));
    Serial.println(MPU->CTRL, HEX);
    
    Serial.print(F("MPU RBAR: 0x"));
    Serial.println(MPU->RBAR, HEX);
    
    Serial.print(F("MPU RASR: 0x"));
    Serial.println(MPU->RASR, HEX);
    
    // Check if MPU is enabled
    Serial.print(F("MPU Enabled: "));
    Serial.println((MPU->CTRL & MPU_CTRL_ENABLE_Msk) ? F("Yes") : F("No"));
    
    // Check if MPU uses default memory map as background region
    Serial.print(F("Default Map Enabled: "));
    Serial.println((MPU->CTRL & MPU_CTRL_PRIVDEFENA_Msk) ? F("Yes") : F("No"));
    // If possible, check FPCCR register
    #ifdef SCB_FPCCR_ASPEN_Msk
    Serial.print(F("FPCCR Register: 0x"));
    Serial.println(SCB->FPCCR, HEX);
    Serial.print(F("  ASPEN (Automatic State Preservation): "));
    Serial.println((SCB->FPCCR & SCB_FPCCR_ASPEN_Msk) ? F("Enabled") : F("Disabled"));
    Serial.print(F("  LSPEN (Lazy State Preservation): "));
    Serial.println((SCB->FPCCR & SCB_FPCCR_LSPEN_Msk) ? F("Enabled") : F("Disabled"));
    #endif
    
    // ===== MEMORY PROTECTION =====
    Serial.println(F("\n----- MEMORY PROTECTION -----"));
    
    Serial.print(F("ACL Count: "));
    // NRF_ACL might not have a SIZE member; using direct register definition if available
    #ifdef ACL_CONFIG_SIZE_Msk
    Serial.println((NRF_ACL->CONFIG & ACL_CONFIG_SIZE_Msk) >> ACL_CONFIG_SIZE_Pos);
   
    
    // Check if ACL is enabled
    Serial.print(F("ACL Enabled: "));
    #ifdef ACL_CONFIG_ENABLE_Msk
    Serial.println((NRF_ACL->CONFIG & ACL_CONFIG_ENABLE_Msk) ? F("Yes") : F("No"));
    #else
    Serial.println(F("ACL enable info not available"));
    #endif
    #else
    Serial.println(F("Memory protection information not available on this platform"));
    #endif
    
    // ===== POWER CRITICAL SETTINGS =====
    Serial.println(F("\n----- POWER CRITICAL SETTINGS -----"));
    

    
    // Check DCDC converter settings - LDO uses more power
    Serial.print(F("DCDC Converter: "));
    Serial.println(NRF_POWER->DCDCEN ? F("Enabled (good)") : F("Disabled (BAD - uses more power)"));
    
    // RAM retention settings - unnecessary retention wastes power
    Serial.println(F("RAM Retention:"));
    #if defined(NRF52840_XXAA)
    // Check each RAM block's retention state
    for (int i = 0; i < 8; i++) {
      uint32_t ram_powerset = 0;
      sd_power_ram_power_get(i, &ram_powerset);
      Serial.print(F("  Section "));
      Serial.print(i);
      Serial.print(F(": "));
      Serial.println(ram_powerset ? F("Retained (uses power)") : F("Off (good for unused RAM)"));
    }
    #endif
    
    // Radio settings
    Serial.println(F("Radio Status:"));
    Serial.print(F("  Radio activity: "));
    if ((NRF_RADIO->STATE & RADIO_STATE_STATE_Msk) != RADIO_STATE_STATE_Disabled) {
      Serial.println(F("ACTIVE (power drain!)"));
    } else {
      Serial.println(F("Disabled (good)"));
    }
    Serial.print(F("  TX Power: "));
    Serial.print(NRF_RADIO->TXPOWER);
    Serial.println(F(" dBm"));
    
    // Check BLE advertising state
    Serial.println(F("BLE Status:"));
    bool advertising = false;
    if (ble().connected()) {
      advertising = false;
    } else {
      advertising = true; // Assume advertising if not connected
    }
    Serial.print(F("  Advertising: "));
    Serial.println(advertising ? F("Yes") : F("No"));

    // Replace ble().connCount() with appropriate connection check
    Serial.print(F("  Connected devices: "));
    Serial.println(ble().connected() ? F("1") : F("0"));
    
    // Check system ON reason
    Serial.print(F("System ON reason: "));
    if (NRF_POWER->RESETREAS == 0) {
      Serial.println(F("Power on or external reset"));
    } else {
      if (NRF_POWER->RESETREAS & POWER_RESETREAS_RESETPIN_Msk)
        Serial.println(F("Reset pin"));
      if (NRF_POWER->RESETREAS & POWER_RESETREAS_DOG_Msk)
        Serial.println(F("Watchdog"));
      if (NRF_POWER->RESETREAS & POWER_RESETREAS_SREQ_Msk)
        Serial.println(F("Software reset"));
      if (NRF_POWER->RESETREAS & POWER_RESETREAS_LOCKUP_Msk)
        Serial.println(F("CPU lock-up"));
      if (NRF_POWER->RESETREAS & POWER_RESETREAS_OFF_Msk)
        Serial.println(F("GPIO wakeup from OFF"));
    }
    
    // Power management configuration
    Serial.println(F("Power Management:"));
    uint8_t sd_enabled = 0;
    sd_softdevice_is_enabled(&sd_enabled);
    Serial.print(F("  SoftDevice enabled: "));
    Serial.println(sd_enabled ? F("Yes") : F("No"));
    
    uint8_t power_mode = 0;
    if (sd_enabled) {
      // We can't directly get the power mode through the API, so we'll report what we know
      Serial.print(F("  Power mode: "));
      // For now, just report the presumed mode
      Serial.println(F("Unknown (check power consumption to determine)"));
      
      // Add general power management information
      Serial.println(F("  Power Optimization:"));
      Serial.print(F("    - Low Power Mode: "));
      Serial.println(F("Can be enabled using sd_power_mode_set(NRF_POWER_MODE_LOWPWR)"));
    }
    
    // Add RAM retention info
    Serial.println(F("\nRAM Power Status:"));
    for (uint8_t i = 0; i < 8; i++) {
      uint32_t ram_powerset = 0;
      if (sd_power_ram_power_get(i, &ram_powerset) == NRF_SUCCESS) {
        Serial.print(F("  RAM block "));
        Serial.print(i);
        Serial.print(F(": "));
        Serial.println(ram_powerset ? F("Retained in System OFF") : F("Not retained"));
      }
    }
    
    // Check USB regulator (major power drain when active)
    Serial.print(F("\nUSB Regulator: "));
    Serial.println(NRF_USBD->ENABLE ? F("Enabled (high power drain if not used)") : F("Disabled"));
    
    // Add USB power status info if available
    uint32_t usbregstatus = 0;
    if (sd_power_usbregstatus_get(&usbregstatus) == NRF_SUCCESS) {
      Serial.print(F("  USB Power Status: "));
      if (usbregstatus & POWER_USBREGSTATUS_VBUSDETECT_Msk) 
        Serial.println(F("VBUS detected"));
      else if (usbregstatus & POWER_USBREGSTATUS_OUTPUTRDY_Msk)
        Serial.println(F("USB power ready"));
      else
        Serial.println(F("No USB power"));
    }
    
    // Check instruction cache (small power impact)
    Serial.print(F("\nInstruction Cache: "));
    Serial.println((NRF_NVMC->ICACHECNF & NVMC_ICACHECNF_CACHEEN_Msk) ? 
                  F("Enabled (better performance)") : 
                  F("Disabled (slightly lower power)"));
    
    // Check floating GPIO inputs (common power drain source)
    Serial.println(F("\nPotentially Floating Inputs (can cause power drain):"));
    int floating_count = 0;
    
    // P0 port
    for (int i = 0; i < 32; i++) {
      uint32_t config = NRF_P0->PIN_CNF[i];
      bool is_input = !((config & GPIO_PIN_CNF_DIR_Msk) >> GPIO_PIN_CNF_DIR_Pos);
      bool input_connected = ((config & GPIO_PIN_CNF_INPUT_Msk) >> GPIO_PIN_CNF_INPUT_Pos);
      bool pull_enabled = ((config & GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos) > 0;
      
      if (is_input && input_connected && !pull_enabled) {
        if (floating_count == 0) {
          Serial.println(F("  Potentially floating inputs:"));
        }
        Serial.print(F("    P0."));
        if (i < 10) Serial.print('0');
        Serial.println(i);
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
          Serial.println(F("  Potentially floating inputs:"));
        }
        Serial.print(F("    P1."));
        if (i < 10) Serial.print('0');
        Serial.println(i);
        floating_count++;
      }
    }
    #endif
    
    if (floating_count == 0) {
      Serial.println(F("  None detected (good)"));
    } else {
      Serial.print(F("  Total floating inputs: "));
      Serial.print(floating_count);
      Serial.println(F(" (FIX THESE!)"));
    }
    
    // Check for active peripheral interrupts that might prevent sleep
    Serial.println(F("\nActive Interrupts (prevent deep sleep):"));
    bool active_interrupts = false;
    
    for (int i = 0; i < 8; i++) {
      if (NVIC->ISER[i] != 0) {
        active_interrupts = true;
        Serial.print(F("  NVIC->ISER["));
        Serial.print(i);
        Serial.print(F("]: 0x"));
        Serial.println(NVIC->ISER[i], HEX);
      }
    }
    
    if (!active_interrupts) {
      Serial.println(F("  None (good for power saving)"));
    }
    
    // Check for active sense configurations on pins (prevent ultra-low power)
    Serial.println(F("\nPins with SENSE enabled (needed for wakeup but use power):"));
    int sense_count = 0;
    
    // P0 port
    for (int i = 0; i < 32; i++) {
      uint32_t config = NRF_P0->PIN_CNF[i];
      uint8_t sense = (config & GPIO_PIN_CNF_SENSE_Msk) >> GPIO_PIN_CNF_SENSE_Pos;
      
      if (sense != GPIO_PIN_CNF_SENSE_Disabled) {
        if (sense_count == 0) {
          Serial.println(F("  Pins with SENSE enabled:"));
        }
        Serial.print(F("    P0."));
        if (i < 10) Serial.print('0');
        Serial.print(i);
        Serial.print(F(": "));
        Serial.println(sense == GPIO_PIN_CNF_SENSE_High ? F("High") : F("Low"));
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
          Serial.println(F("  Pins with SENSE enabled:"));
        }
        Serial.print(F("    P1."));
        if (i < 10) Serial.print('0');
        Serial.print(i);
        Serial.print(F(": "));
        Serial.println(sense == GPIO_PIN_CNF_SENSE_High ? F("High") : F("Low"));
        sense_count++;
      }
    }
    #endif
    
    if (sense_count == 0) {
      Serial.println(F("  None (only needed for specific wakeup sources)"));
    }
    
    // Summary of power optimization status
    Serial.println(F("\n===== POWER OPTIMIZATION SUMMARY ====="));
    
    int critical_issues = 0;
    int warnings = 0;
    
    // Check for critical issues (high power drain)
    if (control & (1 << 2)) {
      Serial.println(F("CRITICAL: FPU context active (3mA drain)"));
      critical_issues++;
    }
    
    if (NRF_USBD->ENABLE && !mcu_.USBConfigured()) {
      Serial.println(F("CRITICAL: USB regulator enabled but USB not connected"));
      critical_issues++;
    }
    
    if (floating_count > 0) {
      Serial.println(F("CRITICAL: Floating inputs detected (fix with pull-up/down)"));
      critical_issues++;
    }
    
    if ((NRF_RADIO->STATE & RADIO_STATE_STATE_Msk) != RADIO_STATE_STATE_Disabled) {
      Serial.println(F("CRITICAL: Radio active"));
      critical_issues++;
    }
    
    // Check for warnings (moderate power impact)
    if (!NRF_POWER->DCDCEN) {
      Serial.println(F("WARNING: DCDC converter disabled (higher power consumption)"));
      warnings++;
    }
    
    if (power_mode == NRF_POWER_MODE_CONSTLAT) {
      Serial.println(F("WARNING: Constant latency mode active (prevents deep sleep)"));
      warnings++;
    }
    
    if (active_interrupts) {
      Serial.println(F("WARNING: Active interrupts may prevent deep sleep"));
      warnings++;
    }
    
    if (lfclk_stat_src != 1) {  // Not using external crystal
      Serial.println(F("NOTE: Not using XTAL for LFCLK (less accurate timing)"));
    }
    
    Serial.print(F("\nTotal critical issues: "));
    Serial.println(critical_issues);
    Serial.print(F("Total warnings: "));
    Serial.println(warnings);
    
    if (critical_issues == 0 && warnings == 0) {
      Serial.println(F("Power configuration looks good!"));
    }

    // ===== ADDITIONAL POWER-CRITICAL PERIPHERALS =====
    Serial.println(F("\n----- ADDITIONAL POWER-CRITICAL PERIPHERALS -----"));
    
    // Check ADC/SAADC state (can consume significant power if enabled)
    Serial.print(F("ADC/SAADC: "));
    Serial.println(NRF_SAADC->ENABLE ? F("Enabled (power drain if not actively used)") : F("Disabled (good)"));
    if (NRF_SAADC->ENABLE) {
      // Check if a conversion is in progress
      Serial.print(F("  Conversion active: "));
      Serial.println(NRF_SAADC->EVENTS_STARTED && !NRF_SAADC->EVENTS_END ? F("Yes (high power)") : F("No"));
    }
    
    // Check watchdog state
    Serial.print(F("Watchdog: "));
    Serial.println(NRF_WDT->RUNSTATUS ? F("Running (periodic wake-ups)") : F("Disabled"));
    if (NRF_WDT->RUNSTATUS) {
      Serial.print(F("  Reload value: "));
      Serial.print(NRF_WDT->CRV);
      Serial.println(F(" (lower values = more frequent wake-ups = more power)"));
    }
    
    // Check debug interface
    Serial.print(F("Debug interface: "));
    #ifdef ENABLE_DEBUG
    Serial.println(F("Enabled (uses power)"));
    #else
    Serial.println(F("Disabled (good for power)"));
    #endif
    
    // Check SWO trace interface
    Serial.print(F("SWO trace: "));
    Serial.println((NRF_CLOCK->TRACECONFIG & CLOCK_TRACECONFIG_TRACEMUX_Msk) ? 
                   F("Enabled (consumes power)") : 
                   F("Disabled (good)"));
    
    // Check TWI/I2C peripherals
    Serial.println(F("\nI2C/TWI peripherals:"));
    Serial.print(F("  TWIM0: "));
    Serial.println(NRF_TWIM0->ENABLE ? F("Enabled") : F("Disabled (good)"));
    Serial.print(F("  TWIM1: "));
    Serial.println(NRF_TWIM1->ENABLE ? F("Enabled") : F("Disabled (good)"));
    Serial.print(F("  TWIS0: "));
    Serial.println(NRF_TWIS0->ENABLE ? F("Enabled") : F("Disabled (good)"));
    Serial.print(F("  TWIS1: "));
    Serial.println(NRF_TWIS1->ENABLE ? F("Enabled") : F("Disabled (good)"));
    
    // Check SPIM (SPI Master) peripherals
    Serial.println(F("\nSPI peripherals:"));
    Serial.print(F("  SPIM0: "));
    Serial.println(NRF_SPIM0->ENABLE ? F("Enabled") : F("Disabled (good)"));
    Serial.print(F("  SPIM1: "));
    Serial.println(NRF_SPIM1->ENABLE ? F("Enabled") : F("Disabled (good)"));
    Serial.print(F("  SPIM2: "));
    Serial.println(NRF_SPIM2->ENABLE ? F("Enabled") : F("Disabled (good)"));
    #if defined(NRF52840_XXAA)
    Serial.print(F("  SPIM3: "));
    Serial.println(NRF_SPIM3->ENABLE ? F("Enabled") : F("Disabled (good)"));
    #endif
    
    // Check UARTE (UART with EasyDMA) peripherals
    Serial.println(F("\nUART peripherals:"));
    Serial.print(F("  UARTE0: "));
    Serial.println(NRF_UARTE0->ENABLE ? F("Enabled") : F("Disabled (good)"));
    Serial.print(F("  UARTE1: "));
    Serial.println(NRF_UARTE1->ENABLE ? F("Enabled") : F("Disabled (good)"));
    
    // Check PWM peripherals
    Serial.println(F("\nPWM peripherals:"));
    Serial.print(F("  PWM0: "));
    Serial.println(NRF_PWM0->ENABLE ? F("Enabled") : F("Disabled (good)"));
    Serial.print(F("  PWM1: "));
    Serial.println(NRF_PWM1->ENABLE ? F("Enabled") : F("Disabled (good)"));
    Serial.print(F("  PWM2: "));
    Serial.println(NRF_PWM2->ENABLE ? F("Enabled") : F("Disabled (good)"));
    Serial.print(F("  PWM3: "));
    Serial.println(NRF_PWM3->ENABLE ? F("Enabled") : F("Disabled (good)"));
    
    // Check active timers
    Serial.println(F("\nTimer peripherals:"));
    Serial.print(F("  TIMER0: "));
    Serial.println(NRF_TIMER0->TASKS_START ? F("Running") : F("Stopped (good)"));
    Serial.print(F("  TIMER1: "));
    Serial.println(NRF_TIMER1->TASKS_START ? F("Running") : F("Stopped (good)"));
    Serial.print(F("  TIMER2: "));
    Serial.println(NRF_TIMER2->TASKS_START ? F("Running") : F("Stopped (good)"));
    Serial.print(F("  TIMER3: "));
    Serial.println(NRF_TIMER3->TASKS_START ? F("Running") : F("Stopped (good)"));
    Serial.print(F("  TIMER4: "));
    Serial.println(NRF_TIMER4->TASKS_START ? F("Running") : F("Stopped (good)"));
    
    // Check RTC peripherals
    Serial.println(F("\nRTC peripherals:"));
    Serial.print(F("  RTC0: "));
    Serial.println(NRF_RTC0->TASKS_START ? F("Running") : F("Stopped (good)"));
    Serial.print(F("  RTC1: "));
    Serial.println(NRF_RTC1->TASKS_START ? F("Running") : F("Stopped (good)"));
    Serial.print(F("  RTC2: "));
    Serial.println(NRF_RTC2->TASKS_START ? F("Running") : F("Stopped (good)"));
    
    // Check battery gauge active state
    Serial.println(F("\nBattery gauge:"));
    Serial.print(F("  MAX17048 active: "));
    bool gauge_active = batteryGauge().begin();
    Serial.println(gauge_active ? F("Yes") : F("No"));
    
    if (gauge_active) {
      // Get voltage and battery level
      uint16_t voltage = batteryGauge().getVoltage();
      uint8_t level = batteryGauge().getBatteryLevel();
      
      Serial.print(F("  Battery voltage: "));
      Serial.print(voltage);
      Serial.println(F(" mV"));
      
      Serial.print(F("  Battery level: "));
      Serial.print(level);
      Serial.println(F("%"));
      
      // Try to detect if battery gauge is in low power mode
      Serial.print(F("  Gauge low power mode: "));
      // This is an indirect detection since we can't directly read the sleep status
      // Instead, check if charge rate can be read
      int16_t charge_rate = batteryGauge().getChargeRate();
      Serial.println(charge_rate == 0 ? F("Possibly in sleep mode (good)") : F("Active mode (uses more power)"));
    }
    
    // Identify additional potential power drains
    Serial.println(F("\nAdditional potential power drains:"));
    
    // Check for active PDM (microphone interface)
    Serial.print(F("  PDM: "));
    Serial.println(NRF_PDM->ENABLE ? F("Enabled (high power drain)") : F("Disabled (good)"));
    
    // Check for active I2S (audio interface)
    Serial.print(F("  I2S: "));
    Serial.println(NRF_I2S->ENABLE ? F("Enabled (high power drain)") : F("Disabled (good)"));
    
    // Check for active QDEC (quadrature decoder for rotary encoders)
    Serial.print(F("  QDEC: "));
    Serial.println(NRF_QDEC->ENABLE ? F("Enabled") : F("Disabled (good)"));
    
    // Check for active COMP (analog comparator)
    Serial.print(F("  COMP: "));
    Serial.println(NRF_COMP->ENABLE ? F("Enabled") : F("Disabled (good)"));
    
    // Check for active LPCOMP (low-power comparator)
    Serial.print(F("  LPCOMP: "));
    Serial.println(NRF_LPCOMP->ENABLE ? F("Enabled") : F("Disabled (good)"));
    
    // Check for active NFC
    #if defined(NRF52840_XXAA)
    Serial.print(F("  NFCT: "));
    Serial.println(NRF_NFCT->TASKS_DISABLE ? F("Disabled (good)") : F("Possibly active (high power drain)"));
    #endif
    
    // Update summary based on new peripherals checked
    if (NRF_SAADC->ENABLE) {
      Serial.println(F("CRITICAL: SAADC enabled but not in use (power drain)"));
      critical_issues++;
    }
    
    if (NRF_PDM->ENABLE) {
      Serial.println(F("CRITICAL: PDM (microphone) interface enabled (high power drain)"));
      critical_issues++;
    }
    
    if (NRF_I2S->ENABLE) {
      Serial.println(F("CRITICAL: I2S (audio) interface enabled (high power drain)"));
      critical_issues++;
    }
    
    #if defined(NRF52840_XXAA)
    if (!NRF_NFCT->TASKS_DISABLE) {
      Serial.println(F("CRITICAL: NFCT (NFC) possibly active (high power drain)"));
      critical_issues++;
    }
    #endif
    
    // Update critical issues and warnings count
    Serial.print(F("\nUpdated total critical issues: "));
    Serial.println(critical_issues);
    Serial.print(F("Updated total warnings: "));
    Serial.println(warnings);

    // End the dump
    Serial.println(F("\n========= END OF SYSTEM STATE DUMP ========="));
    Serial.flush();  // Make sure all data is sent before continuing
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

    //    disableUnusedPeripherals(); // As of this writing, disableUnusedPeripherals() does not provide a measurable power efficiency improvement
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
