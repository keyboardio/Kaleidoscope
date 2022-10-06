# MouseKeys

Have you ever wanted to control the mouse cursor from the comfort of your
keyboard? With this plugin, you can. While it may not replace the mouse in all
situations, there are plenty of cases where one will not have to lift their
hands off the keyboard just to nudge the mouse cursor away a little.

Of course, there are a lot more one can do with the plugin than to nudge the
cursor! Mouse keys are provided for all four *and* diagonal movement; mouse
buttons; and a unique warping mechanism too. And not only these: the speed of
the cursor, the mouse wheel, and that of acceleration can all be configured to
match one's desired behaviour.

## Using the plugin

To use the plugin, simply include the header in your Sketch, tell the firmware
to use the `MouseKeys` object, and place mouse keys on your keymap. It is best
illustrated with an example:

```c++
#include <Kaleidoscope.h>
#include <Kaleidoscope-MouseKeys.h>

// Somewhere in the keymap:
Key_mouseUp, Key_mouseDn, Key_mouseL, Key_mouseR,
Key_mouseBtnL, Key_mouseBtnR

KALEIDOSCOPE_INIT_PLUGINS(
  MouseKeys,
  MouseKeysConfig // optionally add this to make the configuration runtime editable via focus commands. 
);

void setup() {
  Kaleidoscope.setup();
}
```

## Keys provided by the plugin

The plugin provides a number of keys one can put on the keymap, that allow
control of the mouse. They can be divided into a few groups:

### Mouse buttons

Mouse button keys are straightforward; pressing one is the same as pressing the
corresponding button on a physical mouse.  You can hold a mouse button key to
perform drag gestures, as you might expect.  MouseKeys supports five mouse
buttons: left, right, middle, previous, and next.

* `Key_mouseBtnL`, `Key_mouseBtnM`, `Key_mouseBtnR`, `Key_mouseBtnP`,
  `Key_mouseBtnN`: The left, middle, right, previous, and next mouse buttons,
  respectively.


### Cursor movement

When a cursor movement key is pressed, the mouse cursor will begin to move
slowly, then accelerate to full speed.  Both the full speed and the time it
takes to reach full speed are configurable.

The cursor movement keys are as follows:

* `Key_mouseUp`, `Key_mouseDn`, `Key_mouseL`, `Key_mouseR`: Move the cursor up,
  down, left, or right, respectively.
* `Key_mouseUpL`, `Key_mouseUpR`, `Key_mouseDnL`, `Key_mouseDnR`: Move the
  cursor up-left, up-right, down-left, down-right, respectively.

### Scroll wheels

Controlling the scroll wheel is similarly simple.  It does not have
acceleration, but one can control the speed with the
`MouseKeys.setScrollInterval()` function, which controls the length of time
between scroll events.

* `Key_mouseScrollUp`, `Key_mouseScrollDn`: Scroll the mouse wheel up or down,
  respectively.
* `Key_mouseScrollL`, `Key_mouseScrollR`: Scroll the mouse wheel left or right,
  respectively.

## Warping

Warping is one of the most interesting features of the plugin, and is a feature
unique to Kaleidoscope, as far as we can tell. The warping keys position the
mouse cursor within a sector of the screen on first press, and any subsequent
taps will warp within the previously selected sector. For example, pressing the
north-west warp key twice will first jump to the middle of the north-west
sector of your screen, then select the north-west sector of that, and jump to
the middle of it.

To stop warping, use any other mouse key, or hit the "warp end" key.

### Warp grid size

The warp grid size determines how MouseKeys partitions the screen to select the
next position to jump to when pressing a warp key. The plugin provides two grid
sizes to choose from: a *2x2* grid that splits the screen into quadrants, and a
*3x3* grid with nine cells similar to a navigation feature included with some
speech recognition software. By default, the plugin splits the screen into the
2x2 grid.

To change the warp grid size, call the plugin's `setWarpGridSize()` method:

```c++
MouseKeys.setWarpGridSize(MOUSE_WARP_GRID_3X3);
```

#### 2x2 grid

As described above, MouseKeys warps the pointer using a grid model that reflects
locations on the screen. By default, the plugin uses a 2x2 grid. To understand
how warping works, examine this diagram of a screen split into that 2x2 grid:

    +-----------------------|-----------------------+
    |           |           |                       |
    |     G     |    tab    |                       |
    |           |           |                       |
    |-----------|-----------|          tab          |
    |           |           |                       |
    |     B     |    esc    |                       |
    |           |           |                       |
    +-----------------------|-----------------------+
    |                       |                       |
    |                       |                       |
    |                       |                       |
    |           B           |          esc          |
    |                       |                       |
    |                       |                       |
    |                       |                       |
    +-----------------------|-----------------------+

Each quadrant is labed with a key that, when pressed, moves the mouse pointer
to the center of that quadrant. With this layout, pressing <kbd>G</kbd> warps
the pointer to the top-left quadant. Then, the plugin "zooms" into that sector
with a smaller grid so that the next warp key pressed jumps the pointer more
precisely within the sector. In this case, if we press <kbd>esc</kbd> next,
the pointer warps to the bottom-right corner within that quadrant.

