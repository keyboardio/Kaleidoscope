/* Kaleidoscope-Hardware-Keyboardio-Preonic -- Keyboardio Preonic hardware support for Kaleidoscope
 * Copyright 2021-2025 Keyboard.io, inc.
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

#ifdef ARDUINO_PREONIC

#include <Arduino.h>
#include <Wire.h>                                      // For some reason, not including this breaks bluetooth pairing/connecting
#include "kaleidoscope/driver/keyscanner/Base_Impl.h"  // For Base<>
#include "kaleidoscope/driver/speaker/Base_Impl.h"     // For Base<>

#include "kaleidoscope/device/keyboardio/Preonic.h"
#include "kaleidoscope/Runtime.h"

// Includes for detailed shutdown sequence
#include "nrf_power.h"
#include "nrf_gpio.h"
#include "nrf_nvmc.h"

// No nrfx includes needed - using direct register access for reliable shutdown

// Pin number definition (Should be provided by BSP)
// #define NUMBER_OF_PINS (P1_PIN_NUM + P0_PIN_NUM) // Or just 48 for nRF52840 - Assuming BSP defines it

// *** USER MUST DEFINE or VERIFY ***
// Example: Define wakeup pins and pins connected to external components needing specific states
// The matrix pins act as wakeup sources. Their configuration might need adjustment here.
// #define WAKEUP_PIN_1 NRF_GPIO_PIN_MAP(0, 11) // Example - Define Preonic wakeup pins/matrix rows
// #define EXTERNAL_CS_PIN NRF_GPIO_PIN_MAP(0, 25) // Example - Any other external chips?

// Here, we set up aliases to the device's KeyScanner and KeyScannerProps
// in the global namespace within the scope of this file. We'll use these
// aliases to simplify some template initialization code below.
using KeyScannerProps = typename kaleidoscope::device::keyboardio::PreonicProps::KeyScannerProps;
using KeyScanner      = typename kaleidoscope::device::keyboardio::PreonicProps::KeyScanner;
using LEDDriverProps  = typename kaleidoscope::device::keyboardio::PreonicProps::LEDDriverProps;
using SpeakerProps    = typename kaleidoscope::device::keyboardio::PreonicProps::SpeakerProps;

// Template instantiation for the speaker driver - must be in global namespace
namespace kaleidoscope {
namespace driver {
namespace speaker {
template class Base<kaleidoscope::device::keyboardio::PreonicSpeakerProps>;
}  // namespace speaker
}  // namespace driver
}  // namespace kaleidoscope

namespace kaleidoscope {
namespace device {
namespace keyboardio {

// `KeyScannerProps` here refers to the alias set up above. We do not need to
// prefix the `matrix_rows` and `matrix_columns` names within the array
// declaration, because those are resolved within the context of the class, so
// the `matrix_rows` in `KeyScannerProps::matrix_row_pins[matrix_rows]` gets
// resolved as `KeyScannerProps::matrix_rows`.
const uint8_t KeyScannerProps::matrix_rows;
const uint8_t KeyScannerProps::matrix_columns;
constexpr uint8_t KeyScannerProps::matrix_row_pins[matrix_rows];
constexpr uint8_t KeyScannerProps::matrix_col_pins[matrix_columns];
constexpr uint8_t LEDDriverProps::key_led_map[];

// Initialize static members
uint32_t Preonic::last_activity_time_       = 0;
volatile bool Preonic::input_event_pending_ = false;
uint32_t Preonic::last_battery_update_      = 0;  // Initialize to 0 to force first update

// Battery state tracking
uint8_t Preonic::last_battery_level_ = 0;

// Battery monitoring variables
uint16_t Preonic::last_battery_voltage_mv_      = 0;
uint32_t Preonic::last_battery_check_time_      = 0;
uint32_t Preonic::last_warning_time_            = 0;
bool Preonic::warning_active_                   = false;
bool Preonic::shutdown_active_                  = false;
Preonic::BatteryStatus Preonic::battery_status_ = Preonic::BatteryStatus::Normal;

//// `KeyScanner` here refers to the alias set up above, just like in the
//// `KeyScannerProps` case above.
template<>
KeyScanner::row_state_t
  kaleidoscope::driver::keyscanner::NRF52KeyScanner<kaleidoscope::device::keyboardio::PreonicKeyScannerProps>::matrix_state_
    [KeyScannerProps::matrix_rows] = {};

template<>
uint32_t kaleidoscope::driver::keyscanner::NRF52KeyScanner<kaleidoscope::device::keyboardio::PreonicKeyScannerProps>::next_scan_at_ = 0;

// Battery event handling implementations - moved to .cpp to avoid circular dependencies

/**
 * @brief Trigger low battery warning
 * @param active true to activate warning, false to deactivate
 */
