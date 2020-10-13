FROM ubuntu:20.04
LABEL maintainer="Keyboard.io, inc"

RUN apt-get -qq update && \
    DEBIAN_FRONTEND=noninteractive \
    apt-get -qq install -y xz-utils curl make build-essential libxtst-dev cmake ccache

RUN ccache --set-config=cache_dir=/kaleidoscope-persist/ccache/cache


ENV ARDUINO_VERSION "1.8.13"

WORKDIR /usr/local
RUN curl https://downloads.arduino.cc/arduino-${ARDUINO_VERSION}-linux64.tar.xz | \
  xzcat | tar xf - && \
  ln -s arduino-${ARDUINO_VERSION} arduino
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | \
  sh
RUN /usr/local/bin/arduino-cli core install arduino:avr


COPY docker-entrypoint.sh /usr/local/bin/entrypoint

VOLUME ["/kaleidoscope", \
        "/kaleidoscope-src", \
        "/kaleidoscope-persist", \
        "/kaleidoscope/hardware/keyboardio/avr/libraries/Kaleidoscope/testing/googletest/build", \
        "/kaleidoscope/hardware/keyboardio/avr/libraries/Kaleidoscope/_build"]

ENV BOARD_HARDWARE_PATH "/kaleidoscope/hardware"
ENV KALEIDOSCOPE_TEMP_PATH "/kaleidoscope-persist/temp"


ENTRYPOINT ["/usr/local/bin/entrypoint"]

# Clean up APT when done.
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

