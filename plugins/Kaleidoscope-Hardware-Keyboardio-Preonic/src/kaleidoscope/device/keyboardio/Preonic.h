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
#include "kaleidoscope/driver/mcu/nRF52840.h"
#include "kaleidoscope/driver/storage/NRF52Flash.h"
#include "kaleidoscope/driver/ble/Bluefruit.h"
#include "kaleidoscope/driver/speaker/Piezo.h"
#include "nrfx_gpiote.h"
#include "kaleidoscope/driver/battery_gauge/MAX17048.h"
#include "kaleidoscope/driver/battery_charger/BQ24075.h"
#include "kaleidoscope/power_event.h"
#include "InternalFileSystem.h"  // Access to InternalFS for full low-level format


namespace kaleidoscope {
namespace device {
namespace keyboardio {

using Color = kaleidoscope::driver::led::color::GRB;
// Forward declaration of a global function to send keystrokes
// This will be implemented in a plugin-accessible location
void sendKeyStrokes(const char *str);
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
static constexpr uint8_t ENCODER_STEPS_PER_DETENT        = 2;  // Skip every 2nd step for proper detent alignment
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

  /**
   * @brief Common handler for all encoders
   * @param encoder_index The index of the encoder (0-2)
   * @param step The step value: negative for CCW, positive for CW
   */
  static void encoderEventCallback(uint8_t encoder_index, int step) {
    if (!active_scanner_) return;

    // Direction-aware decimation to handle encoder bounce and direction changes
    static int8_t encoder_last_direction[NUM_ENCODERS] = {0};
    static uint8_t encoder_step_counters[NUM_ENCODERS] = {0};

    int8_t direction = (step < 0) ? -1 : 1;

    // Reset counter on direction change to avoid wrong-direction events
    if (encoder_last_direction[encoder_index] != direction) {
      encoder_step_counters[encoder_index]  = 0;
      encoder_last_direction[encoder_index] = direction;
    }

    // Only process every Nth event in the same direction
    if (++encoder_step_counters[encoder_index] % ENCODER_STEPS_PER_DETENT != 0) return;

    if (step < 0) {
      // Counter-clockwise movement
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[encoder_index].ccw.row,
                                     ENCODER_CONFIGS[encoder_index].ccw.col,
                                     true);
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[encoder_index].ccw.row,
                                     ENCODER_CONFIGS[encoder_index].ccw.col,
                                     false);
    } else {
      // Clockwise movement
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[encoder_index].cw.row,
                                     ENCODER_CONFIGS[encoder_index].cw.col,
                                     true);
      active_scanner_->queueKeyEvent(ENCODER_CONFIGS[encoder_index].cw.row,
                                     ENCODER_CONFIGS[encoder_index].cw.col,
                                     false);
    }
  }

  // Wrapper callbacks for each encoder
  static void encoder0Callback(int step) {
    encoderEventCallback(0, step);
  }

  static void encoder1Callback(int step) {
    encoderEventCallback(1, step);
  }

  static void encoder2Callback(int step) {
    encoderEventCallback(2, step);
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
  PreonicKeyScanner::encoder0Callback,
  PreonicKeyScanner::encoder1Callback,
  PreonicKeyScanner::encoder2Callback};

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
  static constexpr uint8_t power_good_pin    = PIN_POWER_GOOD;     // Pin connected to BQ24075 PGOOD output
  static constexpr uint8_t charge_status_pin = PIN_CHARGE_STATUS;  // Pin connected to BQ24075 CHG output
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

  typedef kaleidoscope::driver::mcu::nRF52840Props MCUProps;
  typedef kaleidoscope::driver::mcu::nRF52840<MCUProps> MCU;

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
  static uint32_t last_activity_time_;                     // Used for deep sleep
  static constexpr uint32_t DEEP_SLEEP_TIMEOUT_MS = 2000;  // Enter deep sleep after 10s
  static volatile bool input_event_pending_;
  static uint32_t last_battery_update_;                        // Last battery level update time
  static constexpr uint32_t BATTERY_UPDATE_INTERVAL = 300000;  // 5 minutes in milliseconds

  // Battery monitoring variables and thresholds
  static uint16_t last_battery_voltage_mv_;
  static uint32_t last_battery_check_time_;
  static uint32_t last_warning_time_;
  static bool warning_active_;
  static bool shutdown_active_;
  static constexpr uint32_t BATTERY_CHECK_INTERVAL_MS     = 10000;  // Check battery every 10 seconds
  static constexpr uint32_t WARNING_INTERVAL_MS           = 10000;  // Warning flash interval (10 seconds)
  static constexpr uint32_t WARNING_DURATION_MS           = 1000;   // Warning flash duration (1 second)
  static constexpr uint32_t SHUTDOWN_DURATION_MS          = 10000;  // Shutdown indication duration (10 seconds)
  static constexpr uint16_t BATTERY_WARNING_THRESHOLD_MV  = 3300;   // 3.3V
  static constexpr uint16_t BATTERY_SHUTDOWN_THRESHOLD_MV = 3100;   // 3.1V

  // Battery status enum
  enum class BatteryStatus {
    Normal,    // Battery level is good
    Warning,   // Battery is low (warning threshold)
    Critical,  // Battery is critically low (shutdown threshold)
    Shutdown   // Battery is too low, system will shut down
  };
  static BatteryStatus battery_status_;


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
    // Allow battery alerts to wake us up from sleep.
    // This causes the bluetooth battery level numbers to be updated
    // while the device is sleeping. It also prevents a battery alert
    // from causing a continuous 300uA power drain
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

    uint32_t start_time     = millis();
    bool recovery_keys_held = false;

    while (millis() - start_time < 5000) {

      // Simple matrix scan without debouncing
      uint8_t pressed_count = 0;
      bool r0c9_pressed     = false;
      bool r0c10_pressed    = false;
      bool r0c11_pressed    = false;
      bool r5c6_pressed     = false;

      for (uint8_t row = 0; row < KeyScannerProps::matrix_rows; row++) {
        digitalWrite(KeyScannerProps::matrix_row_pins[row], LOW);
        delayMicroseconds(10);

        for (uint8_t col = 0; col < KeyScannerProps::matrix_columns; col++) {
          if (!digitalRead(KeyScannerProps::matrix_col_pins[col])) {
            pressed_count++;

            // Check for recovery mode keys
            if (row == 0 && col == 9) r0c9_pressed = true;
            if (row == 0 && col == 10) r0c10_pressed = true;
            if (row == 0 && col == 11) r0c11_pressed = true;
            if (row == 5 && col == 6) r5c6_pressed = true;
          }
        }
        digitalWrite(KeyScannerProps::matrix_row_pins[row], HIGH);
      }

      // Check if any other keys are pressed
      if (pressed_count > 4) {
        return false;
      }

      // Check if required keys are held
      if (!r0c9_pressed || !r0c10_pressed || !r0c11_pressed || !r5c6_pressed) {
        return false;
      }

      if (recovery_keys_held == false) {
        enableLEDPower();
        ledDriver().setup();
        ledDriver().setBrightness(100);
        // Set all the LEDs to yellow
        for (uint8_t i = 0; i < PreonicProps::LEDDriverProps::led_count; i++) {
          setCrgbAt(i, CRGB(255, 255, 0));
        }
        syncLeds();
      }
      recovery_keys_held = true;
      delay(10);  // Small delay to prevent busy-waiting
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

    // --- New: perform a complete low-level reformat of InternalFS ---
    InternalFS.end();     // Unmount if mounted
    InternalFS.erase();   // Erase all sectors in the filesystem region
    InternalFS.format();  // Lay down a fresh LittleFS instance
    InternalFS.begin();   // Mount the newly formatted FS

    // After a fresh FS is present, clear keyboard-level storage files
    storage().setup();  // Ensure storage manager is initialised
    storage().erase();  // Delete EEPROM page files & commit defaults

    // Provide user feedback: LEDs green for 2 seconds then reboot
    for (uint8_t i = 0; i < PreonicProps::LEDDriverProps::led_count; i++) {
      setCrgbAt(i, CRGB(0, 255, 0));
    }
    syncLeds();
    delay(2000);

    NVIC_SystemReset();
  }

  bool enterDeepSleep() {
    ble().prepareForSleep();
    disableLEDPower();
    keyScanner().suspendTimer();
    prepareMatrixForSleep();
    configureColumnsForSensing();
    setupGPIOTE();
    speaker().prepareForSleep();

    // Bluefruit hid - process all queue reports, then shut down processing
    kaleidoscope::driver::hid::bluefruit::blehid.prepareForSleep();

    mcu().disableTWIForSleep();
    mcu().disableRTC();
    //mcu().disableTimers(); // Disabling timers seems to make the keyscanner a little sad

    // Disable FPU state preservation to prevent ~3mA power drain in sleep
    mcu().disableFPUForSleep();

    sd_power_mode_set(NRF_POWER_MODE_LOWPWR);

    while (!input_event_pending_) {
      waitForEvent();
    }

    sd_power_mode_set(NRF_POWER_MODE_CONSTLAT);
    restoreMatrixAfterSleep();
    disableColumnSensing();
    //mcu().restoreTimers();
    mcu().restoreRTC();
    mcu().restoreTWIAfterSleep();

    enableLEDPower();

    // Restore BLE stack BEFORE starting HID report processing
    ble().restoreAfterSleep();

    // Start processing BLE HID reports AFTER BLE is restored
    kaleidoscope::driver::hid::bluefruit::blehid.startReportProcessing();

    // Resume the keyscanner timer immediately
    keyScanner().resumeTimer();

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
    if (mcu().USBConfigured()) {
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
    uint8_t new_level = batteryGauge().getBatteryLevel();
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
    mcu().disableFPUForSleep();

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
      if (!mcu().USBConfigured()) {
        // Check for shutdown threshold
        if (isBatteryBelowShutdownThreshold(voltage_mv)) {
          battery_status_ = BatteryStatus::Shutdown;
          // Right now, the best thing we can do is to turn off Bluetooth and the LED and the keyscanner.

          ble().prepareForSleep();
          disableLEDPower();
          keyScanner().suspendTimer();
          prepareMatrixForSleep();
          configureColumnsForSensing();
          setupGPIOTE();
          speaker().prepareForSleep();

          kaleidoscope::driver::hid::bluefruit::blehid.stopReportProcessing();

          ble().stopAdvertising();

          mcu().disableTWIForSleep();
          mcu().disableRTC();
          // mcu().disableTimers();  // Disabling timers seems to make the keyscanner a little sad

          // Disable FPU state preservation to prevent ~3mA power drain in sleep
          mcu().disableFPUForSleep();

          sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
          // vTaskSuspendAll();
          while (1) {
            waitForEvent();
          }


        } else if (isBatteryBelowWarningThreshold(voltage_mv)) {
          // Check for warning threshold

          if (battery_status_ != BatteryStatus::Warning) {
            battery_status_ = BatteryStatus::Warning;
            // Reset warning timing to ensure immediate warning
            last_warning_time_ = now - WARNING_INTERVAL_MS;
          }
        } else {  // Normal operation

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


  void initializeSerialForDebugging() {
    Serial.begin(9600);
    while (!Serial) {
      delay(100);
    }
  }

  void setup() {
    // Uncomment this to enable serial to start up before ~anything else is initialized, so you can see debug
    // output during startup
    // initializeSerialForDebugging();

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

    mcu().disableUnusedPeripherals();  // As of this writing, disableUnusedPeripherals() does not provide a measurable power efficiency improvement
    // Turn on the LED power


    device::Base<PreonicProps>::setup();
    last_activity_time_      = millis();
    last_battery_check_time_ = millis();
    last_warning_time_       = millis();
    warning_active_          = false;
    shutdown_active_         = false;
    battery_status_          = BatteryStatus::Normal;
    updateBatteryLevel();
  }

  Stream &serialPort() {
    return Serial;  // For now, we *always* use USB Serial
    if (getHostConnectionMode() == MODE_USB && mcu().USBConfigured()) {
      return Serial;
    } else {
      return ble().serialPort();
    }
  }

  void initSerial() {
    Serial.begin(9600);
  }
  void complete_system_shutdown(void);

 private:
  // System shutdown helper functions
  void shutdownApplicationLayer();
  void shutdownBLEStack();
  uint32_t shutdownSoftDeviceServices();
  void shutdownPeripherals();
  void shutdownCommunicationPeripherals();
  void shutdownTimerPeripherals();
  void shutdownAnalogPeripherals();
  void shutdownAudioPeripherals();
  void shutdownUSBPeripherals();
  void shutdownGPIOAndInterrupts();
  void shutdownClocks();
  void clearAllNVICInterrupts();
  void configureGPIOForPowerOff();
  void enterSystemOff();

 public:
  static void setInputEventPending() {
    input_event_pending_ = true;
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

