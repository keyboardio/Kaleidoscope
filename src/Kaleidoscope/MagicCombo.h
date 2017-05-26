/* -*- mode: c++ -*-
 * Kaleidoscope-MagicCombo -- Magic combo framework
 * Copyright (C) 2016, 2017  Gergely Nagy
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

#pragma once

#include <Kaleidoscope.h>

namespace KaleidoscopePlugins {
class MagicCombo : public KaleidoscopePlugin {
  public:
    typedef struct {
        uint32_t leftHand, rightHand;
    } dictionary_t;

    MagicCombo (void);

    virtual void begin (void) final;

    static void configure (const dictionary_t dictionary[]);
    static uint16_t minInterval;

  private:
    static const dictionary_t *dictionary;
    static uint32_t endTime;

    static void loopHook (bool postClear);
};
};

void magicComboActions (uint8_t comboIndex, uint32_t leftHand, uint32_t rightHand);

extern KaleidoscopePlugins::MagicCombo MagicCombo;
