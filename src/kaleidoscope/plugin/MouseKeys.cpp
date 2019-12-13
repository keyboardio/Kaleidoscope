/* Kaleidoscope-MouseKeys - Mouse keys for Kaleidoscope.
 * Copyright (C) 2017-2018  Keyboard.io, Inc.
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

#include <Arduino.h>

#include "kaleidoscope/Runtime.h"
#include "Kaleidoscope-MouseKeys.h"
#include "kaleidoscope/keyswitch_state.h"

namespace kaleidoscope {
namespace plugin {

uint8_t MouseKeys_::mouseMoveIntent;

uint8_t MouseKeys_::speed = 1;
uint16_t MouseKeys_::speedDelay = 1;

uint8_t MouseKeys_::accelSpeed = 1;
uint16_t MouseKeys_::accelDelay = 64;

uint8_t MouseKeys_::wheelSpeed = 1;
uint16_t MouseKeys_::wheelDelay = 50;

uint16_t MouseKeys_::move_start_time_;
uint16_t MouseKeys_::accel_start_time_;
uint16_t MouseKeys_::wheel_start_time_;

void MouseKeys_::setWarpGridSize(uint8_t grid_size) {
  MouseWrapper.warp_grid_size = grid_size;
}

void MouseKeys_::setSpeedLimit(uint8_t speed_limit) {
  MouseWrapper.speedLimit = speed_limit;
}

void MouseKeys_::scrollWheel(uint8_t keyCode) {
  if (!Runtime.hasTimeExpired(wheel_start_time_, wheelDelay))
    return;

  wheel_start_time_ = Runtime.millisAtCycleStart();

  if (keyCode & KEY_MOUSE_UP)
    kaleidoscope::Runtime.hid().mouse().move(0, 0, wheelSpeed);
  else if (keyCode & KEY_MOUSE_DOWN)
    kaleidoscope::Runtime.hid().mouse().move(0, 0, -wheelSpeed);
  else if (keyCode & KEY_MOUSE_LEFT)
    kaleidoscope::Runtime.hid().mouse().move(0, 0, 0, -wheelSpeed);
  else if (keyCode & KEY_MOUSE_RIGHT)
    kaleidoscope::Runtime.hid().mouse().move(0, 0, 0, wheelSpeed);
}

EventHandlerResult MouseKeys_::afterEachCycle() {
  kaleidoscope::Runtime.hid().mouse().sendReport();
  kaleidoscope::Runtime.hid().mouse().releaseAllButtons();
  mouseMoveIntent = 0;

  return EventHandlerResult::OK;
}

EventHandlerResult MouseKeys_::beforeReportingState() {
  if (mouseMoveIntent == 0) {
    MouseWrapper.accelStep = 0;
    return EventHandlerResult::OK;
  }

  if (!Runtime.hasTimeExpired(move_start_time_, speedDelay))
    return EventHandlerResult::OK;

  move_start_time_ = Runtime.millisAtCycleStart();

  int8_t moveX = 0, moveY = 0;

  if (Runtime.hasTimeExpired(accel_start_time_, accelDelay)) {
    if (MouseWrapper.accelStep < 255 - accelSpeed) {
      MouseWrapper.accelStep += accelSpeed;
    }
    accel_start_time_ = Runtime.millisAtCycleStart();
  }

  if (mouseMoveIntent & KEY_MOUSE_UP)
    moveY -= speed;
  if (mouseMoveIntent & KEY_MOUSE_DOWN)
    moveY += speed;

  if (mouseMoveIntent & KEY_MOUSE_LEFT)
    moveX -= speed;
  if (mouseMoveIntent & KEY_MOUSE_RIGHT)
    moveX += speed;

  MouseWrapper.move(moveX, moveY);

  return EventHandlerResult::OK;
}

EventHandlerResult MouseKeys_::onKeyswitchEvent(Key &mappedKey, KeyAddr key_addr, uint8_t keyState) {
  if (mappedKey.getFlags() != (SYNTHETIC | IS_MOUSE_KEY))
    return EventHandlerResult::OK;

  if (mappedKey.getKeyCode() & KEY_MOUSE_BUTTON && !(mappedKey.getKeyCode() & KEY_MOUSE_WARP)) {
    uint8_t button = mappedKey.getKeyCode() & ~KEY_MOUSE_BUTTON;

    if (keyIsPressed(keyState)) {
      // Reset warp state on initial mouse button key-down only so we can use
      // warp keys to drag-and-drop:
      if (keyToggledOn(keyState)) {
        MouseWrapper.reset_warping();
      }

      kaleidoscope::Runtime.hid().mouse().pressButtons(button);
    } else if (keyToggledOff(keyState)) {
      kaleidoscope::Runtime.hid().mouse().releaseButtons(button);
      MouseWrapper.end_warping();
    }
  } else if (!(mappedKey.getKeyCode() & KEY_MOUSE_WARP)) {
    if (keyToggledOn(keyState)) {
      move_start_time_ = Runtime.millisAtCycleStart();
      accel_start_time_ = Runtime.millisAtCycleStart();
      wheel_start_time_ = Runtime.millisAtCycleStart() - wheelDelay;
    }
    if (keyIsPressed(keyState)) {
      if (mappedKey.getKeyCode() & KEY_MOUSE_WHEEL) {
        scrollWheel(mappedKey.getKeyCode());
      } else {
        mouseMoveIntent |= mappedKey.getKeyCode();
      }
    } else if (keyToggledOff(keyState)) {
      /* If a mouse key toggles off, we want to explicitly stop moving (or
       * scrolling) in that direction. We want to do this to support use-cases
       * where we send multiple reports per cycle (such as macros), and can't
       * rely on the main loop clearing the report for us. We do not want to
       * clear the whole report either, because we want any other mouse keys
       * to still have their desired effect. Therefore, we selectively stop
       * movement or scrolling. */
      mouseMoveIntent &= ~mappedKey.getKeyCode();
      bool x = false, y = false, vWheel = false, hWheel = false;

      if (mappedKey.getKeyCode() & KEY_MOUSE_UP ||
          mappedKey.getKeyCode() & KEY_MOUSE_DOWN) {
        if (mappedKey.getKeyCode() & KEY_MOUSE_WHEEL) {
          vWheel = true;
        } else {
          y = true;
        }
      } else if (mappedKey.getKeyCode() & KEY_MOUSE_LEFT ||
                 mappedKey.getKeyCode() & KEY_MOUSE_RIGHT) {
        if (mappedKey.getKeyCode() & KEY_MOUSE_WHEEL) {
          hWheel = true;
        } else {
          x = true;
        }
      }

      kaleidoscope::Runtime.hid().mouse().stop(x, y, vWheel, hWheel);
    }
  } else if (keyToggledOn(keyState)) {
    if (mappedKey.getKeyCode() & KEY_MOUSE_WARP && mappedKey.getFlags() & IS_MOUSE_KEY) {
      MouseWrapper.warp(((mappedKey.getKeyCode() & KEY_MOUSE_WARP_END) ? WARP_END : 0x00) |
                        ((mappedKey.getKeyCode() & KEY_MOUSE_UP) ? WARP_UP : 0x00) |
                        ((mappedKey.getKeyCode() & KEY_MOUSE_DOWN) ? WARP_DOWN : 0x00) |
                        ((mappedKey.getKeyCode() & KEY_MOUSE_LEFT) ? WARP_LEFT : 0x00) |
                        ((mappedKey.getKeyCode() & KEY_MOUSE_RIGHT) ? WARP_RIGHT : 0x00));
    }
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

EventHandlerResult MouseKeys_::onSetup(void) {
  kaleidoscope::Runtime.hid().mouse().setup();
  kaleidoscope::Runtime.hid().absoluteMouse().setup();

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::plugin::MouseKeys_ MouseKeys;
