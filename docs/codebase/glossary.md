# Glossary

This document is intended to name and describe the concepts, functions and data structures inside Kaleidoscope.

It is, as yet, incredibly incomplete.

When describing an identifier of any kind from the codebase, it should be
written using identical capitalization to its use in the code and surrounded by backticks: `identifierName`

## Firmware Terminology

These terms commonly arise when discussing the firmware.

### Keyswitch

A single physical input, such as a keyswitch or other input like a knob or a slider

### Key number

An integer representing a Keyswitch’s position in the “Physical Layout”. Represented in the code by the `KeyAddr` type.

### Physical Layout

A mapping of keyswitches to key numbers

### Key binding

A mapping from a key number to a behavior. 

### `Key`

A representation of a specific behavior. Most often a representation of a specific entry in the USB HID Usage Tables.

### Keymap

A list of key bindings for all keyswitchess on the Physical Layout. Represented in the code by the `KeyMap` type.

### Keymaps

An ordered list of all the Keymaps installed on a keyboard. 

### Layer

An entry in that ordered list of keymaps. Each layer has a unique id number that does not change. Layer numbers start at 0.

### Active Layer Stack

An ordered list of all the currently-active layers, in the order they should be evaluated when figuring out what a key does.

### Live keys

A representation of the current state of the keyboard's keys, where non-transparent entries indicate keys that are active (logically—usually, but not necessarily, physically held). Represented in the code by the `LiveKeys` type (and the `live_keys` object).


## Keyswitch state

### Pressed

The state of a keyswitch that has been actuated by the user or a routine acting on behalf of the user 

### Unpressed

The state of a keyswitch that is not currently actuated

### Toggled on

The state of a keyswitch that was not pressed during the last scan cycle and is now pressed.

### Toggled off

The state of a keyswitch that was pressed during the last scan cycle and is no longer pressed.




### Cycle

The `loop` method in one's sketch file is the heart of the firmware. It runs -
as the name suggests - in a loop. We call these runs cycles. A lot of things
happen within a cycle: from key scanning, through key event handling, LED
animations, and so on and so forth.

### Event handler

A function, usually provided by a [`Plugin`](#plugin) that is run by a [`Hook`](#hook).

At the time of this writing, the following event handlers are run by hooks:

 - `onSetup`: Run once, when the plugin is initialised during
   `Kaleidoscope.setup()`.
 - `beforeEachCycle`: Run as the first thing at the start of each [cycle](#cycle).
 - `onKeyswitchEvent`: Run for every non-idle key, in each [cycle](#cycle) the
   key isn't idle in. If a key gets pressed, released, or is held, it is not
   considered idle, and this event handler will run for it too.
 - `beforeReportingState`: Runs each [cycle](#cycle) right before sending the
   various reports (keys pressed, mouse events, etc) to the host.
 - `afterEachCycle`: Runs at the very end of each [cycle](#cycle).

### Hook

A point where the core firmware calls [event handlers](#event-handler), allowing
[plugins](#plugin) to augment the firmware behaviour, by running custom code.

### Plugin

An Arduino library prepared to work with Kaleidoscope. They implement methods
from the `kaleidoscope::Plugin` (usually a subset of them). See [event
handlers](#event-handler) above for a list of methods.

## Testing

These terms arise when discussing the testing framworks and related tests.

### Sim Harness

An abstraction used in testing to inject events into or invoke actions on the
simulated firmware. This abstraction comprises half of the interface to the
[test simulator](#test-simulator).

### Sim State

An abstraction used in testing to encapsulate, snapshot, and examine firmware
state. This abstraction comprises half of the interface to the [test
simulator](#test-simulator).

### Test

An indivial assertion or expectation that must hold for a [test
case](#test-case) to pass.

### Test Case

An indivual `TEST*` macro invocation. Its body consists of one or [tests](#test)
and optionally other code, e.g. to invoke the [test harness](#test-harness).
Note that gtest uses the non-standard term _Test_ for what we call a [Test
Case](#test-case).

### Test File

An individual file containing one or more [test suites](#test-suite).

### Test Fixture

A class comprising setup, teardown, and other code and common state to make
writing [test cases](#test-case) easieer. A fresh object of the fixture class
associated with a [test suite](#test-suite) is constructed for each run of each
[teset case](#test-case) in the [test suite](#test-suite).

### Test Simulator

An abstraction wrapping a virtual firmware build that allows performing
actions against the virtual firmware and reading state out of the virtual
firmware. The interface to the test simular is comprised of the [sim
harness](#sim-harness) and the [sim state](#sim-state).

### Test Suite

A collection of related [test cases](#test-case), optionally with an associated
[test fixture](#test-fixture).
