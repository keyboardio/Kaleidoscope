#! /bin/bash
set -e

install -d /kaleidoscope/hardware/keyboardio \
           /kaleidoscope/hardware/keyboardio/avr/libraries/Kaleidoscope \
           /kaleidoscope-persist/temp  \
           /kaleidoscope-persist/ccache/cache

echo "Syncing the bundle..."
tar xf /kaleidoscope-src/bundle.tar -C /kaleidoscope/hardware/keyboardio

echo "Syncing Kaleidoscope..."
tar xf /kaleidoscope-src/kaleidoscope.tar -C /kaleidoscope/hardware/keyboardio/avr/libraries/Kaleidoscope

ln -s /kaleidoscope/hardware/keyboardio/virtual/libraries/Kaleidoscope \
      /kaleidoscope/hardware/keyboardio/avr/libraries/Kaleidoscope

cd /kaleidoscope/hardware/keyboardio/avr/libraries/Kaleidoscope

/bin/bash -c "$*"
