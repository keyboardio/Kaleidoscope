# Kaleidoscope-Devel-ArduinoTrace

A development and debugging aid, this plugin imports and initializes an embedded copy of the ArduinoTrace library from https://github.com/bblanchon/ArduinoTrace

It is primarly intended for use on our simulator, though in theory, it should work when run on normal hardware, too

## Using the plugin

The plugin comes with reasonable defaults (see below), and can be used out of
the box, without any further configuration:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-Devel-ArduinoTrace.h>

/* ... */

void setup () {
  Kaleidoscope.setup ();
  TRACE()
}


void someMethod(uint8_t value) {
	uint8_t other_value;

	TRACE()
	DUMP(value)
	other_value = someOtherMethod(value);
	DUMP(other_value)
}
```

Running in the simulator, you should see output like:

```
basic-keypress.ino:492: void setup()
Runtime.cpp:51: void kaleidoscope::Runtime_::loop()
Runtime.cpp:53: millis_at_cycle_start_ = 4
```


While this plugin is primarily intended to be used in the Kaleidoscope simulator, it should work on actual hardware. On the simulator, output is directed to DebugStderr. On hardware, it defaults to Serial.

To configure ArduinoTrace, there are a number of constants you can `#define` before you `#include` the plugin. They're documented [upstream][upstream:docs].


## Plugin methods

This plugin does not itself offer up any API methods or use any
plugin hooks, instead exposing the "TRACE" and "DUMP" macros provided
by ArduinoTrace


## Further reading

Have a look at the [docs][upstream:docs] for ArduinoTrace on GitHub.

 [upstream:docs]: https://github.com/bblanchon/ArduinoTrace#arduinotrace
