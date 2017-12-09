# Kaleidoscope-MyOldFriend

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-MyOldFriend.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-MyOldFriend

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

> Hello darkness, my old friend
> I've come to talk with you again

Support performing custom actions whenever the host suspends, resumes, or is
sleeping. By default, the LEDs will be turned off on suspend, and the previous
LED mode restored on resume.

## Using the plugin

To use the plugin, one needs to include the header, and activate it. No further
configuration is necessary, unless one wants to perform custom actions.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-MyOldFriend.h>

void setup () {
  Kaleidoscope.setup ();

  Kaleidoscope.use(&MyOldFriend);
}
```

## Plugin methods

The plugin provides the `MyOldFriend` object, which has the following methods:

### `.toggleLEDs(event)`

> Turns LEDs off on suspend, restores the previous LED mode on resume. This is
> called by `myOldFriendEventHandler()` by default.

## Overrideable methods

### `myOldFriendEventHandler(event)`

> The `myOldFriendEventHandler` method is the brain of the plugin: this function
> tells it what action to perform in response to the various events.
>
> Currently supported events are: `kaleidoscope::MyOldFriend::Suspend` is fired
> once when the host suspends; `kaleidoscope::MyOldFriend::Sleep` is fired every
> cycle while the host is suspended; `kaleidoscope::MyOldFriend::Resume` is
> fired once when the host wakes up.
>
> The default implementation calls `MyOldFriend.toggleLEDs`. When overriding the
> function, the default is lost.

## Dependencies

* [Kaleidoscope-LEDControl](https://github.com/keyboardio/Kaleidoscope-LEDControl)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-MyOldFriend/blob/master/examples/MyOldFriend/MyOldFriend.ino
