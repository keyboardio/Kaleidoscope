#pragma once

#include "Kaleidoscope-LEDControl.h"
#include "LEDUtils.h"

namespace kaleidoscope {
	class LEDDigitalRainEffect : public LEDMode {
		public:
			LEDDigitalRainEffect(void) {}

		protected:
			void update(void) final;

		private:
			/**
			 * Timer position for raindrops falling.
			 *
			 * This ticks upwards linearly and resets after DROP_TICKS.
			 */
			uint8_t drop = 0;

			/**
			 * Number of ticks before raindrops fall.
			 */
			static const uint8_t DROP_TICKS = 28;
			
			/**
			 * Probability divisor for new drops.
			 *
			 * The inverse of this number (1/n) gives the probability
			 * each time DROP_TICKS frames have elapsed
			 * that a new raindrop will appear in a given column.
			 */
			static const uint8_t NEW_DROP_PROBABILITY = 18;

			/**
			 * Map of intensities for each pixel.
			 *
			 * Intensity 0xff stays for a full cycle of DROP_TICKS,
			 * then linearly drops off to zero with time.
			 */
			uint8_t map[COLS][ROWS] = {{0}};

			/**
			 * Intensity at which full green is used.
			 *
			 * When a pixel is at this intensity,
			 * full green with no red nor blue is used.
			 * Above this intensity red and blue are added
			 * to lighten the colour.
			 */
			static const uint8_t FULL_GREEN_INTENSITY = 0xd0;

			/**
			 * Get colour from intensity.
			 */
			cRGB getColor(uint8_t intensity);
	};
}

extern kaleidoscope::LEDDigitalRainEffect LEDDigitalRainEffect;
