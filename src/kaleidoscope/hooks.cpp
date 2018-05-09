#include "kaleidoscope/hooks.h"

namespace kaleidoscope {

// The following weak symbols are overwritten by
// using the KALEIDOSCOPE_INIT_PLUGINS(...) macro
// in the the firmware sketch. Their only purpose is
// to provide backwards compatibility during the transition
// from the old implementation of the hooking/plugin system
// to the new one. The weak symbols ensure that there
// are no undefined symbols if KALEIDOSCOPE_INIT_PLUGINS(...)
// is not used. This allows legacy sketches to be used
// during the transition phase.

__attribute__((weak))
EventHandlerResult Hooks::onSetup() {
  return EventHandlerResult::OK;
}

__attribute__((weak))
EventHandlerResult Hooks::beforeEachCycle() {
  return EventHandlerResult::OK;
}

__attribute__((weak))
EventHandlerResult Hooks::onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
  return EventHandlerResult::OK;
}

__attribute__((weak))
EventHandlerResult Hooks::beforeReportingState() {
  return EventHandlerResult::OK;
}

__attribute__((weak))
EventHandlerResult Hooks::afterEachCycle() {
  return EventHandlerResult::OK;
}

} // namespace kaleidoscope
