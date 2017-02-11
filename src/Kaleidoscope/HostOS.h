/* -*- mode: c++ -*-
 * Kaleidoscope-HostOS -- Host OS detection and tracking for Kaleidoscope
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
  namespace HostOS {
    typedef enum {
      LINUX,
      OSX,
      WINDOWS,
      OTHER,

      AUTO = 0xff,
    } Type;

    class Base : public KaleidoscopePlugin {
    public:
      virtual void begin (void) final;

      Type os (void);
      void os (Type osType);

    protected:
      virtual void autoDetect (void) = 0;
      Type osType;

    private:
      bool isConfigured = false;
    };
  };
};
