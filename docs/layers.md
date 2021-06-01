# Layers

Layers are an integral part of Kaleidoscope, but a part that is perhaps harder
to master than many other things in the firmware. On these pages, we'll make an
attempt at explaining layers, what you can do with them, how, and a few common
use-cases.

We'll start with a quick use-case guide, before diving deeper into explanations!

## How do I...?

### How do I switch to a layer, so I can type multiple keys from there?

You can use `LockLayer(n)` or `MoveToLayer(n)`, depending on whether you want
other layers to be active at the same time or not. `LockLayer(n)` allows you to
build up a stack of layers, while with `MoveToLayer(n)` only the selected layer
will be active, without any stacking.

### How do I do make layer switching act similar to modifiers?

If you want the layer switch to be active only while the key is held, like in
the case of modifiers, the `ShiftToLayer(n)` method does just that.

While switching layers this way is similar to how modifiers work, there are
subtle differences. For a longer explanation, see
[later](#layers-transparency-and-how-lookup-works).

## Layer theory

First of all, the most important thing to remember is that layers are like a
piece of foil, you can place many of them on top of each other, and see through
uncovered parts. In other words, you can have multiple layers all active at the
same time! As we'll see a few paragraphs later, this can be a very powerful
thing.

To better explain how this works in practice, lets look at what layer-related
keys we can place on the keymap first. Armed with that knowledge, we'll then
explore a few use-cases.

## Layer keys

- `LockLayer(n)`: Locking a layer will activate it when the key toggles on, and
  the layer will remain active until unlocked (with `UnlockLayer(n)` or by
  pressing `LockLayer(n)` again), even if we release the layer key
  meanwhile. Think of it like a `Caps lock` or `Num lock` key.
- `ShiftToLayer(n)`: Unlike `LockLayer`, this only activates the layer until the
  key is held. Once the key is released, the layer deactivates. This behaviour
  is very similar to that of modifiers.
- `MoveToLayer(n)`: Moving to a layer is very similar to locking it, the only
  exception is that moving disables all other layers, so only the moved to layer
  will be active. This allows us to have a less powerful, but simpler way of
  dealing with layers, as we'll see below.
- `Key_KeymapNext_Momentary` / `Key_KeymapPrevious_Momentary`: These activate
  the next or the previous layer, momentarily, like `ShiftToLayer(n)`. What it
  considers `next`, is one layer higher than the currently highest active layer.
  Similarly, `previous` is one below the currently highest active layer.

## Use cases

### Locked layers

Locked layers are most useful when you'll want to spend more time on the target
layer. One such case is the numpad: when using it, we usually want to enter
longer numbers, or use the mathematical operator keys as well. Just imagine
hitting a layer lock key, and the right half of your keyboard turning into a
numpad! It's closer than the numpad on traditional full-size keyboards, thus
less hand movement is required!

### Shifted layers

There are many great examples for shifted layers, such as a symbols layer. Let's
say we have a number of often used symbols which we want easy access to,
preferably near the home row. For example, the various parentheses, brackets and
the like are often used in programming. Having them on the home row is
incredibly convenient. In most cases, we only need this layer for a very short
time, for a symbol or two. As such, locking the layer would be
counter-productive. Instead, we use a layer shift key, like if it was a
modifier.

As a concrete example, let's imagine a small, ortholinear keyboard, like the
Planck. On the bottom row, on the right side of the space bar, we'd have a layer
shift key (lets call that `Fn` for now), that takes us to the symbol layer. On
the symbol layer, we'd have `{`, `}`, `[`, `]`, `(`, and `)` on the home row. To
input `{`, we'd press `Fn + d`, for example. This is still two presses, very
much like `Shift + [`, but the keys are more convenient, because we use stronger
fingers to press them.

Another - and perhaps an even better - example would be a navigation layer, with
cursor keys laid over `WASD`. The reason why this would be a better example, is
because in this case, we often want to use modifiers along with the cursor keys,
such as `Shift` or `Control`. With a shifted layer, if we have transparent keys
at positions where the modifiers are on the base layer, we don't have to repeat
the modifier layout on the shifted layer! This makes it easier to experiment
with one's layout, because if we move modifiers, we only have to do that on one
layer.

### Moving to layers

Moving to a layer is very similar to locking one. The only difference is that
moving disables all other layers. This in turn, has consequences: we can't
return to the previous layer state by repeating the same key.  Unlocking a layer
that has been activated by `MoveToLayer(n)` will instead cause Kaleidoscope to
fall back to the default base layer.

The major advantage of moving to a layer - as opposed to locking one - is the
cognitive load. With moving, there is no transparency.[^1] There is only one layer
active at any given time. It's a simpler concept to grasp.

## Layers, transparency, and how lookup works

The thing that confuses many people about layers is that they can have
transparency. What even is a transparent key? Remember the first paragraphs:
layers are like a foil. They're see-through, unless parts of it are obstructed.
They're like overrides. Any layer you place on top of the existing stack, will
override keys in the layers below.

When you have multiple layers active, to figure out what a key does, the
firmware will first look at the key position on the most recently activated
layer, and see if there's a non-transparent key there. If there is, it will use
that. If there isn't, it will start walking backwards on the stack of _active_
layers to find the highest one with a non-transparent key. The first one it
finds is whose key it will use. If it finds none, then a transparent key will
act like a blank one, and do nothing.[^1]

It is important to note that transparent keys are looked up from active layers
only, from most recently activated to least. Lets consider that we have three
layers, 0, 1, and 2. On a given position, we have a non-transparent key on
layers 0 and 1, but the same position is transparent on layer 2. If we have
layer 0 and 2 active, the key will be looked up from layer 0, because layer 2 is
transparent. If we activate layer 1 too, it will be looked up from there, since
layer 1 is higher in the stack than layer 0. In this case, since we activated
layer 1 most recently, layer 2 wouldn't even be looked at.

As we just saw, another important factor is that layers are ordered by their
order of activation. Whether you activate layer 1 or 2 first, matters. Lets look
at another example: we have three layers, 0, 1, and 2. On a given position, we
have a non-transparent key on every layer. If we have just layer 0 active, it
will be looked up from there. If we activate layer 2, then the firmware will
look there first. If we activate layer 1 as well, then - since now layer 1 is
the most recently activated layer - the firmware will look the code up from
layer 1, without looking at layer 2. It would only look at layer 2 if the key
was transparent on layer 1.

[^1]: Except that the base layer is always active implicitly, so if all active
    layers are transparent for a particular key, its value will come from the
    base layer.
