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


#include KALEIDOSCOPE_HARDWARE_H
#include "key_events.h"
#include "kaleidoscope/hid.h"
#include "layers.h"

#define HOOK_MAX 64

extern HARDWARE_IMPLEMENTATION KeyboardHardware;

#ifndef VERSION
#define VERSION "locally-built"
#endif

#define KEYMAP_SIZE (sizeof(keymaps) / ROWS / COLS / sizeof(Key))

#define USE_PLUGINS(plugins...) Kaleidoscope.use(plugins)

class KaleidoscopePlugin {
 public:
  virtual void begin(void) = 0;
};

class Kaleidoscope_ {
 public:
  Kaleidoscope_(void);

  void setup(const byte keymap_count) {
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

  static void replaceEventHandlerHook(eventHandlerHook oldHook, eventHandlerHook newHook);
  static void appendEventHandlerHook(eventHandlerHook hook);
  static void useEventHandlerHook(eventHandlerHook hook);

  typedef void (*loopHook)(bool postClear);
  static loopHook loopHooks[HOOK_MAX];

  static void replaceLoopHook(loopHook oldHook, loopHook newHook);
  static void appendLoopHook(loopHook hook);
  static void useLoopHook(loopHook hook);

  static bool focusHook(const char *command);
};

extern Kaleidoscope_ Kaleidoscope;

#define FOCUS_HOOK_KALEIDOSCOPE FOCUS_HOOK(Kaleidoscope.focusHook,  \
                                           "layer.on\n"             \
                                           "layer.off\n"            \
                                           "layer.getState")

/* -- DEPRECATED aliases; remove them when there are no more users. -- */

void event_handler_hook_use(Kaleidoscope_::eventHandlerHook hook) __attribute__((deprecated));
void loop_hook_use(Kaleidoscope_::loopHook hook) __attribute__((deprecated));
