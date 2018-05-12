# Kaleidoscope-HostPowerManagement

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-HostPowerManagement.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-HostPowerManagement

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

Support performing custom actions whenever the host suspends, resumes, or is
sleeping. Additionally, this plugin provides optional support for the keyboard
to wake the host up from suspend.

## Using the plugin

To use the plugin, one needs to include the header, and activate it. No further
configuration is necessary, unless one wants to perform custom actions.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-HostPowerManagement.h>

KALEIDOSCOPE_INIT_PLUGINS(HostPowerManagement);

void setup () {
  Kaleidoscope.setup ();

  HostPowerManagement.enableWakeup();
}
```

## Plugin methods

The plugin provides the `HostPowerManagement` object, which has the following methods:

### `.enableWakeup()`

> Enables host wakeup support. When enabled, pressing any key on the keyboard
> will wake the host up.
>
> Once enabled, it **cannot** be disabled again.

## Overrideable methods

### `hostPowerManagementEventHandler(event)`

> The `hostPowerManagementEventHandler` method is the brain of the plugin: this function
> tells it what action to perform in response to the various events.
>
> Currently supported events are: `kaleidoscope::HostPowerManagement::Suspend` is fired
> once when the host suspends; `kaleidoscope::HostPowerManagement::Sleep` is fired every
> cycle while the host is suspended; `kaleidoscope::HostPowerManagement::Resume` is
> fired once when the host wakes up.
>
> The default implementation is empty.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-HostPowerManagement/blob/master/examples/HostPowerManagement/HostPowerManagement.ino
