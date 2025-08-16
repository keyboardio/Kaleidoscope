/* Kaleidoscope-LEDIndicators -- LED indicators for connection status and other events
 * Copyright 2024-2025 Keyboard.io, inc.
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

#include "kaleidoscope/plugin/LEDIndicators.h"
#include <Arduino.h>  // for delay()
#include "kaleidoscope/plugin/LEDControl.h"
#include "kaleidoscope/plugin/LEDControl/LEDUtils.h"

namespace kaleidoscope {
namespace plugin {

// Initialize static members

cRGB LEDIndicators::color_green  = {0, 255, 0};
cRGB LEDIndicators::color_red     = {255, 0, 0};
cRGB LEDIndicators::color_off     = {0, 0, 0};
cRGB LEDIndicators::color_blue    = {0, 0, 255};
cRGB LEDIndicators::color_orange  = {255, 165, 0};

uint8_t LEDIndicators::num_indicator_slots  = 5;  // Default to 5 slots
KeyAddr LEDIndicators::slot_leds[MAX_SLOTS] = {
  KeyAddr(0, 0),
  KeyAddr(0, 1),
  KeyAddr(0, 2),
  KeyAddr(0, 3),
  KeyAddr(0, 4),
};  // Default LED mapping

// Initialize static indicator array
kaleidoscope::plugin::LEDIndicators::Indicator LEDIndicators::indicators_[MAX_SLOTS] = {};


// Setup the plugin
EventHandlerResult LEDIndicators::onSetup() {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  for (uint8_t i = 0; i < num_indicator_slots; i++) {
    uint8_t led_index = Runtime.device().getLedIndex(slot_leds[i]);
    if (led_index >= Runtime.device().led_count) {
      // Invalid LED configuration - disable that slot
      slot_leds[i] = KeyAddr::none();
    }
  }

  // Initialize all indicators to inactive
  clearAllIndicators();


  return EventHandlerResult::OK;
}

// Show a temporary indicator
void LEDIndicators::showIndicator(KeyAddr led_addr,
                                  IndicatorEffect effect,
                                  cRGB color1,
                                  cRGB color2,
                                  uint16_t duration_ms,
                                  uint16_t effect_cycles) {
  if (!Runtime.has_leds) {
    return;
  }
  if (!led_addr.isValid()) {  // Valid LED
    return;
  }

  // Find an available indicator slot
  uint8_t slot = 0xFF;
  for (uint8_t i = 0; i < MAX_SLOTS; i++) {
    if (!indicators_[i].active || indicators_[i].key_addr == led_addr) {
      slot = i;
      break;
    }
  }

  if (slot == 0xFF) {
    return;
  }

  // Set up the indicator
  indicators_[slot].active        = true;
  indicators_[slot].key_addr      = led_addr;
  indicators_[slot].effect        = effect;
  indicators_[slot].color1        = color1;
  indicators_[slot].color2        = color2;
  indicators_[slot].start_time    = Runtime.millisAtCycleStart();
  indicators_[slot].duration_ms   = duration_ms;
  indicators_[slot].delay_ms      = 0;  // No delay by default
  indicators_[slot].effect_cycles = effect_cycles;
  indicators_[slot].current_cycle = 0;
  indicators_[slot].last_update   = Runtime.millisAtCycleStart();
  indicators_[slot].is_global     = false;  // Not a global indicator

  // Immediately update the indicator
  updateIndicator(slot);
}

// Show a global indicator that affects all configured slots
void LEDIndicators::showGlobalIndicator(IndicatorEffect effect,
                                        cRGB color1,
                                        cRGB color2,
                                        uint16_t duration_ms,
                                        uint16_t effect_cycles) {
  if (!Runtime.has_leds) {
    return;
  }

  // Find an available indicator slot
  uint8_t slot = 0xFF;
  for (uint8_t i = 0; i < MAX_SLOTS; i++) {
    if (!indicators_[i].active) {
      slot = i;
      break;
    }
  }

  if (slot == 0xFF) {
    return;
  }

  // Set up the global indicator
  indicators_[slot].active        = true;
  indicators_[slot].key_addr      = KeyAddr::none();  // Not tied to a specific LED
  indicators_[slot].effect        = effect;
  indicators_[slot].color1        = color1;
  indicators_[slot].color2        = color2;
  indicators_[slot].start_time    = Runtime.millisAtCycleStart();
  indicators_[slot].duration_ms   = duration_ms;
  indicators_[slot].delay_ms      = 0;  // No delay for global indicators
  indicators_[slot].effect_cycles = effect_cycles;
  indicators_[slot].current_cycle = 0;
  indicators_[slot].last_update   = Runtime.millisAtCycleStart();
  indicators_[slot].is_global     = true;  // This is a global indicator

  // Immediately update all LEDs
  for (uint8_t i = 0; i < num_indicator_slots; i++) {
    KeyAddr led_addr = getLEDForSlot(i);
    if (led_addr.isValid()) {
      cRGB color = computeCurrentColor(indicators_[slot]);
      ::LEDControl.setCrgbAt(led_addr, color);
    }
  }
}

// Show a temporary indicator with delay
void LEDIndicators::showIndicatorWithDelay(KeyAddr led_addr,
                                           IndicatorEffect effect,
                                           cRGB color1,
                                           cRGB color2,
                                           uint16_t duration_ms,
                                           uint16_t delay_ms,
                                           uint16_t effect_cycles) {
  if (!Runtime.has_leds) {
    return;
  }
  if (!led_addr.isValid()) {
    return;
  }

  // Find an available indicator slot - but DON'T check for matching key_addr
  // since we want delayed indicators to not block the LED until they activate
  uint8_t slot = 0xFF;
  for (uint8_t i = 0; i < MAX_SLOTS; i++) {
    if (!indicators_[i].active) {
      slot = i;
      break;
    }
  }

  if (slot == 0xFF) {
    return;
  }

  // Set up the indicator with delay
  indicators_[slot].active        = true;
  indicators_[slot].key_addr      = led_addr;
  indicators_[slot].effect        = effect;
  indicators_[slot].color1        = color1;
  indicators_[slot].color2        = color2;
  indicators_[slot].start_time    = Runtime.millisAtCycleStart();
  indicators_[slot].duration_ms   = duration_ms;
  indicators_[slot].delay_ms      = delay_ms;
  indicators_[slot].effect_cycles = effect_cycles;
  indicators_[slot].current_cycle = 0;
  indicators_[slot].last_update   = Runtime.millisAtCycleStart();
  indicators_[slot].is_global     = false;  // Not a global indicator
}

// Clear a specific indicator
void LEDIndicators::clearIndicator(KeyAddr key_addr) {
  if (!Runtime.has_leds)
    return;

  for (uint8_t i = 0; i < MAX_SLOTS; i++) {
    if (indicators_[i].key_addr == key_addr && indicators_[i].active) {
      indicators_[i].active = false;
      ::LEDControl.refreshAt(key_addr);
    }
  }
}

// Clear all indicators
void LEDIndicators::clearAllIndicators() {
  if (!Runtime.has_leds)
    return;

  for (uint8_t i = 0; i < MAX_SLOTS; i++) {
    indicators_[i].active = false;
  }
}

// Called before LEDs are synchronized
EventHandlerResult LEDIndicators::beforeSyncingLeds() {
  if (!Runtime.has_leds) {
    return EventHandlerResult::OK;
  }

  // Update and apply all active indicators
  for (uint8_t i = 0; i < MAX_SLOTS; i++) {
    if (indicators_[i].active) {
      uint32_t elapsed = Runtime.millisAtCycleStart() - indicators_[i].start_time;
      
      // Check if we're still in the delay period
      if (elapsed < indicators_[i].delay_ms) {
        // During delay, don't update the LED at all - let other indicators use it
        continue;
      }
      
      // Adjust elapsed time to account for delay
      uint32_t effective_elapsed = elapsed - indicators_[i].delay_ms;
      
      if (indicators_[i].duration_ms > 0 && effective_elapsed >= indicators_[i].duration_ms) {
        bool was_global = indicators_[i].is_global;
        KeyAddr expiring_led = indicators_[i].key_addr;
        indicators_[i].active = false;
        
        if (was_global) {
          // Global indicator expired - refresh all slots unless another indicator is using them
          for (uint8_t slot = 0; slot < num_indicator_slots; slot++) {
            KeyAddr led_addr = getLEDForSlot(slot);
            if (led_addr.isValid()) {
              bool another_indicator_waiting = false;
              for (uint8_t j = 0; j < MAX_SLOTS; j++) {
                if (j != i && indicators_[j].active && 
                    (indicators_[j].is_global || indicators_[j].key_addr == led_addr)) {
                  another_indicator_waiting = true;
                  break;
                }
              }
              if (!another_indicator_waiting) {
                ::LEDControl.refreshAt(led_addr);
              }
            }
          }
        } else {
          // Single LED indicator expired
          bool another_indicator_waiting = false;
          for (uint8_t j = 0; j < MAX_SLOTS; j++) {
            if (j != i && indicators_[j].active && 
                (indicators_[j].is_global || indicators_[j].key_addr == expiring_led)) {
              another_indicator_waiting = true;
              break;
            }
          }
          if (!another_indicator_waiting) {
            ::LEDControl.refreshAt(expiring_led);
          }
        }
      } else {
        updateIndicator(i);
      }
    }
  }

  return EventHandlerResult::OK;
}

// Update an indicator's state
void LEDIndicators::updateIndicator(uint8_t index) {
  if (index >= MAX_SLOTS)
    return;

  Indicator &indicator = indicators_[index];
  if (!indicator.active)
    return;

  // Update the LED(s) with the computed color
  cRGB color = computeCurrentColor(indicator);
  
  if (indicator.is_global) {
    // Global indicator - update all configured slots
    for (uint8_t i = 0; i < num_indicator_slots; i++) {
      KeyAddr led_addr = getLEDForSlot(i);
      if (led_addr.isValid()) {
        ::LEDControl.setCrgbAt(led_addr, color);
      }
    }
  } else {
    // Single LED indicator
    ::LEDControl.setCrgbAt(indicator.key_addr, color);
  }
}

// Compute the current color for an indicator based on its effect
cRGB LEDIndicators::computeCurrentColor(const Indicator &indicator) {
  uint32_t elapsed = Runtime.millisAtCycleStart() - indicator.start_time;
  
  // Account for delay - effects start after the delay period
  // Note: This should never be called during delay period due to checks in beforeSyncingLeds
  if (elapsed < indicator.delay_ms) {
    // This shouldn't happen, but if it does, return the current LED color unchanged
    // We should not be updating the LED during delay period
    return ::LEDControl.getCrgbAt(indicator.key_addr);
  }
  elapsed -= indicator.delay_ms;

  switch (indicator.effect) {
  case IndicatorEffect::Blink: {
    // Simple 500ms on/off
    bool on = (elapsed / 500) % 2 == 0;
    return on ? indicator.color1 : indicator.color2;
  }

  case IndicatorEffect::Breathe: {
    // Use 8-bit counter for brightness to avoid floating point
    uint8_t brightness = ((elapsed / 8) % 256);
    // Make it smooth by mirroring second half
    if (brightness >= 128) {
      brightness = 255 - brightness;
    }
    brightness = brightness * 2;  // Scale to full range

    return {
      static_cast<uint8_t>((indicator.color1.r * brightness) >> 8),
      static_cast<uint8_t>((indicator.color1.g * brightness) >> 8),
      static_cast<uint8_t>((indicator.color1.b * brightness) >> 8)};
  }

  case IndicatorEffect::Grow: {
    // Simple linear growth over 1 second
    uint16_t brightness = (elapsed > 1000) ? 255 : ((elapsed * 255) / 1000);
    return {
      static_cast<uint8_t>((indicator.color1.r * brightness) >> 8),
      static_cast<uint8_t>((indicator.color1.g * brightness) >> 8),
      static_cast<uint8_t>((indicator.color1.b * brightness) >> 8)};
  }

  case IndicatorEffect::Shrink: {
    // Simple linear decay over 1 second
    uint16_t brightness = (elapsed > 1000) ? 0 : (((1000 - elapsed) * 255) / 1000);
    return {
      static_cast<uint8_t>((indicator.color1.r * brightness) >> 8),
      static_cast<uint8_t>((indicator.color1.g * brightness) >> 8),
      static_cast<uint8_t>((indicator.color1.b * brightness) >> 8)};
  }

  case IndicatorEffect::Pulse: {
    // Each pulse cycle lasts 400ms (200ms on, 200ms off)
    uint32_t cycle_time = 400;
    uint32_t cycle_pos = elapsed % cycle_time;
    
    if (cycle_pos < 200) {
      // Pulse on for 200ms
      return indicator.color1;
    } else {
      // Off for 200ms
      return indicator.color2;
    }
  }

  default:
    return indicator.color1;
  }
}

// Check if any global indicators are currently active
bool LEDIndicators::hasActiveGlobalIndicator() {
  for (uint8_t i = 0; i < MAX_SLOTS; i++) {
    if (indicators_[i].active && indicators_[i].is_global) {
      // Check if it's actually running (not still in delay period)
      uint32_t elapsed = Runtime.millisAtCycleStart() - indicators_[i].start_time;
      if (elapsed >= indicators_[i].delay_ms) {
        return true;
      }
    }
  }
  return false;
}

// Get the remaining duration of any active global indicator
uint16_t LEDIndicators::getGlobalIndicatorRemainingTime() {
  uint16_t max_remaining = 0;
  for (uint8_t i = 0; i < MAX_SLOTS; i++) {
    if (indicators_[i].active && indicators_[i].is_global && indicators_[i].duration_ms > 0) {
      uint32_t elapsed = Runtime.millisAtCycleStart() - indicators_[i].start_time;
      if (elapsed < indicators_[i].delay_ms) {
        // Still in delay period
        uint16_t remaining = indicators_[i].duration_ms + (indicators_[i].delay_ms - elapsed);
        if (remaining > max_remaining) {
          max_remaining = remaining;
        }
      } else {
        // Already running
        uint32_t effective_elapsed = elapsed - indicators_[i].delay_ms;
        if (effective_elapsed < indicators_[i].duration_ms) {
          uint16_t remaining = indicators_[i].duration_ms - effective_elapsed;
          if (remaining > max_remaining) {
            max_remaining = remaining;
          }
        }
      }
    }
  }
  return max_remaining;
}

// Check if a specific LED has an active indicator
bool LEDIndicators::hasActiveIndicatorForLED(KeyAddr led_addr) {
  for (uint8_t i = 0; i < MAX_SLOTS; i++) {
    if (indicators_[i].active) {
      // Check if it's a global indicator (affects all LEDs)
      if (indicators_[i].is_global) {
        // Check if this LED is one of our configured slots
        for (uint8_t slot = 0; slot < num_indicator_slots; slot++) {
          if (getLEDForSlot(slot) == led_addr) {
            // This LED is affected by the global indicator
            // Check if the indicator is actually running (not in delay)
            uint32_t elapsed = Runtime.millisAtCycleStart() - indicators_[i].start_time;
            if (elapsed >= indicators_[i].delay_ms) {
              return true;
            }
          }
        }
      } else if (indicators_[i].key_addr == led_addr) {
        // Direct indicator for this LED
        // Check if it's actually running (not in delay)
        uint32_t elapsed = Runtime.millisAtCycleStart() - indicators_[i].start_time;
        if (elapsed >= indicators_[i].delay_ms) {
          return true;
        }
      }
    }
  }
  return false;
}

// Handle connection status changes
EventHandlerResult LEDIndicators::onHostConnectionStatusChanged(uint8_t device_id, HostConnectionStatus status) {
  // Handle USB connections (device_id 0) specially
  if (device_id == 0) {
    
    switch (status) {
    case HostConnectionStatus::Connecting:
      // USB power detected but no data connection - pulse orange 3 times on all LEDs
      clearAllIndicators();
      showGlobalIndicator(IndicatorEffect::Pulse,
                         color_orange,
                         color_off,
                         1200,  // 3 pulses at 400ms each
                         3);
      break;
    case HostConnectionStatus::Connected:
      // USB data connection established - fade up green on all LEDs
      clearAllIndicators();
      showGlobalIndicator(IndicatorEffect::Grow,
                         color_green,
                         color_off,
                         1000,  // 1 second duration
                         1);
      break;
    case HostConnectionStatus::Disconnected:
      // USB fully disconnected - show orange shrink effect on all LEDs
      clearAllIndicators();
      showGlobalIndicator(IndicatorEffect::Shrink,
                         color_orange,
                         color_off,
                         1000,  // 1 second duration
                         1);    // 1 cycle
      break;
    default:
      // Other USB states don't need special indicators
      break;
    }
    return EventHandlerResult::OK;
  }

  // For BLE devices (device_id 1-4), adjust to zero-indexed slot
  uint8_t slot = device_id - 1;
  
  // If there's a global indicator running, delay until it's done
  uint16_t delay_ms = getGlobalIndicatorRemainingTime();
  
  switch (status) {
  case HostConnectionStatus::Connecting:
    showIndicatorWithDelay(getLEDForSlot(slot),
                           IndicatorEffect::Blink,
                           color_blue,
                           color_off,
                           30000,  // 30 second duration
                           delay_ms,  // Only delay if USB indicators are active
                           10);    // 10 cycles
    break;
  case HostConnectionStatus::Connected:
    // Clear any existing indicators on this slot (like Advertising) before showing Connected
    clearIndicator(getLEDForSlot(slot));
    showIndicatorWithDelay(getLEDForSlot(slot),
                           IndicatorEffect::Grow,
                           color_blue,
                           color_off,
                           5000,  // 5 second duration
                           delay_ms,  // Only delay if USB indicators are active
                           1);    // 1 cycle
    break;
  case HostConnectionStatus::PairingFailed:
    showIndicator(getLEDForSlot(slot),
                  IndicatorEffect::Shrink,
                  color_red,
                  color_off,
                  10000,  // 10 second duration
                  1);     // 1 cycle
    break;
  case HostConnectionStatus::Disconnected:
    showIndicatorWithDelay(getLEDForSlot(slot),
                           IndicatorEffect::Shrink,
                           color_blue,
                           color_off,
                           5000,  // 5 second duration
                           delay_ms,  // Only delay if USB indicators are active
                           1);    // 1 cycle
    break;
  case HostConnectionStatus::PairingSuccess:
    showIndicator(getLEDForSlot(slot),
                  IndicatorEffect::Grow,
                  color_blue,
                  color_off,
                  10000,  // 10 second duration
                  1);     // 1 cycle
    break;
  case HostConnectionStatus::DeviceSelected:
    // Clear any previous indicators on this slot before showing DeviceSelected
    clearIndicator(getLEDForSlot(slot));
    showIndicatorWithDelay(getLEDForSlot(slot),
                           IndicatorEffect::Grow,
                           color_blue,
                           color_off,
                           2000,  // 2 second duration
                           delay_ms,  // Only delay if USB indicators are active
                           1);    // 1 cycle
    break;
  case HostConnectionStatus::DeviceUnselected:
    clearIndicator(getLEDForSlot(slot));
    break;
  case HostConnectionStatus::Advertising:
    // Clear any previous indicators on this slot before showing Advertising
    clearIndicator(getLEDForSlot(slot));
    showIndicatorWithDelay(getLEDForSlot(slot),
                           IndicatorEffect::Blink,
                           color_blue,
                           color_off,
                           30000,  // 30 second duration
                           delay_ms,  // Only delay if USB indicators are active
                           10);    // 10 cycles
    break;
  default:
    break;
  }
  return EventHandlerResult::OK;
}


// Handle power events
EventHandlerResult LEDIndicators::onPowerEvent(PowerEvent event, uint16_t voltage_mv) {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  // Use all indicator slots for battery events
  switch (event) {
  case PowerEvent::BatteryWarningOn:
    // Show red flashing on all LEDs for battery warning
    for (uint8_t i = 0; i < num_indicator_slots; i++) {
      KeyAddr led_addr = getLEDForSlot(i);
      if (led_addr.isValid()) {
        // Red solid for battery warning (all LEDs)
        // Shows for 1 second every 10 seconds, as defined in the requirements
        showIndicator(led_addr, IndicatorEffect::Solid, color_red, color_off, 1000, 0);
      }
    }
    break;

  case PowerEvent::BatteryWarningOff:
    // Clear any battery warning indicators
    for (uint8_t i = 0; i < num_indicator_slots; i++) {
      KeyAddr led_addr = getLEDForSlot(i);
      if (led_addr.isValid()) {
        clearIndicator(led_addr);
      }
    }
    break;

  case PowerEvent::BatteryShutdown:
    // Show solid red on all LEDs for shutdown (will stay on for 10 seconds before shutdown)
    for (uint8_t i = 0; i < num_indicator_slots; i++) {
      KeyAddr led_addr = getLEDForSlot(i);
      if (led_addr.isValid()) {
        // Solid red for battery critical shutdown
        showIndicator(led_addr, IndicatorEffect::Solid, color_red, color_off, 10000, 0);
      }
    }
    break;

  case PowerEvent::PowerSourceUSB:
  case PowerEvent::PowerSourceBattery:
    // Power source changes don't need visual indicators
    break;
  }

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LEDIndicators LEDIndicators;
