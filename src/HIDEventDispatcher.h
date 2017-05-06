#pragma once
#include "KeyboardioHID.h"
#ifdef KEYBOARDIO_HAS_PLUGGABLE_USB
#include "EventDispatcher.h"

/** The HIDEventDispatcher is an EventDispatcher implementation
 * that knows how to send key reports to the standard KeyboardioHID
 * keyboard interface. */
class HIDEventDispatcher : public EventDispatcher {
public:
  // Ensure that we call the parent constructor and get registered
  HIDEventDispatcher() : EventDispatcher() {}

  void begin() override {
    // TODO: intelligently use BootKeyboard at the right time
    Keyboard.begin();
    SystemControl.begin();
    ConsumerControl.begin();
  }

  void queryConnectionTypes(uint8_t &connectionMask) override {
    // We are USB, so enable those bits
    connectionMask |= Usb;
  }

  void consumerPress(uint8_t connectionMask, uint8_t keyCode) override {
    if (connectionMask & Usb) {
      ConsumerControl.press(keyCode);
    }
  }

  void consumerRelease(uint8_t connectionMask, uint8_t keyCode) override {
    if (connectionMask & Usb) {
      ConsumerControl.release(keyCode);
    }
  }

  void systemPress(uint8_t connectionMask, uint8_t keyCode) override {
    if (connectionMask & Usb) {
      SystemControl.press(keyCode);
    }
  }

  void systemRelease(uint8_t connectionMask, uint8_t keyCode) override {
    if (connectionMask & Usb) {
      SystemControl.release();
    }
  }

  void keyPress(uint8_t connectionMask, uint8_t keyCode) override {
    if (connectionMask & Usb) {
      Keyboard.press(keyCode);
    }
  }

  void keyRelease(uint8_t connectionMask, uint8_t keyCode) override {
    if (connectionMask & Usb) {
      Keyboard.release(keyCode);
    }
  }

  void keyReleaseAll(uint8_t connectionMask) override {
    if (connectionMask & Usb) {
      Keyboard.releaseAll();
    }
  }

  void keySendReport(uint8_t connectionMask) override {
    if (connectionMask & Usb) {
      Keyboard.sendReport();
    }
  }
};

#endif
