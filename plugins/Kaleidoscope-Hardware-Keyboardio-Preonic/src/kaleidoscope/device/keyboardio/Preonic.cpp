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
//#include "nrf_sdh.h" // For nrf_sdh_disable_request (though likely not used by Kaleidoscope)
//#include "nrf_sdh_ble.h" // For BLE stack events (referenced in comments)
//#include "nrf_drv_clock.h" // For clock functions (optional, SD disable handles HFCLK)
#include "nrf_gpio.h"
//#include "app_timer.h" // Assuming app_timer is used (Kaleidoscope might use its own system)
// #include "nrf_log.h" // Using Kaleidoscope logging instead
#include "nrf_nvmc.h"  // For UICR access if needed for NFC

//#include "nrf_drv_power.h" // For USB power events (optional)

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
Preonic::TimerState Preonic::timer_state_   = {};
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
 * Based on comprehensive shutdown sequence.
 * NOTE: Assumes necessary application state variables (e.g., for BLE)
 *       are accessible. Adapt as needed.
 *       Measurement MUST be done without debugger connected.
 *
 * REQUIRES USER VERIFICATION/IMPLEMENTATION FOR:
 *   - Stopping Kaleidoscope timers/event loop activity
 *   - Stopping Kaleidoscope BLE activity (advertising, connections)
 *   - Final GPIO configuration for matrix pins / specific external components
 */
