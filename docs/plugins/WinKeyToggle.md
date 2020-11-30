# WinKeyToggle

If you ever played games on Windows on a traditional keyboard, you likely ran
into the issue of the Windows key: in the heat of the moment, you accidentally
hit the windows key, and find yourself out of the game on the desktop, with the
Start menu open. Annoying, is it? So you'd like to *temporarily* disable the key
while gaming, and this plugin will help you achieve that.

This plugin provides a method to toggle the windows keys on and off. Bind it to
a macro, or a magic combo, and you have an easy way to toggle the key on and
off.

## Using the extension

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-MagicCombo.h>
#include <Kaleidoscope-WinKeyToggle.h>

enum { WINKEY_TOGGLE };

void toggleWinKey(uint8_t combo_index) {
  WinKeyToggle.toggle();
}

USE_MAGIC_COMBOS(
[WINKEY_TOGGLE] = {
  .action = toggleWinKey,
  .keys = {R3C6, R3C9} // Left Fn + Right Fn
});

KALEIDOSCOPE_INIT_PLUGINS(MagicCombo, WinKeyToggle);

void setup() {
  Kaleidoscope.setup();
}
```

## Plugin properties

The extension provides a `WinKeyToggle` singleton object, with the following
method:

### `.toggle`

> Toggles the Windows keys on and off.
>
> Defaults to off.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/WinKeyToggle/WinKeyToggle.ino
