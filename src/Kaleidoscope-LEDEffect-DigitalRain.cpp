#include "Kaleidoscope-LEDEffect-DigitalRain.h"
#include <stdlib.h>

namespace kaleidoscope {
	void LEDDigitalRainEffect::update(void) {
		uint8_t col;
		uint8_t row;
		
		// Decay intensities
		for (col = 0; col < 16; col++) {
			for (row = 0; row < 4; row++) {
				if (row == 0 && drop == 0 && rand() < RAND_MAX / NEW_DROP_PROBABILITY) {
					map[col][row] = 0xff;
				} else if (map[col][row] > 0 && map[col][row] < 0xff) {
					map[col][row]--;
				}

				::LEDControl.setCrgbAt(row, col, getColor(map[col][row]));
			}
		}

		// Drop the raindrops one row periodically
		if (++drop >= DROP_TICKS) {
			// Reset the drop timer
			drop = 0;

			for (row = 3; row > 0; row--) {
				for (col = 0; col < 16; col++) {
					// If this pixel is on the bottom row and bright,
					// allow it to start decaying
					if (row == 3 && map[col][row] == 0xff) {
						map[col][row]--;
					}

					// Check if the pixel above is bright
					if (map[col][row - 1] == 0xff) {
						// Allow old bright pixel to decay
						map[col][row - 1]--;

						// Make this pixel bright
						map[col][row] = 0xff;
					}
				}
			}
		}
	}

	cRGB LEDDigitalRainEffect::getColor(uint8_t intensity) {
		uint8_t boost;

		// At high intensities start at full white
		// but drop off very quickly to full green
		if (intensity > FULL_GREEN_INTENSITY) {
			boost = (uint8_t) ((uint16_t) 0xc0 * (intensity - FULL_GREEN_INTENSITY) / (0xff - FULL_GREEN_INTENSITY));
			return {boost, 0xff, boost};
		}
		return {0, (uint8_t) ((uint16_t) 0xff * intensity / FULL_GREEN_INTENSITY), 0};
	}
}

kaleidoscope::LEDDigitalRainEffect LEDDigitalRainEffect;