void Preonic::triggerBatteryWarning(bool active) {
  // Use kaleidoscope::Runtime.handlePowerEvent() to trigger the warning event
  if (active) {
    speaker().playTone(1500, 100);
    delay(100);
    speaker().playTone(1500, 100);
    delay(100);
    speaker().playTone(1500, 100);


    kaleidoscope::Runtime.handlePowerEvent(kaleidoscope::PowerEvent::BatteryWarningOn, last_battery_voltage_mv_);
  } else {
    kaleidoscope::Runtime.handlePowerEvent(kaleidoscope::PowerEvent::BatteryWarningOff, last_battery_voltage_mv_);
  }
}

/**
 * @brief Puts the system into System OFF mode after thorough peripheral shutdown.
 *
 * Refactored to use proper Nordic APIs and modular design for maintainability.
 * Performs systematic shutdown of all peripherals before entering system OFF mode.
 */
void Preonic::complete_system_shutdown(void) {
  // Shutdown sequence - order is important
  shutdownApplicationLayer();
  shutdownBLEStack();

  // Use SoftDevice services while still available
  uint32_t sd_result = shutdownSoftDeviceServices();

  // After SoftDevice is disabled, use nrfx drivers and direct register access
  shutdownPeripherals();
  configureGPIOForPowerOff();
  clearAllNVICInterrupts();
  shutdownClocks();

  // Final system power off
  enterSystemOff();
}

/**
 * @brief Stop Kaleidoscope application layer activity
 */
void Preonic::shutdownApplicationLayer() {
  // Stop any application timers and event processing
  // This is where we'd stop Kaleidoscope's main event loop if needed
}

/**
 * @brief Stop BLE stack activity
 */
void Preonic::shutdownBLEStack() {
  // Stop BLE advertising and disconnect any connections
  ble().stopAdvertising();
  // Note: BLE disconnection is handled by the SoftDevice shutdown
}

/**
 * @brief Use SoftDevice services before disabling it
 * @return Error code from SoftDevice operations
 */
uint32_t Preonic::shutdownSoftDeviceServices() {
  uint32_t err_code;

  // Configure RAM retention while SoftDevice is still active
  uint32_t ram_clr_mask = 0xFFFFFFFF;       // Clear all sections possible
  sd_power_ram_power_clr(0, ram_clr_mask);  // Ignore errors

  // Disable SoftDevice - LAST SoftDevice API call
  err_code = sd_softdevice_disable();
  if (err_code != NRF_SUCCESS) {
    NVIC_SystemReset();  // Critical failure, reset the system
  }

  return err_code;
}

/**
 * @brief Shutdown all peripherals using proper APIs
 */
void Preonic::shutdownPeripherals() {
  shutdownCommunicationPeripherals();
  shutdownAnalogPeripherals();
  shutdownTimerPeripherals();
  shutdownAudioPeripherals();
  shutdownUSBPeripherals();
  shutdownGPIOAndInterrupts();
}

/**
 * @brief Shutdown communication peripherals (UART, SPI, I2C)
 */
