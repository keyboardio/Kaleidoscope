/* -*- mode: c++ -*-
 * Akela -- Animated Keyboardio Extension Library for Anything
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

#include <Akela-Core.h>

#define AKELA_MAGICCOMBO_TIMEOUT DEFAULT_TIMEOUT

namespace Akela {
  class MagicCombo : public KeyboardioPlugin {
  public:
    typedef struct {
      uint32_t leftHand, rightHand;
    } dictionary_t;

    MagicCombo (void);

    virtual void begin (void) final;

    static void configure (const dictionary_t dictionary[], uint8_t timeout);
    static void configure (const dictionary_t dictionary[]) { configure (dictionary, AKELA_MAGICCOMBO_TIMEOUT); };

  private:
    static const dictionary_t *dictionary;
    static uint8_t timeOut;
    static uint8_t timer;

    static void loopHook (bool postClear);
  };
};

void magicComboActions (uint8_t comboIndex, uint32_t leftHand, uint32_t rightHand);

extern Akela::MagicCombo MagicCombo;
