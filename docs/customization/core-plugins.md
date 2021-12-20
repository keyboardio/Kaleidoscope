# Core plugin overview

This is an annotated list of some of Kaleidoscope's most important core plugins. You may also want to consult the [automatically generated list of all plugins bundled with Kaleidoscope](../plugin_list).


You can find a list of third-party plugins not distributed as part of Kaleidoscope [on the forums][forum:plugin-list].

 [forum:plugin-list]:  https://community.keyboard.io/c/programming/discuss-kaleidoscope-plugins-one-thread-per-plugin/

## EEPROM-Keymap

[EEPROM-Keymap Documentation](../plugins/Kaleidoscope-EEPROM-Keymap.md)

While keyboards usually ship with a keymap programmed in, to be able to change that keymap, without flashing new firmware, we need a way to place the keymap into a place we can update at run-time, and which persists across reboots. Fortunately, we have a bit of EEPROM on the keyboard, and can use it to store either the full keymap (and saving space in the firmware then), or store an overlay there. In the latter case, whenever there is a non-transparent key on the overlay, we will use that instead of the keyboard default.

In short, this plugin allows us to change our keymaps, without having to compile and flash new firmware. It does so through the use of the Focus plugin.

## Escape-OneShot

[Escape-OneShot Documentation](../plugins/Kaleidoscope-Escape-OneShot.md)

Turn the Esc key into a special key, that can cancel any active OneShot effect - or act as the normal Esc key if none are active. For those times when one accidentally presses a one-shot key, or change their minds.

## Leader

[Leader Documentation](../plugins/Kaleidoscope-Leader.md)

Leader keys are a kind of key where when they are tapped, all following keys are swallowed, until the plugin finds a matching sequence in the dictionary, it times out, or fails to find any possibilities. When a sequence is found, the corresponding action is executed, but the processing still continues. If any key is pressed that is not the continuation of the existing sequence, processing aborts, and the key is handled normally.

This behaviour is best described with an example. Suppose we want a behaviour where ``LEAD u`` starts unicode input mode, and ``LEAD u h e a r t`` should result in a heart symbol being input, and we want ``LEAD u 0 0 e 9 SPC`` to input é, and any other hex code that follows ``LEAD u``, should be handled as-is, and passed to the host. Obviously, we can't have all of this in a dictionary.

So we put ``LEAD u`` and ``LEAD u h e a r t`` in the dictionary only. The first will start unicode input mode, the second will type in the magic sequence that results in the symbol, and then aborts the leader sequence processing. With this setup, if we type ``LEAD u 0``, then ``LEAD u`` will be handled first, and start unicode input mode. Then, at the 0, the plugin notices it is not part of any sequence, so aborts leader processing, and passes the key on as-is, and it ends up being sent to the host. Thus, we covered all the cases of our scenario!

## Macros

[Macros Documentation](../plugins/Kaleidoscope-Macros.md)

Macros are a standard feature on many keyboards and powered ones are no exceptions. Macros are a way to have a single key-press do a whole lot of things under the hood: conventionally, macros play back a key sequence, but with Kaleidoscope, there is much more we can do. Nevertheless, playing back a sequence of events is still the primary use of macros.

Playing back a sequence means that when we press a macro key, we can have it play pretty much any sequence. It can type some text for us, or invoke a complicated shortcut - the possibilities are endless!

## MagicCombo

[MagicCombo Documentation](../plugins/Kaleidoscope-MagicCombo.md)

The MagicCombo extension provides a way to perform custom actions when a particular set of keys are held down together. The functionality assigned to these keys are not changed, and the custom action triggers as long as all keys within the set are pressed. The order in which they were pressed do not matter.

This can be used to tie complex actions to key chords.

## OneShot

[OneShot Documentation](../plugins/Kaleidoscope-OneShot.md)

One-shots are a new kind of behaviour for your standard modifier and momentary layer keys: instead of having to hold them while pressing other keys, they can be tapped and released, and will remain active until any other key is pressed. In short, they turn ``Shift, A`` into ``Shift+A``, and ``Fn, 1`` to ``Fn+1``. The main advantage is that this allows us to place the modifiers and layer keys to positions that would otherwise be awkward when chording. Nevertheless, they still act as normal when held, that behaviour is not lost.

Furthermore, if a one-shot key is tapped two times in quick succession, it becomes sticky, and remains active until disabled with a third tap. This can be useful when one needs to input a number of keys with the modifier or layer active, and still does not wish to hold the key down. If this feature is undesirable, unset the ``OneShot.double_tap_sticky property`` (see later).

To make multi-modifier, or multi-layer shortcuts possible, one-shot keys remain active if another one-shot of the same type is tapped, so ``Ctrl, Alt, b`` becomes ``Ctrl+Alt+b``, and ``L1, L2, c`` is turned into ``L1+L2+c``.

## Qukeys

[Qukeys Documentation](../plugins/Kaleidoscope-Qukeys.md)

A Qukey is a key that has two possible values, usually a modifier and a printable character. The name is a play on the term "qubit" (short for "quantum bit") from quantum computing. The value produced depends on how long the key press lasts, and how it is used in combination with other keys (roughly speaking, whether the key is "tapped" or "held").

The _primary_ value (a printable character) of a Qukey is output if the key is "tapped" (i.e. quickly pressed and released). If it is held long enough, it will instead produce the Qukey's _alternate_ value (usually a modifier). It will also produce that alternate value if a subsequent key is tapped after the initial keypress of the Qukey, even if both keys are released before the time it takes to produce the alternate value on its own. This makes it feasible for most people to use Qukeys on home-row keys, without slowing down typing. In this configuration, it can become very comfortable to use modifier combinations, without needing to move one's hands from the home position at all.

