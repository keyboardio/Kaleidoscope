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
			// Timer position for raindrops falling
			uint8_t drop = 0;

			// How many ticks until the raindrops fall one row
			static const uint8_t DROP_TICKS = 28;
			
			// Probability divisor (1/n) that a new raindrop will appear when
			// raindrop fall counter resets
			static const uint8_t NEW_DROP_PROBABILITY = 18;

			// Intensity at which full green kicks in
			static const uint8_t FULL_GREEN_INTENSITY = 0xd0;

			// Map of intensities for each pixel
			uint8_t map[16][4] = {{0}};

			// Get colour from intensity
			cRGB getColor(uint8_t intensity);
	};
}

extern kaleidoscope::LEDDigitalRainEffect LEDDigitalRainEffect;
