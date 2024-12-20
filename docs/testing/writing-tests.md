# Writing tests 

## Simulator tests

### Adding a New Test Case

If you're just getting started with testing in Kaleidoscope, you almost certainly want to add a simulator test.

Simulator tests are composed of a `.ino` sketch file which sets up a keyboard layout and any plugins you need for your test and a `test.ktest` file which simulates a typist's behavior and checks the keyboard's response. The `.ino` file must be named the same as the directory it's in. So if you're creating a new test in `tests/features/ai-keypresses`, your `.ino` file would be named `ai-keypresses.ino`

Each simulator test lives in its own directory underneath `tests/`

The simplest test is `tests/examples/basic-keypress`:

```
basic-keypress.ino
sketch.yaml
test.ktest
```

`basic-keypress.ino` looks like this:

```
#include <Kaleidoscope.h>

KEYMAPS(
    [0] = KEYMAP_STACKED
    (
        ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
        ___, Key_A, Key_S, ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___,
        ___,

        ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___, ___, ___, ___,
        ___, ___, ___, ___,
        ___
    ),
)

void setup() {
  Kaleidoscope.setup();
}

void loop() {
  Kaleidoscope.loop();
}
```

All it does is set up the keys we need to test.

The `test.ktest` file looks like this:

```
VERSION 1

KEYSWITCH A 2 1
KEYSWITCH S 2 2

# ==============================================================================
NAME Keys active when pressed

RUN 4 ms
PRESS A
RUN 1 cycle
EXPECT keyboard-report Key_A # Key A should be pressed

RUN 4 ms
RELEASE A
RUN 1 cycle
EXPECT keyboard-report empty # No keys should be pressed

RUN 4 ms
PRESS A
PRESS S
RUN 1 ms
EXPECT keyboard-report Key_A # Key A should be pressed
EXPECT keyboard-report Key_A, Key_S # A and S should be pressed

RUN 4 ms
RELEASE A
RELEASE S
RUN 1 cycle
# A is released first because of scan order
EXPECT keyboard-report Key_S # Key S should be pressed
EXPECT keyboard-report empty # No keys should be pressed

RUN 5 ms
```

The most complicated part of it is the `KEYSWITCH` mapping at the top, which tells the test infrastructure what you mean when you say things like `PRESS <KEYNAME>` or `RELEASE <KEYNAME>`. The `A` and `B` in the example above are just human readable names for which simulated physical keys are getting `PRESS`ed, `RELEASE`ed or `TAP`ped. 

The `EXPECT` lines for the `keyboard-report` are looking for the same `Key_` identifiers you'd put in an arduino sketch.

The third file you need in your example is the `sketch.yaml` file. This tell the simulator what kind of virtual device to test on. As of right now, the only devices that we know work in the simulator are the `atreus` and the `model01`.

Unless you have reason to do something else, your `sketch.yaml` should look like this:

```
default_fqbn: keyboardio:virtual:model01
```

To learn how to run the simulator tests, head on over to [Running Tests](running-tests.md)

## Testing with gtest/gmock

Before feature requests or bug fixes can be merged into master, the folowing
steps should be taken:

- Create a new test suite named after the issue, e.g. `Issue840`.
- Add a test case named `Reproduces` that reproduces the bug. It should fail if
  the bug is present and pass if the bug is fixed.
- Merge the proposed fix into a temporary testing branch.
- The reproduction test should fail.
- Add a test called "HasNotRegresed" that detects a potential regression.
  It should pass with the fix and fail before the fix.
- Comment out and keep the `Reproduces` test case as documentation.

For an example, see keyboardio:Kaleidoscope/tests/issue_840.




### Adding a low-level test

For general information on writing test with gtest/gmock see [gtest
docs](https://github.com/google/googletest/tree/master/googletest/docs) and
[gmock docs](https://github.com/google/googletest/tree/master/googlemock/docs).

1. Create a new test file, if appropriate.
1. Choose a new test suite name and create a new test fixture, if appropriate.
1. Write your test case.

The final include in any test file should be `#include
"testing/setup-googletest.h"` which should be followed by the macro
invocation `SETUP_GOOGLETEST()`. This will take care of including headers
commonly used in tests in addtion to gtest and gmock headers.

Any test fixtures should inherit from `VirtualDeviceTest` which wraps the test
sim APIs and provides common setup and teardown functionality. The appropriate
header is already imported by `setup-googletest.h`

### Test Infrastructure

If you need to modify or extend test infrastructure to support your use case,
it can currently be found under `keyboardio:Kaleidoscope/testing`.
