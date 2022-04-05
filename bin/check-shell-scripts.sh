#!/usr/bin/env bash

: "${KALEIDOSCOPE_DIR:=$(pwd)}"
cd "${KALEIDOSCOPE_DIR}" || exit 1

ERROR_COUNT=0

while read -r SCRIPT; do
    shellcheck "${SCRIPT}"
    (( ERROR_COUNT += $? ))
done < <(grep -E -n -r -l '(env (ba)?sh)|(/bin/(ba)?sh)' "${KALEIDOSCOPE_DIR}/bin")

exit $ERROR_COUNT
