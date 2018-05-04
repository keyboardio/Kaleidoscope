#include "details/hooks.h"

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
void Hooks::init() {}

__attribute__((weak))
bool Hooks::eventHandlerHook(Key &mappedKey, byte row, byte col, uint8_t keyState) {
  return true;
}

__attribute__((weak))
void Hooks::preReportHook() {}

__attribute__((weak))
void Hooks::postReportHook() {}

} // namespace kaleidoscope
