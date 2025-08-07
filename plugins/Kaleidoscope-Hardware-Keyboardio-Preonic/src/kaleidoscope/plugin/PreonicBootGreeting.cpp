/* Kaleidoscope-Hardware-Keyboardio-Preonic -- Keyboardio Preonic hardware support for Kaleidoscope
 * Copyright 2016-2025 Keyboard.io, inc.
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

#include "kaleidoscope/plugin/PreonicBootGreeting.h"

#include <stdint.h>  // for uint16_t, uint8_t

#include "kaleidoscope/Runtime.h"                     // for Runtime, Runtime_
#include "kaleidoscope/device/device.h"               // for cRGB
#include "kaleidoscope/event_handler_result.h"        // for EventHandlerResult, EventHandlerResult::OK
#include "kaleidoscope/plugin/LEDControl.h"           // for LEDControl
#include "kaleidoscope/plugin/LEDControl/LEDUtils.h"  // for hsvToRgb
#include "kaleidoscope/driver/color/GammaCorrection.h"
#include "kaleidoscope/plugin/LEDEffect-Rainbow.h"  // for LEDRainbowWaveEffect

namespace kaleidoscope {
namespace plugin {

// Apply gamma correction to a color component
static uint8_t gammaCorrect(uint8_t value) {
  return pgm_read_byte(&kaleidoscope::driver::color::gamma_correction[value]);
}

// Apply gamma correction to an RGB color
static cRGB gammaCorrectRGB(cRGB color) {
  return CRGB(gammaCorrect(color.r), gammaCorrect(color.g), gammaCorrect(color.b));
}

bool PreonicBootGreetingEffect::done_          = false;
uint16_t PreonicBootGreetingEffect::start_time = 0;
uint16_t PreonicBootGreetingEffect::timeout    = FADE_IN_DURATION + FLAPPING_DURATION + FADE_OUT_DURATION;

// Create a rainbow wave effect instance to use directly
static LEDRainbowWaveEffect rainbow_effect;
static LEDRainbowWaveEffect::TransientLEDMode *rainbow_mode = nullptr;

EventHandlerResult PreonicBootGreetingEffect::onSetup() {
  // Initialize the rainbow effect with 3x speed and 20% brighter
  rainbow_effect.update_delay(33);  // 3x faster (100/3 ≈ 33)
  rainbow_effect.brightness(154);   // 20% brighter (128 * 1.2 ≈ 154)

  // Create the transient mode instance
  rainbow_mode = new LEDRainbowWaveEffect::TransientLEDMode(&rainbow_effect);

  return EventHandlerResult::OK;
}

EventHandlerResult PreonicBootGreetingEffect::afterEachCycle() {
  if (!Runtime.has_leds)
    return EventHandlerResult::OK;

  // If already done, bail
  if (done_) {
    return EventHandlerResult::OK;
  }

  // Only run for 'timeout' milliseconds
  if (Runtime.hasTimeExpired(start_time, timeout)) {
    done_ = true;
    // Turn off all LEDs when done
    ::LEDControl.setCrgbAt(TOP_LEFT_LED, CRGB(0, 0, 0));
    ::LEDControl.setCrgbAt(TOP_RIGHT_LED, CRGB(0, 0, 0));
    ::LEDControl.setCrgbAt(BOTTOM_LEFT_LED, CRGB(0, 0, 0));
    ::LEDControl.setCrgbAt(BOTTOM_RIGHT_LED, CRGB(0, 0, 0));
    ::LEDControl.syncLeds();
    return EventHandlerResult::OK;
  }

  uint16_t elapsed = Runtime.millisAtCycleStart() - start_time;

  // Calculate overall brightness fade
  uint8_t brightness_scale = 255;

  if (elapsed < FADE_IN_DURATION) {
    // Fade in phase
    brightness_scale = (elapsed * 255U) / FADE_IN_DURATION;
  } else if (elapsed < FADE_IN_DURATION + FLAPPING_DURATION) {
    // Full brightness phase
    brightness_scale = 255U;
  } else {
    // Fade out phase
    uint16_t fade_elapsed = elapsed - FADE_IN_DURATION - FLAPPING_DURATION;
    if (fade_elapsed >= FADE_OUT_DURATION) {
      brightness_scale = 0;
    } else {
      brightness_scale = 255U - ((fade_elapsed * 255U) / FADE_OUT_DURATION);
    }
  }

  // Set the rainbow effect brightness based on our fade (20% brighter)
  rainbow_effect.brightness((brightness_scale * 154U) / 255U);  // Scale to 20% brighter max brightness

  // Let the rainbow wave effect do all the color calculations
  if (rainbow_mode) {
    rainbow_mode->update();
  }

  // Now get just our 4 LEDs and set them manually for the butterfly pattern
  // Top and bottom pairs get different colors from the rainbow
  cRGB top_left    = ::LEDControl.getCrgbAt(TOP_LEFT_LED);
  cRGB bottom_left = ::LEDControl.getCrgbAt(BOTTOM_LEFT_LED);

  // Set both sides to match - butterfly wings are symmetric
  ::LEDControl.setCrgbAt(TOP_LEFT_LED, top_left);
  ::LEDControl.setCrgbAt(TOP_RIGHT_LED, top_left);
  ::LEDControl.setCrgbAt(BOTTOM_LEFT_LED, bottom_left);
  ::LEDControl.setCrgbAt(BOTTOM_RIGHT_LED, bottom_left);

  /* BUTTERFLY EFFECT ON HOLD - keeping for later
  uint16_t brightness = 0;
  
  if (elapsed < FADE_IN_DURATION) {
    // Fade in phase: gradually increase brightness
    brightness = (elapsed * 255U) / FADE_IN_DURATION;
  } else if (elapsed < FADE_IN_DURATION + FLAPPING_DURATION) {
    // Flapping phase: wings fade from bright to dim orange
    uint16_t flap_elapsed = elapsed - FADE_IN_DURATION;
    uint16_t cycle_pos = flap_elapsed % FLAP_CYCLE_MS;
    
    // Create asymmetric wave like real butterfly wings
    // Quick upstroke (30% of cycle), slower downstroke (70% of cycle)
    uint16_t triangle_val;
    uint16_t upstroke_duration = (FLAP_CYCLE_MS * 3U) / 10U;  // 30% of cycle
    if (cycle_pos < upstroke_duration) {
      // Quick upstroke: 0 to 255 (dim to bright)
      triangle_val = 255U - ((cycle_pos * 255U) / upstroke_duration);
    } else {
      // Slower downstroke: 255 to 0 (bright to dim)
      uint16_t downstroke_pos = cycle_pos - upstroke_duration;
      uint16_t downstroke_duration = FLAP_CYCLE_MS - upstroke_duration;
      triangle_val = (downstroke_pos * 255U) / downstroke_duration;
    }
    
    brightness = 255U - (triangle_val / 2U);  // Range from 255 to 128 (bright to dim)
  } else {
    // Fade out phase: gradually decrease brightness
    uint16_t fade_elapsed = elapsed - FADE_IN_DURATION - FLAPPING_DURATION;
    if (fade_elapsed >= FADE_OUT_DURATION) {
      brightness = 0;
    } else {
      brightness = 255U - ((fade_elapsed * 255U) / FADE_OUT_DURATION);
    }
  }
  
  // Apply brightness to orange colors - keep orange ratio intact
  uint16_t base_red   = (ORANGE_RED * brightness) / 255U;
  uint16_t base_green = (ORANGE_GREEN * brightness) / 255U;
  uint16_t base_blue  = (ORANGE_BLUE * brightness) / 255U;
  
  // Top wings stay at full calculated brightness (front wings)
  uint16_t top_red = base_red;
  uint16_t top_green = base_green;
  uint16_t top_blue = base_blue;
  
  // Bottom wings are 50% dimmer and fade more dramatically for depth perception
  uint16_t bottom_brightness = (brightness * brightness) / 255U;  // Use brightness^2
  bottom_brightness = (bottom_brightness * 128U) / 255U;  // Then make 50% dimmer
  uint16_t bottom_red = (ORANGE_RED * bottom_brightness) / 255U;
  uint16_t bottom_green = (ORANGE_GREEN * bottom_brightness) / 255U;
  uint16_t bottom_blue = (ORANGE_BLUE * bottom_brightness) / 255U;
  
  cRGB top_color = CRGB((uint8_t)top_red, (uint8_t)top_green, (uint8_t)top_blue);
  cRGB bottom_color = CRGB((uint8_t)bottom_red, (uint8_t)bottom_green, (uint8_t)bottom_blue);

  // Set LED colors
  ::LEDControl.setCrgbAt(TOP_LEFT_LED, top_color);
  ::LEDControl.setCrgbAt(TOP_RIGHT_LED, top_color);
  ::LEDControl.setCrgbAt(BOTTOM_LEFT_LED, bottom_color);
  ::LEDControl.setCrgbAt(BOTTOM_RIGHT_LED, bottom_color);
  */

  return EventHandlerResult::OK;
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::PreonicBootGreetingEffect PreonicBootGreetingEffect;