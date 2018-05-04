#pragma once

namespace kaleidoscope {

class Kaleidoscope_;

class Plugin {

  friend class Kaleidoscope_;

 protected:

  /** Initial plugin setup hook.
   * All plugins are supposed to provide a singleton object, statically
   * initialized at compile-time (with few exceptions). Because of this, the
   * order in which they are instantiated is unspecified, and cannot be relied
   * upon. For this reason, one's expected to explicitly initialize, "use" the
   * plugins one wishes to, by calling `Kaleidoscope.use()` with a list of plugin
   * object pointers.
   *
   * This function will in turn call the `begin` function of each plugin,
   * so that they can perform any initial setup they wish, such as registering
   * event handler or loop hooks. This is the only time this function will be
   * called. It is intentionally protected, and accessible by the `Kaleidoscope`
   * class only.
   */
  virtual void begin(void) {};

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
  //       plugins is the polymorphic PluginHookAdapter class

  void init() {
    // By letting the new init method call the legacy begin() method, we make
    // sure that the old hooking interface is supported even if
    // KALEIDOSCOPE_INIT_PLUGINS() is used to register a plugin that relies on
    // the legacy begin() method to initialize itself and register hooks.
    //
    this->begin();
  }

  // This handler is supposed to return false if no other handlers are
  // supposed to be called after it, true otherwise.
  //
  // The handler is allowed to modify the mappedKey that is therefore
  // passed by reference.
  //
  bool eventHandlerHook(Key &mappedKey, byte row, byte col, uint8_t keyState) {
    return true; // Always allow other handlers to continue
  }

  void preReportHook() {}
  void postReportHook() {}
};

} // namespace kaleidoscope

typedef kaleidoscope::Plugin KaleidoscopePlugin __attribute__((deprecated("class KaleidoscopePlugin is deprecated. Please derive plugins from kaleidoscope::Plugin instead.")));
