#!/usr/bin/env bash

: "${KALEIDOSCOPE_DIR:=$(pwd)}"
cd "${KALEIDOSCOPE_DIR}" || exit 1

: "${VERBOSE:=}"

"${KALEIDOSCOPE_DIR}/bin/format-code.py" \
    --exclude-dir 'testing/googletest' \
    --exclude-file 'generated-testcase.cpp' \
    --verbose \
    src plugins testing
