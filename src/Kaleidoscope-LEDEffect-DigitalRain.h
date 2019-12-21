#pragma once

#include "kaleidoscope/Runtime.h"
#include <Kaleidoscope-LEDControl.h>

namespace kaleidoscope { namespace plugin {
	class LEDDigitalRainEffect : public LEDMode {
		public:
			LEDDigitalRainEffect(void) {}

			/**
			 * Number of milliseconds it takes for a drop to decay
			 * from full intensity to zero.
			 */
			static uint16_t DECAY_MS;

			/**
			 * Number of milliseconds before raindrops fall.
			 */
			static uint16_t DROP_MS;

			/**
			 * Probability divisor for new drops.
			 *
			 * The inverse of this number (1/n) gives the probability
			 * each time DROP_MS frames have elapsed
			 * that a new raindrop will appear in a given column.
			 */
			static uint8_t NEW_DROP_PROBABILITY;

			/**
			 * Intensity at which full green is used.
			 *
			 * When a pixel is at this intensity,
			 * full green with no red nor blue is used.
			 * Above this intensity red and blue are added
			 * to lighten the colour.
			 */
			static uint8_t PURE_GREEN_INTENSITY;

			/**
			 * Maximum brightness boost.
			 *
			 * The maximum brightness boost added to pure green
			 * when a pixel is at full intensity.
			 *
			 * 0xff would give full white for full-intensity pixels,
			 * while zero would give pure green for all intensities
			 * above PURE_GREEN_INTENSITY.
			 */
			static uint8_t MAXIMUM_BRIGHTNESS_BOOST;

			/**
			 * Colour channel.
			 *
			 * This can be changed to set the colour
			 * to red (0), green (1), or blue (2).
			 */
			static uint8_t COLOR_CHANNEL;

		protected:
			virtual void update(void) final;

		private:
			/**
			 * Timer at start of drop cycle.
			 */
			uint32_t dropStartTimestamp = 0;

			/**
			 * Timer at previous tick.
			 */
			uint32_t previousTimestamp = 0;

			/**
			 * Keep track of whether raindrops fell on the last
			 * tick.
			 */
			bool justDropped = false;

			/**
			 * Number of columns
			 */
			uint8_t COLS = Runtime.device().matrix_columns;

			/**
			 * Number of rows
			 */
			uint8_t ROWS = Runtime.device().matrix_rows;

			/**
			 * Map of intensities for each pixel.
			 *
			 * Intensity 0xff stays for a full DROP_MS,
			 * then linearly drops off to zero with time.
			 */
			uint8_t map[Runtime.device().matrix_columns][Runtime.device().matrix_rows] = {{0}};

			/**
			 * Get colour from intensity.
			 */
			cRGB getColorFromIntensity(uint8_t intensity);

			/**
			 * Get colour from primary and secondary components.
			 */
			cRGB getColorFromComponents(uint8_t primary, uint8_t secondary);
	};
}}

extern kaleidoscope::plugin::LEDDigitalRainEffect LEDDigitalRainEffect;
