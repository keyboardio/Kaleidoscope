# Kaleidoscope's host keymap system

The host_keymap system enables specifying keymaps in a natural fashion
by using characters or character strings instead of Kaleidoscope's C++ `Key_...` constants. Furthermore, it allows to select one or more host keymaps and automagically use their mappings from unicode keys to Kaleidoscope's key constants.

## Usage

To use host keymaps you must include on or more keymap specification
headers at the top of the firmware sketch, e.g.

```cpp
#include "kaleidoscope/host_keymap/linux/us/standard/keymap.h"
```

Next, you must make the newly host keymap available at global scope, somewhere
before you specify the keymap. This is done by invoking macro `USE_HOST_KEYMAP`, e.g. as

```cpp
USE_HOST_KEYMAP(linux, us, standard)
```

This declares the us standard keymap as the default host keymap. Any characters (unicode/ascii string literals) that you define in the 
`KEYMAP` definition will be interpreted with respect to this 
default host keymap.

You can now use key characters like e.g. "a" instead of Key_A in you
keymap definitions.

```cpp
KEYMAPS(
[0] = KEYMAP_STACKED
(
XXX,   L"a", L"b", ...
)
)
```

## C++ char and wchar_t strings

Please note the prefix L in the above KEYMAP definition in front of the letter strings. That prefix tells C++ that the character string literals
is to be interpreted as wide character string (possibly a unicode string). You can omit the prefix L for ASCII characters only.

## Key mapping agnostic KEYMAP formulation

The great advantage of host keymaps is that you do not have to consider
the question what Kaleidoscope Key to generate in order for a specific
keycode to be generated on the host.

Let's look at the german standard keymap for an example why this is usefull. 

The german language has some special non-ascii characters, called 
umlauts, those are ä, ö, ü and ß. Those are placed in different places
of the keymap, partially on keys where on the us english keymap there are 
special characters.

By using Kaleidoscope's Key constants, it is pretty difficult to 
define a german keymap as one has first to find out, which Key constant
generates which of the Umlauts. But that's not all. Some of the 
characters are in different places, compared to the us english standard layout.

With Kaleidoscope's host keymaps, this is now a simple task. Just 
include the german keymap and use the german characters (normal characters, umlauts and symbols) in your keymap. Under the hood
Kaleidoscope will take care to generate the appropriate keycodes
for the correct german characters to be generated when the german
keyboard layout is active on the host.

## Specifying individual characters

Individual characters can be directly specified in the keymap as "a", "+", ...

## Case specific version of alphabetical characters

For alphabetical characters the shift modifier
is automatcially added when upper case letters like "A" are used 
in the KEYMAP.

## Adding modifiers to keys

To conveniently add modifiers, you can specify strings like "c+a" which
will be interpreted as ctrl+a and will generate LCTRL(Key_A).

Similar to the c-modifier for the control key, there are several more
modifier keys, such as

s: shift
c: control
a: alt
m: meta
g: gui

You can add several modifiers at once. "s+c+a" is equal
to LSHIFT(LCTRL(Key_A)).

## Left and right hand modifier versions

For some modifiers there are left and right hand versions. By default,
the left hand version will be used. For those characters that support
a right hand version (e.g. alt), use the capital modifier character "A"
instead.

## Special keys for non printable actions

Every standard keyboard has special keys that triggers special characters, like line feed
being emitted or other actions that are represented by non-printable
characters (e.g. print screen).

For some of these characters, Kaleidoscope supplies the unicode 
symbols that are printed on common keyboards. Have a look in
header `kaleidoscope/host_keymap/unicode.h`.

## Toggling hand keys 

Kaleidoscope does not only allow for modifiers to be added
to keys in the keymap like `LCTRL(Key_A)` but also to assign the 
modifier key itself to a key in the keymap, like e.g. `Key_LeftShift`.

In header `kaleidoscope/host_keymap/unicode.h` there are unicode 
symbols for all modifiers defined, like e.g. '⌃' for control, '⇧' for shift, and so on. Those unicode symbols generated the left hand version
of each modifier. To generate a right hand version, just prefix the 
modifier symbol with the character 'r' when used in the KEYMAP, e.g. 
like L"r+⇧" to generate `Key_RightShift` instead.

## Separator characters

The characters " ", "\t" and "+" are considered as separators and are
ignored unless being the last character in a sequence (the key-character). The specifications "c + a", "ca" and "c a" are thus equal
and all generate `LCTRL(Key_A)`.

## Modifier key symbols

To make things a little more convenient, there are unicode versions of modifier keys as well

⇧: shift
⌃: control
⌥: alt
⌘: meta
⌘: gui

You can use them e.g. as L"⌥+a" to define a key that equals LALT(Key_A).

Important: Always prefix with L if any non ASCII characters are involved.

## Escaping separators

If one of the separator keys (' ', '\t' and '+') is meant to be output,
it must be escaped by prefixing a "#". This means that "c+# " will generate LCTRL(Key_Spacebar).
The escape character "#" itself must also be escaped. "c+##" will
generate LCTRL(LSHIFT(Key_3)) (in case the us standard host keymap being
the default keymap).

## Adding new keymaps

Alternative keymaps can be easily defined by using the
HOST_KEYMAP_LINUX macro that is defined in
`kaleidoscope/host_keymap/linux.h`.
See the `keymap.h` headers in the subdirectories of `kaleidoscope/host_keymap/linux` for examples how this might look.

## Using more than one host keymap in the same Kaleidoscope KEYMAP

Use the macro MAP_WITH_HOST_KEYMAP to define convenience macros
that help to work with different keymaps in the same sketch, like e.g.

```cpp
#include "kaleidoscope/host_keymap/linux/us/standard/keymap.h"
#include "kaleidoscope/host_keymap/linux/de/standard/keymap.h"

// Important, only supply strings with normal double quotes to macros
// D and U. They will automatically treated as wide characters strings.
//
#define D(KEY_STRING) MAP_WITH_HOST_KEYMAP(linux, de, standard, L##KEY_STRING)
#define U(KEY_STRING) MAP_WITH_HOST_KEYMAP(linux, us, standard, L##KEY_STRING)

// Note: Characters 'z' and 'y' are swapped on a german keyboard in comparison
//       to an us standard keyboard.
//
static_assert(D("z") == U("y"), "");
static_assert(D("y") == U("z"), "");
   
KEYMAPS(
  [0] = KEYMAP_STACKED
  (
      XXX,   "1",   "2",   "3",   "4",   "5",   XXX,
      "`",   "q",   "w",   "e",   "r",   "t",   "\t",
     L"⇞",   "a",   D("z"),   U("d"),   "f",   "g",
     L"⇟",   "z",   "x",   "c",   "v",   "b",   L"⎋",

     L"⌃", L"⌫", L"⌘", L"⇧",
      XXX,

      XXX,   "6",   "7",   "8",   "9",   "0",   XXX,
     L"⎆",   "y",   "u",   "i",   "o",   "p",   "=",
             "h",   "j",   "k",   "l",   ";",  "\"",
      XXX,   "n",   "m",   ",",   ".",   "/",   "-",

    L"r⇧", L"r⌥",  L"␣", L"r⌃",
      XXX
  )
)
```
