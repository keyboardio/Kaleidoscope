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

extern HARDWARE_IMPLEMENTATION KeyboardHardware;

#ifndef VERSION
#define VERSION "locally-built"
#endif

#define KEYMAP_SIZE (sizeof(keymaps) / ROWS / COLS / sizeof(Key))

/*
 * The `USE_PLUGINS()` macro is a clever hack, to make it seem like
 * `Kaleidoscope.use()` is type-safe. It pushes its arguments into an
 * appropriately typed array, so anything that does not fit the criteria, will
 * trigger a compiler error.
 *
 * It then never uses the array, and passes the plugins over to
 * `Kaleidoscope.use`, adding the trailing `NULL`, making it even easier to use.
 *
 * Since the array this macro creates is never used, the compiler will optimize
 * it out fully. As such, by using this macro, we incur neither any size
 * penalties, nor any run-time penalties. Everything happens at compile-time.
 */
#define USE_PLUGINS(plugins...) ({                              \
          static KaleidoscopePlugin *__p[] = {plugins, NULL};   \
          Kaleidoscope.use(plugins, NULL);                      \
        })

class KaleidoscopePlugin {
  public:
    virtual void begin(void) = 0;
};

class Kaleidoscope_ {
  public:
    Kaleidoscope_(void);

    void setup(const byte keymap_count) {
        setup();
    };
    void setup(void);
    void loop(void);
    void use(KaleidoscopePlugin *plugin, ...) __attribute__((sentinel));

    // ---- hooks ----
    typedef Key (*eventHandlerHook)(Key mappedKey, byte row, byte col, uint8_t keyState);
    typedef void (*loopHook)(bool postClear);

    template<typename T> struct listItem {
        T hook;
        listItem<T> *next;
    };

    static listItem<eventHandlerHook> *eventHandlerRootNode;
    static listItem<loopHook> *loopHookRootNode;

    /*
     * In most cases, one only wants a single copy of a hook. On the other hand,
     * plugins that depend on other plugins, may want to make it easier for the
     * end-user to use the plugin, and call the setup function of the dependent
     * plugins too. In case the end-user calls the same setup function, we'd end up
     * with hooks registered multiple times.
     *
     * To avoid this, protection against double-registration has been
     * introduced. The `useHook` functions will only allow one copy of the hook.
     * The `appendHook` functions will, on the other hand, just append the
     * hooks, and not care about protection.
     */
    static void appendHook(listItem<eventHandlerHook> *newNode);
    static void appendHook(listItem<loopHook> *newNode);
    static void useHook(listItem<eventHandlerHook> *newNode);
    static void useHook(listItem<loopHook> *newNode);

  private:
    template<typename T> static void appendHook(T **rootNode, T *newNode);
    template<typename T> static void useHook(T **rootNode, T *newNode);

    static void runLoopHooks(bool postClear);
};

extern Kaleidoscope_ Kaleidoscope;

#define event_handler_hook_use(hook) {                                \
      static Kaleidoscope_::listItem<Kaleidoscope_::eventHandlerHook> \
        eventHandlerHookNode = {&hook, NULL};                            \
      Kaleidoscope.useHook(&eventHandlerHookNode);                    \
  }
#define event_handler_hook_append(hook) {                             \
      static Kaleidoscope_::listItem<Kaleidoscope_::eventHandlerHook> \
        eventHandlerHookNode = {&hook, NULL};                            \
      Kaleidoscope.appendHook(&eventHandlerHookNode);                 \
  }

#define loop_hook_use(hook) {                                 \
      static Kaleidoscope_::listItem<Kaleidoscope_::loopHook> \
        loopHookNode = {&hook, NULL};                            \
      Kaleidoscope.useHook(&loopHookNode);                    \
  }
#define loop_hook_append(hook) {                               \
      static Kaleidoscope_::listItem<Kaleidoscope_::loopHook>  \
        loopHookNode = {&hook, NULL};                             \
      Kaleidoscope.appendHook(&loopHookNode);                  \
  }
