# Unicode

The `Unicode` extension makes it easier to write plugins that input Unicode
symbols on the host. Because inputting Unicode varies from OS to OS, this helper
library was made to hide most of the differences. All one has to do, is set up
the `HostOS` singleton properly, and the `Unicode` library will handle the rest,
by providing an easy interface for inputting Unicode symbols by their 32-bit
codepoints.

## Using the extension

Using the extension is as simple as including the header, registering it with
`Kaleidoscope.use()`, and then using any of the methods provided by the
`Unicode` singleton object.

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-EEPROM-Settings.h>
#include <Kaleidoscope-HostOS.h>
#include <Kaleidoscope-Unicode.h>

KALEIDOSCOPE_INIT_PLUGINS(EEPROMSettings, HostOS, Unicode);

void setup() {
  Kaleidoscope.setup();

  Unicode.type(0x2328);
}
```

## Extension methods

The extension provides a number of methods on the `Unicode` object, but also has
symbols that can be [overridden](#overrideable-methods), to add custom
functionality.

### `.type(code_point)`

> Starts the Unicode input method using the [`.start()`](#start) method, inputs
> the `code_point` using [`.typeCode()`](#typeCode), and finishes up with
> the [`.end()`](#end) method. For each hexadecimal digit sent to the host,
> the [`.input()`](#input) method will also be called.
>
> This method is most useful when one knows the code point of the Unicode symbol
> to enter ahead of time, when the code point does not depend on anything else.

### `.typeCode(code_point)`

> Inputs the hex codes for `code_point`, and the hex codes only. Use when the
> input method is to be started and ended separately.
>
> For example, a macro that starts Unicode input, and switches to a layer full
> of macros that send the hex codes is one scenario where this function is of
> use.

### `.start()`

> Starts the Unicode input method. The way it starts it, depends on the host
> operating system.

### `.input()`

> If the host operating system requires keys being held during the Unicode
> input, this function will hold them for us.

### `.end()`

> Finishes the Unicode input method, in an OS-specific way.

### `.input_delay([delay])`

> Sets or returns (if called without an argument) the number of milliseconds to
> wait between inputting each part of the sequence. In some cases, inputting too
> fast does not give the host enough time to process, and a delay is needed.
>
> Defaults to zero, no delay.

## Overridable methods

### `hexToKey(hex_digit)`

> A function that returns a `Key` struct, given a 8-bit hex digit. For most
> uses, the built-in version of this function is sufficient, but if the keymap
> on the OS-side has any of the hexadecimal symbols on other scancodes than
> QWERTY, this function should be overridden to use the correct scan codes.

### `unicodeCustomStart()`

> If the host OS type is set to `kaleidoscope::hostos::Custom`, then this function will
> be called whenever the [`.start()`](#start) method is called. The default
> implementation does nothing, and should be overridden to implement the custom
> magic needed to enter unicode input mode.

### `unicodeCustomInput()`

> If the host OS type is set to `kaleidoscope::hostos::Custom`, then this function will
> be called whenever the [`.input()`](#input) method is called. The default
> implementation does nothing, and should be overridden to implement the custom
> magic needed while inputting the hex code itself (such as holding additional
> keys).

### `unicodeCustomEnd()`

> If the host OS type is set to `kaleidoscope::hostos::Custom`, then this function will
> be called whenever the [`.end()`](#end) method is called. The default
> implementation does nothing, and should be overridden to implement the custom
> magic needed to leave unicode input mode.

## Dependencies

* [Kaleidoscope-HostOS](Kaleidoscope-HostOS.md)

## Other Configuration

On OS X/macOS, you'll need to change the input method to be "Unicode Hex Input".
You can do this by going to System Preferences > Keyboard > Input Sources, clicking
the `+` button, selecting it from the list, then setting it as the active input method.

On Windows, you will need to change a registry key to enable the input method
our unicode plugin uses. Under `HKEY_Current_User/Control Panel/Input Method`,
set `EnableHexNumpad` to `"1"`. If the key does not exist, you need to create
it, and use `REG_SZ` as the type.

## Further reading

Starting from the [example][plugin:example] is the recommended way of getting
started with the plugin.

 [plugin:example]: /examples/Keystrokes/Unicode/Unicode.ino
