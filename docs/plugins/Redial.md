# Redial

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

// Place Key_Redial somewhere on the keymap...

KALEIDOSCOPE_INIT_PLUGINS(Redial);

void setup() {
  Kaleidoscope.setup();
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

## Dependencies

* [Kaleidoscope-Ranges](Ranges.md)

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/Redial/Redial.ino
