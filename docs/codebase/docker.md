# Docker

It's possible to use Docker to run Kaleidoscope's test suite.

## Running tests in Docker

```
# make docker-simulator-tests
```

## Cleaning out stale data in the Docker image:

```
# make docker-clean
```


## Removing the Kaleidoscope Docker image entirely:

```
# docker volume rm kaleidoscope-persist
# docker volume rm kaleidoscope-googletest-build
# docker volume rm kaleidoscope-build
# docker image rm kaleidoscope/docker
```