void Preonic::shutdownCommunicationPeripherals() {
  // For system shutdown, direct register access is actually the most reliable approach
  // because it doesn't depend on driver initialization state or build configuration

  // Helper lambda for UARTE shutdown
  auto shutdownUARTE = [](NRF_UARTE_Type *uarte) {
    uarte->TASKS_STOPTX = 1;
    uarte->TASKS_STOPRX = 1;
    uarte->ENABLE       = 0;
    uarte->INTENCLR     = 0xFFFFFFFF;
  };

  // Helper lambda for legacy UART shutdown
  auto shutdownUART = [](NRF_UART_Type *uart) {
    uart->TASKS_STOPTX = 1;
    uart->TASKS_STOPRX = 1;
    uart->ENABLE       = 0;
    uart->INTENCLR     = 0xFFFFFFFF;
  };

  // Disable UARTs - stop tasks first, then disable, then clear interrupts
  shutdownUARTE(NRF_UARTE0);
  shutdownUARTE(NRF_UARTE1);
  shutdownUART(NRF_UART0);

  // Helper lambda for SPI shutdown
  auto shutdownSPIM = [](NRF_SPIM_Type *spim) {
    spim->TASKS_STOP = 1;
    spim->ENABLE     = 0;
    spim->INTENCLR   = 0xFFFFFFFF;
  };

  // Disable SPI - stop tasks first for proper shutdown
  shutdownSPIM(NRF_SPIM0);
  shutdownSPIM(NRF_SPIM1);
  shutdownSPIM(NRF_SPIM2);
  shutdownSPIM(NRF_SPIM3);

  // Helper lambda for I2C/TWI shutdown
  auto shutdownTWIM = [](NRF_TWIM_Type *twim) {
    twim->TASKS_STOP = 1;
    twim->ENABLE     = 0;
    twim->INTENCLR   = 0xFFFFFFFF;
  };

  // Disable I2C/TWI - stop tasks first
  shutdownTWIM(NRF_TWIM0);
  shutdownTWIM(NRF_TWIM1);
}

/**
 * @brief Shutdown timer peripherals (Timers, RTCs, PWM)
 */
void Preonic::shutdownTimerPeripherals() {
  // Follow Nordic's recommended shutdown sequence: stop tasks, clear events, disable interrupts, then disable

  // Helper lambda for PWM shutdown to reduce code duplication
  auto shutdownPWM = [](NRF_PWM_Type *pwm) {
    pwm->TASKS_STOP = 1;
    pwm->ENABLE     = 0;
    pwm->INTENCLR   = 0xFFFFFFFF;
  };

  // Disable PWM instances
  shutdownPWM(NRF_PWM0);
  shutdownPWM(NRF_PWM1);
  shutdownPWM(NRF_PWM2);
  shutdownPWM(NRF_PWM3);

  // Helper lambda for Timer shutdown
  auto shutdownTimer = [](NRF_TIMER_Type *timer) {
    timer->TASKS_STOP  = 1;
    timer->TASKS_CLEAR = 1;
    timer->INTENCLR    = 0xFFFFFFFF;
  };

  // Disable Timers - proper shutdown sequence
  shutdownTimer(NRF_TIMER0);
  shutdownTimer(NRF_TIMER1);
  shutdownTimer(NRF_TIMER2);
  shutdownTimer(NRF_TIMER3);
  shutdownTimer(NRF_TIMER4);

  // Helper lambda for RTC shutdown
  auto shutdownRTC = [](NRF_RTC_Type *rtc) {
    rtc->TASKS_STOP = 1;
    rtc->EVTENCLR   = 0xFFFFFFFF;
    rtc->INTENCLR   = 0xFFFFFFFF;
  };

  // Disable RTCs - proper shutdown sequence
  shutdownRTC(NRF_RTC0);
  shutdownRTC(NRF_RTC1);
  shutdownRTC(NRF_RTC2);
}

/**
 * @brief Shutdown analog peripherals (ADC, COMP, TEMP) and sensors
 */
