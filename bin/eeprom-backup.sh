#!/usr/bin/env bash

# creates a backup of eeprom configuration in a subdirectory of $pwd.
# depends on focus-send. 

focus="focus-send"
subdir=kaleidoskope-eeprom-$(date -Iseconds)
mkdir "$subdir"
CMDS=$(focus-send help | grep -vE '^(help|eeprom|device|plugins|version)')
for cmd in $CMDS; do
  echo "$cmd"
  "$focus" "$cmd" > "$subdir"/"$cmd"
done
