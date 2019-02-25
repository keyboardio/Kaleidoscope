# Glossary

This document is intended to name and describe the concepts, functions and data structures inside Kaleidoscope.

It is, as yet, incredibly incomplete.

Entries should be included in dictionary order. When describing an identifier of any kind from the codebase, it should be
written using identical capitalization to its use in the code and surrounded by backticks: `identifierName`

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
 - `onKeyswitchEvent2`: Run for every non-idle key, in each [cycle](#cycle) the
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