Qukeys can be defined to produce any two keys, including other plugin keys and keys with modifier flags applied. For example, one could define a Qukey to produce ``Shift + 9`` when tapped, and a OneShot ``Ctrl`` when held.

It is also possible to use Qukeys like SpaceCadet (see below), by setting the primary value to a modifier, and the alternate value to a printable character (e.g. ``(``). In that case, the behavior is reversed, and the alternate value will only be used if the key is pressed and released without any rollover to a subsequent key press.

## ShapeShifter

[ShapeShifter Documentation](../plugins/Kaleidoscope-ShapeShifter.md)

ShapeShifter is a plugin that makes it considerably easier to change what symbol is input when a key is pressed together with ``Shift``. If one wants to rearrange the symbols on the number row for example, without modifying the layout on the operating system side, this plugin is where one can turn to.

What it does, is very simple: if any key in its dictionary is found pressed while ``Shift`` is held, it will press another key instead of the one triggering the event. For example, if it sees ``Shift + 1`` pressed together, which normally results in a ``!``, it will press ``4`` instead of ``1``, inputting ``$``.

## SpaceCadet

[SpaceCadet Documentation](../plugins/Kaleidoscope-SpaceCadet.md)

Space Cadet is a way to make it more convenient to input parens - those ``(`` and ``)`` things -, symbols that a lot of programming languages use frequently. If you are working with Lisp, you are using these all the time.

What it does, is that it turns your left and right ``Shift`` keys into parens if you tap and release them, without pressing any other key while holding them. Therefore, to input, say, ``(print foo)``, you don't need to press ``Shift``, hold it, and press ``9`` to get a ``(``, you simply press and release ``Shift``, and continue writing. You use it as if you had a dedicated key for parens!

But if you wish to write capital letters, you hold it, as usual, and you will not see any parens when you release it. You can also hold it for a longer time, and it still would act as a ``Shift``, without the parens inserted on release: this is useful when you want to augment some mouse action with ``Shift``, to select text, for example.

After getting used to the Space Cadet style of typing, you may wish to enable this sort of functionality on other keys, as well. Fortunately, the Space Cadet plugin is configurable and extensible to support adding symbols to other keys. Along with ``(`` on your left ``Shift`` key and ``)`` on your right ``Shift`` key, you may wish to add other such programming mainstays as ``{`` to your left-side ``cmd`` key, ``}`` to your right-side ``alt`` key, [ to your left ``Control`` key, and ``]`` to your right ``Control`` key. You can map the keys in whatever way you may wish to do, so feel free to experiment with different combinations and discover what works best for you!

## TapDance

[TapDance Documentation](../plugins/Kaleidoscope-TapDance.md)

Tap-dance keys are general purpose, multi-use keys, which trigger a different action based on the number of times they were tapped in sequence. As an example to make this clearer, one can have a key that inputs ``A`` when tapped once, inputs ``B`` when tapped twice, and lights up the keyboard in Christmas colors when tapped a third time.

This behaviour is most useful in cases where we have a number of things we perform rarely, where tapping a single key repeatedly is not counter-productive. Such cases include - for example - multimedia forward / backward keys: forward on single tap, backward on double. Of course, one could use modifiers to achieve a similar effect, but that's two keys to use, this is only one. We can also hide some destructive functionality behind a number of taps: reset the keyboard after 4 taps, and light up LEDs in increasingly frightful colors until then.

How does it work?

To not interfere with normal typing, tap-dance keys have two ways to decide when to call an action: they either get interrupted, or they time out. Every time a tap-dance key is pressed, the timer resets, so one does not have to finish the whole tapping sequence within a short time limit. The tap-dance counter continues incrementing until one of these cases happen.

When a tap-dance key is pressed and released, and nothing is pressed on the keyboard until the timeout is reached, then the key will time out, and trigger an action. Which action, depends on the number of times it has been tapped up until this point.

When a tap-dance key is pressed and released, and another key is hit before the timer expires, then the tap-dance key will trigger an action first, perform it, and only then will the firmware continue handling the interrupting key press. This is to preserve the order of keys pressed.

In both of these cases, the ``tapDanceAction`` will be called, with ``tapDanceIndex`` set to the index of the tap-dance action (as set in the keymap), the ``tapCount``, and tapDanceAction set to either ``kaleidoscope::TapDance::Interrupt``, or ``kaleidoscope::TapDance::Timeout``. If we continue holding the key, then as long as it is held, the same function will be called with tapDanceAction set to ``kaleidoscope::TapDance::Hold``. When the key is released, after either an Interrupt or Timeout action was triggered, the function will be called with tapDanceAction set to ``kaleidoscope::TapDance::Release``.

These actions allow us to create sophisticated tap-dance setups, where one can tap a key twice and hold it, and have it repeat, for example.

There is one additional value the tapDanceAction parameter can ``take: kaleidoscope::TapDance::Tap``. It is called with this argument for each and every tap, even if no action is to be triggered yet. This is so that we can have a way to do some side-effects, like light up LEDs to show progress, and so on.

## TopsyTurvy

[TopsyTurvy Documentation](../plugins/Kaleidoscope-TopsyTurvy.md)

TopsyTurvy is a plugin that inverts the behaviour of the Shift key for some selected keys. That is, if configured so, it will input ``!`` when pressing the ``1`` key without ``Shift``, but with the modifier pressed, it will input the original ``1`` symbol.
