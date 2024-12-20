# Testing Kaleidoscope

Kaleidoscope includes a simulator that can pretend (to a certain extent) to be a keyboard for the purpose of testing.

On most UNIX-like systems, you can run Kaleidoscope's simulator tests by running

```
make simulator-tests
```


During development, when you may be running your tests very frequently, it's sometimes useful to run a subset of tests.

You can control the directory that Kaleidoscope searches for test suites with the 'TEST_PATH' variable.

To only run tests in subdirectories of the 'tests/hid' directory, you'd write:

```
make simulator-tests TEST_PATH=hid
```

If you'd rather run the simulator in a Docker environment, you can do that with the following command, although it will be a good deal slower than running them directly on your system:

```
make docker-simulator-tests
```

