#pragma once

// This defines the names and argument signatures of all hook functions
// and event handler methods.
//
// A note to developers: This is the only place where new hooks/event handlers
// need to be added, apart from the place where the hook function is called.
//
// _FOR_EACH_EVENT_HANDLER macro can be called passing any operation
// macro function as argument OP whose parameters are the following
//
// HOOK_NAME, SHOULD_ABORT_ON_CONSUMED_EVENT, SIGNATURE, ARGS_LIST, ...
//
// Any additional parameters that are added to an invokation
// of _FOR_EACH_EVENT_HANDLER are passed through to OP.
//
// This is an extended application of an idiom commonly refered to as X Macros.
//
#define _FOR_EACH_EVENT_HANDLER(OP, ...)                                __NL__ \
                                                                        __NL__ \
   OP(onSetup, false,                                                   __NL__ \
               (),(), ##__VA_ARGS__)                                    __NL__ \
                                                                        __NL__ \
   /* Called at the very start of each cycle, before gathering          __NL__ \
    * events, before doing anything else.                               __NL__ \
    */                                                                  __NL__ \
   OP(beforeEachCycle, false,                                           __NL__ \
               (), (), ##__VA_ARGS__)                                   __NL__ \
                                                                        __NL__ \
   /* Function called for every non-idle key, every cycle, so it        __NL__ \
    * can decide what to do with it. It can modify the key (which is    __NL__ \
    * passed by reference for this reason), and decide whether          __NL__ \
    * further handles should be tried. If it returns                    __NL__ \
    * EventHandlerResult::OK, other handlers will also get a chance     __NL__ \
    * to react to the event. If it returns anything else, Kaleidoscope  __NL__ \
    * will stop processing there.                                       __NL__ \
    */                                                                  __NL__ \
   OP(onKeyswitchEvent, true,                                           __NL__ \
               (Key &mappedKey, byte row, byte col, uint8_t keyState),  __NL__ \
               (mappedKey, row, col, keyState), ##__VA_ARGS__)          __NL__ \
                                                                        __NL__ \
   /* Called before reporting our state to the host. This is the        __NL__ \
    * last point in a cycle where a plugin can alter what gets          __NL__ \
    * reported to the host.                                             __NL__ \
    */                                                                  __NL__ \
   OP(beforeReportingState, false,                                      __NL__ \
               (),(),##__VA_ARGS__)                                     __NL__ \
                                                                        __NL__ \
   /* Called at the very end of a cycle, after everything's             __NL__ \
    * said and done.                                                    __NL__ \
    */                                                                  __NL__ \
   OP(afterEachCycle, false,                                            __NL__ \
               (),(),##__VA_ARGS__)
