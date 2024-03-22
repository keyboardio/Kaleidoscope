/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2017-2020  Bart Nagel
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
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

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-LEDControl.h>

namespace kaleidoscope {
namespace plugin {

class LEDDigitalRainEffect : public Plugin,
                             public LEDModeInterface,
                             public AccessTransientLEDMode {
 public:
  LEDDigitalRainEffect() { led_mode_name_ = "DigitalRain"; }
  explicit LEDDigitalRainEffect(const char *led_mode_name) { led_mode_name_ = led_mode_name; }

  /**
   * Color channel enum.
   */
  enum class ColorChannel { RED,
                            GREEN,
                            BLUE };

  /**
   * Get or set the number milliseconds it takes for a drop to decay
   * from full intensity to zero.
   */
  uint16_t getDecayMs() {
    return decay_ms_;
  }
  void setDecayMs(uint16_t decayMs) {
    decay_ms_ = decayMs;
  }

  /**
   * Get or set the number of milliseconds before raindrops fall.
   */
  uint16_t getDropMs() {
    return drop_ms_;
  }
  void setDropMs(uint16_t dropMs) {
    drop_ms_ = dropMs;
  }

  /**
   * Get or set the probability divisor for new drops.
   *
   * The inverse of this number (1/n) gives the probability
   * each time raindrops fall one row (see getDropMs/setDropMs) has elapsed
   * that a new raindrop will appear in a given column.
   */
  uint8_t getNewDropProbability() {
    return new_drop_probability_;
  }
  void setNewDropProbability(uint8_t newDropProbability) {
    new_drop_probability_ = newDropProbability;
  }

  /**
   * Get or set the intensity at which we display neither a tint nor shade.
   *
   * Intensity falls from 255 to 0.
   *
   * When a pixel's intensity level reaches this value,
   * full green with no red nor blue is used.
   * Above this intensity red and blue are added
   * to tint the color towards white,
   * and below this intensity the green channel is dimmed.
   */
  uint8_t getTintShadeRatio() {
    return tint_shade_ratio_;
  }
  void setTintShadeRatio(uint8_t tintShadeRatio) {
    tint_shade_ratio_ = tintShadeRatio;
  }

  /**
   * Get or set the maximum tint.
   *
   * This is the maximum tint of a pixel,
   * used when it is at full intensity.
   * More specifically, this is the maximum value
   * which will be in the red and blue channels.
   *
   * As such, 0xff would give full white for full-intensity pixels,
   * while zero would give pure green for all intensities
   * at and above the intensity described by the tint/shade ratio.
   */
  uint8_t getMaximumTint() {
    return maximum_tint_;
  }
  void setMaximumTint(uint8_t maximumTint) {
    maximum_tint_ = maximumTint;
  }

  /**
   * Get or set the color channel.
   *
   * This can be changed to set the color
   * to red, green, or blue.
   */
  ColorChannel getColorChannel() {
    return color_channel_;
  }
  void setColorChannel(ColorChannel colorChannel) {
    color_channel_ = colorChannel;
  }

  class TransientLEDMode : public LEDMode {
   public:
    explicit TransientLEDMode(const LEDDigitalRainEffect *parent)
      : parent_(parent) {}

   protected:
    void update() final;

   private:
    /**
     * Pointer to parent class, which houses configuration.
     */
    const LEDDigitalRainEffect *parent_;

    /**
     * Timer at start of drop cycle.
     */
    uint32_t drop_start_timestamp_ = 0;

    /**
     * Timer at previous tick.
     */
    uint32_t previous_timestamp_ = 0;

    /**
     * Keep track of whether raindrops fell on the last
     * tick.
     */
    bool just_dropped_ = false;

    /**
     * Map of intensities for each pixel.
     *
     * Intensity 0xff stays for a full drop_ms_,
     * then linearly drops off to zero with time.
     */
    uint8_t map_[Runtime.device().matrix_columns][Runtime.device().matrix_rows] = {{0}};

    /**
     * Get color from intensity.
     */
    cRGB get_color_from_intensity_(uint8_t intensity);

    /**
     * Get color from primary and secondary components.
     */
    cRGB get_color_from_components_(uint8_t primary, uint8_t secondary);

    friend class LEDDigitalRainEffect;
  };

 private:
  /**
   * Values which the public accessors get and set,
   * and their defaults.
   */
  uint16_t decay_ms_            = 2000;
  uint16_t drop_ms_             = 180;
  uint8_t new_drop_probability_ = 18;
  uint8_t tint_shade_ratio_     = 0xd0;
  uint8_t maximum_tint_         = 0xc0;
  ColorChannel color_channel_   = ColorChannel::GREEN;
};

}  // namespace plugin
}  // namespace kaleidoscope

extern kaleidoscope::plugin::LEDDigitalRainEffect LEDDigitalRainEffect;
