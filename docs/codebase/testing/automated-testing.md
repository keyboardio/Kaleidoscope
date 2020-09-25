# Automated Testing

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

### Adding a New Test Case

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

### Style

TODO(obra): Fill out this section to your liking.

You can see samples of the desired test style in the [example tests](#examples).

### Examples

All existing tests are examples and may be found under
`keyboardio:Kaleidoscope/tests`.

## Testing with Aglais/Papilio

TODO(obra): Write (or delegate the writing of) this section.
