# Kaleidoscope-Redial

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/keyboardio/Kaleidoscope-Redial.svg?branch=master
 [travis:status]: https://travis-ci.org/keyboardio/Kaleidoscope-Redial

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

If you ever wanted to just repeat the last key pressed, no matter what it was,
this plugin is made for you. It allows you to configure a key that will repeat
whatever the last previously pressed key was. Of course, one can limit which
keys are remembered...

## Using the plugin

To use the plugin, we'll need to enable it, and configure a key to act as the
"redial" key. This key should be on the keymap too.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-Redial.h>
#include <kaleidoscope-Ranges.h>

enum {
  REDIAL = kaleidoscope::ranges::SAFE_START,
};
#define Key_Redial (Key) {.raw = REDIAL}

// Place Key_Redial somewhere on the keymap...

KALEIDOSCOPE_INIT_PLUGINS(Redial);

void setup() {
  Kaleidoscope.setup();

  Redial.key = Key_Redial;
}
```

## Overrideable plugin methods

### `bool shouldRemember(Key mapped_key)`

> If one wants to change what keys the plugin remembers, simply override the
> `kaleidoscope::Redial::shouldRemember` function. Whenever a key is to be
> remembered, this function will be called with the key as argument. It should
> return `true` if the key should be remembered (and repeated by Redial),
> `false` otherwise.
>
> By default, the plugin will remember alphanumeric keys only.

## Plugin properties

The `Redial` object has only one property, the key to trigger it.

### `.key`

> The key to trigger the redial effect. Be aware that whatever key you specify
> here, will have its action shadowed by the redial functionality. Choose
> something unused, see the example sketch for one way to do that.
>
> There is no default.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Kaleidoscope-Redial/blob/master/examples/Redial/Redial.ino