The warping keys for the 2x2 grid are the following:

* `Key_mouseWarpNW`, `Key_mouseWarpNE`, `Key_mouseWarpSW`, `Key_mouseWarpSE`:
  Warp towards the north-west, north-east, south-west, or south-east quadrants,
  respectively.
* `Key_mouseWarpEnd`: End the warping sequence, resetting it to the default
  state. Using any of the warping keys after this will start from the whole
  screen again.

#### 3x3 grid

A 3x3 warp grid assigns a key to each of nine sectors of the screen. The next
diagram shows a screen with a key label that warps to each sector. As we can
see, pressing <kbd>W</kbd> warps the pointer into the top-left sector, and
pressing <kbd>V</kbd> warps to the bottom-right corner within that sector:

    +-----------------|-----------------|-----------------+
    |  W  |  E  |  R  |                 |                 |
    |-----|-----|-----|                 |                 |
    |  S  |  D  |  F  |        E        |        R        |
    |-----|-----|-----|                 |                 |
    |  X  |  C  |  V  |                 |                 |
    +-----------------|-----------------|-----------------+
    |                 |                 |                 |
    |                 |                 |                 |
    |        S        |        D        |        F        |
    |                 |                 |                 |
    |                 |                 |                 |
    +-----------------|-----------------|-----------------+
    |                 |                 |                 |
    |                 |                 |                 |
    |        X        |        C        |        V        |
    |                 |                 |                 |
    |                 |                 |                 |
    +-----------------|-----------------|-----------------+

To use a 3x3 warp grid, we may need to remap some keys. A suggested warp key
mapping is shown below on the left side of a keyboard with a QWERTY layout:

       W | E | R  T      A - End Warping      (Key_mouseWarpEnd)
      ---|---|---        W - Warp NW Sector   (Key_mouseWarpNW)
    A  S | D | F  G      E - Warp N Sector    (Key_mouseWarpN)
      ---|---|---        R - Warp NE Sector   (Key_mouseWarpNE)
       X | C | V  B      S - Warp W Sector    (Key_mouseWarpW)
                         D - Warp/Zoom Center (Key_mouseWarpIn)
                         F - Warp E Sector    (Key_mouseWarpE)
                         X - Warp SW Sector   (Key_mouseWarpSW)
                         C - Warp S Sector    (Key_mouseWarpS)
                         V - Warp SE Sector   (Key_mouseWarpSE)
                         T - Right Click      (Key_mouseBtnR)
                         G - Left Click       (Key_mouseBtnL)
                         B - Middle Click     (Key_mouseBtnM)

This example layout replaces the default directional mouse keys and sets the
warp keys in a comfortable position for a warp-only configuration. Of course,
a Kaleidoscope user may retain the directional keys and map the warp keys
elsewhere according to his or her liking.

A 3x3 warp grid layout contains all of the keys from the 2x2 grid layout with
the following additions:

* `Key_mouseWarpN`, `Key_mouseWarpE`, `Key_mouseWarpS`, `Key_mouseWarpW`:
  Warp towards the north, east, south, and west sectors, respectively.
* `Key_mouseWarpIn`: Warp to the center sector of the grid. The plugin will
  continue to "zoom" into center of the current cell with each consecutive
  press of this key.

## Plugin methods

The plugin provides a `MouseKeys` object, with the following methods and
properties available:

### `.setCursorInitSpeed(speed)`/`.getCursorInitSpeed()`

> Controls (or returns) the current starting speed value for mouse cursor
> movement.  When a mouse movement key is pressed, the cursor starts moving at
> this speed, then accelerates.  The number is abstract, but linear, with higher
> numbers representing faster speeds.  Default starting speed is `1`.

### `.setCursorBaseSpeed(speed)`/`.getCursorBaseSpeed()`

> Controls (or returns) the current top speed value for mouse cursor movement.
> When a mouse movement key is pressed, the cursor accelerates until it reaches
> this speed.  The number is abstract, but linear, with higher numbers
> representing faster speeds.  Default full-speed value is `50`.

### `.setCursorAccelDuration(duration)`/`.getCursorAccelDuration()`

> Controls (or returns) the current time it takes for the mouse cursor to reach
> full speed (in milliseconds), starting from when the first movement key is
> pressed.  Default value is `800` ms.

### `.setScrollInterval(interval)`/`.getScrollInterval()`

> Controls (or returns) the current scrolling speed, by setting the time between
> mouse scroll reports (in milliseconds).  Default value is `50` ms.

### `.setWarpGridSize(size)`

> This method changes the size of the grid used for [warping](#warping). The
> following are valid sizes: `MOUSE_WARP_GRID_2X2`, `MOUSE_WARP_GRID_3X3`

## Further reading

There is an [example][plugin:example] that demonstrates how to use this plugin.

[plugin:example]: /examples/Features/MouseKeys/MouseKeys.ino
