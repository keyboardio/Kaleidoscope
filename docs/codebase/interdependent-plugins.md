# Developing interdependent plugins

Say you have two Kaleidoscope plugins or, more general, two Arduino libraries `A` and `B`. Let's assume `B` depends on `A` in a sense that `B` references symbols (functions/variables) defined in `A`. Both libraries define header files `a_header.h` and `b_header.h` that specify their exported symbols.

The following sketch builds as expected.
```cpp
// my_sketch.ino
#include "b_header.h"
#include "a_header.h"
...
```

If the header appear in opposite order the linker will throw undefined symbol errors regarding missing symbols from `A`.
```cpp
// my_sketch.ino
#include "a_header.h"
#include "b_header.h"
...
```
The reason for this somewhat unexpected behavior is that the order of libraries' occurrence in the linker command line is crucial. The linker must see library `B` first to determine which symbols it needs to extract from `A`. If it encounters `A` first, it completely neglects its symbols as there are no references to it at that point.

To be on the safe side and only if the sketch does not reference symbols from `A` directly, it is better to include the headers in the following way.
```cpp
// header_b.h
#include "header_a.h"
...
```

```cpp
// my_sketch.ino
// Do not include a_header.h directly. It is already included by b_header.h.
#include "b_header.h"
...
```
Note: I did no thorough research on how Arduino internally establishes the linker command line, e.g. with respect to a recursive traversal of the include-tree. This means, I am not sure how the link command line order is generated when header files that are included by the main `.ino` do include other files that provide definitions of library symbols in different orders. There might be additional pitfalls when header includes are more complex given a larger project.
