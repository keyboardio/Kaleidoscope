# Kaleidoscope v2.0

> Currently at 1.92-beta, in development!

See [UPGRADING.md](UPGRADING.md) for more detailed instructions about upgrading from earlier versions (even earlier betas). This is just a list of noteworthy changes.

* [New features](#new-features)
* [New plugins](#new-plugins)
* [Breaking changes](#breaking-changes)
* [Bugfixes](#bugfixes)

## New features

### KALEIDOSCOPE_API_VERSION bump

`KALEIDOSCOPE_API_VERSION` has been bumped to **2** due to the plugin API
changes mentioned below. It does not mean that version two of the API is final,
though. The bump is there so plugins can check it, and make compile-time
decisions based on it. Such as whether to compile for the version one, or for
the version two API.

The API version will remain the same, even if we introduce breaking changes -
until a stable release is made from the v2 branch. From that point onwards, the
API version will change with further breaking changes.

### New plugin API

A new plugin API was introduced in May 2018, which replaces the old system. The new system is hopefully easier to use and develop for:

 - It does not require one to register / use hooks anymore. Implementing the interface provided by `kaleidoscope::Plugin` is all that is required.
 - The new system has more hook points, and the method names are much more clear now.

Plugins under Keyboardio control have all been updated to use the new API, and they no longer support the older one.

See [UPGRADING.md](UPGRADING.md#new-plugin-api) for more information.

### Transition to a monorepo

We heard a lot of complaints that while the plugin architecture of Kaleidoscope is great, having so many plugins scattered around in dozens of repositories is a huge barrier of entry for potential contributors, and a significant pain point for end-users to update. For these reasons and more, we merged almost all plugins into the Kaleidoscope repository.

While at first it may seem that this is a move towards a monolithic architecture, rest assured, it is not. The plugin APIs are still a core part of Kaleidoscope, it isn't going anywhere. We merely rearranged the sources, is all. Nothing else changes.

Some headers and names did change, however, see [UPGRADING.md](UPGRADING.md#source-code-and-namespace-rearrangement) for more information.

### Bidirectional communication for plugins

The bi-directional communication protocol formerly implemented by `Kaleidoscope-Focus` has been partially pulled into core, using the new plugin system mentioned above. The new system makes it a lot easier for both end-users and developers to use the feature.

See [UPGRADING.md](UPGRADING.md#bidirectional-communication-for-plugins) for more information.

### Consistent timing

Numerous plugins use timers, most of them directly calling `millis()`. This has the disadvantage that calls within a main loop cycle will be inconsistent, which makes timing synchronization across plugins hard. The newly introduced `Kaleidoscope.millisAtCycleStart()` function helps dealing with this issue.

See [UPGRADING.md](UPGRADING.md#consistent-timing) for more information.

### USB detach / attach

It is now possible to detach, and re-attach the USB link from/to the host, without resetting the device. The intent of this feature (as implemented by the `Kaleidoscope.detachFromHost()` and `Kaleidoscope.attachToHost()` methods) is to allow configuration changes without rebooting.

See the [Kaleidoscope-USB-Quirks][plugin:USB-Quirks] plugin for a use-case.

 [plugin:USB-Quirks]: https://github.com/keyboardio/Kaleidoscope-USB-Quirks

## New plugins

### IdleLEDs

The [IdleLEDs](doc/plugin/IdleLEDs.md) plugin is a simple, yet, useful one: it will turn the keyboard LEDs off after a period of inactivity, and back on upon the next key event.

## Breaking changes

### The `RxCy` macros and peeking into the keyswitch state

The `RxCy` macros changed from being indexes into a per-hand bitmap to being an index across the whole keyboard. This mostly affected the [MagicCombo](doc/plugin/MagicCombo.md) plugin.

Please see the [relevant upgrade notes](UPGRADING.md#the-rxcy-macros-and-peeking-into-the-keyswitch-state) for more information.

## Bugfixes

We fixed way too many issues to list here, so we're going to narrow it down to the most important, most visible ones.

### Support for BIOSes, EFI, login prompts, etc

Keyboards report keys pressed to the host via either of two protocols: the boot protocol, or the report protocol. The boot protocol is the simpler, and it is what older BIOSes, EFI, and certain OS login prompts (or hard disk password prompts and the like) require. Until recently, the firmware wasn't able to provide this protocol, only the more advanced report one, which is required for N-key roll-over.

We now support the boot protocol, and on operating systems that fully conform to the USB specification, this works automatically. For all others, one can implement a way to force one mode or the other. See the [factory firmware][Model01-Firmware] for an example how to achieve this.

 [Model01-Firmware]: https://github.com/keyboardio/Model01-Firmware
