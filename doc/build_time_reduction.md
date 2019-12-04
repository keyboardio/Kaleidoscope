# Build time optimization

This is a follow up to discussion https://github.com/keyboardio/Kaleidoscope/issues/736.

## TL;DR

This PR reduces firmware build times by ~20%.

## Motivation

Firmware builds take longer than necessary. That's especially annoying if frequent rebuilds are required during development or if one needs to wait till the smoke-examples test bench runs through before submitting a PR.

## Reasons for build time accumulation

Kaleidoscope has one central header `Kaleidoscope.h` that is meant to simplify handling of the firmware for users. This header pulls in a lot of stuff and is unfortunately included in many places of the firmware core. Thus, many compilation units include stuff that is not necessary for them to build. Also, `Kaleidoscope.h` includes stuff that is only required in the sketch, like the mostly templated infrastructure of the plugin API and transient LED modes, just to name two of them.

## Ways to improve this

### Conditionally excluding parts of the code

A first attempt was to reduce build times by introducing a wrapper compiler that command-line defines a macro that can be used to distinguish the sketch compilation unit from all others. This was meant to suppress infrastructure stuff for all other compilation units and only make it available to the sketch.
This comes with two major drawbacks, namely the wrapper compiler and the burden to figure out what to suppress and how to do it. Especially as this must be done in a way that it remains maintainable.
After experimenting with this for a while, the idea was abandoned.

### Cleaning up includes

#### Where/what to clean up

A second attempt to reduce build time was far more successful. Instead of excluding parts of the code, the whole include management was cleaned up - partially automatized - which lead to a build time decrease of approx. 20%.

Before we go into detail how that works, let's draw a distinction between different types of headers that can be found in Kaleidoscope.

* sketch-only headers (headers that are exclusively meant to be included by the sketch)
* library internal headers (included within Kaleidoscope and not meant to be included directly in the sketch)
* private headers (mostly in kaleidoscope_internal)

Next, some rules that are commonly regarded as good practice in C/C++ programming and if respected keep build times low and also re-build times (due to less unnecessary include dependencies).

* headers should only include other headers in very rare cases 
* forward declare whenever possible (therefore, prefer call by reference to call by value) to avoid unnecessary includes
* always try to include headers directly, not via other headers (prevents unnecessary dependencies and improves transparency (what lives where))
* name headers in a way that they reflect the content (simplifies include management and finding appropriate headers)

There are sometimes, for good reasons, exceptions to some of these rules. A typical example are the service-headers that many widely used C++ libraries support. Those are often named by the project and pull in a lot of stuff, sometimes all features, a library offers. This is meant to spare users finding out what symbols are defined where. The downside of this approach is that such headers are really costly on the including side, especially if only few features are required but the compiler sees and parses all of them.
This becomes even worse if such service-headers are included by other central headers of the own project which are in turn included by many files. Such service-headers could easily cause an explosion of build times when used carelessly. That's why it is recommended to use feature-headers instead and preferably include them only in source files (.cpp) and never in headers. This is especially important for the libraries that depend on other libraries.

In Kaleidoscope, an example for such a central service-header is `Kaleidoscope.h`. Also most of the headers directly living in `src`, that originally were the Arduino-library headers of individual plugins before the mono-repo fusion. All those headers are there for good reasons. The trade off between usability and build times should clearly be on the usability side. That's why no attempt is made to speed up sketch build time. 

Things are very much different for most other headers and source files. For those the good-practice rules of inclusion should be respected. Any other compilation units apart from the sketch should really build as fast as possible.

#### Include hell

Unfortunately, a include hell in Kaleidoscope comes from the fact that many compilation units depend on class `Kaleidoscope` that is defined in header `kaleidoscope/Kaleidoscope.h`. The big problem is that this header pulls in a huge lot of other stuff. That's why currently every compilation unit pulls in great parts of Kaleidoscope's headers without actually needing them to build.

#### Refactoring Kaleidoscope.h

The first step in refactoring was to move the definition of class `Kaleidoscope` to a separate header. This header is called `Kaleidoscope_Class.h` due to a lack of better ideas. Both the central header `Kaleidoscope.h` but also the header of class `Kaleidoscope` deserve to be named `Kaleidoscope.h`. To remain backward compatible, we definitely have to stick with the name for the central header. Maybe we might consider ?another name for class `Kaleidoscope`? Just an idea that's fairly off topic.

After this change, all compilation units that only need class `Kaleidoscope` can include `Kaleidoscope_Class.h`. This already removes a lot of unnecessary dependencies.

#### Include-what-you-use

After factoring out class `Kaleidoscope` there are still a lot of unnecessary includes left as not all `#include "kaleidoscope/Kaleidoscope.h"` can be replaced by `#include "kaleidoscope/Kaleidoscope_Class.h"`. That's because a great number of such includes actually mean to pull in other stuff that is indirectly included by `Kaleidoscope.h`, e.g. `layer.h` or `hooks.h`. For authors it was probably tempting in the past to just include `Kaleidoscope.h` instead of finding out where a symbol or macro actually lives, knowing that one include would solve all problems.

Fortunately, there is a free tool named [include-what-you-use](https://include-what-you-use.org/) (IWYU), that helps to deal with such problems. It's clang based analyser that can serve as a drop-in replacement for the gcc or clang compiler.

IWYU compiles source files (and also headers if necessary) and provides suggestions about where to remove unnecessary includes or which indirect includes could be replaced by direct ones. Moreover, it also helps with replacing some includes by forward declarations. It is obvious that to provide suggestions about the latter, it needs to analyse the entire AST, which explains why it is based on clang in the first place.

Unfortunately, IWYU cannot be used to analyse Kaleidoscope directly. That's because IWYU cannot replace avr-gcc. There's just no avr-clang or avr-IWYU that could deal with all the Arduino/AVR specific compiler flags.

Luckily, we have virtual builds that already work with x86 compilers. With few changes to the code, Kaleidoscope also builds with latest clang.

We let IWYU run on all headers in `src/kaleidoscope` and `src/kaleidoscope_internal` and replaced include sections with what IWYU suggested. Only in rare cases we had to manually replace some of the suggested standard headers with those available with Arduino. That's because of the virtual build.

The result is a 20% build time reduction (determined by looking at the build time of the stock firmware).

It may be beneficial to repeat this IWYU-procedure once in a while to keep the amount of unnecessary includes low.
FWIW, IWYU comes with a auto-replacement script but that cannot be used (with AVR builds) due to the need to manually replace specific header suggestions.

## Changes

This PR comes with two commits. The first one extracts header `Kaleidoscope_Class.h` from `Kaleidoscope.h`.  The second commit contains the IWYU optimization.

These changes mostly affect include directives being removed, changed or replaced by forward declarations. Any other changes part of the first commit were necessary to enable factoring out class `Kaleidoscope`.
