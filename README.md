# Kaleidoscope-Qukeys

![status][st:experimental] [![Build Status][travis:image]][travis:status]

 [travis:image]: https://travis-ci.org/gedankenlab/Kaleidoscope-Qukey.svg?branch=master
 [travis:status]: https://travis-ci.org/gedankenlab/Kaleidoscope-Qukey

 [st:stable]: https://img.shields.io/badge/stable-✔-black.svg?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.svg?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.svg?style=flat&colorA=dfb317&colorB=494e52

## Concept

This Kaleidoscope plugin will allow you to overload keys on your
keyboard so that they produce one keycode (i.e. symbol) when tapped,
and a different keycode -- most likely a modifier (e.g. `shift` or
`alt`) when held. There are already two Kaleidoscop plugins that
provide this same functionality
([DualUse](https://github.com/keyboardio/Kaleidoscope-DualUse) and
[SpaceCadet](https://github.com/keyboardio/Kaleidoscope-SpaceCadet)),
but those plugins were designed primarily to allow you to overload
keys whose primary function is as a modifier, and use them to produce
printable keycodes when tapped. The `Qukey` design is different; it's
meant to overload letter keys that are usually tapped and let you use
them as alternate modifier keys (though the design is flexible enough
that any keycode can be the secondary one -- I just haven't thought of
a realistic use for that yet).

## Design goals

* I want users to be able to type at fast pace on `Qukey`s, without
  accidentally triggering the secondary function, and preserving the
  order of input keystrokes.

* I want users to be able to rapidly invoke the secondary function of
  a `Qukey` without having to wait for a timeout.

* I want `Qukey`s to be useful as modifiers with other input devices
  (e.g. a mouse), without having to wait a long time for a
  timeout. I'm guessing that 200ms is acceptable, but not anything
  longer than that.

* I want physical *keys* on the keyboard to be defined independently
  of each other. I don't want the plugin to act on *keycodes*; simply
  translating one keycode to another.

## Schrödinger's Key

The idea is that a `Qukey` will work just like a particle in a
superposition of quantum states until some event causes the "waveform"
to "collapse" into just one of the two states. (The name "qukey" is a
reference to "qubit", a term used in quantum computing.)

When a `Qukey` is pressed, its state will be indeterminate (like a
superposition of quantum states); no keycode for that key will be
transmitted to the host until some other event (or sequence of events)
causes the state to be decided. Once the state is decided, the `Qukey`
will stay in that state until the key is released. The possible
triggers for "collapsing" the state of a `Qukey` are:

* Timeout

* Release of the `qukey`

* Press and subsequent release of another key

### Timeout

When a `qukey` times out, it falls into its secondary state (probably
a modifier).The timeout should be fairly short ­ just a bit longer
than the longest "reasonable" time of a tap, such that you very rarely
(ideally never) hold a `qukey` long enough to trigger the secondary
function when intending a tap, but short enough that you don't feel a
need to wait a while to use it with a mouse.

### Release of `qukey`

If the `qukey` is released before it times out, and before any other
keys are pressed, it falls into its primary state. This is slightly
different from the timeout case, because we then add the `qukey`'s
primary keycode to the HID report when the key toggles off (rather
than when it toggles on, which is the behaviour of a normal key). It
will get cleared at the end of the cycle, but there's still no need to
send any extra HID reports.

### Interactions with subsequent keys

This is where things get tricky. Because fast typists might have
multiple keys pressed simultaneously, we need to keep track of a
sequence of subsequent keypresses, starting with the first `qukey` to
toggle on. Basically, if any subsequent key is released before a
`qukey` that was pressed before it, that `qukey` should fall into its
secondary state (e.g. a modifier).

In order to do this, we need to suppress the output of any subsequent
keys, as well as the `qukey`. Basically, this means storing a list of
keys that have been pressed, but which have not affected the report
yet. This also means that when a key in that list toggles off, every
`qukey` preceding it in the list has its state decided.

When a subsequent key is released, we now need to send a series of HID
reports to preserve the order that the keypresses were detected by the
keyboard. To do that, we add the one keycode at a time to the report
and send it, until we reach the next `qukey` in the list that has an
indeterminate state.

The really tricky part is that the key *releases* might come in any
order, so if we have a sequence that goes like this, we could get into
trouble:

1. press `qk(A,shift)`
2. press `B`
3. press `C`
4. press `qk(D,ctrl)`
5. press `space`
6. release `B`

In this case, `B` is released after the second `qukey` was pressed,
but I don't think it's actually a problem. Maybe.

We might also send a series of reports when a `qukey` times out, of
course, but this is much more straightforward.

### Another way to address overlap

An alternative method is to only allow the `qukey` to fall into its
secondary (modifier) state if there is only one non-`qukey` press
following. So, in the previous example, once step 3 was reached, the
`qukey` would become `A`. I don't think this works as well, but it
would err more on the side of defaulting to the primary key, which
would mean fewer unintended modifiers while typing, and enforce
(slightly) more deliberate usage to get the secondary keycode. This
would make for another interesting possibility, though divergent from
the idea that I started with ­ the state could change while the key is
pressed:

1. press `qk(A,shift)`
2. press `B`
3. press `C` -> output: `abc`
4. release `B` -> output: `shift` held?

It wouldn't require any additional data structures, so the algorithm
to use could be user-configurable.

## Data structures

I want to store a table, indexed by key id (layer, row & column). Each
entry would be an object containing:

* primary keycode (or `Key` object)
* secondary keycode (or `Key` object)
* time limit (set when key toggles on, based on timeout)
* flags (bitfield ­ maybe useful)

I think I need the flags byte in order to signal which state the
`qukey` has once its state has collapsed, but maybe I can get away
with just using the time limit value. I could set that to zero (or
maybe some other constant?) to signal that the `qukey` is in its
primary state, and if the state collapses to secondary before the
timeout, just reset the time limit to `millis()`. Or maybe I never
need that, because it should only send the primary keycode as a "tap"
in a single report.

In addition, I need to store a list of keypresses, ideally in
something like `std::queue`, though I think that's not available to
Arduino (it looks like someone has written a `QueueArray` library,
though). This would just be a FIFO buffer of key ids (row & column ­
maybe layer, too, but that might even be undesirable, because we might
try to expand `Qukey` to allow layer switching, too).