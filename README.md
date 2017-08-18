# Kaleidoscope-CycleTimeReport

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-CycleTimeReport.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-CycleTimeReport

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

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

void setup (void) {
  Serial.begin(9600);
  Kaleidoscope.use (&CycleTimeReport);

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

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-CycleTimeReport/blob/master/examples/CycleTimeReport/CycleTimeReport.ino
