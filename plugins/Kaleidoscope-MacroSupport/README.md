# MacroSupport

This plugin provides the supplemental key array used by the Macros and DynamicMacros plugins, and is necessary for the proper functioning of those plugins.

## Using the plugin

Any firmware sketch that uses either Macros or DynamicMacros automatically includes this plugin, so there's no need to add it explicitly.  If your sketch doesn't require either type of Macros key, however, you can still make use of the MacroSupport plugin's helper methods (`tap()`, `press()`, _et al_).  In that case, you should include the MacroSupport header file, and include it in `KALEIDOSCOPE_INIT_PLUGINS()`:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-MacroSupport.h>

// Other plugin code that calls `MacroSupport.press()` (for example)

KALEIDOSCOPE_INIT_PLUGINS(
  MacroSupport,
  // Other plugin(s) that make use of MacroSupport
);

void setup() {
  Kaleidoscope.setup ();
}
```

## Plugin methods

The plugin provides a `MacroSupport` object, which contains a supplemental array of virtual keys that it adds to USB Keyboard reports.  Other plugins and user code can interact with it via the following methods:

### `.press(key)`

> Sends a key press event for `key`, and will keep that virtual key active in
> the supplemental virtual keys array.

### `.release(key)`

> Sends a key release event for `key`, and removes it from the supplemental
> virtual keys array.

### `.clear()`

> Releases all active virtual keys held by MacroSupport.  This both empties the
> supplemental keys array (see above) and sends a release event for each key
> stored there.

### `.tap(key)`

> Sends an immediate press and release event for `key` with no delay, using an
> invalid key address.  This method doesn't actually use the supplemental keys
> array, but is provided here for convenience and simplicity.

It is not necessary to use either the Macros (or DynamicMacros) to make use of MacroSupport.  When using it with custom code, however, please remember that the supplemental active keys array it provides will be shared by all clients (e.g. Macros, user-defined Leader or TapDance functions), so if you want more than one of those clients to be active simultaneously, be aware that calles to `MacroSupport.clear()` will affect all of them, not just the caller.
