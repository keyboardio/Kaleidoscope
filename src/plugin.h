#pragma once

#ifndef KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
#define KALEIDOSCOPE_ENABLE_V1_PLUGIN_API 1
#endif

namespace kaleidoscope {

class Kaleidoscope_;

class Plugin {

  friend class Kaleidoscope_;

 protected:

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
#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
  virtual void begin() {};
#endif

 public:
  enum class Result {
    OK,
    EVENT_CONSUMED,
    ERROR,
  };

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
  //       plugins is the polymorphic `PluginMethod` class.

  Result onSetup() {
    // By letting the new `onSetup()` method call the legacy begin() method, we
    // make sure that the old hooking interface is supported even if
    // KALEIDOSCOPE_INIT_PLUGINS() is used to register a plugin that relies on
    // the legacy `begin()` method to initialize itself and register hooks.
    //
#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
    this->begin();
#endif
    return Result::OK;
  }

  // Called at the very start of each cycle, before gathering events, before
  // doing anything else.
  Result beforeEachCycle() {
    return Result::OK;
  }

  // Function called for every non-idle key, every cycle, so it can decide what
  // to do with it. It can modify the key (which is passed by reference for this
  // reason), and decide whether further handles should be tried. If it returns
  // Result::OK, other handlers will also get a chance to react to the event. If
  // it returns anything else, Kaleidoscope will stop processing there.
  Result onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
    return Result::OK;
  }

  // Called before reporting our state to the host. This is the last point in a
  // cycle where a plugin can alter what gets reported to the host.
  Result beforeReportingState() {
    return Result::OK;
  }

  // Called at the very end of a cycle, after everything's said and done.
  Result afterEachCycle() {
    return Result::OK;
  }
};

} // namespace kaleidoscope

typedef kaleidoscope::Plugin KaleidoscopePlugin __attribute__((deprecated("class KaleidoscopePlugin is deprecated. Please derive plugins from kaleidoscope::Plugin instead.")));