#define NVIC_NUM_INTERRUPTS 69
void Preonic::complete_system_shutdown(void) {
  //systemOff();
  uint32_t err_code;

  // --- 1. Stop Application Activity ---
  // *** USER ACTION: Need Kaleidoscope equivalent to stop its timers/event processing ***

  // --- 2. Stop BLE Activity (CRUCIAL before SoftDevice disable) ---
  // ble().stopAdvertising();
  //ble().disconnect();
  // ble().end(); // Removed: BLEBluefruit driver does not have .end()

  // --- 4. Disable SoftDevice ---
  err_code = sd_softdevice_disable();
  if (err_code != NRF_SUCCESS) {
    NVIC_SystemReset();  // Critical failure, reset the system
                         // return; // Should not be reached
  }


  // --- 5. Disable Core Features (DCDC, Cache) ---
  // Disable DCDC converter (likely enabled by Adafruit core)
  // Disable Instruction Cache
  NRF_NVMC->ICACHECNF &= ~NVMC_ICACHECNF_CACHEEN_Msk;

  // --- 6. Disable ALL Application and System Peripherals ---
  // SAADC
  NRF_SAADC->ENABLE   = (SAADC_ENABLE_ENABLE_Disabled << SAADC_ENABLE_ENABLE_Pos);
  NRF_SAADC->INTENCLR = 0xFFFFFFFF;

  // UARTE / UART
  NRF_UARTE0->ENABLE   = (UARTE_ENABLE_ENABLE_Disabled << UARTE_ENABLE_ENABLE_Pos);
  NRF_UARTE0->INTENCLR = 0xFFFFFFFF;
  NRF_UARTE1->ENABLE   = (UARTE_ENABLE_ENABLE_Disabled << UARTE_ENABLE_ENABLE_Pos);
  NRF_UARTE1->INTENCLR = 0xFFFFFFFF;
  NRF_UART0->ENABLE    = (UART_ENABLE_ENABLE_Disabled << UART_ENABLE_ENABLE_Pos);  // Also disable legacy UART just in case
  NRF_UART0->INTENCLR  = 0xFFFFFFFF;

  // SPIM / SPI / SPIS
  NRF_SPIM0->ENABLE   = (SPIM_ENABLE_ENABLE_Disabled << SPIM_ENABLE_ENABLE_Pos);
  NRF_SPIM0->INTENCLR = 0xFFFFFFFF;
  NRF_SPIM1->ENABLE   = (SPIM_ENABLE_ENABLE_Disabled << SPIM_ENABLE_ENABLE_Pos);
  NRF_SPIM1->INTENCLR = 0xFFFFFFFF;
  NRF_SPIM2->ENABLE   = (SPIM_ENABLE_ENABLE_Disabled << SPIM_ENABLE_ENABLE_Pos);
  NRF_SPIM2->INTENCLR = 0xFFFFFFFF;
  NRF_SPIM3->ENABLE   = (SPIM_ENABLE_ENABLE_Disabled << SPIM_ENABLE_ENABLE_Pos);
  NRF_SPIM3->INTENCLR = 0xFFFFFFFF;
  // Add SPIS/SPI disable if they could be active

  // TWIM / TWI / TWIS
  NRF_TWIM0->ENABLE   = (TWIM_ENABLE_ENABLE_Disabled << TWIM_ENABLE_ENABLE_Pos);
  NRF_TWIM0->INTENCLR = 0xFFFFFFFF;
  NRF_TWIM1->ENABLE   = (TWIM_ENABLE_ENABLE_Disabled << TWIM_ENABLE_ENABLE_Pos);
  NRF_TWIM1->INTENCLR = 0xFFFFFFFF;
  // Add TWIS/TWI disable if they could be active


  // Disable GPIOTE (GPIO Tasks and Events)
  NRF_GPIOTE->INTENCLR  = 0xFFFFFFFF;
  NRF_GPIOTE->CONFIG[0] = 0;  // Clear config for all channels (0-7)
  NRF_GPIOTE->CONFIG[1] = 0;
  NRF_GPIOTE->CONFIG[2] = 0;
  NRF_GPIOTE->CONFIG[3] = 0;
  NRF_GPIOTE->CONFIG[4] = 0;
  NRF_GPIOTE->CONFIG[5] = 0;
  NRF_GPIOTE->CONFIG[6] = 0;
  NRF_GPIOTE->CONFIG[7] = 0;
  // NRF_GPIOTE->ENABLE = 0; // There isn't a global ENABLE register for GPIOTE

  // Disable PPI (Programmable Peripheral Interconnect)
  NRF_PPI->CHENCLR = 0xFFFFFFFF;  // Disable all channels
  // NRF_PPI->ENABLE = 0; // No PPI Enable register

  // Disable EGU (Event Generator Unit)

  NRF_EGU0->INTENCLR = 0xFFFFFFFF;
  NRF_EGU1->INTENCLR = 0xFFFFFFFF;
  NRF_EGU2->INTENCLR = 0xFFFFFFFF;
  NRF_EGU3->INTENCLR = 0xFFFFFFFF;
  NRF_EGU4->INTENCLR = 0xFFFFFFFF;
  NRF_EGU5->INTENCLR = 0xFFFFFFFF;


  // Other Peripherals (Disable just in case)
  NRF_COMP->TASKS_STOP = 1;
  NRF_COMP->ENABLE     = (COMP_ENABLE_ENABLE_Disabled << COMP_ENABLE_ENABLE_Pos);
  NRF_COMP->INTENCLR   = 0xFFFFFFFF;

  // --- 7. Configure GPIOs for System OFF ---
  // Step 1: Explicitly disable sensing on all pins
  for (uint32_t i = 0; i < NUMBER_OF_PINS; ++i) {
    NRF_P0->PIN_CNF[i & 0x1F] &= ~GPIO_PIN_CNF_SENSE_Msk;
    if (i >= 32) {  // Handle Port 1 if applicable (nRF52840)
      NRF_P1->PIN_CNF[i & 0x1F] &= ~GPIO_PIN_CNF_SENSE_Msk;
    }
  }


  // Step 2: Placeholder for specific pin configs (e.g., matrix rows for wakeup)
  // *** USER ACTION: Configure Preonic matrix pins and any other specific needs ***

  // --- 8. Configure RAM Retention ---
  uint32_t ram_clr_mask = 0xFFFFFFFF;                               // Clear all sections possible
  err_code              = sd_power_ram_power_clr(0, ram_clr_mask);  // Using the version confirmed to compile
                                                                    // Ignore errors


  // Manual disable for TinyUSB / Adafruit Core
  NRF_USBD->USBPULLUP = (USBD_USBPULLUP_CONNECT_Disabled << USBD_USBPULLUP_CONNECT_Pos);
  NRF_USBD->ENABLE    = (USBD_ENABLE_ENABLE_Disabled << USBD_ENABLE_ENABLE_Pos);
  // Disable USB Regulator (CRITICAL for low power)
  // NRF_USBREG->ENABLE = ... // Removed: Incorrect register access for Adafruit Core.
  // Disabling NRF_USBD->ENABLE should handle regulator.
  // Disable USB power detection interrupts
  NRF_POWER->INTENCLR = (POWER_INTENCLR_USBDETECTED_Clear << POWER_INTENCLR_USBDETECTED_Pos) |
                        (POWER_INTENCLR_USBREMOVED_Clear << POWER_INTENCLR_USBREMOVED_Pos) |
                        (POWER_INTENCLR_USBPWRRDY_Clear << POWER_INTENCLR_USBPWRRDY_Pos);


  // PWM
  NRF_PWM0->ENABLE   = (PWM_ENABLE_ENABLE_Disabled << PWM_ENABLE_ENABLE_Pos);
  NRF_PWM0->INTENCLR = 0xFFFFFFFF;
  NRF_PWM1->ENABLE   = (PWM_ENABLE_ENABLE_Disabled << PWM_ENABLE_ENABLE_Pos);
  NRF_PWM1->INTENCLR = 0xFFFFFFFF;
  NRF_PWM2->ENABLE   = (PWM_ENABLE_ENABLE_Disabled << PWM_ENABLE_ENABLE_Pos);
  NRF_PWM2->INTENCLR = 0xFFFFFFFF;
  NRF_PWM3->ENABLE   = (PWM_ENABLE_ENABLE_Disabled << PWM_ENABLE_ENABLE_Pos);
  NRF_PWM3->INTENCLR = 0xFFFFFFFF;


  // Timers
  NRF_TIMER0->TASKS_STOP = 1;
  NRF_TIMER0->INTENCLR   = 0xFFFFFFFF;
  NRF_TIMER1->TASKS_STOP = 1;
  NRF_TIMER1->INTENCLR   = 0xFFFFFFFF;
  NRF_TIMER2->TASKS_STOP = 1;
  NRF_TIMER2->INTENCLR   = 0xFFFFFFFF;
  NRF_TIMER3->TASKS_STOP = 1;
  NRF_TIMER3->INTENCLR   = 0xFFFFFFFF;
  NRF_TIMER4->TASKS_STOP = 1;
  NRF_TIMER4->INTENCLR   = 0xFFFFFFFF;

  // RTCs
  NRF_RTC0->TASKS_STOP = 1;
  NRF_RTC0->EVTENCLR   = 0xFFFFFFFF;
  NRF_RTC0->INTENCLR   = 0xFFFFFFFF;
  NRF_RTC1->TASKS_STOP = 1;
  NRF_RTC1->EVTENCLR   = 0xFFFFFFFF;
  NRF_RTC1->INTENCLR   = 0xFFFFFFFF;
  NRF_RTC2->TASKS_STOP = 1;
  NRF_RTC2->EVTENCLR   = 0xFFFFFFFF;
  NRF_RTC2->INTENCLR   = 0xFFFFFFFF;


  speaker().playTone(2000, 500);
  delay(1000);


  NRF_QDEC->TASKS_STOP = 1;
  NRF_QDEC->ENABLE     = (QDEC_ENABLE_ENABLE_Disabled << QDEC_ENABLE_ENABLE_Pos);
  NRF_QDEC->INTENCLR   = 0xFFFFFFFF;
  speaker().playTone(2000, 500);
  delay(1000);

  NRF_I2S->ENABLE   = (I2S_ENABLE_ENABLE_Disabled << I2S_ENABLE_ENABLE_Pos);
  NRF_I2S->INTENCLR = 0xFFFFFFFF;
  speaker().playTone(2000, 500);
  delay(1000);

  NRF_PDM->ENABLE   = (PDM_ENABLE_ENABLE_Disabled << PDM_ENABLE_ENABLE_Pos);
  NRF_PDM->INTENCLR = 0xFFFFFFFF;
  speaker().playTone(2000, 500);
  delay(1000);

  NRF_NFCT->TASKS_DISABLE = 1;
  // NRF_NFCT->ENABLE = ... // Removed: NFCT uses TASKS_DISABLE, not ENABLE register.
  NRF_NFCT->INTENCLR = 0xFFFFFFFF;
  speaker().playTone(2000, 500);
  delay(1000);
  NRF_LPCOMP->TASKS_STOP = 1;
  NRF_LPCOMP->ENABLE     = (LPCOMP_ENABLE_ENABLE_Disabled << LPCOMP_ENABLE_ENABLE_Pos);
  NRF_LPCOMP->INTENCLR   = 0xFFFFFFFF;
  speaker().playTone(2000, 500);
  delay(1000);

  // --- 9. Prepare for Sleep ---
  // Clear any pending NVIC interrupts
  for (IRQn_Type irq = (IRQn_Type)0; irq < ((IRQn_Type)NVIC_NUM_INTERRUPTS); irq = (IRQn_Type)((int)irq + 1)) {
    NVIC_ClearPendingIRQ(irq);
  }

  NRF_POWER->RESETREAS = 0xFFFFFFFF;
  delay(5);  // Small delay

  // Disable FPU state preservation to prevent ~3mA power drain
  mcu().disableFPUForSleep();

  __disable_irq();  // Disable interrupts globally AFTER clearing pending

  // --- 3. Stop Clocks (Do this early) ---
  NRF_CLOCK->TASKS_HFCLKSTOP = 1;
  NRF_CLOCK->TASKS_LFCLKSTOP = 1;

  // --- 10. Enter System OFF Mode ---

  // Disable ADC
  NRF_SAADC->ENABLE = 0;

  // Disable all PWM instances
  NRF_PWM0->ENABLE   = 0;
  NRF_PWM0->INTENCLR = 0xFFFFFFFF;
  NRF_PWM1->ENABLE   = 0;
  NRF_PWM1->INTENCLR = 0xFFFFFFFF;
  NRF_PWM2->ENABLE   = 0;
  NRF_PWM2->INTENCLR = 0xFFFFFFFF;
  NRF_PWM3->ENABLE   = 0;
  NRF_PWM3->INTENCLR = 0xFFFFFFFF;

  // Disable I2C/TWI interfaces
  NRF_TWIM0->ENABLE = 0;
  NRF_TWIM1->ENABLE = 0;
  NRF_TWIS0->ENABLE = 0;
  NRF_TWIS1->ENABLE = 0;

  // Disable all SPI interfaces
  NRF_SPIM0->ENABLE = 0;
  NRF_SPIM1->ENABLE = 0;
  NRF_SPIM2->ENABLE = 0;
  NRF_SPIM3->ENABLE = 0;
  NRF_SPIS0->ENABLE = 0;
  NRF_SPIS1->ENABLE = 0;
  NRF_SPIS2->ENABLE = 0;

  // Disable NFC
  NRF_NFCT->TASKS_DISABLE = 1;
  NRF_NFCT->INTENCLR      = 0xFFFFFFFF;

  // Disable I2S (Audio interface)
  NRF_I2S->ENABLE   = 0;
  NRF_I2S->INTENCLR = 0xFFFFFFFF;

  // Disable PDM (Pulse Density Modulation interface)
  NRF_PDM->ENABLE   = 0;
  NRF_PDM->INTENCLR = 0xFFFFFFFF;

  // Disable USBD (USB Device)
  NRF_USBD->ENABLE   = 0;
  NRF_USBD->INTENCLR = 0xFFFFFFFF;

  // Disable COMP (Comparator)
  NRF_COMP->TASKS_STOP = 1;
  NRF_COMP->ENABLE     = 0;
  NRF_COMP->INTENCLR   = 0xFFFFFFFF;

  // Disable LPCOMP (Low Power Comparator)
  NRF_LPCOMP->TASKS_STOP = 1;
  NRF_LPCOMP->ENABLE     = 0;
  NRF_LPCOMP->INTENCLR   = 0xFFFFFFFF;

  // Disable QDEC (Quadrature Decoder)
  NRF_QDEC->TASKS_STOP = 1;
  NRF_QDEC->ENABLE     = 0;
  NRF_QDEC->INTENCLR   = 0xFFFFFFFF;

  // Disable Event Generator Units (EGU)
  NRF_EGU0->INTENCLR = 0xFFFFFFFF;
  NRF_EGU1->INTENCLR = 0xFFFFFFFF;
  NRF_EGU2->INTENCLR = 0xFFFFFFFF;
  NRF_EGU3->INTENCLR = 0xFFFFFFFF;
  NRF_EGU4->INTENCLR = 0xFFFFFFFF;
  NRF_EGU5->INTENCLR = 0xFFFFFFFF;

  // Disable Timers
  NRF_TIMER0->TASKS_STOP = 1;
  NRF_TIMER0->INTENCLR   = 0xFFFFFFFF;
  NRF_TIMER1->TASKS_STOP = 1;
  NRF_TIMER1->INTENCLR   = 0xFFFFFFFF;
  NRF_TIMER2->TASKS_STOP = 1;
  NRF_TIMER2->INTENCLR   = 0xFFFFFFFF;
  NRF_TIMER3->TASKS_STOP = 1;
  NRF_TIMER3->INTENCLR   = 0xFFFFFFFF;
  NRF_TIMER4->TASKS_STOP = 1;
  NRF_TIMER4->INTENCLR   = 0xFFFFFFFF;


  //   For Timers (TIMER0-4)
  NRF_TIMER0->TASKS_STOP        = 1;  // Stop timer operation
  NRF_TIMER0->TASKS_CLEAR       = 1;  // Clear counter value
  NRF_TIMER0->EVENTS_COMPARE[0] = 0;  // Clear all compare match events
  NRF_TIMER0->EVENTS_COMPARE[1] = 0;
  NRF_TIMER0->EVENTS_COMPARE[2] = 0;
  NRF_TIMER0->EVENTS_COMPARE[3] = 0;
  NRF_TIMER0->EVENTS_COMPARE[4] = 0;
  NRF_TIMER0->EVENTS_COMPARE[5] = 0;
  NRF_TIMER0->INTENCLR          = 0xFFFFFFFF;  // Disable all interrupts
  NVIC_DisableIRQ(TIMER0_IRQn);                // Disable the IRQ
  NVIC_ClearPendingIRQ(TIMER0_IRQn);           // Clear any pending interrupts
  NRF_TIMER0->SHORTS = 0;                      // Disable all shortcuts

  NRF_TIMER1->TASKS_STOP        = 1;  // Stop timer operation
  NRF_TIMER1->TASKS_CLEAR       = 1;  // Clear counter value
  NRF_TIMER1->EVENTS_COMPARE[0] = 0;  // Clear all compare match events
  NRF_TIMER1->EVENTS_COMPARE[1] = 0;
  NRF_TIMER1->EVENTS_COMPARE[2] = 0;
  NRF_TIMER1->EVENTS_COMPARE[3] = 0;
  NRF_TIMER1->EVENTS_COMPARE[4] = 0;
  NRF_TIMER1->EVENTS_COMPARE[5] = 0;
  NRF_TIMER1->INTENCLR          = 0xFFFFFFFF;  // Disable all interrupts
  NVIC_DisableIRQ(TIMER1_IRQn);                // Disable the IRQ
  NVIC_ClearPendingIRQ(TIMER1_IRQn);           // Clear any pending interrupts
  NRF_TIMER1->SHORTS = 0;                      // Disable all shortcuts

  NRF_TIMER2->TASKS_STOP        = 1;  // Stop timer operation
  NRF_TIMER2->TASKS_CLEAR       = 1;  // Clear counter value
  NRF_TIMER2->EVENTS_COMPARE[0] = 0;  // Clear all compare match events
  NRF_TIMER2->EVENTS_COMPARE[1] = 0;
  NRF_TIMER2->EVENTS_COMPARE[2] = 0;
  NRF_TIMER2->EVENTS_COMPARE[3] = 0;
  NRF_TIMER2->EVENTS_COMPARE[4] = 0;
  NRF_TIMER2->EVENTS_COMPARE[5] = 0;
  NRF_TIMER2->INTENCLR          = 0xFFFFFFFF;  // Disable all interrupts
  NVIC_DisableIRQ(TIMER2_IRQn);                // Disable the IRQ
  NVIC_ClearPendingIRQ(TIMER2_IRQn);           // Clear any pending interrupts
  NRF_TIMER2->SHORTS = 0;                      // Disable all shortcuts

  NRF_TIMER3->TASKS_STOP        = 1;  // Stop timer operation
  NRF_TIMER3->TASKS_CLEAR       = 1;  // Clear counter value
  NRF_TIMER3->EVENTS_COMPARE[0] = 0;  // Clear all compare match events
  NRF_TIMER3->EVENTS_COMPARE[1] = 0;
  NRF_TIMER3->EVENTS_COMPARE[2] = 0;
  NRF_TIMER3->EVENTS_COMPARE[3] = 0;
  NRF_TIMER3->EVENTS_COMPARE[4] = 0;
  NRF_TIMER3->EVENTS_COMPARE[5] = 0;
  NRF_TIMER3->INTENCLR          = 0xFFFFFFFF;  // Disable all interrupts
  NVIC_DisableIRQ(TIMER3_IRQn);                // Disable the IRQ
  NVIC_ClearPendingIRQ(TIMER3_IRQn);           // Clear any pending interrupts
  NRF_TIMER3->SHORTS = 0;                      // Disable all shortcuts

  NRF_TIMER4->TASKS_STOP        = 1;  // Stop timer operation
  NRF_TIMER4->TASKS_CLEAR       = 1;  // Clear counter value
  NRF_TIMER4->EVENTS_COMPARE[0] = 0;  // Clear all compare match events
  NRF_TIMER4->EVENTS_COMPARE[1] = 0;
  NRF_TIMER4->EVENTS_COMPARE[2] = 0;
  NRF_TIMER4->EVENTS_COMPARE[3] = 0;
  NRF_TIMER4->EVENTS_COMPARE[4] = 0;
  NRF_TIMER4->EVENTS_COMPARE[5] = 0;
  NRF_TIMER4->INTENCLR          = 0xFFFFFFFF;  // Disable all interrupts
  NVIC_DisableIRQ(TIMER4_IRQn);                // Disable the IRQ
  NVIC_ClearPendingIRQ(TIMER4_IRQn);           // Clear any pending interrupts
  NRF_TIMER4->SHORTS = 0;                      // Disable all shortcuts


  NRF_RTC0->TASKS_STOP        = 1;
  NRF_RTC0->EVENTS_TICK       = 0;  // Clear tick event
  NRF_RTC0->EVENTS_COMPARE[0] = 0;  // Clear all compare events
  NRF_RTC0->EVENTS_COMPARE[1] = 0;
  NRF_RTC0->EVENTS_COMPARE[2] = 0;
  NRF_RTC0->EVENTS_COMPARE[3] = 0;
  NRF_RTC0->EVTENCLR          = 0xFFFFFFFF;  // Disable all events
  NRF_RTC0->INTENCLR          = 0xFFFFFFFF;  // Disable all interrupts
  NVIC_DisableIRQ(RTC0_IRQn);                // Disable the IRQ
  NVIC_ClearPendingIRQ(RTC0_IRQn);           // Clear any pending interrupts

  NRF_RTC1->TASKS_STOP = 1;

  NRF_RTC1->EVENTS_TICK       = 0;  // Clear tick event
  NRF_RTC1->EVENTS_COMPARE[0] = 0;  // Clear all compare events
  NRF_RTC1->EVENTS_COMPARE[1] = 0;
  NRF_RTC1->EVENTS_COMPARE[2] = 0;
  NRF_RTC1->EVENTS_COMPARE[3] = 0;
  NRF_RTC1->EVTENCLR          = 0xFFFFFFFF;  // Disable all events
  NRF_RTC1->INTENCLR          = 0xFFFFFFFF;  // Disable all interrupts
  NVIC_DisableIRQ(RTC1_IRQn);                // Disable the IRQ
  NVIC_ClearPendingIRQ(RTC1_IRQn);           // Clear any pending interrupts

  NRF_RTC2->TASKS_STOP        = 1;
  NRF_RTC2->EVENTS_TICK       = 0;  // Clear tick event
  NRF_RTC2->EVENTS_COMPARE[0] = 0;  // Clear all compare events
  NRF_RTC2->EVENTS_COMPARE[1] = 0;
  NRF_RTC2->EVENTS_COMPARE[2] = 0;
  NRF_RTC2->EVENTS_COMPARE[3] = 0;
  NRF_RTC2->EVTENCLR          = 0xFFFFFFFF;  // Disable all events
  NRF_RTC2->INTENCLR          = 0xFFFFFFFF;  // Disable all interrupts
  NVIC_DisableIRQ(RTC2_IRQn);                // Disable the IRQ
  NVIC_ClearPendingIRQ(RTC2_IRQn);           // Clear any pending interrupts

  NRF_WDT->INTENCLR = 0xFFFFFFFF;  // Disable all interrupts

  NRF_RNG->TASKS_STOP = 1;
  NRF_RNG->INTENCLR   = 0xFFFFFFFF;  // Disable all interrupts


  // Disable Real-Time Counters (RTC)
  NRF_RTC0->TASKS_STOP = 1;
  NRF_RTC0->EVTENCLR   = 0xFFFFFFFF;
  NRF_RTC0->INTENCLR   = 0xFFFFFFFF;
  NRF_RTC1->TASKS_STOP = 1;
  NRF_RTC1->EVTENCLR   = 0xFFFFFFFF;
  NRF_RTC1->INTENCLR   = 0xFFFFFFFF;
  NRF_RTC2->TASKS_STOP = 1;
  NRF_RTC2->EVTENCLR   = 0xFFFFFFFF;
  NRF_RTC2->INTENCLR   = 0xFFFFFFFF;

  // Disable WDT (Watchdog Timer)
  NRF_WDT->INTENCLR = 0xFFFFFFFF;

  // Disable RNG (Random Number Generator)
  NRF_RNG->TASKS_STOP = 1;
  NRF_RNG->INTENCLR   = 0xFFFFFFFF;

  // Disable TEMP (Temperature sensor)
  NRF_TEMP->TASKS_STOP = 1;
  NRF_TEMP->INTENCLR   = 0xFFFFFFFF;


  // Disable PPI (Programmable Peripheral Interconnect)
  NRF_PPI->CHENCLR  = 0xFFFFFFFF;  // Disable all channels
  NRF_POWER->DCDCEN = 1;


  // Disable all peripherals to achieve lowest power consumption

  // Disable UART/UARTE - Clear events and disable interrupts
  NRF_UARTE0->TASKS_STOPTX = 1;
  NRF_UARTE0->TASKS_STOPRX = 1;
  NRF_UARTE0->EVENTS_ENDRX = 0;
  NRF_UARTE0->EVENTS_ENDTX = 0;
  NRF_UARTE0->EVENTS_ERROR = 0;
  NRF_UARTE0->INTENCLR     = 0xFFFFFFFF;
  NVIC_DisableIRQ(UARTE0_UART0_IRQn);
  NVIC_ClearPendingIRQ(UARTE0_UART0_IRQn);
  NRF_UARTE0->ENABLE = 0;

  NRF_UARTE1->TASKS_STOPTX = 1;
  NRF_UARTE1->TASKS_STOPRX = 1;
  NRF_UARTE1->EVENTS_ENDRX = 0;
  NRF_UARTE1->EVENTS_ENDTX = 0;
  NRF_UARTE1->EVENTS_ERROR = 0;
  NRF_UARTE1->INTENCLR     = 0xFFFFFFFF;
  NVIC_DisableIRQ(UARTE1_IRQn);
  NVIC_ClearPendingIRQ(UARTE1_IRQn);
  NRF_UARTE1->ENABLE = 0;

  NRF_UART0->TASKS_STOPTX  = 1;
  NRF_UART0->TASKS_STOPRX  = 1;
  NRF_UART0->EVENTS_RXDRDY = 0;
  NRF_UART0->EVENTS_TXDRDY = 0;
  NRF_UART0->INTENCLR      = 0xFFFFFFFF;
  NRF_UART0->ENABLE        = 0;

  // Disable ADC
  NRF_SAADC->TASKS_STOP     = 1;
  NRF_SAADC->EVENTS_STARTED = 0;
  NRF_SAADC->EVENTS_END     = 0;
  NRF_SAADC->INTENCLR       = 0xFFFFFFFF;
  NVIC_DisableIRQ(SAADC_IRQn);
  NVIC_ClearPendingIRQ(SAADC_IRQn);
  NRF_SAADC->ENABLE = 0;

  // Disable all PWM instances
  NRF_PWM0->TASKS_STOP       = 1;
  NRF_PWM0->EVENTS_STOPPED   = 0;
  NRF_PWM0->EVENTS_SEQEND[0] = 0;
  NRF_PWM0->EVENTS_SEQEND[1] = 0;
  NRF_PWM0->INTENCLR         = 0xFFFFFFFF;
  NVIC_DisableIRQ(PWM0_IRQn);
  NVIC_ClearPendingIRQ(PWM0_IRQn);
  NRF_PWM0->ENABLE = 0;

  NRF_PWM1->TASKS_STOP       = 1;
  NRF_PWM1->EVENTS_STOPPED   = 0;
  NRF_PWM1->EVENTS_SEQEND[0] = 0;
  NRF_PWM1->EVENTS_SEQEND[1] = 0;
  NRF_PWM1->INTENCLR         = 0xFFFFFFFF;
  NVIC_DisableIRQ(PWM1_IRQn);
  NVIC_ClearPendingIRQ(PWM1_IRQn);
  NRF_PWM1->ENABLE = 0;

  NRF_PWM2->TASKS_STOP       = 1;
  NRF_PWM2->EVENTS_STOPPED   = 0;
  NRF_PWM2->EVENTS_SEQEND[0] = 0;
  NRF_PWM2->EVENTS_SEQEND[1] = 0;
  NRF_PWM2->INTENCLR         = 0xFFFFFFFF;
  NVIC_DisableIRQ(PWM2_IRQn);
  NVIC_ClearPendingIRQ(PWM2_IRQn);
  NRF_PWM2->ENABLE = 0;

  NRF_PWM3->TASKS_STOP       = 1;
  NRF_PWM3->EVENTS_STOPPED   = 0;
  NRF_PWM3->EVENTS_SEQEND[0] = 0;
  NRF_PWM3->EVENTS_SEQEND[1] = 0;
  NRF_PWM3->INTENCLR         = 0xFFFFFFFF;
  NVIC_DisableIRQ(PWM3_IRQn);
  NVIC_ClearPendingIRQ(PWM3_IRQn);
  NRF_PWM3->ENABLE = 0;

  // Disable I2C/TWI interfaces
  NRF_TWIM0->TASKS_STOP     = 1;
  NRF_TWIM0->EVENTS_STOPPED = 0;
  NRF_TWIM0->INTENCLR       = 0xFFFFFFFF;
  NVIC_DisableIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);
  NVIC_ClearPendingIRQ(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);
  NRF_TWIM0->ENABLE = 0;

  NRF_TWIM1->TASKS_STOP     = 1;
  NRF_TWIM1->EVENTS_STOPPED = 0;
  NRF_TWIM1->INTENCLR       = 0xFFFFFFFF;
  NVIC_DisableIRQ(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn);
  NVIC_ClearPendingIRQ(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn);
  NRF_TWIM1->ENABLE = 0;

  NRF_TWIS0->INTENCLR = 0xFFFFFFFF;
  NRF_TWIS0->ENABLE   = 0;

  NRF_TWIS1->INTENCLR = 0xFFFFFFFF;
  NRF_TWIS1->ENABLE   = 0;

  // Disable all SPI interfaces
  NRF_SPIM0->TASKS_STOP     = 1;
  NRF_SPIM0->EVENTS_STOPPED = 0;
  NRF_SPIM0->EVENTS_ENDRX   = 0;
  NRF_SPIM0->EVENTS_END     = 0;
  NRF_SPIM0->INTENCLR       = 0xFFFFFFFF;
  // IRQ already disabled in TWIM0 section (shared IRQ)
  NRF_SPIM0->ENABLE = 0;

  NRF_SPIM1->TASKS_STOP     = 1;
  NRF_SPIM1->EVENTS_STOPPED = 0;
  NRF_SPIM1->EVENTS_ENDRX   = 0;
  NRF_SPIM1->EVENTS_END     = 0;
  NRF_SPIM1->INTENCLR       = 0xFFFFFFFF;
  // IRQ already disabled in TWIM1 section (shared IRQ)
  NRF_SPIM1->ENABLE = 0;

  NRF_SPIM2->TASKS_STOP     = 1;
  NRF_SPIM2->EVENTS_STOPPED = 0;
  NRF_SPIM2->EVENTS_ENDRX   = 0;
  NRF_SPIM2->EVENTS_END     = 0;
  NRF_SPIM2->INTENCLR       = 0xFFFFFFFF;
  NVIC_DisableIRQ(SPIM2_SPIS2_SPI2_IRQn);
  NVIC_ClearPendingIRQ(SPIM2_SPIS2_SPI2_IRQn);
  NRF_SPIM2->ENABLE = 0;

  NRF_SPIM3->TASKS_STOP     = 1;
  NRF_SPIM3->EVENTS_STOPPED = 0;
  NRF_SPIM3->EVENTS_ENDRX   = 0;
  NRF_SPIM3->EVENTS_END     = 0;
  NRF_SPIM3->INTENCLR       = 0xFFFFFFFF;
  NVIC_DisableIRQ(SPIM3_IRQn);
  NVIC_ClearPendingIRQ(SPIM3_IRQn);
  NRF_SPIM3->ENABLE = 0;

  NRF_SPIS0->INTENCLR = 0xFFFFFFFF;
  NRF_SPIS0->ENABLE   = 0;

  NRF_SPIS1->INTENCLR = 0xFFFFFFFF;
  NRF_SPIS1->ENABLE   = 0;

  NRF_SPIS2->INTENCLR = 0xFFFFFFFF;
  NRF_SPIS2->ENABLE   = 0;

  // Disable NFC
  NRF_NFCT->TASKS_DISABLE        = 1;
  NRF_NFCT->EVENTS_FIELDDETECTED = 0;
  NRF_NFCT->EVENTS_FIELDLOST     = 0;
  NRF_NFCT->INTENCLR             = 0xFFFFFFFF;
  NVIC_DisableIRQ(NFCT_IRQn);
  NVIC_ClearPendingIRQ(NFCT_IRQn);

  // Disable I2S (Audio interface)
  NRF_I2S->TASKS_STOP      = 1;
  NRF_I2S->EVENTS_RXPTRUPD = 0;
  NRF_I2S->EVENTS_STOPPED  = 0;
  NRF_I2S->EVENTS_TXPTRUPD = 0;
  NRF_I2S->INTENCLR        = 0xFFFFFFFF;
  NVIC_DisableIRQ(I2S_IRQn);
  NVIC_ClearPendingIRQ(I2S_IRQn);
  NRF_I2S->ENABLE = 0;

  // Disable PDM (Pulse Density Modulation interface)
  NRF_PDM->TASKS_STOP     = 1;
  NRF_PDM->EVENTS_STARTED = 0;
  NRF_PDM->EVENTS_STOPPED = 0;
  NRF_PDM->EVENTS_END     = 0;
  NRF_PDM->INTENCLR       = 0xFFFFFFFF;
  NVIC_DisableIRQ(PDM_IRQn);
  NVIC_ClearPendingIRQ(PDM_IRQn);
  NRF_PDM->ENABLE = 0;

  // Disable USBD (USB Device)
  NRF_USBD->INTENCLR = 0xFFFFFFFF;
  NVIC_DisableIRQ(USBD_IRQn);
  NVIC_ClearPendingIRQ(USBD_IRQn);
  NRF_POWER->TASKS_LOWPWR = 1;  // Go to low power mode
  NRF_USBD->ENABLE        = 0;

  // Disable COMP (Comparator)
  NRF_COMP->TASKS_STOP   = 1;
  NRF_COMP->EVENTS_READY = 0;
  NRF_COMP->EVENTS_DOWN  = 0;
  NRF_COMP->EVENTS_UP    = 0;
  NRF_COMP->EVENTS_CROSS = 0;
  NRF_COMP->INTENCLR     = 0xFFFFFFFF;
  NVIC_DisableIRQ(COMP_LPCOMP_IRQn);
  NVIC_ClearPendingIRQ(COMP_LPCOMP_IRQn);
  NRF_COMP->ENABLE = 0;

  // Disable LPCOMP (Low Power Comparator)
  NRF_LPCOMP->TASKS_STOP   = 1;
  NRF_LPCOMP->EVENTS_READY = 0;
  NRF_LPCOMP->EVENTS_DOWN  = 0;
  NRF_LPCOMP->EVENTS_UP    = 0;
  NRF_LPCOMP->EVENTS_CROSS = 0;
  NRF_LPCOMP->INTENCLR     = 0xFFFFFFFF;
  // IRQ already disabled in COMP section (shared IRQ)
  NRF_LPCOMP->ENABLE = 0;

  // Disable QDEC (Quadrature Decoder)
  NRF_QDEC->TASKS_STOP       = 1;
  NRF_QDEC->EVENTS_REPORTRDY = 0;
  NRF_QDEC->EVENTS_ACCOF     = 0;
  NRF_QDEC->EVENTS_DBLRDY    = 0;
  NRF_QDEC->EVENTS_STOPPED   = 0;
  NRF_QDEC->INTENCLR         = 0xFFFFFFFF;
  NVIC_DisableIRQ(QDEC_IRQn);
  NVIC_ClearPendingIRQ(QDEC_IRQn);
  NRF_QDEC->ENABLE = 0;

  // Disable Event Generator Units (EGU)
  NRF_EGU0->INTENCLR = 0xFFFFFFFF;
  NVIC_DisableIRQ(SWI0_EGU0_IRQn);
  NVIC_ClearPendingIRQ(SWI0_EGU0_IRQn);

  NRF_EGU1->INTENCLR = 0xFFFFFFFF;
  NVIC_DisableIRQ(SWI1_EGU1_IRQn);
  NVIC_ClearPendingIRQ(SWI1_EGU1_IRQn);

  NRF_EGU2->INTENCLR = 0xFFFFFFFF;
  NVIC_DisableIRQ(SWI2_EGU2_IRQn);
  NVIC_ClearPendingIRQ(SWI2_EGU2_IRQn);

  NRF_EGU3->INTENCLR = 0xFFFFFFFF;
  NVIC_DisableIRQ(SWI3_EGU3_IRQn);
  NVIC_ClearPendingIRQ(SWI3_EGU3_IRQn);

  NRF_EGU4->INTENCLR = 0xFFFFFFFF;
  NVIC_DisableIRQ(SWI4_EGU4_IRQn);
  NVIC_ClearPendingIRQ(SWI4_EGU4_IRQn);

  NRF_EGU5->INTENCLR = 0xFFFFFFFF;
  NVIC_DisableIRQ(SWI5_EGU5_IRQn);
  NVIC_ClearPendingIRQ(SWI5_EGU5_IRQn);

  // Disable Timers
  NRF_TIMER0->TASKS_STOP        = 1;
  NRF_TIMER0->TASKS_CLEAR       = 1;
  NRF_TIMER0->EVENTS_COMPARE[0] = 0;
  NRF_TIMER0->EVENTS_COMPARE[1] = 0;
  NRF_TIMER0->EVENTS_COMPARE[2] = 0;
  NRF_TIMER0->EVENTS_COMPARE[3] = 0;
  NRF_TIMER0->EVENTS_COMPARE[4] = 0;
  NRF_TIMER0->EVENTS_COMPARE[5] = 0;
  NRF_TIMER0->INTENCLR          = 0xFFFFFFFF;
  NVIC_DisableIRQ(TIMER0_IRQn);
  NVIC_ClearPendingIRQ(TIMER0_IRQn);
  NRF_TIMER0->SHORTS = 0;

  NRF_TIMER1->TASKS_STOP        = 1;
  NRF_TIMER1->TASKS_CLEAR       = 1;
  NRF_TIMER1->EVENTS_COMPARE[0] = 0;
  NRF_TIMER1->EVENTS_COMPARE[1] = 0;
  NRF_TIMER1->EVENTS_COMPARE[2] = 0;
  NRF_TIMER1->EVENTS_COMPARE[3] = 0;
  NRF_TIMER1->EVENTS_COMPARE[4] = 0;
  NRF_TIMER1->EVENTS_COMPARE[5] = 0;
  NRF_TIMER1->INTENCLR          = 0xFFFFFFFF;
  NVIC_DisableIRQ(TIMER1_IRQn);
  NVIC_ClearPendingIRQ(TIMER1_IRQn);
  NRF_TIMER1->SHORTS = 0;

  NRF_TIMER2->TASKS_STOP        = 1;
  NRF_TIMER2->TASKS_CLEAR       = 1;
  NRF_TIMER2->EVENTS_COMPARE[0] = 0;
  NRF_TIMER2->EVENTS_COMPARE[1] = 0;
  NRF_TIMER2->EVENTS_COMPARE[2] = 0;
  NRF_TIMER2->EVENTS_COMPARE[3] = 0;
  NRF_TIMER2->EVENTS_COMPARE[4] = 0;
  NRF_TIMER2->EVENTS_COMPARE[5] = 0;
  NRF_TIMER2->INTENCLR          = 0xFFFFFFFF;
  NVIC_DisableIRQ(TIMER2_IRQn);
  NVIC_ClearPendingIRQ(TIMER2_IRQn);
  NRF_TIMER2->SHORTS = 0;

  NRF_TIMER3->TASKS_STOP        = 1;
  NRF_TIMER3->TASKS_CLEAR       = 1;
  NRF_TIMER3->EVENTS_COMPARE[0] = 0;
  NRF_TIMER3->EVENTS_COMPARE[1] = 0;
  NRF_TIMER3->EVENTS_COMPARE[2] = 0;
  NRF_TIMER3->EVENTS_COMPARE[3] = 0;
  NRF_TIMER3->EVENTS_COMPARE[4] = 0;
  NRF_TIMER3->EVENTS_COMPARE[5] = 0;
  NRF_TIMER3->INTENCLR          = 0xFFFFFFFF;
  NVIC_DisableIRQ(TIMER3_IRQn);
  NVIC_ClearPendingIRQ(TIMER3_IRQn);
  NRF_TIMER3->SHORTS = 0;

  NRF_TIMER4->TASKS_STOP        = 1;
  NRF_TIMER4->TASKS_CLEAR       = 1;
  NRF_TIMER4->EVENTS_COMPARE[0] = 0;
  NRF_TIMER4->EVENTS_COMPARE[1] = 0;
  NRF_TIMER4->EVENTS_COMPARE[2] = 0;
  NRF_TIMER4->EVENTS_COMPARE[3] = 0;
  NRF_TIMER4->EVENTS_COMPARE[4] = 0;
  NRF_TIMER4->EVENTS_COMPARE[5] = 0;
  NRF_TIMER4->INTENCLR          = 0xFFFFFFFF;
  NVIC_DisableIRQ(TIMER4_IRQn);
  NVIC_ClearPendingIRQ(TIMER4_IRQn);
  NRF_TIMER4->SHORTS = 0;

  // Disable Real-Time Counters (RTC)
  NRF_RTC0->TASKS_STOP        = 1;
  NRF_RTC0->TASKS_CLEAR       = 1;
  NRF_RTC0->EVENTS_TICK       = 0;
  NRF_RTC0->EVENTS_OVRFLW     = 0;
  NRF_RTC0->EVENTS_COMPARE[0] = 0;
  NRF_RTC0->EVENTS_COMPARE[1] = 0;
  NRF_RTC0->EVENTS_COMPARE[2] = 0;
  NRF_RTC0->EVENTS_COMPARE[3] = 0;
  NRF_RTC0->EVTENCLR          = 0xFFFFFFFF;
  NRF_RTC0->INTENCLR          = 0xFFFFFFFF;
  NVIC_DisableIRQ(RTC0_IRQn);
  NVIC_ClearPendingIRQ(RTC0_IRQn);

  // Critical: Disable RTC1 which FreeRTOS uses for tick in this implementation
  NRF_RTC1->TASKS_STOP        = 1;
  NRF_RTC1->TASKS_CLEAR       = 1;
  NRF_RTC1->EVENTS_TICK       = 0;
  NRF_RTC1->EVENTS_OVRFLW     = 0;
  NRF_RTC1->EVENTS_COMPARE[0] = 0;
  NRF_RTC1->EVENTS_COMPARE[1] = 0;
  NRF_RTC1->EVENTS_COMPARE[2] = 0;
  NRF_RTC1->EVENTS_COMPARE[3] = 0;
  NRF_RTC1->EVTENCLR          = 0xFFFFFFFF;
  NRF_RTC1->INTENCLR          = 0xFFFFFFFF;
  NVIC_DisableIRQ(RTC1_IRQn);
  NVIC_ClearPendingIRQ(RTC1_IRQn);

  NRF_RTC2->TASKS_STOP        = 1;
  NRF_RTC2->TASKS_CLEAR       = 1;
  NRF_RTC2->EVENTS_TICK       = 0;
  NRF_RTC2->EVENTS_OVRFLW     = 0;
  NRF_RTC2->EVENTS_COMPARE[0] = 0;
  NRF_RTC2->EVENTS_COMPARE[1] = 0;
  NRF_RTC2->EVENTS_COMPARE[2] = 0;
  NRF_RTC2->EVENTS_COMPARE[3] = 0;
  NRF_RTC2->EVTENCLR          = 0xFFFFFFFF;
  NRF_RTC2->INTENCLR          = 0xFFFFFFFF;
  NVIC_DisableIRQ(RTC2_IRQn);
  NVIC_ClearPendingIRQ(RTC2_IRQn);

  // Disable WDT (Watchdog Timer)
  // Note: Watchdog can't be disabled once started, configure for maximum timeout
  NRF_WDT->CONFIG   = 0;  // No reset on timeout
  NRF_WDT->INTENCLR = 0xFFFFFFFF;
  NVIC_DisableIRQ(WDT_IRQn);
  NVIC_ClearPendingIRQ(WDT_IRQn);

  // Disable RNG (Random Number Generator)
  NRF_RNG->TASKS_STOP    = 1;
  NRF_RNG->EVENTS_VALRDY = 0;
  NRF_RNG->INTENCLR      = 0xFFFFFFFF;
  NVIC_DisableIRQ(RNG_IRQn);
  NVIC_ClearPendingIRQ(RNG_IRQn);

  // Disable TEMP (Temperature sensor)
  NRF_TEMP->TASKS_STOP     = 1;
  NRF_TEMP->EVENTS_DATARDY = 0;
  NRF_TEMP->INTENCLR       = 0xFFFFFFFF;
  NVIC_DisableIRQ(TEMP_IRQn);
  NVIC_ClearPendingIRQ(TEMP_IRQn);

  // Disable GPIOTE (GPIO Tasks and Events)
  NRF_GPIOTE->INTENCLR = 0xFFFFFFFF;

  // Clear all GPIOTE events
  for (int i = 0; i < 8; i++) {
    NRF_GPIOTE->EVENTS_IN[i] = 0;
    NRF_GPIOTE->CONFIG[i]    = 0;  // Disable all configurations
  }
  NRF_GPIOTE->EVENTS_PORT = 0;

  NVIC_DisableIRQ(GPIOTE_IRQn);
  NVIC_ClearPendingIRQ(GPIOTE_IRQn);

  // Disable PPI (Programmable Peripheral Interconnect)
  NRF_PPI->CHENCLR = 0xFFFFFFFF;  // Disable all channels

  // Clear all PPI channel assignments
  for (int i = 0; i < 16; i++) {
    NRF_PPI->CH[i].EEP = 0;
    NRF_PPI->CH[i].TEP = 0;
  }
  NRF_QSPI->TASKS_DEACTIVATE = 1;
  NRF_QSPI->EVENTS_READY     = 0;
  NRF_QSPI->INTENCLR         = 0xFFFFFFFF;
  NVIC_DisableIRQ(QSPI_IRQn);
  NVIC_ClearPendingIRQ(QSPI_IRQn);
  NRF_QSPI->ENABLE = 0;

  // Configure all GPIO pins to input with no pull to minimize power consumption
  // Skip pins 0 and 1 and 18.
  for (int i = 2; i < 32; i++) {
    if (i == 18) continue;
    NRF_P0->PIN_CNF[i] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                         (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) |
                         (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                         (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                         (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
  }

  // Configure all P1 pins as input with no pull
  for (int i = 0; i < 16; i++) {
    NRF_P1->PIN_CNF[i] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                         (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) |
                         (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos) |
                         (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                         (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
  }

  disableLEDPower();

  // Clear all pending interrupts in all NVIC banks
  for (int i = 0; i < 8; i++) {
    NVIC->ICER[i] = 0xFFFFFFFF;  // Disable all interrupts
    NVIC->ICPR[i] = 0xFFFFFFFF;  // Clear all pending interrupts
  }

  // Stop the clocks
  // First stop LFCLK needed by RTC1 (FreeRTOS tick)
  NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
  NRF_CLOCK->LFCLKSRC            = 0;
  NRF_CLOCK->TASKS_LFCLKSTOP     = 1;

  // Wait for LFCLK to stop by checking the LFCLKSTAT register
  while ((NRF_CLOCK->LFCLKSTAT & CLOCK_LFCLKSTAT_STATE_Msk) != 0);

  // Stop HFCLK
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTOP     = 1;

  // Disable power-hungry features
  NRF_POWER->DCDCEN = 1;  // Disable DC/DC converter
  NRF_POWER->POFCON = 0;  // Disable power failure comparator

  // Check if SoftDevice is enabled
  uint8_t sd_enabled = 0;
  err_code           = sd_softdevice_is_enabled(&sd_enabled);

  if ((err_code == NRF_SUCCESS) && sd_enabled) {


    // Power off the system
    sd_power_system_off();
  } else {
    // Turn off instruction cache if available
    NRF_NVMC->ICACHECNF &= ~NVMC_ICACHECNF_CACHEEN_Msk;

    // Direct system power off
    NRF_POWER->SYSTEMOFF = 1;
  }

  // Should never reach here
  while (1) {
    // This is never reached.
    enterSerialDfu();
    // Ensure no code runs if we somehow get here
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
