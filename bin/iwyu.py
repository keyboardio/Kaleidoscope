#!/usr/bin/env python
# -*- coding: utf-8 -*-
# ------------------------------------------------------------------------------
# Copyright (c) 2022 Michael Richters <gedankenexperimenter@gmail.com>

# This is free and unencumbered software released into the public domain.

# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.

# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

# For more information, please refer to <http://unlicense.org/>
# ------------------------------------------------------------------------------

"""This is a script for maintenance of the headers included in Kaleidoscope source
files.  It is not currently possible to run this automatically on all
Kaleidoscope source files, because of the peculiarities therein.  It uses
llvm/clang to determine which headers should be included in a given file, but
there's no avr-clang, so we're limited to using the virtual hardware device.

It takes any number of source files as its input, examines them and updates the
list of header `#include` directives.

It is safe to run on most Kaleidoscope source files, and a good idea to run it
on new ones (after staging them, so you can easily see what changes have been
made).
"""

# Example invocation:
# $ git ls-files -m | grep '\.\(h\|cpp\)' | bin/iwyu.py

import os
import shutil
import subprocess
import sys

def main():
    """Organize includes in Kaleidoscpe source files."""

    iwyu = shutil.which('include-what-you-use')
    print(f'IWYU: {iwyu}')
    iwyu_flags = [
        '-Xiwyu', '--no_fwd_decls', # No forward declarations
        '-x', 'c++',
    ]

    fix_includes = shutil.which('fix_includes.py')
    print(f'fix_includes: {fix_includes}')

    clang = os.getenv('CLANG_COMPILER')
    if clang is None:
        clang = shutil.which('clang')
    print(f'clang: {clang}')

    result = subprocess.run([clang, '-print-resource-dir'],
                            capture_output=True, check=True)
    clang_resource_dir = result.stdout.decode('utf-8').rstrip()
    system_include_dir = os.path.join(clang_resource_dir, 'include')

    kaleidoscope_dir = os.getenv('KALEIDOSCOPE_DIR')
    if kaleidoscope_dir is None:
        kaleidoscope_dir = os.getcwd()
    kaleidoscope_src_dir = os.path.join(kaleidoscope_dir, 'src')
    print(f'KALEIDOSCOPE_DIR: {kaleidoscope_dir}')

    virtual_hardware_dir = os.path.join(
        kaleidoscope_dir, '.arduino', 'user', 'hardware', 'keyboardio', 'virtual'
    )
    virtual_arduino_core_dir = os.path.join(virtual_hardware_dir, 'cores', 'arduino')
    virtual_model01_dir = os.path.join(virtual_hardware_dir, 'variants', 'model01')
    virtual_keyboardiohid_dir = os.path.join(virtual_hardware_dir,
                                             'libraries', 'KeyboardioHID', 'src')

    clang_flags = [
        '-c',
        '-g',
        '-Wall',
        '-Wextra',
        '-std=gnu++14',  # Not `c++14`, because we're using clang, not gcc
        '-ffunction-sections',
        '-fdata-sections',
        '-fno-threadsafe-statics',
        '-MMD',
        '-Woverloaded-virtual',
        '-Wno-unused-parameter',
        '-Wno-unused-variable',
        '-Wno-ignored-qualifiers',
        '-Wno-type-limits',
        '-D' + 'KALEIDOSCOPE_VIRTUAL_BUILD=1',
        '-D' + 'KEYBOARDIOHID_BUILD_WITHOUT_HID=1',
        '-D' + 'USBCON=dummy',
        '-D' + 'ARDUINO_ARCH_AVR=1',
        '-D' + 'ARDUINO=10607',
        '-D' + 'ARDUINO_AVR_MODEL01',
        '-D' + 'ARDUINO_ARCH_VIRTUAL',
        '-D' + 'USB_VID=0x1209',
        '-D' + 'USB_PID=0x2301',
        '-D' + 'USB_MANUFACTURER="Keyboardio"',
        '-D' + 'USB_PRODUCT="Model 01"',
        '-D' + 'KALEIDOSCOPE_HARDWARE_H="Kaleidoscope-Hardware-Keyboardio-Model01.h"',
        '-D' + 'TWI_BUFFER_LENGTH=32',
        '-I' + system_include_dir,
        '-I' + kaleidoscope_src_dir,
        '-I' + virtual_arduino_core_dir,
        '-I' + virtual_model01_dir,
        '-I' + virtual_keyboardiohid_dir,
    ]

    plugins_dir = os.path.join(kaleidoscope_dir, 'plugins')
    for basename in os.listdir(plugins_dir):
        plugin_dir = os.path.join(plugins_dir, basename)
        if not os.path.isdir(plugin_dir):
            continue
        clang_flags.append('-I' + os.path.join(plugin_dir, 'src'))

    for arg in [iwyu] + iwyu_flags:
        print(arg)
    for arg in clang_flags:
        print(arg)

    for source_file in sys.argv[1:]:
        iwyu_cmd = [iwyu] + iwyu_flags + clang_flags + [source_file]
        print('------------------------------------------------------------')
        print(f'File:  {source_file}')

        # Sometimes, it's useful to force IWYU to make changes, or to have a
        # more definitive marker of whether or not it failed due to compilation
        # errors (which may differ between IWYU and normal compilation,
        # unfortunately).  If so, the follwing code can be uncommented.  It adds
        # a harmless `#include` at the end of the file, which will be removed if
        # this script runs successfully.

        # with open(source_file, "rb+") as fd:
        #     fd.seek(-1, 2)
        #     char = fd.read(1)
        #     if char != b'\n':
        #         print('missing newline at end of file')
        #         fd.write(b'\n')
        #     if source_file != 'src/kaleidoscope/HIDTables.h':
        #         fd.write(b'#include "kaleidoscope/HIDTables.h"')

        iwyu_proc = subprocess.run(iwyu_cmd, capture_output=True, check=False)

        fix_includes_cmd = [
            fix_includes,
            '--update_comments',
            '--nosafe_headers',
            # Don't change the order of headers in existing files, because some
            # of them have been changed from what IWYU will do.  For new files,
            # use `--reorder` instead.
            '--noreorder',
        ]
        subprocess.run(fix_includes_cmd, input=iwyu_proc.stderr, check=False)

        # Optionally, we can write the output of `include-what-you-use` to a
        # file for debugging purposes:
        # with open(source_file + '.iwyu', 'wb') as fd:
        #     fd.write(iwyu_proc.stderr)

if __name__ == "__main__":
    main()
