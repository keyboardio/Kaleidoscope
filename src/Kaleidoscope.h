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
#include <avr/wdt.h>

#include KALEIDOSCOPE_HARDWARE_H
#include "key_events.h"
#include "layers.h"

#define HOOK_MAX 64

extern HARDWARE_IMPLEMENTATION KeyboardHardware;

#ifndef VERSION
#define VERSION "locally-built"
#endif

#define KEYMAP_SIZE (sizeof(keymaps) / ROWS / COLS / sizeof(Key))

class KaleidoscopePlugin {
 public:
  virtual void begin(void) = 0;
};

class Kaleidoscope_ {
  public:
    Kaleidoscope_(void);

    void setup(const byte keymap_count);
    void loop(void);
    void use(KaleidoscopePlugin *plugin, ...) __attribute__((sentinel));

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
    typedef Key (*eventHandlerHook)(Key mappedKey, byte row, byte col, uint8_t keyState);
    static eventHandlerHook eventHandlers[HOOK_MAX];

    static void replaceEventHandlerHook(eventHandlerHook oldHook, eventHandlerHook newHook);
    static void appendEventHandlerHook(eventHandlerHook hook);
    static void useEventHandlerHook(eventHandlerHook hook);

    typedef void (*loopHook)(bool postClear);
    static loopHook loopHooks[HOOK_MAX];

    static void replaceLoopHook(loopHook oldHook, loopHook newHook);
    static void appendLoopHook(loopHook hook);
    static void useLoopHook(loopHook hook);
};

extern Kaleidoscope_ Kaleidoscope;

/* -- DEPRECATED aliases; remove them when there are no more users. -- */

#define event_handler_hook_use(hook) Kaleidoscope.useEventHandlerHook(hook);
#define event_handler_hook_append(hook) Kaleidoscope.appendEventHandlerHook(hook)
#define event_handler_hook_replace(oldHook, newHook) Kaleidoscope.replaceEventHandlerHook(oldHook, newHook)

#define loop_hook_use(hook) Kaleidoscope.useLoopHook(hook)
#define loop_hook_append(hook) Kaleidoscope.appendLoopHook(hook)
#define loop_hook_replace(oldHook, newHook) Kaleidoscope.replaceLoopHook(oldHook, newHook)
