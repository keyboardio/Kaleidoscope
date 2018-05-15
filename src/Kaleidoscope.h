#pragma once

#include <Arduino.h>

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project KeyboardIO here

#define TS(X) //Serial.print(micros() );Serial.print("\t");Serial.println(X);

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include KALEIDOSCOPE_HARDWARE_H
#include "key_events.h"
#include "kaleidoscope/hid.h"
#include "layers.h"
#include "macro_map.h"
#include "kaleidoscope_internal/event_dispatch.h"
#include "kaleidoscope_internal/deprecations.h"
#include "macro_helpers.h"
#include "plugin.h"

#define HOOK_MAX 64

extern HARDWARE_IMPLEMENTATION KeyboardHardware;

#ifndef VERSION
#define VERSION "locally-built"
#endif

/** Kaleidoscope API (major) version.
 *
 * The API is guaranteed to be backwards compatible for the entire duration of a
 * major version. However, breaking changes may come, and result in a major
 * version bump. To help migration, the `KALEIDOSCOPE_API_VERSION` macro can be
 * used to check the major version provided by the Kaleidoscope we are compiling
 * against. This can be used to error out with a helpful message, or change how
 * the API is used - it is entirely up to the plugin or sketch author. The point
 * of this macro is to let them easily check the version.
 */
#define KALEIDOSCOPE_API_VERSION 2

/** Required Kaleidoscope major version.
 *
 * For the sake of convenience, defining `KALEIDOSCOPE_REQUIRED_API_VERSION`
 * before including `Kaleidoscope.h` itself will result in comparing its value
 * to `KALEIDOSCOPE_API_VERSION`. If they differ, a helpful error message is
 * printed.
 *
 * Done so that a new API version would result in a helpful error message,
 * instead of cryptic compile errors.
 */
#if defined(KALEIDOSCOPE_REQUIRED_API_VERSION) && (KALEIDOSCOPE_REQUIRED_API_VERSION != KALEIDOSCOPE_API_VERSION)
#define xstr(a) str(a)
#define str(a) #a
static_assert(KALEIDOSCOPE_REQUIRED_API_VERSION == KALEIDOSCOPE_API_VERSION,
              "Kaleidoscope API version mismatch! We have version " xstr(KALEIDOSCOPE_API_VERSION)
              " available, but version " xstr(KALEIDOSCOPE_REQUIRED_API_VERSION) " is required.");
#endif

const uint8_t KEYMAP_SIZE DEPRECATED(KEYMAP_SIZE) = 0;

namespace kaleidoscope {

class Kaleidoscope_ {
 public:
  Kaleidoscope_(void);

  void setup(const byte keymap_count) DEPRECATED(KEYMAP_SIZE) {
    setup();
  }
  void setup(void);
  void loop(void);

  /** Returns the timer as it was at the start of the cycle.
   * The goal of this method is two-fold:
   *  - To reduce the amount of calls to millis(), providing something cheaper.
   *  - To have a consistent timer value for the whole duration of a cycle.
   *
   * This cached value is updated at the start of each cycle as the name
   * implies. It is recommended to use this in plugins over millis() unless
   * there is good reason not to.
   */
  static uint32_t millisAtCycleStart() {
    return millis_at_cycle_start_;
  }

  // ---- Kaleidoscope.use() ----

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
  // First, we have the zero-argument version, which will satisfy the tail case.
  inline void use() {
  }

  // Then, the one-argument version, that gives us type safety for a single
  // plugin.
  inline void DEPRECATED(USE) use(kaleidoscope::Plugin *p) {
    p->begin();
  }

  // We have a no-op with a int argument, as a temporary hack until we remove
  // the last instance of a NULL-terminated Kaleidoscope.use() call.
  inline void use(int) {
  }

  // And the magic is in the last one, a template. The first parameter is
  // matched out by the compiler, and passed to one of the functions above. The
  // rest of the parameter pack (which may be an empty set in a recursive case),
  // are passed back to either ourselves, or the zero-argument version a few
  // lines above.
  template <typename... Plugins>
  void DEPRECATED(USE) use(kaleidoscope::Plugin *first, Plugins&&... plugins) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    use(first);
    use(plugins...);
#pragma GCC diagnostic pop
  }
#endif

  // ---- hooks ----

  /*
   * In most cases, one only wants a single copy of a hook. On the other hand,
   * plugins that depend on other plugins, may want to make it easier for the
   * end-user to use the plugin, and call the setup function of the dependent
   * plugins too. In case the end-user calls the same setup function, we'd end up
   * with hooks registered multiple times.
   *
   * To avoid this, protection against double-registration has been introduced.
   * The `event_handler_hook_use` and `loop_hook_use` functions will only allow
   * one copy of the hook. The `event_handler_hook_append` and `loop_hook_append`
   * functions will, on the other hand, just append the hooks, and not care about
   * protection.
   */
  typedef Key(*eventHandlerHook)(Key mappedKey, byte row, byte col, uint8_t keyState);
  static eventHandlerHook eventHandlers[HOOK_MAX];

  static void replaceEventHandlerHook(eventHandlerHook oldHook, eventHandlerHook newHook)
  DEPRECATED(EVENT_HANDLER_HOOK);
  static void appendEventHandlerHook(eventHandlerHook hook)
  DEPRECATED(EVENT_HANDLER_HOOK);
  static void useEventHandlerHook(eventHandlerHook hook)
  DEPRECATED(EVENT_HANDLER_HOOK);

  typedef void (*loopHook)(bool postClear);
  static loopHook loopHooks[HOOK_MAX];

  static void replaceLoopHook(loopHook oldHook, loopHook newHook)
  DEPRECATED(LOOP_HOOK);
  static void appendLoopHook(loopHook hook)
  DEPRECATED(LOOP_HOOK);
  static void useLoopHook(loopHook hook)
  DEPRECATED(LOOP_HOOK);

  static bool focusHook(const char *command);

 private:
  static uint32_t millis_at_cycle_start_;
};

extern kaleidoscope::Kaleidoscope_ Kaleidoscope;

} // namespace kaleidoscope

// For compatibility reasons we enable class Kaleidoscope_ also to be available
// in global namespace.
//
typedef kaleidoscope::Kaleidoscope_  Kaleidoscope_;

// For compatibility reasons we enable the global variable Kaleidoscope
// in global namespace.
//
using kaleidoscope::Kaleidoscope;

#define FOCUS_HOOK_KALEIDOSCOPE FOCUS_HOOK(Kaleidoscope.focusHook,  \
                                           "layer.on\n"             \
                                           "layer.off\n"            \
                                           "layer.getState")

/* -- DEPRECATED aliases; remove them when there are no more users. -- */

void event_handler_hook_use(kaleidoscope::Kaleidoscope_::eventHandlerHook hook)
DEPRECATED(EVENT_HANDLER_HOOK);
void loop_hook_use(kaleidoscope::Kaleidoscope_::loopHook hook)
DEPRECATED(LOOP_HOOK);

void __USE_PLUGINS(kaleidoscope::Plugin *plugin, ...) DEPRECATED(USE);

#define USE_PLUGINS(...) __USE_PLUGINS(__VA_ARGS__, NULL)

// Use this function macro to register plugins with Kaleidoscope's
// hooking system. The macro accepts a list of plugin instances that
// must have been instantiated at global scope.
//
#define KALEIDOSCOPE_INIT_PLUGINS(...) _KALEIDOSCOPE_INIT_PLUGINS(__VA_ARGS__)
