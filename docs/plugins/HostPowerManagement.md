# HostPowerManagement

Support performing custom actions whenever the host suspends, resumes, or is
sleeping.

## Using the plugin

To use the plugin, one needs to include the header, and activate it. No further
configuration is necessary, unless one wants to perform custom actions.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-HostPowerManagement.h>

KALEIDOSCOPE_INIT_PLUGINS(HostPowerManagement);

void setup () {
  Kaleidoscope.setup ();
}
```

## Plugin methods

The plugin provides the `HostPowerManagement` object, with no public methods.

## Overrideable methods

### `hostPowerManagementEventHandler(event)`

> The `hostPowerManagementEventHandler` method is the brain of the plugin: this function
> tells it what action to perform in response to the various events.
>
> Currently supported events are:
> `kaleidoscope::plugin::HostPowerManagement::Suspend` is fired once when the
> host suspends; `kaleidoscope::plugin::HostPowerManagement::Sleep` is fired
> every cycle while the host is suspended;
> `kaleidoscope::plugin::HostPowerManagement::Resume` is fired once when the
> host wakes up.
>
> The default implementation is empty.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Features/HostPowerManagement/HostPowerManagement.ino

## Caveats

On some systems, there can be a long delay between suspending/sleeping the host and the
firmware responding to it and calling `hostPowerManagementEventHandler()`. In particular,
on macOS, it can take 30 seconds or more after invoking "sleep" mode on the host before
the keyboard responds. One user reports that it can take more than a minute, so if this
plugin doesn't appear to be working, please wait a few minutes and check again.