void Preonic::shutdownAnalogPeripherals() {
  // Use proper shutdown sequence for all analog peripherals and sensors

  // Disable QDEC (sensor peripheral that may use GPIO)
  NRF_QDEC->TASKS_STOP = 1;
  NRF_QDEC->ENABLE     = 0;
  NRF_QDEC->INTENCLR   = 0xFFFFFFFF;

  // Disable ADC/SAADC
  NRF_SAADC->TASKS_STOP = 1;
  NRF_SAADC->ENABLE     = 0;
  NRF_SAADC->INTENCLR   = 0xFFFFFFFF;

  // Disable COMP
  NRF_COMP->TASKS_STOP = 1;
  NRF_COMP->ENABLE     = 0;
  NRF_COMP->INTENCLR   = 0xFFFFFFFF;

  // Disable LPCOMP
  NRF_LPCOMP->TASKS_STOP = 1;
  NRF_LPCOMP->ENABLE     = 0;
  NRF_LPCOMP->INTENCLR   = 0xFFFFFFFF;

  // Disable temperature sensor (no ENABLE register)
  NRF_TEMP->TASKS_STOP = 1;
  NRF_TEMP->INTENCLR   = 0xFFFFFFFF;

  // Disable RNG (no ENABLE register)
  NRF_RNG->TASKS_STOP = 1;
  NRF_RNG->INTENCLR   = 0xFFFFFFFF;
}

/**
 * @brief Shutdown audio peripherals (I2S, PDM)
 */
void Preonic::shutdownAudioPeripherals() {
  // Disable I2S - direct register access for safety
  NRF_I2S->ENABLE   = 0;
  NRF_I2S->INTENCLR = 0xFFFFFFFF;

  // Disable PDM - direct register access for safety
  NRF_PDM->ENABLE   = 0;
  NRF_PDM->INTENCLR = 0xFFFFFFFF;
}

/**
 * @brief Shutdown USB peripherals
 */
void Preonic::shutdownUSBPeripherals() {
  // Disable USB device with proper shutdown sequence
  NRF_USBD->USBPULLUP = 0;
  NRF_USBD->ENABLE    = 0;
  NRF_USBD->INTENCLR  = 0xFFFFFFFF;

  // Disable USB power detection interrupts
  NRF_POWER->INTENCLR = (POWER_INTENCLR_USBDETECTED_Clear << POWER_INTENCLR_USBDETECTED_Pos) |
                        (POWER_INTENCLR_USBREMOVED_Clear << POWER_INTENCLR_USBREMOVED_Pos) |
                        (POWER_INTENCLR_USBPWRRDY_Clear << POWER_INTENCLR_USBPWRRDY_Pos);
}

/**
 * @brief Shutdown GPIO and interrupt systems
 */
void Preonic::shutdownGPIOAndInterrupts() {
  // Disable NFC/NFCT
  NRF_NFCT->TASKS_DISABLE = 1;
  NRF_NFCT->INTENCLR      = 0xFFFFFFFF;

  // Disable GPIOTE hardware directly (should be after sensor peripherals that might use it)
  NRF_GPIOTE->INTENCLR = 0xFFFFFFFF;
  for (int i = 0; i < 8; i++) {
    NRF_GPIOTE->EVENTS_IN[i] = 0;
    NRF_GPIOTE->CONFIG[i]    = 0;
  }
  NRF_GPIOTE->EVENTS_PORT = 0;

  // Disable PPI - direct register access
  NRF_PPI->CHENCLR = 0xFFFFFFFF;
  for (int i = 0; i < 16; i++) {
    NRF_PPI->CH[i].EEP = 0;
    NRF_PPI->CH[i].TEP = 0;
  }

  // Disable EGU - direct register access (no nrfx disable API)
  NRF_EGU0->INTENCLR = 0xFFFFFFFF;
  NRF_EGU1->INTENCLR = 0xFFFFFFFF;
  NRF_EGU2->INTENCLR = 0xFFFFFFFF;
  NRF_EGU3->INTENCLR = 0xFFFFFFFF;
  NRF_EGU4->INTENCLR = 0xFFFFFFFF;
  NRF_EGU5->INTENCLR = 0xFFFFFFFF;
}

