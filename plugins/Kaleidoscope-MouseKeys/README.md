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

KALEIDOSCOPE_INIT_PLUGINS(MouseKeys);

void setup() {
  Kaleidoscope.setup ();
}
```

## Keys provided by the plugin

The plugin provides a number of keys one can put on the keymap, that allow
control of the mouse. They can be divided into a few groups:

### Cursor movement

The simplest set of keys are the mouse cursor movement keys. These move the
cursor one direction or the other, with speed and acceleration factored in. When
a mouse cursor movement key is held down, it will move `.speed` pixels each
`.speedDelay` milliseconds without acceleration. But when `.accelSpeed` is
non-zero (and it is not zero by default,
see [below](#accelspeed-and-acceldelay)), the speed will increase by
`.accelSpeed` every `.accelDelay` milliseconds. Thus, unless configured
otherwise, holding a direction will move that way at increasing speed.

One can hold more than one key down at the same time, and the cursor will move
towards a direction that is the combination of the keys held. For example,
holding the "mouse up" and "mouse right" keys together will move the cursor
diagonally up and right.

The cursor movement keys are as follows:

* `Key_mouseUp`, `Key_mouseDn`, `Key_mouseL`, `Key_mouseR`: Move the cursor up,
  down, left, or right, respectively.
* `Key_mouseUpL`, `Key_mouseUpR`, `Key_mouseDnL`, `Key_mouseDnR`: Move the
  cursor up-left, up-right, down-left, down-right, respectively.

### Scroll wheel

Controlling the scroll wheel is similarly simple. It does not have acceleration,
but one can control the speed with the `.wheelSpeed` and `.wheelDelay`
properties (see below).

* `Key_mouseScrollUp`, `Key_mouseScrollDn`: Scroll the mouse wheel up or down,
  respectively.
* `Key_mouseScrollL`, `Key_mouseScrollR`: Scroll the mouse wheel left or right,
  respectively.

### Buttons

Buttons are even simpler than movement: there is no movement speed, nor
acceleration involved. One just presses them.

* `Key_mouseBtnL`, `Key_mouseBtnM`, `Key_mouseBtnR`, `Key_mouseBtnP`,
  `Key_mouseBtnN`: The left, middle, right, previous, and next mouse buttons,
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
       X | C | V  B      S - Warp E Sector    (Key_mouseWarpE)
                         D - Warp/Zoom Center (Key_mouseWarpIn)
                         F - Warp W Sector    (Key_mouseWarpW)
                         K - Warp SE Sector   (Key_mouseWarpSE)
                         C - Warp S Sector    (Key_mouseWarpS)
                         V - Warp SW Sector   (Key_mouseWarpSW)
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

### `.speed` and `.speedDelay`

> These two control the speed of the mouse cursor, when a movement key is held.
> The former, `.speed`, controls the amount of pixels the cursor moves, when it
> has to move, and defaults to 1. The latter, `.speedDelay` is the amount of
> time - in milliseconds - to wait between two movements, and defaults to 0, no
> delay.

### `.accelSpeed` and `.accelDelay`

> These two properties control the speed of acceleration. The former,
> `.accelSpeed`, controls how much the speed shall be increased at each step,
> while the second, `.accelDelay`, controls how often (in milliseconds)
> acceleration should be applied.
>
> They default to 1 pixel and 50 milliseconds, respectively.

### `.wheelSpeed` and `.wheelDelay`

> The last two properties supported by the plugin control the mouse wheel
> scrolling speed. The former, `.wheelSpeed`, controls the amount of ticks the
> wheel shall scroll, and defaults to 1. The second, `.wheelDelay`, controls the
> delay between two scroll events, and defaults to 50 milliseconds.

### `.setSpeedLimit`

> This method sets the maximum speed after which acceleration stops.
> The default is 127, and the minimum value is 16 (things will not work
> properly below 16).

### `.setWarpGridSize`

> This method changes the size of the grid used for [warping](#warping). The
> following are valid sizes: `MOUSE_WARP_GRID_2X2`, `MOUSE_WARP_GRID_3X3`
