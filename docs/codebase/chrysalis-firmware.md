# Chrysalis-enabled firmware

In this document, we'll go over what is required - at a minimum - to let a
sketch work with Chrysalis, assuming the hardware itself is supported by it.

## Basic requirements

Any sketch that wants to work with Chrysalis has to have at a mininum, the
[FocusSerial][plugin:focus-serial] and [EEPROM-Settings][plugin:eeprom-settings]
libraries included, and the `Focus`, `FocusEEPROMCommand`,
`FocusSettingsCommand` (provided by `FocusSerial`), and `EEPROMSettings`
(provided by `EEPROM-Settings`) plugins enabled, by listing them in
`KALEIDOSCOPE_INIT_PLUGINS()`.

To edit the keymap, the sketch will also need
[EEPROM-Keymap][plugin:eeprom-keymap] included, enabled, and configured. To have
colormap editing support, [Colormap][plugin:colormap] is also required. If
`Colormap` is enabled, its dependencies,
[LEDPaletteTheme][plugin:led-palette-theme] and [LEDControl][plugin:led-control]
must also be enabled.

These are the absolute essentials to have basic functionality in Chrysalis.
There are a lot of other plugins included in most Chrysalis-enabled firmware
that further enhance the configurability. For guidelines about what to include
in sketches that are intended to be shipped with Chrysalis, see the next
section.

## Firmware shipped with Chrysalis

Firmware we ship with Chrysalis should come with reasonable defaults, nothing
surprising, strange, or uncommon should be enabled by default, unless users of
the device can be reasonably expected to know about these features. One such
case is when Kaleidoscope is not the default on a particular device, and the
firmware we ship with Chrysalis mimics the original default firmware. Whatever
features the original firmware had, are safe to include in the
Kaleidoscope-based one too, if Kaleidoscope supports a given feature.

With that said, the recommended list of features in a Chrysalis-enabled firmware are:

### The Basics

At a minimum, we need to support the keyboard's basic features. As such, we'll
need the required plugins first: `Focus`, `FocusEEPROMCommand`, and
`FocusSettingsCommand` (provided by [FocusSerial][plugin:focus-serial]), and
`EEPROMSettings` (provided by [EEPROM-Settings][plugin:eeprom-settings]).

Because we're talking keyboards, we will also need `EEPROMKeymap`, as provided
by the [EEPROM-Keymap][plugin:eeprom-keymap] plugin.

If the device has per-key LEDs, the sketch should also include the `LEDOff` and
`LEDControl` (provided by `LEDControl`), `LEDPaletteTheme`
(provided by [LED-Palette-Theme][plugin:led-palette-theme]), `ColormapEffect`
and `DefaultColormap` plugins, provided by the [Colormap][plugin:colormap]
plugin. The latter is optional, depending on whether there's enough space on the
device to include a default palette and colormap.

The number of layers configured for `ColormapEffect` and `EEPROMKeymap` **must**
be identical, and the number of default layers should leave ample opportunity
for the end-user to customize them. They should not be restricted to the number
of layers the keyboard ships with out of the box, unless storage space
restrictions prevent us from having more. A sensible default is eight layers for
both, as that is the limit the Chrysalis-configurable secondary actions and
one-shot keys support.

If the hardware supports it, including the `HardwareTestMode` plugin is highly
recommended, likewise for [HostPowerManagement][plugin:host-power-management].

When `HostPowerManagement` is included, and the hardware has LEDs, then the
sketch should be set up so that upon suspending, the LEDs are turned off, and
they're turned back on when the host is resumed.

### Additional Features

Space permitting, the following features are recommended to be included in any
firmware we expect to ship with Chrysalis, roughly in order of importance:

- Mouse keys via the [MouseKeys][plugin:mousekeys] plugin.
- Secondary actions via [Qukeys][plugin:qukeys].
- OneShot functionality via [OneShot][plugin:oneshot] and `EscapeOneShot`, and `EscapeOneShotConfig` (provided by [EscapeOneShot][plugin:escape-oneshot]).
- Default LED mode configurability via
  [DefaultLEDModeConfig][plugin:default-led-mode-config], if the device supports
  LEDs.
- Automatically turning LEDs off after some time, via `IdleLEDs` and
  `PersistentIdleLEDs`, provided by th [IdleLEDs][plugin:idle-leds] plugin.
- Chrysalis-editable macros via [DynamicMacros][plugins:dynamic-macros],
  configured to leave a fair amount of storage space available for macros, space
  permitting. Around 512 bytes is a reasonable minimum, with 4096 bytes being a
  very generous amount. At this time, more than 4k storage space reserved for
  macros is likely unnecessary.

All of these should be configured for sensible, not surprising behaviour.
Additional plugins on top of these may be enabled if they make sense, but the
principle of least surprise should always be kept in mind.

 [plugin:focus-serial]: ../plugins/FocusSerial.md
 [plugin:eeprom-settings]: ../plugins/EEPROM-Settings.md
 [plugin:eeprom-keymap]: ../plugins/EEPROM-Keymap.md
 [plugin:colormap]: ../plugins/Colormap.md
 [plugin:led-palette-theme]: ../plugins/LED-Palette-Theme.md
 [plugin:host-power-management]: ../plugins/HostPowerManagement.md
 [plugin:mousekeys]: ../plugins/MouseKeys.md
 [plugin:qukeys]: ../plugins/Qukeys.md
 [plugin:oneshot]: ../plugins/OneShot.md
 [plugin:escape-oneshot]: ../plugins/Escape-OneShot.md
 [plugin:default-led-mode-config]: ../plugins/DefaultLEDModeConfig.md
 [plugin:idle-leds]: ../plugins/IdleLEDs.md
 [plugin:dynamic-macros]: ../plugins/DynamicMacros.md