/**
 * @brief Clear all NVIC interrupts
 */
void Preonic::clearAllNVICInterrupts() {
  // Clear all pending interrupts in all NVIC banks
  for (int i = 0; i < 8; i++) {
    NVIC->ICER[i] = 0xFFFFFFFF;  // Disable all interrupts
    NVIC->ICPR[i] = 0xFFFFFFFF;  // Clear all pending interrupts
  }
}

/**
 * @brief Configure GPIO pins for minimum power consumption
 */
void Preonic::configureGPIOForPowerOff() {
  // Disable LED power
  disableLEDPower();

  // Configuration for minimal power consumption
  const uint32_t low_power_pin_config =
    (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
    (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) |
    (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
    (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
    (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);

  // Configure all P0 GPIO pins to minimize power consumption
  // Skip pins 0, 1, and 18 (likely used for crystal/debug)
  for (int i = 2; i < 32; i++) {
    if (i == 18) continue;
    NRF_P0->PIN_CNF[i] = low_power_pin_config;
  }

  // Configure all P1 pins
  for (int i = 0; i < 16; i++) {
    NRF_P1->PIN_CNF[i] = low_power_pin_config;
  }
}

/**
 * @brief Shutdown clock systems
 */
void Preonic::shutdownClocks() {
  // Manual clock shutdown - most reliable for system shutdown
  // Stop LFCLK first
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
  NRF_CLOCK->LFCLKSRC            = 0;
  NRF_CLOCK->TASKS_LFCLKSTOP     = 1;

  // Wait for LFCLK to stop
  while ((NRF_CLOCK->LFCLKSTAT & CLOCK_LFCLKSTAT_STATE_Msk) != 0);

  // Stop HFCLK
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTOP     = 1;
}

/**
 * @brief Enter system OFF mode
 */
void Preonic::enterSystemOff() {
  // Disable FPU state preservation to prevent power drain
  mcu().disableFPUForSleep();

  // Disable instruction cache
  NRF_NVMC->ICACHECNF &= ~NVMC_ICACHECNF_CACHEEN_Msk;

  // Configure power settings
  NRF_POWER->DCDCEN = 1;  // Enable DC/DC converter for efficiency
  NRF_POWER->POFCON = 0;  // Disable power failure comparator

  // Clear reset reasons
  NRF_POWER->RESETREAS = 0xFFFFFFFF;

  // Disable interrupts globally
  __disable_irq();

  // Direct system power off (SoftDevice already disabled)
  NRF_POWER->SYSTEMOFF = 1;

  // Should never reach here
  while (1) {
    __WFI();  // Wait for interrupt (low power state)
  }
}

/**
 * @brief Trigger battery shutdown sequence
 */
void Preonic::triggerBatteryShutdown() {
  if (shutdown_active_)
    return;

  shutdown_active_ = true;

  // Use member function speaker()
  speaker().playTone(6000, 200);

  // Indicate shutdown (all LEDs red?)
  kaleidoscope::Runtime.handlePowerEvent(kaleidoscope::PowerEvent::BatteryShutdown, last_battery_voltage_mv_);

  // Wait for the shutdown indication period
  delay(SHUTDOWN_DURATION_MS);

  // Call the member function
  Preonic::complete_system_shutdown();
}

void disablePWMForSleep() {
  // Disable all PWM instances
  NRF_PWM0->ENABLE   = 0;
  NRF_PWM0->INTENCLR = 0xFFFFFFFF;
  NRF_PWM1->ENABLE   = 0;
  NRF_PWM1->INTENCLR = 0xFFFFFFFF;
  NRF_PWM2->ENABLE   = 0;
  NRF_PWM2->INTENCLR = 0xFFFFFFFF;
  NRF_PWM3->ENABLE   = 0;
  NRF_PWM3->INTENCLR = 0xFFFFFFFF;
}

}  // namespace keyboardio
}  // namespace device
}  // namespace kaleidoscope

#endif /* ARDUINO_ARCH_NRF52 */
