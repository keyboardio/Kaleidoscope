#!/usr/bin/env bash

# restores eeprom configuration previously extracted with `eeprom-backup.sh`
# reads all files in a directory, by default $PWD. depends on focus-send. 

focus="focus-send"
subdir=${1:-.}
for cmd in "$subdir"/*; do
  basename "$cmd"
  "$focus" "$(basename "$cmd")" "$(cat "$cmd")"
done
