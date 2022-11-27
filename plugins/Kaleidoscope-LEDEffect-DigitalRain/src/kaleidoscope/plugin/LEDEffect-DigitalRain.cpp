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

#include <Kaleidoscope-LEDEffect-DigitalRain.h>
#include <stdlib.h>

namespace kaleidoscope {
namespace plugin {

void LEDDigitalRainEffect::TransientLEDMode::update() {
  static constexpr uint8_t rows = Runtime.device().matrix_rows;
  static constexpr uint8_t cols = Runtime.device().matrix_columns;

  uint8_t col;
  uint8_t row;

  // By how much intensity should each pixel decay,
  // based on how much time has passed since we last ran?
  uint8_t decayAmount = 0xff * (Runtime.millisAtCycleStart() - previous_timestamp_) / parent_->decay_ms_;

  // Decay intensities and possibly make new raindrops
  for (col = 0; col < cols; col++) {
    for (row = 0; row < rows; row++) {
      if (row == 0 && just_dropped_ && rand() < RAND_MAX / parent_->new_drop_probability_) {
        // This is the top row, pixels have just fallen,
        // and we've decided to make a new raindrop in this column
        map_[col][row] = 0xff;
      } else if (map_[col][row] > 0 && map_[col][row] < 0xff) {
        // Pixel is neither full intensity nor totally dark;
        // decay it
        if (map_[col][row] <= decayAmount) {
          map_[col][row] = 0;
        } else {
          map_[col][row] -= decayAmount;
        }
      }

      // Set the colour for this pixel
      ::LEDControl.setCrgbAt(KeyAddr(row, col), get_color_from_intensity_(map_[col][row]));
    }
  }

  // Drop the raindrops one row periodically
  if (Runtime.hasTimeExpired(drop_start_timestamp_, parent_->drop_ms_)) {
    // Remember for next time that this just happened
    just_dropped_ = true;

    // Reset the timestamp
    drop_start_timestamp_ = Runtime.millisAtCycleStart();

    // Remember for next tick that we just dropped
    just_dropped_ = true;

    for (row = rows - 1; row > 0; row--) {
      for (col = 0; col < cols; col++) {
        // If this pixel is on the bottom row and bright,
        // allow it to start decaying
        if (row == rows - 1 && map_[col][row] == 0xff) {
          --map_[col][row];
        }

        // Check if the pixel above is bright
        if (map_[col][row - 1] == 0xff) {
          // Allow old bright pixel to decay
          map_[col][row - 1]--;

          // Make this pixel bright
          map_[col][row] = 0xff;
        }
      }
    }
  } else {
    just_dropped_ = false;
  }

  // Update previous timestamp variable to now
  // so we can tell how much time has passed next time we run
  previous_timestamp_ = Runtime.millisAtCycleStart();
}

cRGB LEDDigitalRainEffect::TransientLEDMode::get_color_from_intensity_(uint8_t intensity) {
  uint8_t boost;

  // At high intensities start at light green
  // but drop off very quickly to full green
  if (intensity > parent_->tint_shade_ratio_) {
    boost = (uint8_t)((uint16_t)parent_->maximum_tint_ * (intensity - parent_->tint_shade_ratio_) / (0xff - parent_->tint_shade_ratio_));
    return get_color_from_components_(0xff, boost);
  }
  return get_color_from_components_((uint8_t)((uint16_t)0xff * intensity / parent_->tint_shade_ratio_), 0);
}

cRGB LEDDigitalRainEffect::TransientLEDMode::get_color_from_components_(uint8_t primary, uint8_t secondary) {
  switch (parent_->color_channel_) {
  case LEDDigitalRainEffect::ColorChannel::RED:
    return CRGB(primary, secondary, secondary);
  case LEDDigitalRainEffect::ColorChannel::GREEN:
    return CRGB(secondary, primary, secondary);
  case LEDDigitalRainEffect::ColorChannel::BLUE:
    return CRGB(secondary, secondary, primary);
  default:
    return CRGB(0, 0, 0);
  }
}

}  // namespace plugin
}  // namespace kaleidoscope

kaleidoscope::plugin::LEDDigitalRainEffect LEDDigitalRainEffect;
