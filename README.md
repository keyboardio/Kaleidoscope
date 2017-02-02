# Akela-Syster

![status][st:experimental]

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

Syster is a way to input symbols in a different way: instead of macros, Leader
sequences or the like, we trigger the special input mode, and enter the symbol's
name. Once finished, we hit `Space`, and this plugin will do the rest: delete
everything we typed, look up an action for the entered symbol, and execute that.

There are a number of ways this can be useful, but the easiest showcase is
symbolic Unicode input: `SYSTER coffee SPACE` turns into `☕`, witch just a
little code.

## Using the plugin

To use the plugin, one needs to include the header, and set up a function that
will handle the symbol actions:

```c++
#include <Akela-Syster.h>
#include <Akela-Unicode.h>

void systerAction (Akela::Syster::action_t action, const char *symbol) {
  switch (action) {
  case Akela::Syster::StartAction:
    Unicode.type (0x2328);
    break;
  case Akela::Syster::EndAction:
    handle_key_event (Key_Backspace, 255, 255, IS_PRESSED | INJECTED);
    Keyboard.sendReport ();
    handle_key_event (Key_Backspace, 255, 255, WAS_PRESSED | INJECTED);
    Keyboard.sendReport ();
    break;
  case Akela::Syster::SymbolAction:
    Serial.print ("systerAction: symbol=");
    Serial.println (symbol);
    if (strcmp (symbol, "coffee") == 0) {
      Unicode.type (0x2615);
    }
    break;
  }
}

void setup () {
  Serial.begin (9600);

  Keyboardio.setup (KEYMAP_SIZE);
  Keyboardio.use (&Unicode, &Syster, NULL);
}
```

**Note** that we need to use the `Syster` object before any other that adds or
changes key behaviour! Failing to do so may result in unpredictable behaviour.

## Plugin methods

The plugin provides the `Syster` object, with no public methods. There are two
methods outside of the object, however, that can be overridden:

### `systerAction(action, symbol)`

> Called whenever an action needs to be taken, which can happen in three cases:

> First, when the `Syster` key is pressed, and the alternate processing starts.
> In this case, `action` will be set to `Akela::Syster::StartAction`, and
> `symbol` will be `NULL`. This function can be used to do some setup to make it
> more obvious that the Syster input mode is active, such as sending a Unicode
> symbol to the host, or lighting up LEDs, or anything else we'd like.
>
> Second, when the sequence is finished with a `Space`. In this case, `action`
> will be set to `Akela::Syster::EndAction`, and `symbol` will be `NULL`. This
> can be used to undo anything that the start action did, if need be.
>
> Third, when the action for the symbol should be made. In this case, `action`
> is set to `Akela::Syster::SymbolAction`, and `symbol` will be a C string. It
> is up to us, what we do with this information, how we handle it.

### `keyToChar(key)`

> A function that turns a keycode into a character. If using QWERTY on the host,
> the default implementation is sufficient. When using something else, you may
> have to reimplement this function.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: https://github.com/keyboardio/Akela-Syster/blob/master/examples/Syster/Syster.ino
