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
"""This script runs clang-format on Kaleidoscope's codebase."""

import argparse
import glob
import logging
import os
import re
import subprocess
import sys


# ==============================================================================
def parse_args(args):
    """Parse command line parameters

    Args:
      args (List[str]): command line parameters as list of strings
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
        '-v',
        '--verbose',
        dest='loglevel',
        help="Verbose output",
        action='store_const',
        const=logging.INFO,
    )
    parser.add_argument(
        '-q',
        '--quiet',
        dest='loglevel',
        help="Suppress all non-error output",
        action='store_const',
        const=logging.ERROR,
    )
    parser.add_argument(
        '-X',
        '--exclude-dir',
        metavar="<path>",
        dest='exclude_dirs',
        help="Exclude dir from search (path relative to the pwd)",
        action='append',
        default=[],
    )
    parser.add_argument(
        '-x',
        '--exclude-file',
        metavar="<file>",
        dest='exclude_files',
        help="Exclude <file> (base name only, not a full path) from formatting",
        action='append',
        default=[],
    )
    parser.add_argument(
        '-e',
        '--regex',
        metavar="<regex>",
        dest='src_re_str',
        help="Regular expression for matching source file names",
        default=r'\.(cpp|h|hpp|inc|ino)$',
    )
    parser.add_argument(
        '-f',
        '--force',
        action='store_true',
        help="Format code even if there are unstaged changes",
    )
    parser.add_argument(
        '--check',
        action='store_true',
        help="Check for changes after formatting",
    )
    parser.add_argument(
        'targets',
        metavar="<search_dir>",
        nargs='+',
        help="""A list of files and/or directories to search for source files to format""",
    )
    return parser.parse_args(args)


# ==============================================================================
def setup_logging(loglevel):
    """Setup basic logging

    Args:
      loglevel (int): minimum loglevel for emitting messages
    """
    logformat = "%(message)s"
    logging.basicConfig(
        level=loglevel,
        stream=sys.stdout,
        format=logformat,
        datefmt="",
    )
    return logging.getLogger()


# ==============================================================================
def format_code(path, opts, clang_format_cmd):
    """Run clang-format on a directory."""
    logging.info("Formatting code in %s...", path)

    src_regex = re.compile(opts.src_re_str)

    src_files = []

    for root, dirs, files in os.walk(path):
        for exclude_path in opts.exclude_dirs:
            exclude_path = exclude_path.rstrip(os.path.sep)
            if os.path.dirname(exclude_path) == root:
                exclude_dir = os.path.basename(exclude_path)
                if exclude_dir in dirs:
                    dirs.remove(exclude_dir)

        for name in files:
            if name in opts.exclude_files:
                continue
            if src_regex.search(name):
                src_files.append(os.path.join(root, name))

    proc = subprocess.run(clang_format_cmd + src_files)
    if proc.returncode != 0:
        logging.error("Error: clang-format returned non-zero status: %s", proc.returncode)
    return


# ==============================================================================
def build_file_list(path, src_regex):
    """Docstring"""

    # If the specified path is a filename, return it (as a list), regardless of
    # whether or not it matches the regex.
    if os.path.isfile(path):
        return [path]

    # If the specified path is not valid, just return an empty list.
    if not os.path.isdir(path):
        return []

    # The specified path is a directory, so we search recursively for files
    # contained therein that match the specified regular expression.
    source_files = []
    for root, dirs, files in os.walk(path):
        # First, ignore all dotfiles (and directories).
        dotfiles = set(glob.glob('.*'))
        dirs = set(dirs) - dotfiles
        files = set(dirs) - dotfiles

        # Check for a list of file glob patterns that should be excluded.
        if IWYU_IGNORE_FILE in files:
            with open(os.path.join(root, IWYU_IGNORE_FILE)) as f:
                for pattern in f.read().splitlines():
                    matches = set(glob.glob(os.path.join(root, pattern)))
                    dirs = set(dirs) - matches
                    files = set(files) - matches

        # Add all matching files to the list of source files to be formatted.
        for f in filter(src_regex.search, files):
            source_files.append(os.path.join(root, f))

    return source_files


# ==============================================================================
def warn_if_output(byte_str, msg):
    """Convert a string of bytes to a UTF-8 string, break it on newlines.  If
    there is any output, print a warning message, followed by each line,
    indented by four spaces."""
    lines = byte_str.decode('utf-8').splitlines()
    if '' in lines:
        lines.remove('')
    if len(lines) > 0:
        logging.warning('%s', msg)
        for line in lines:
            logging.warning('    %s', line)
    return


# ==============================================================================
def main(cli_args):
    """Parse command-line arguments and format source files."""
    args = parse_args(cli_args)
    if args.loglevel is None:
        args.loglevel = logging.WARNING
    setup_logging(args.loglevel)

    clang_format = os.getenv('CLANG_FORMAT_CMD')
    if clang_format is None:
        clang_format = 'clang-format'

    clang_format_cmd = [clang_format, '-i']

    git_diff_cmd = ['git', 'diff', '--exit-code']

    proc = subprocess.run(git_diff_cmd + ['--name-only'], capture_output=True)
    if proc.returncode != 0:
        warn_if_output(proc.stdout, 'Warning: you have unstaged changes to these files:')
        if not args.force:
            logging.warning(
                'Formatting aborted. Stage your changes or use --force to override.')
            sys.exit(proc.returncode)

    if args.loglevel >= logging.WARNING:
        git_diff_cmd.append('--quiet')
    elif args.loglevel <= logging.INFO:
        git_diff_cmd.append('--name-only')

    for path in args.targets:
        format_code(path, args, clang_format_cmd)

    if args.check:
        logging.warning('Checking for changes made by the formatter...')

        proc = subprocess.run(git_diff_cmd + ['--cached'], capture_output=True)
        if proc.returncode != 0:
            logging.warning(
                'Warning: Your working tree has staged changes. ' +
                'Committed changes might not pass this check.')
            warn_if_output(proc.stdout, 'The following files have unstaged changes:')

        proc = subprocess.run(git_diff_cmd, capture_output=True)
        if proc.returncode != 0:
            warn_if_output(proc.stdout, 'The following files have changes:')
            logging.error(
                'Check failed: Please commit formatting changes before submitting.')
            sys.exit(proc.returncode)
    return


# ==============================================================================
if __name__ == "__main__":
    main(sys.argv[1:])
