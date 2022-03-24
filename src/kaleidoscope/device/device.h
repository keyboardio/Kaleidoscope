/* -*- mode: c++ -*-
 * Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2022  Keyboard.io, Inc.
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

/** @file kaleidoscope/device/device.h
 *
 * Provider of the following types:
 *  - `kaleidoscope::Device`
 *  - `kaleidoscope::Device::KeyAddr`
 *  - `kaleidoscope::DeviceProps`
 *  - `kaleidoscope::device::HID`
 *  - `kaleidoscope::device::Storage`
 *  - `kaleidoscope::device::StorageProps`
 */

// This file includes whatever hardware plugin is specified during the build.
// Depending on which device is targeted, you can find the sources in one of the
// following files under the `plugins` directory:
//
// `Kaleidoscope-Hardware-<make>-<model>/src/kaleidoscope/device/<make>/<model>.h`
//
// Each hardware device header defines a types `kaleidoscope::Device` and
// `kaleidoscope::DeviceProps` by using the `EXPORT_DEVICE()` preprocessor
// macro, which then becomes available to client code by means of this header.
// Client code should not include specific hardware device headers directly.
//
// `KALEIDOSCOPE_HARDWARE_H` gets defined during the build process like this:
// The fully-qualified board name (fqbn) is read from the `sketch.json` file (in
// the same directory as the sketch's *.ino file), and that tells Arduino which
// core to use and which definition to read from `boards.txt`, which adds
// compiler flags that set `KALEIDOSCOPE_HARDWARE_H` to the appropriate header
// for the hardware plugin.
//
// For a description of the API for the `kaleidoscope::Device` class, see the
// base device class definition in the following file:
//
// `kaleidoscope/device/Base.h`

#ifdef KALEIDOSCOPE_VIRTUAL_BUILD
#include "kaleidoscope/device/virtual/Virtual.h"  // IWYU pragma: export
#else
#include KALEIDOSCOPE_HARDWARE_H
#endif
