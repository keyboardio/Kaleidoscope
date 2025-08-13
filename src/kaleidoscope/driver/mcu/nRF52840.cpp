// ABOUTME: TinyUSB callback implementation for nRF52840 USB event handling
// ABOUTME: Provides event-driven USB connection detection for Kaleidoscope

#ifdef ARDUINO_ARCH_NRF52

#include "Adafruit_TinyUSB.h"
#include "kaleidoscope/driver/mcu/nRF52840.h"
#include "kaleidoscope/hooks.h"
#include "kaleidoscope/host_connection_status.h"
#include <Arduino.h>

// Debug speaker pin - PIN_SPEAKER is 18 on Preonic
#define DEBUG_SPEAKER_PIN 18

/**
 * @brief Configure speaker pin using Arduino functions
 */
static void setupDebugSpeaker() {
  pinMode(DEBUG_SPEAKER_PIN, OUTPUT);
}

/**
 * @brief Play multiple debug tones with gaps using Arduino tone()
 * @param count Number of tones to play
 */
static void playDebugTones(uint8_t count) {
  setupDebugSpeaker();
  
  for (uint8_t i = 0; i < count; i++) {
    tone(DEBUG_SPEAKER_PIN, 1000, 200);  // 1kHz tone for 200ms
    delay(250);  // Wait for tone to finish plus gap
  }
}

// TinyUSB callbacks must be in global scope with C linkage
extern "C" {

/**
 * @brief Callback invoked when USB device is mounted (configured by host)
 * This indicates an active data connection is established
 */
void tud_mount_cb(void) {
  // Notify the nRF52840 driver that USB data connection is established
  kaleidoscope::driver::mcu::nRF52840<kaleidoscope::driver::mcu::nRF52840Props>::handleUSBMount();
  
  // Broadcast host connection status change event for USB device
  kaleidoscope::driver::mcu::nRF52840<kaleidoscope::driver::mcu::nRF52840Props>::broadcastUSBState(true);
}

/**
 * @brief Callback invoked when USB device is unmounted (disconnected from host)
 * This indicates the data connection has been lost
 */
void tud_umount_cb(void) {
  // Notify the nRF52840 driver that USB data connection is lost
  kaleidoscope::driver::mcu::nRF52840<kaleidoscope::driver::mcu::nRF52840Props>::handleUSBUnmount();
  
  // Broadcast host connection status change event for USB device
  kaleidoscope::driver::mcu::nRF52840<kaleidoscope::driver::mcu::nRF52840Props>::broadcastUSBState(false);
}

/**
 * @brief Callback invoked when USB device is suspended
 * @param remote_wakeup_en true if remote wakeup feature is enabled
 */
void tud_suspend_cb(bool remote_wakeup_en) {
  // USB is still connected but host has entered suspend state
  // Device should reduce power consumption
  (void)remote_wakeup_en; // Unused parameter
}

/**
 * @brief Callback invoked when USB device is resumed from suspend
 */
void tud_resume_cb(void) {
  // USB connection resumed from suspend state
  // Normal operation can continue
}

} // extern "C"

namespace kaleidoscope {
namespace driver {
namespace mcu {

template<typename _Props>
void nRF52840<_Props>::broadcastUSBState(bool connected) {
  if (connected) {
    // Data connection established
    kaleidoscope::Hooks::onHostConnectionStatusChanged(USB_DEVICE_ID, kaleidoscope::HostConnectionStatus::Connected);
  } else {
    // Data connection lost - check if we still have power
    if (instance_ && instance_->USBPowerDetected()) {
      // Still have USB power but no data - this is "connecting" state
      kaleidoscope::Hooks::onHostConnectionStatusChanged(USB_DEVICE_ID, kaleidoscope::HostConnectionStatus::Connecting);
    } else {
      // Fully disconnected
      kaleidoscope::Hooks::onHostConnectionStatusChanged(USB_DEVICE_ID, kaleidoscope::HostConnectionStatus::Disconnected);
    }
  }
}

// Explicit template instantiation  
template void nRF52840<nRF52840Props>::broadcastUSBState(bool);

} // namespace mcu
} // namespace driver
} // namespace kaleidoscope

#endif // ARDUINO_ARCH_NRF52