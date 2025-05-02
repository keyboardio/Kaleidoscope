/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2019-2025 Keyboard.io, inc.
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

/* MCU driver for nRF52840, which uses the Adafruit TinyUSB library */

#pragma once

#include <Arduino.h>  // NVIC_Reset
#include "Adafruit_TinyUSB.h"
#include "kaleidoscope/driver/mcu/Base.h"  // for Base, BaseProps
#include <Wire.h>
namespace kaleidoscope {
namespace driver {
namespace mcu {

struct nRF52840Props : public kaleidoscope::driver::mcu::BaseProps {
};

#ifndef KALEIDOSCOPE_VIRTUAL_BUILD
template<typename _Props>
class nRF52840 : public kaleidoscope::driver::mcu::Base<_Props> {
 public:
  void detachFromHost() {
    TinyUSBDevice.detach();
  }
  void attachToHost() {
    TinyUSBDevice.attach();
  }
  bool USBConfigured() {
    /* "mounted" is how TinyUSB spells "configured" */
    return TinyUSBDevice.mounted();
  }
  bool pollUSBReset() {
    return false;
  }
  void setUSBResetHook(void (*hook)()) {
    (void)hook;
  }

  void setup() {
  }

  /**
   * @brief Structure to track timer states for sleep/wake
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

  /**
   * @brief Structure to store TWI state during sleep
   */
  struct TWIState {
    uint32_t frequency;
    uint32_t pin_scl;
    uint32_t pin_sda;
    bool enabled;
  };

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
   * @brief Restore previously disabled timers
   * @details Restores TIMER2, TIMER3 and TIMER4 that we explicitly disabled
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
    // Usually we want to avoid disabling RTC0 as it's often used by system
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


 private:
  static TimerState timer_state_;
  static TWIState twi_state_;
};

template<typename _Props>
typename nRF52840<_Props>::TimerState nRF52840<_Props>::timer_state_;

template<typename _Props>
typename nRF52840<_Props>::TWIState nRF52840<_Props>::twi_state_;

#else
template<typename _Props>
class nRF52840 : public kaleidoscope::driver::mcu::Base<_Props> {};
#endif  // ifndef KALEIDOSCOPE_VIRTUAL_BUILD

}  // namespace mcu
}  // namespace driver
}  // namespace kaleidoscope
