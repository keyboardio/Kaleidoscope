# Testing Kaleidoscope

This is not yet proper documentation about running or writing tests, just some rough notes.

Kaleidoscope includes a simulator that can pretend (to a certain extent) to be a keyboard for the purpose of testing.

On most UNIX-like systems, you can run Kaleidoscope's simulator tests by running

```
make simulator-tests
```

Our simulator currently has some weird linking issues on macOS, so the easiest way to run tests on macOS is using Docker.


```
make docker-simulator-tests
```

During development, when you may be running your tests very frequently, it's sometimes useful to run a subset of tests.

You can control the directory that Kaleidoscope searches for test suites with the 'TEST_PATH' variable.

To only run tests in subdirectories of the 'tests/hid' directory, you'd write:

```
make simulator-tests TEST_PATH=tests/hid
```
or

```
make docker-simulator-tests TEST_PATH=tests/hid
```

