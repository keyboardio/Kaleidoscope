FROM ubuntu:20.04
LABEL maintainer="Keyboard.io, inc"

RUN apt-get -qq update && \
    DEBIAN_FRONTEND=noninteractive \
    apt-get -qq install -y xz-utils curl make build-essential libxtst-dev cmake ccache vim git

RUN ccache --set-config=cache_dir=/kaleidoscope-persist/ccache/cache


RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | \
  sh

RUN ARDUINO_DIRECTORIES_DATA=/arduino-cli/data \
    ARDUINO_BOARD_MANAGER_ADDITIONAL_URLS=https://raw.githubusercontent.com/keyboardio/boardsmanager/master/devel/package_kaleidoscope_devel_index.json \
    /bin/arduino-cli config init
RUN ARDUINO_DIRECTORIES_DATA=/arduino-cli/data /bin/arduino-cli update
RUN ARDUINO_DIRECTORIES_DATA=/arduino-cli/data /bin/arduino-cli core update-index
RUN ARDUINO_DIRECTORIES_DATA=/arduino-cli/data /bin/arduino-cli core install arduino:avr
RUN ARDUINO_DIRECTORIES_DATA=/arduino-cli/data /bin/arduino-cli core install "keyboardio:avr-tools-only"
RUN ARDUINO_DIRECTORIES_DATA=/arduino-cli/data /bin/arduino-cli core install "keyboardio:gd32-tools-only"


COPY docker-entrypoint.sh /usr/local/bin/entrypoint

VOLUME ["/kaleidoscope", \
        "/kaleidoscope-src", \
        "/kaleidoscope-persist", \
        "/kaleidoscope/testing/googletest/build", \
        "/kaleidoscope/_build"]

ENV KALEIDOSCOPE_TEMP_PATH "/kaleidoscope-persist/temp"


ENTRYPOINT ["/usr/local/bin/entrypoint"]

# Clean up APT when done.
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

