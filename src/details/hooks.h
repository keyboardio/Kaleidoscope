#pragma once

class kaleidoscope_;

namespace kaleidoscope {

class KeyEvent;
union Key;

// The reason why the hook rooting entry point functions live within
// class Hooks and not directly within a namespace is that we want
// to restrict who is alowed to trigger hooks. This is mainly to prevent
// user code from calling hook methods.
//
// A note to maintainers: When you add new hooks that are supposed to
// be called from other places than the friend classes and functions listed
// below, just add a suitable friend declaration.

class Hooks {
  // The following friend declarations restrict access to the hook routing system.

  // Kaleidoscope_ calls init, preReportHook and postReportHook
  //
  friend class Kaleidoscope_;

  // ::handleKeyswitchEvent(...) calls eventHandlerHook
  //
  friend void ::handleKeyswitchEvent(Key mappedKey, byte row, byte col, uint8_t keyState);

 private:

  // The following private functions are just to be called by friend classes
  // and functions.

  static void init();

  static bool eventHandlerHook(Key &mappedKey, const EventKey &eventKey);

  static void preReportHook();
  static void postReportHook();
};

} // namespace kaleidoscope
