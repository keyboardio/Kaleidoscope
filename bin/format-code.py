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
"""This script runs clang-format on a Kaleidoscope repository."""

import argparse
import logging
import os
import re
import shutil
import subprocess
import sys

sys.dont_write_bytecode = True

from common import check_git_diff, setup_logging, split_on_newlines, split_on_nulls


# ==============================================================================
def parse_args(args):
    """Parse command line parameters

    Args:
      args (list[str]): command line parameters as list of strings
          (for example  ``["--help"]``).

    Returns:
      :obj:`argparse.Namespace`: command line parameters namespace
    """
    parser = argparse.ArgumentParser(
        description="""
        Recursively search specified directories and format source files with
        clang-format.  By default, it operates on Arduino C++ source files with
        extensions: *.{cpp,h,hpp,inc,ino}.""")
    parser.add_argument(
        '-q',
        '--quiet',
        dest='loglevel',
        action='store_const',
        const=logging.ERROR,
        default=logging.WARNING,
        help="""
        Suppress output except warnings and errors.""",
    )
    parser.add_argument(
        '-v',
        '--verbose',
        action='store_const',
        dest='loglevel',
        const=logging.INFO,
        help="""
        Output verbose debugging information.""",
    )
    parser.add_argument(
        '-d',
        '--debug',
        action='store_const',
        dest='loglevel',
        const=logging.DEBUG,
        help="""
        Save output from `include-what-you-use` for processed files beside the originals, with
        a '.iwyu' suffix, for debugging purposes.""",
    )
    parser.add_argument(
        '-X',
        '--exclude-dir',
        action='append',
        dest='exclude_dirs',
        default=[],
        metavar="<path>",
        help="""
        Exclude dir from search (path relative to the pwd)""",
    )
    parser.add_argument(
        '-x',
        '--exclude-file',
        action='append',
        dest='exclude_files',
        default=[],
        metavar="<file>",
        help="""
        Exclude <file> (base name only, not a full path) from formatting""",
    )
    parser.add_argument(
        '-r',
        '--regex',
        dest='regex',
        default=r'\.(cpp|h|hpp|inc|ino)$',
        metavar="<regex>",
        help="""
        Regular expression for matching source file names""",
    )
    parser.add_argument(
        '-z',
        '-0',
        action='store_const',
        dest='input_splitter',
        const=split_on_nulls,
        default=split_on_newlines,
        help="""
        When reading target filenames from standard input, break on NULL characters instead
        of newlines.""",
    )
    parser.add_argument(
        '-f',
        '--force',
        action='store_true',
        help="""
        Format code even if there are unstaged changes""",
    )
    parser.add_argument(
        '--check',
        action='store_true',
        help="""
        Check for changes after formatting by running `git diff --exit-code`.  If there are any
        changes after formatting, a non-zero exit code is returned.""",
    )
    parser.add_argument(
        'targets',
        metavar="<search_dir>",
        nargs='*',
        help="""
        A list of files and/or directories to search for source files to format.""",
    )
    return parser.parse_args(args)


# ==============================================================================
def main():
    """Parse command-line arguments and format source files.
    """
    # Parse command-line argumets:
    opts = parse_args(sys.argv[1:])
    # Set up logging system:
    setup_logging(opts.loglevel)

    # ----------------------------------------------------------------------
    # Unless we've been given the `--force` option, check for unstaged changes to avoid
    # clobbering any work in progress:
    exit_code = 0
    if not opts.force:
        changed_files = check_git_diff()
        if len(changed_files) > 0:
            logging.error("Working tree has unstaged changes; aborting")
            return 1

    # Locate `clang-format` executable:
    clang_format_exe = os.getenv('KALEIDOSCOPE_CODE_FORMATTER')
    if clang_format_exe is None:
        clang_format_exe = shutil.which('clang-format')
    logging.debug("Found `clang-format` executable: %s", clang_format_exe)
    clang_format_cmd = [clang_format_exe, '-i']
    if opts.loglevel <= logging.INFO:
        clang_format_cmd.append('--verbose')

    # ----------------------------------------------------------------------
    # Read targets from command line:
    targets = opts.targets
    logging.debug("CLI target parameters: %s", targets)

    # If stdin is a pipe, read target filenames from it:
    if not sys.stdin.isatty():
        targets += opts.input_splitter(sys.stdin.read())
    logging.debug("All targets: %s", targets)

    # Prepare exclusion lists.  The file excludes are basenames only, and the dirs get
    # converted to absolute path names.
    exclude_files = set(opts.exclude_files)
    exclude_dirs = set(os.path.abspath(_) for _ in opts.exclude_dirs)

    # Convert target paths to absolute, and remove any that are excluded:
    target_paths = set(os.path.abspath(_) for _ in targets if _ not in exclude_dirs)
    logging.debug("Target paths: %s", target_paths)

    # Build separate sets of target files and dirs.  Later, we'll search target dirs and add
    # matching target files to the files set.
    target_files = set()
    target_dirs = set()
    for t in target_paths:
        if os.path.isfile(t):
            target_files.add(os.path.abspath(t))
        elif os.path.isdir(t):
            target_dirs.add(os.path.abspath(t))
    logging.debug("Target files after separating: %s", target_files)
    logging.debug("Target dirs after separating: %s", target_dirs)

    # Remove excluded filenames:
    target_files -= set(_ for _ in target_files if os.path.basename(_) in exclude_files)

    # Remove files and dirs in excluded dirs:
    target_files -= set(_ for _ in target_files for x in exclude_dirs if _.startswith(x))
    target_dirs -= set(_ for _ in target_dirs for x in exclude_dirs if _.startswith(x))

    # Compile regex for matching files to be formatted:
    target_matcher = re.compile(opts.regex)

    # Remove target files that don't match the regex:
    logging.debug("Target files before matching regex: %s", target_files)
    target_files = set(_ for _ in target_files if target_matcher.search(_))
    logging.debug("Target files after matching regex: %s", target_files)

    # Search target dirs for non-excluded files, and add them to `target_files`:
    logging.debug("Searching target dirs: %s", target_dirs)
    for path in target_dirs:
        for root, dirs, files in os.walk(path):
            # Prune excluded dirs
            for x in exclude_dirs:
                if x in (os.path.join(root, _) for _ in dirs):
                    dirs.remove(os.path.basename(x))
            # Add non-excluded files
            for f in files:
                if target_matcher.search(f) and f not in exclude_files:
                    target_files.add(os.path.join(root, f))

    if len(target_files) == 0:
        logging.error("No target files found; exiting.")
        return 1

    # Run clang-format on target files:
    proc = subprocess.run(clang_format_cmd + sorted(target_files))
    if proc.returncode != 0:
        logging.error("Error: clang-format returned non-zero status: %s", proc.returncode)
        return proc.returncode
    else:
        logging.info("Finished formatting target files.")

    # If we've been asked to check for changes made by the formatter:
    if opts.check:
        logging.warning('Checking for changes made by the formatter...')
        changed_files = check_git_diff()
        if len(changed_files) == 0:
            logging.warning("No files changed.  Congratulations!")
        else:
            logging.warning("Found files with changes after formatting:")
            exit_code = 1
        for f in changed_files:
            logging.warning("    %s", f)

    return exit_code


# ==============================================================================
if __name__ == "__main__":
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        logging.info("Aborting")
        sys.exit(1)
