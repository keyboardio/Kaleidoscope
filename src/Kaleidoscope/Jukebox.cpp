/* -*- mode: c++ -*-
 * Kaleidoscope-LEDEffects -- An assorted collection of LED effects for Kaleidoscope
 * Copyright (C) 2016  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope-LEDEffects.h>

KaleidoscopePlugins::LEDEffects::TriColor JukeboxEffect((cRGB) {
    0xc8, 0xe8, 0xee
} /* TM */,
(cRGB) {
    0xc3, 0xee, 0x8c
} /* VCO */,
(cRGB) {
    0x21, 0x38, 0xd7
} /* RN */);

KaleidoscopePlugins::LEDEffects::TriColor JukeboxAlternateEffect((cRGB) {
    0xc8, 0xe8, 0xee
} /* TM */,
(cRGB) {
    0x21, 0x38, 0xd7
} /* RN */,
(cRGB) {
    0xc3, 0xee, 0x8c
} /* VCO */);
