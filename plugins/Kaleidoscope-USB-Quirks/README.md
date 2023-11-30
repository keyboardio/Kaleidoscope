# USB-Quirks

USB-Quirks provides a few methods to deal with more obscure parts of the USB spec, such as changing the behavior around the boot protocol. These are in a separate plugin, because these features are not part of the USB spec, and are often workarounds for various issues. See the provided methods for more information about what they're useful for.

## Using the plugin

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-Macros.h>
#include <Kaleidoscope-USB-Quirks.h>

KALEIDOSCOPE_INIT_PLUGINS(USBQuirks, Macros);

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == 0) {
    USBQuirks.toggleKeyboardProtocol();
  }
  return MACRO_NONE;
}

void setup() {
  Kaleidoscope.setup();
}
```

## Plugin methods

The plugin provides one object, `USBQuirks`, which provides the following method:

### `.toggleKeyboardProtocol()`

> Toggle whether the keyboard is able to send extended key reports (the
> default), or instead always sends boot reports, regardless of the
> protocol requested by the host. Switching the toggle causes the keyboard
> to detach and then re-attach to the host. (This re-attach is necessary to
> force re-enumeration with a different Report Descriptor.)
>
> Switching the toggle also lights up a key indicating the mode being
> switched to: by default, `B` for boot reports only, and `N` for extended
> reports enabled.
>
> The extended key report supports n-key rollover (NKRO), and is actually a
> hybrid, having a prefix containing the boot report, for compatibility
> with older hosts. The boot report only supports 6-key rollover (6KRO),
> and is meant to support constrained hosts, such as BIOS, UEFI, or other
> pre-boot environments. The keyboard changes protocols as requested by the
> host.
>
> The USB HID specification requires that hosts explicitly request boot
> protocol if they need it, and that devices default to the non-boot
> protocol. Some hosts do not follow the specification, and expect boot
> protocol, even without requesting it. The backwards compatibility prefix
> of the hybrid extended report should accommodate some of these hosts.
> This toggle helps to work with hosts that neither request boot protocol
> nor tolerate the longer hybrid report.

### `.setKeys(Key boot_led, Key nkro_led)`

> Set which keys to light up to indicate the target mode. Defaults to
> `(Key_B, Key_N)`.
