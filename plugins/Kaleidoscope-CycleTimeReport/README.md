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

void setup () {
  Kaleidoscope.serialPort().begin(9600);
  Kaleidoscope.setup ();
}
```

## Plugin methods

The plugin provides a single object, `CycleTimeReport`, with the following
methods:

### `.setReportInterval(interval)`

> Sets the length of time between reports to `interval` milliseconds.  The
> default is `1000`, so it will report once per second.

### `.report(mean_cycle_time)`

> Reports the average (mean) cycle time since the previous report.  This method
> is called automatically, once per report interval (see above).  By default, it
> does so over `Serial`.
>
> It can be overridden, to change how the report looks, or to make the report
> toggleable, among other things.
>
> It takes no arguments, and returns nothing.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Features/CycleTimeReport/CycleTimeReport.ino
