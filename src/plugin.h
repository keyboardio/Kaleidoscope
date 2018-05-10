#pragma once

#include "kaleidoscope/eventhandlerresult.h"
#include "kaleidoscope_internal/event_dispatch.h"
#include "event_handlers.h"

#ifndef KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
#define KALEIDOSCOPE_ENABLE_V1_PLUGIN_API 1
#endif

namespace kaleidoscope {

class Kaleidoscope_;

// A base class that implements default noop versions of all event
// handler methods.
//
// A note to developers:
//
//     The only purpose of class EventHandlerBasePlugin
//     is to enable the use of _FOR_EACH_EVENT_HANDLER
//     to define default event handlers. This is currently not possible
//     inside class Plugin directly as it would collide with
//     the separate definition of onSetup(). This additional
//     definition is, however, necessary to support the V1 plugin API.
//
//     TODO: As soon as the V1 plugin API is removed from Kaleidoscope,
//     class EventHandlerBasePlugin can be removed and its content
//     be moved to class Plugin.
//
class EventHandlerBasePlugin {

 public:

#define DEFINE_AND_IMPLEMENT_EVENT_HANDLER_METHOD(                             \
    HOOK_NAME, SHOULD_ABORT_ON_CONSUMED_EVENT, SIGNATURE, ARGS_LIST)    __NL__ \
                                                                        __NL__ \
  EventHandlerResult HOOK_NAME SIGNATURE {                              __NL__ \
    return EventHandlerResult::OK;                                      __NL__ \
  }

  _FOR_EACH_EVENT_HANDLER(DEFINE_AND_IMPLEMENT_EVENT_HANDLER_METHOD)

#undef DEFINE_AND_IMPLEMENT_EVENT_HANDLER_METHOD
};

class Plugin : public EventHandlerBasePlugin {

  friend class Kaleidoscope_;

 protected:

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
  /** Legacy plugin setup hook.

   * In version one of the plugin API, `plugin->begin()` was called at
   * `Kaleidoscope.use()` time, to perform boot-time initialization. We needed
   * this because plugins are supposed to provide a singleton object, statically
   * initialized at compile-time (with few exceptions). Because of this, the
   * order in which they are instantiated is unspecified, and cannot be relied
   * upon. For this reason, one's expected to explicitly initialize, "use" the
   * plugins one wishes to, by calling `Kaleidoscope.use()` with a list of
   * plugin object pointers.
   *
   * This is the only time this function will be called. It is intentionally
   * protected, and accessible by the `Kaleidoscope` class only.
   *
   * Also, this method is deprecated in favour of the V2 API, which you can read
   * about below.
   */
  virtual void begin() {}
#endif

 public:
  // The following callbacks handle the synchronized communication
  // between the Kaleidoscope core and its plugins.
  //
  // A plugin can, but does not have to implement all provided hook methods.
  // Calls to unimplemented hook methods are entirely optimized away by the compiler.
  // Thus, a non-implemented hook causes neither memory consumption nor
  // run-time overhead.
  //
  // Note: All hook methods in this class are non virtual on purpose as the actual
  //       interface between the Kaleidoscope singleton class and the
  //       plugins is the polymorphic `EventHandler` class.

  EventHandlerResult onSetup() {
#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
    // By letting the new `onSetup()` method call the legacy begin() method, we
    // make sure that the old hooking interface is supported even if
    // KALEIDOSCOPE_INIT_PLUGINS() is used to register a plugin that relies on
    // the legacy `begin()` method to initialize itself and register hooks.
    //
    this->begin();
#endif
    return EventHandlerResult::OK;
  }

};

} // namespace kaleidoscope

typedef kaleidoscope::Plugin KaleidoscopePlugin __attribute__((deprecated("class KaleidoscopePlugin is deprecated. Please derive plugins from kaleidoscope::Plugin instead.")));
