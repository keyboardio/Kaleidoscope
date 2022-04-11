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
"""Utilities shared by Kaleidoscope code maintenance tools."""

import logging
import os
import subprocess
import sys

from contextlib import contextmanager


# ==============================================================================
@contextmanager
def cwd(temp_wd):
    """Execute code in a different working directory
    
    Parameters:
        `temp_wd` (`str`): The name of a directory to (temporarily) change to

    Change the current working directory, then automatically restore the previous working
    directory when done.  Invoke `cwd()` like this:
    ```py
    with cwd(temp_wd):
        ...
    ```
    """
    old_wd = os.getcwd()
    os.chdir(temp_wd)
    try:
        yield
    finally:
        os.chdir(old_wd)


# ==============================================================================
def split_on_newlines(string):
    """Split the string using newlines as the separator."""
    return string.splitlines()


# ------------------------------------------------------------------------------
def split_on_nulls(string):
    """Split the input string using NULL characters as the separator."""
    targets = [_ for _ in string.split('\0') if _ != '']
    return targets or []


# ==============================================================================
def setup_logging(loglevel):
    """Set up basic logging."""
    logformat = "%(message)s"
    logging.basicConfig(
        level=loglevel,
        stream=sys.stdout,
        format=logformat,
        datefmt="",
    )
    return logging.getLogger()


# ==============================================================================
def check_git_diff():
    """Check for unstaged changes with `git diff`

    Returns: a list of the names of files with unstaged changes

    This check isn't perfect, because it can give false positives (if there are unrelated
    unstaged changes).
    """
    git_diff_cmd = ['git', 'diff', '-z', '--exit-code', '--name-only']

    changed_files = []
    proc = subprocess.run(git_diff_cmd, capture_output=True)
    if proc.returncode != 0:
        changed_files = split_on_nulls(proc.stdout.decode('utf-8'))
    return changed_files


# ==============================================================================
if __name__ == "__main__":
    sys.exit(1)
