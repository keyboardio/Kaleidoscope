#!/usr/bin/env python3
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
"""When building Kaleidoscope, the compiled object files are linked together
into a static archive. This static archive has a very simple structure, and only
stores filenames, not paths, not even relative ones. As such, we can't have
files with the same name, because they will conflict, and one will override the
other.

To avoid this situation, this script will find all cpp source files (we don't
need to care about header-only things, those do not result in an object file),
and will comb through them to find conflicting filenames.

If a conflict is found, it will print all files that share the name, and will
exit with an error at the end. It does not exit at the first duplicate, but will
find and print all of them.

If no conflict is found, the script just prints its status message and exits
with zero."""

import os
import re
import sys

cpp_regex = re.compile('.*\.cpp')


def find_duplicates(root):
    """Search for files with the same basename, but in different directories in
    the tree under <root>.  Prints a message for each conflict found, and
    returns a count of the number of non-unique basenames."""

    # Search the specified tree for matching basenames:
    basenames = {}
    for dir_path, dirs, files in os.walk(root):
        for file_name in files:
            if cpp_regex.match(file_name):
                if file_name not in basenames:
                    basenames[file_name] = []
                basenames[file_name].append(dir_path)

    conflict_count = 0
    for file_name, dirs in basenames.items():
        # Prune unique basenames from the dict:
        if len(dirs) <= 1:
            continue

        conflict_count += 1
        # Print info about basenames with conflicts:
        print(f"Conflict found for file name '{file_name}':")
        for root in dirs:
            path = os.path.join(root, file_name)
            print(f'  -> {path}')

    return conflict_count


def main(args):
    print('Searching for conflicting filenames...')
    exit_code = 0
    for path in args:
        exit_code += find_duplicates(path)
    if exit_code != 0:
        sys.exit(exit_code)
    print('No filename conflicts found.')


if __name__ == "__main__":
    main(sys.argv[1:])
