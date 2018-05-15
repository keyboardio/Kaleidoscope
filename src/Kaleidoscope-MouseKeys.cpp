#include <Arduino.h>

#include "Kaleidoscope-MouseKeys.h"
#include "MouseWrapper.h"
#include "Kaleidoscope.h"

uint8_t MouseKeys_::mouseMoveIntent;

uint8_t MouseKeys_::speed = 1;
uint16_t MouseKeys_::speedDelay = 1;

uint8_t MouseKeys_::accelSpeed = 1;
uint16_t MouseKeys_::accelDelay = 64;

uint8_t MouseKeys_::wheelSpeed = 1;
uint16_t MouseKeys_::wheelDelay = 50;

uint32_t MouseKeys_::accelEndTime;
uint32_t MouseKeys_::endTime;
uint32_t MouseKeys_::wheelEndTime;

void MouseKeys_::setWarpGridSize(uint8_t grid_size) {
  MouseWrapper.warp_grid_size = grid_size;
}

void MouseKeys_::scrollWheel(uint8_t keyCode) {
  if (millis() < wheelEndTime)
    return;

  wheelEndTime = millis() + wheelDelay;

  if (keyCode & KEY_MOUSE_UP)
    kaleidoscope::hid::moveMouse(0, 0, wheelSpeed);
  else if (keyCode & KEY_MOUSE_DOWN)
    kaleidoscope::hid::moveMouse(0, 0, -wheelSpeed);
  else if (keyCode & KEY_MOUSE_LEFT)
    kaleidoscope::hid::moveMouse(0, 0, 0, -wheelSpeed);
  else if (keyCode & KEY_MOUSE_RIGHT)
    kaleidoscope::hid::moveMouse(0, 0, 0, wheelSpeed);
}

kaleidoscope::EventHandlerResult MouseKeys_::afterEachCycle() {
  kaleidoscope::hid::sendMouseReport();
  kaleidoscope::hid::releaseAllMouseButtons();
  mouseMoveIntent = 0;

  return kaleidoscope::EventHandlerResult::OK;
}

kaleidoscope::EventHandlerResult MouseKeys_::beforeReportingState() {
  if (mouseMoveIntent == 0) {
    MouseWrapper.accelStep = 0;
    endTime = 0;
    accelEndTime = 0;
    return kaleidoscope::EventHandlerResult::OK;
  }

  if (millis() < endTime)
    return kaleidoscope::EventHandlerResult::OK;

  endTime = millis() + speedDelay;

  int8_t moveX = 0, moveY = 0;

  if (millis() >= accelEndTime) {
    if (MouseWrapper.accelStep < 255 - accelSpeed) {
      MouseWrapper.accelStep += accelSpeed;
    }
    accelEndTime = millis() + accelDelay;
  }

  if (mouseMoveIntent & KEY_MOUSE_UP)
    moveY = -speed;
  else if (mouseMoveIntent & KEY_MOUSE_DOWN)
    moveY = speed;

  if (mouseMoveIntent & KEY_MOUSE_LEFT)
    moveX = -speed;
  else if (mouseMoveIntent & KEY_MOUSE_RIGHT)
    moveX = speed;

  MouseWrapper.move(moveX, moveY);

  return kaleidoscope::EventHandlerResult::OK;
}

kaleidoscope::EventHandlerResult MouseKeys_::onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
  if (mappedKey.flags != (SYNTHETIC | IS_MOUSE_KEY))
    return kaleidoscope::EventHandlerResult::OK;

  if (mappedKey.keyCode & KEY_MOUSE_BUTTON && !(mappedKey.keyCode & KEY_MOUSE_WARP)) {
    uint8_t button = mappedKey.keyCode & ~KEY_MOUSE_BUTTON;

    if (keyIsPressed(keyState)) {
      // Reset warp state on initial mouse button key-down only so we can use
      // warp keys to drag-and-drop:
      if (keyToggledOn(keyState)) {
        MouseWrapper.reset_warping();
      }

      MouseWrapper.pressButton(button);
    } else if (keyToggledOff(keyState)) {
      MouseWrapper.release_button(button);
    }
  } else if (!(mappedKey.keyCode & KEY_MOUSE_WARP)) {
    if (keyToggledOn(keyState)) {
      endTime = millis() + speedDelay;
      accelEndTime = millis() + accelDelay;
      wheelEndTime = 0;
    }
    if (keyIsPressed(keyState)) {
      if (mappedKey.keyCode & KEY_MOUSE_WHEEL) {
        scrollWheel(mappedKey.keyCode);
      } else {
        mouseMoveIntent |= mappedKey.keyCode;
      }
    }
  } else if (keyToggledOn(keyState)) {
    if (mappedKey.keyCode & KEY_MOUSE_WARP && mappedKey.flags & IS_MOUSE_KEY) {
      MouseWrapper.warp(((mappedKey.keyCode & KEY_MOUSE_WARP_END) ? WARP_END : 0x00) |
                        ((mappedKey.keyCode & KEY_MOUSE_UP) ? WARP_UP : 0x00) |
                        ((mappedKey.keyCode & KEY_MOUSE_DOWN) ? WARP_DOWN : 0x00) |
                        ((mappedKey.keyCode & KEY_MOUSE_LEFT) ? WARP_LEFT : 0x00) |
                        ((mappedKey.keyCode & KEY_MOUSE_RIGHT) ? WARP_RIGHT : 0x00));
    }
  }

  return kaleidoscope::EventHandlerResult::EVENT_CONSUMED;
}

kaleidoscope::EventHandlerResult MouseKeys_::onSetup(void) {
  MouseWrapper.begin();

  return kaleidoscope::EventHandlerResult::OK;
}

// Legacy V1 API
#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
void MouseKeys_::begin() {
  onSetup();
  Kaleidoscope.useEventHandlerHook(legacyEventHandler);
  Kaleidoscope.useLoopHook(legacyLoopHook);
}

Key MouseKeys_::legacyEventHandler(Key mapped_key, byte row, byte col, uint8_t key_state) {
  kaleidoscope::EventHandlerResult r = MouseKeys.onKeyswitchEvent(mapped_key, row, col, key_state);
  if (r == kaleidoscope::EventHandlerResult::OK)
    return mapped_key;
  return Key_NoKey;
}

void MouseKeys_::legacyLoopHook(bool is_post_clear) {
  if (is_post_clear) {
    MouseKeys.afterEachCycle();
  } else {
    MouseKeys.beforeReportingState();
  }
}
#endif

MouseKeys_ MouseKeys;
