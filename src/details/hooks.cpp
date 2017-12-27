#include "details/hooks.h"

namespace kaleidoscope {

__attribute__((weak))
void Hooks::init() {}

__attribute__((weak))
bool Hooks::eventHandlerHook(Key &mappedKey, const EventKey &eventKey) {
  return true;
}

__attribute__((weak))
void Hooks::preReportHook() {}

__attribute__((weak))
void Hooks::postReportHook() {}

} // namespace kaleidoscope
