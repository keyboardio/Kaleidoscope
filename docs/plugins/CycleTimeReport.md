# CycleTimeReport

A development and debugging aid, this plugin will measure average mainloop times
(in microseconds) and print it to `Serial` periodically. While not the most
reliable way to measure the speed of processing, it gives a reasonable
indication nevertheless.

## Using the plugin

The plugin comes with reasonable defaults (see below), and can be used out of
the box, without any further configuration:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-CycleTimeReport.h>

KALEIDOSCOPE_INIT_PLUGINS(CycleTimeReport);

void setup (void) {
  Kaleidoscope.serialPort().begin(9600);
  Kaleidoscope.setup ();
}
```

## Plugin methods

The plugin provides a single object, `CycleTimeReport`, with the following
property. All times are in milliseconds.

### `.average_loop_time`

> A read-only by contract value, the average time of main loop lengths between
> two reports.

## Overrideable methods

### `cycleTimeReport()`

> Reports the average loop time. By default, it does so over `Serial`, every
> time when the report period is up.
>
> It can be overridden, to change how the report looks, or to make the report
> toggleable, among other things.
>
> It takes no arguments, and returns nothing, but has access to
> `CycleTimeReport.average_loop_time` above.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Features/CycleTimeReport/CycleTimeReport.ino
