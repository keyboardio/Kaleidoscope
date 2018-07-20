#pragma once

#include <Arduino.h>
#include <EEPROM.h>

namespace kaleidoscope {
union Key;
}

#include "plugin.h"
#include "event_handlers.h"

// Forward declaration required to enable friend declarations
// in class Hooks.
class kaleidoscope_;
extern void handleKeyswitchEvent(kaleidoscope::Key mappedKey, byte row, byte col, uint8_t keyState);

namespace kaleidoscope {

// The reason why the hook routing entry point functions live within
// class Hooks and not directly within a namespace is, that we want
// to restrict who is allowed to trigger hooks, mainly to prevent
// user code from calling hook methods.
//
// A note to maintainers: When you add new hooks that are supposed to
// be called from other places than the friend classes and functions listed
// below, just add a suitable friend declaration.

class Hooks {

  // The following friend declarations restrict access to
  // the hook routing system.

  // Kaleidoscope_ calls Hooks::onSetup, Hooks::beforeReportingState
  // and Hooks::afterEachCycle.
  friend class Kaleidoscope_;

  // ::handleKeyswitchEvent(...) calls Hooks::onKeyswitchEvent.
  friend void ::handleKeyswitchEvent(kaleidoscope::Key mappedKey,
                                     byte row, byte col, uint8_t keyState);

 public:

  static EventHandlerResult onStorageReport();

  template<typename T>
  T& getFromStorage(kaleidoscope::Plugin &plugin, uint16_t offset, T& t) {
    uint16_t plugin_offset = getStorageOffset(plugin);
    return EEPROM.get(plugin_offset + offset, t);
  }

  template<typename T>
  T& getFromStorage(kaleidoscope::Plugin &plugin, T& t) {
    return getFromStorage(plugin, 0, t);
  }

  template<typename T>
  const T& putToStorage(kaleidoscope::Plugin &plugin, uint16_t offset, T& t) {
    uint16_t plugin_offset = getStorageOffset(plugin);
    return EEPROM.put(plugin_offset + offset, t);
  }

  template<typename T>
  const T& putToStorage(kaleidoscope::Plugin &plugin, T& t) {
    return putToStorage(plugin, 0, t);
  }

  template<typename Plugin__>
  static uint16_t getStorageOffset(Plugin__ &plugin);

  // The following private functions are just to be called by classes
  // and functions that are declared as friends above.

#define DEFINE_WEAK_HOOK_FUNCTION(                                             \
    HOOK_NAME, SHOULD_ABORT_ON_CONSUMED_EVENT, SIGNATURE, ARGS_LIST)    __NL__ \
                                                                        __NL__ \
   static EventHandlerResult HOOK_NAME SIGNATURE;

  _FOR_EACH_EVENT_HANDLER(DEFINE_WEAK_HOOK_FUNCTION)

#undef DEFINE_WEAK_HOOK_FUNCTION
};

}
