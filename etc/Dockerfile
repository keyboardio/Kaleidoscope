FROM debian:stable-slim
LABEL maintainer="Keyboard.io, inc"

RUN apt-get -qq update
RUN apt-get -qq install -y xz-utils curl git-core make build-essential libxtst-dev cmake

ENV ARDUINO_VERSION "1.8.13"

WORKDIR /usr/local
RUN curl https://downloads.arduino.cc/arduino-${ARDUINO_VERSION}-linux64.tar.xz | \
  xzcat | tar xf - && \
  ln -s arduino-${ARDUINO_VERSION} arduino
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | \
  sh
RUN /usr/local/bin/arduino-cli core install arduino:avr

VOLUME ["/kaleidoscope/hardware/keyboardio"]
ENV BOARD_HARDWARE_PATH "/kaleidoscope/hardware"
WORKDIR /kaleidoscope/hardware/keyboardio/avr/libraries/Kaleidoscope
ENTRYPOINT ["/bin/bash"]
