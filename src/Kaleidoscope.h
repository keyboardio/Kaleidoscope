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
#include "details/preprocessor_macro_map.h"
#include "details/hook_rooting.h"
#include "macro_functions.h"
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
#define KALEIDOSCOPE_API_VERSION 1

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

const uint8_t KEYMAP_SIZE
__attribute__((deprecated("Kaleidoscope.setup() does not require KEYMAP_SIZE anymore."))) = 0;

namespace kaleidoscope {

class Kaleidoscope_ {
 public:
  Kaleidoscope_(void);

  void setup(const byte keymap_count)
  __attribute__((deprecated("The keymap_count argument (and the KEYMAP_SIZE macro) are unused, and can be safely removed."))) {
    setup();
  }
  void setup(void);
  void loop(void);

  // ---- Kaleidoscope.use() ----

  // First, we have the zero-argument version, which will satisfy the tail case.
  inline void use() {
  }

  // Then, the one-argument version, that gives us type safety for a single
  // plugin.
  inline void use(KaleidoscopePlugin *p) {
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
  void use(KaleidoscopePlugin *first, Plugins&&... plugins) {
    use(first);
    use(plugins...);
  }

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
  __attribute__((deprecated("Kaleidoscope::replaceEventHandlerHook(...) is deprecated. Please implement KaleidoscopePlugin.eventHandlerHook(...) instead.")));
  static void appendEventHandlerHook(eventHandlerHook hook)
  __attribute__((deprecated("Kaleidoscope::appendEventHandlerHook(...) is deprecated. Please implement KaleidoscopePlugin.eventHandlerHook(...) instead.")));
  static void useEventHandlerHook(eventHandlerHook hook)
  __attribute__((deprecated("Kaleidoscope::useEventHandlerHook(...) is deprecated. Please implement KaleidoscopePlugin.eventHandlerHook(...) instead.")));

  typedef void (*loopHook)(bool postClear);
  static loopHook loopHooks[HOOK_MAX];

  static void replaceLoopHook(loopHook oldHook, loopHook newHook)
  __attribute__((deprecated("Kaleidoscope::replaceLoopHook(...) is deprecated. Please implement KaleidoscopePlugin.beginLoopHook(...) or KaleidoscopePlugin.postReportHook(...) instead.")));
  static void appendLoopHook(loopHook hook)
  __attribute__((deprecated("Kaleidoscope::replaceLoopHook(...) is deprecated. Please implement KaleidoscopePlugin.beginLoopHook(...) or KaleidoscopePlugin.postReportHook(...) instead.")));
  static void useLoopHook(loopHook hook)
  __attribute__((deprecated("Kaleidoscope::replaceLoopHook(...) is deprecated. Please implement KaleidoscopePlugin.beginLoopHook(...) or KaleidoscopePlugin.postReportHook(...) instead.")));

  static bool focusHook(const char *command);
};

extern kaleidoscope::Kaleidoscope_ Kaleidoscope;

} // namespace kaleidoscope

// For compatibility reasons we enable class Kaleidoscope_ also to be available
// in global namespace.
//
typedef kaleidoscope::Kaleidoscope_  Kaleidoscope_ __attribute__((deprecated("class Kaleidoscope_ in global namespace is deprecated. Please use kaleidoscope::Kaleidoscope_ instead.")));

// For compatibility reasons we enable the global variable Kaleidoscope
// in global namespace.
//
__attribute__((deprecated("Kaleidoscope in global namespace is deprecated. Please use kaleidoscope::Kaleidoscope instead.")))
extern kaleidoscope::Kaleidoscope_ Kaleidoscope;

#define FOCUS_HOOK_KALEIDOSCOPE FOCUS_HOOK(Kaleidoscope.focusHook,  \
                                           "layer.on\n"             \
                                           "layer.off\n"            \
                                           "layer.getState")

/* -- DEPRECATED aliases; remove them when there are no more users. -- */

void event_handler_hook_use(kaleidoscope::Kaleidoscope_::eventHandlerHook hook)
__attribute__((deprecated("Use Kaleidoscope.useEventHandlerHook instead")));
void loop_hook_use(kaleidoscope::Kaleidoscope_::loopHook hook)
__attribute__((deprecated("Use Kaleidoscope.useLoopHook instead")));

void __USE_PLUGINS(KaleidoscopePlugin *plugin, ...)
__attribute__((deprecated("Use Kaleidoscope.use(...) instead")));

#define USE_PLUGINS(...) __USE_PLUGINS(__VA_ARGS__, NULL)

// Use this function macro to register plugins with Kaleidoscope's
// hooking system. The macro accepts a list of plugin instances that
// must have been instanciated at global scope.
//
#define KALEIDOSCOPE_INIT_PLUGINS(...) _KALEIDOSCOPE_INIT_PLUGINS(__VA_ARGS__)
