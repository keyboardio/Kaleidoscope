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
"""Run `include-what-you-use` on Kaleidoscope sources

This is a script for maintenance of the headers included in Kaleidoscope source
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

import argparse
import glob
import logging
import os
import re
import shlex
import shutil
import subprocess
import sys

sys.dont_write_bytecode = True

from common import cwd, setup_logging, split_on_newlines, split_on_nulls


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
        Run `include-what-you-use` on source files given as command-line arguments and/or read
        from standard input.  When reading target filenames from standard input, they should be
        either absolute or relative to the current directory, and each line of input (minus the
        line-ending character(s) is treated as a filename.""")
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
        '-r',
        '--regex',
        action='store',
        dest='regex',
        default=r'\.(h|cpp)$',
        help="""
        A regular expression for matching filenames Only the basename of the file is
        matched, and the regex is only used when searching a directory for files to process,
        not on target filenames specified in arguments or read from standard input.""",
    )
    parser.add_argument(
        '-i',
        '--ignores_file',
        action='store',
        dest='ignores_file',
        default='.iwyu_ignore',
        metavar='<ignores_file>',
        help="""
        The name of a file (relative to KALEIDOSCOPE_DIR) that contains a list of glob patterns
        that will be ignored when a target directory is searched for filenames that match
        <regex>.""",
    )
    parser.add_argument(
        '-I',
        '--include',
        action='append',
        dest='include_dirs',
        metavar='<dir>',
        help="""
        Add <dir> to the list of directories that will be searched for header files.""",
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
        '--update_comments',
        action='store_true',
        help="""
        Call `include-what-you-use` with `--update_comments` to always rewrite its 'for'
        comments regarding which symbols are used.""",
    )
    parser.add_argument(
        'targets',
        nargs='*',
        metavar="<target>",
        help="""
        A list of target files and/or directories to search for source files to format.  Any
        target file will be processed, regardless of the filename.  Any target directory will
        be recursively searched for files matching the regular expression given by --regex.
        Filenames and directories beginning with a '.' will always be excluded from the search,
        but can still be processed if specified as a command-line target.""",
    )
    return parser.parse_args(args)


# ==============================================================================
def main():
    """Main entry point function."""
    # Parse command-line arguments:
    opts = parse_args(sys.argv[1:])
    # Set up logging system:
    setup_logging(opts.loglevel)

    # ----------------------------------------------------------------------
    # Find include-what-you-use:
    iwyu = shutil.which('include-what-you-use')
    logging.debug("Found `include-what-you-use` executable: %s", iwyu)
    iwyu_opts = [
        '--no_fwd_decls',  # No forward declarations
        '--max_line_length=100',
    ]
    if opts.update_comments:
        iwyu_opts.append('--update_comments')
    # Prepend '-Xiwyu' to each `include-what-you-use` option:
    iwyu_opts = [_ for opt in iwyu_opts for _ in ('-Xiwyu', opt)]

    # ----------------------------------------------------------------------
    # Find fix_includes:
    fix_includes = shutil.which('fix_includes.py')
    logging.debug("Found `fix_includes` executable: %s", fix_includes)

    # ----------------------------------------------------------------------
    # Find clang (first checking environment variable):
    clang = os.getenv('CLANG_COMPILER')
    if clang is None:
        clang = shutil.which('clang')
    logging.debug("Found `clang` executable: %s", clang)

    # Get system include dir from `clang`:
    clang_cmd = [clang, '-print-resource-dir']
    logging.debug("Running command: `%s`", shlex.join(clang_cmd))
    result = subprocess.run(clang_cmd, capture_output=True, check=True)
    clang_resource_dir = result.stdout.decode('utf-8').rstrip()
    system_include_dir = os.path.join(clang_resource_dir, 'include')
    logging.debug("Using system include dir: %s", system_include_dir)

    # ----------------------------------------------------------------------
    # Get $KALEIDOSCOPE_DIR from enironment, falling back on `pwd`:
    kaleidoscope_dir = os.getenv('KALEIDOSCOPE_DIR')
    if kaleidoscope_dir is None:
        kaleidoscope_dir = os.getcwd()
    logging.debug("Using Kaleidoscope dir: %s", kaleidoscope_dir)
    kaleidoscope_src_dir = os.path.join(kaleidoscope_dir, 'src')

    # Define locations of other dirs to find Arduino libraries:
    virtual_hardware_dir = os.path.join(
        kaleidoscope_dir, '.arduino', 'user', 'hardware', 'keyboardio', 'virtual')
    logging.debug("Using virtual hardware dir: %s", virtual_hardware_dir)

    virtual_arduino_core_dir = os.path.join(virtual_hardware_dir, 'cores', 'arduino')
    logging.debug("Using virtual arduino core: %s", virtual_arduino_core_dir)

    virtual_model01_dir = os.path.join(virtual_hardware_dir, 'variants', 'model01')
    logging.debug("Using virtual Model01 dir: %s", virtual_model01_dir)

    virtual_keyboardiohid_dir = os.path.join(
        virtual_hardware_dir, 'libraries', 'KeyboardioHID', 'src')
    logging.debug("Using virtual KeyboardioHID dir: %s", virtual_keyboardiohid_dir)

    # ----------------------------------------------------------------------
    # Create the long list of options passed to `clang` via `include-what-you-use`.
    # First, we tell it we're using C++:
    clang_opts = ['-x', 'c++']
    # General compiler options:
    clang_opts += [
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
        '-Wno-pragma-once-outside-header',
    ]

    # Variables we define to do a Kaleidoscope build:
    defines = [
        'KALEIDOSCOPE_VIRTUAL_BUILD=1',
        'KEYBOARDIOHID_BUILD_WITHOUT_HID=1',
        'USBCON=dummy',
        'ARDUINO_ARCH_AVR=1',
        'ARDUINO=10607',
        'ARDUINO_AVR_MODEL01',
        'ARDUINO_ARCH_VIRTUAL',
        'USB_VID=0x1209',
        'USB_PID=0x2301',
        'USB_MANUFACTURER="Keyboardio"',
        'USB_PRODUCT="Model 01"',
        'KALEIDOSCOPE_HARDWARE_H="Kaleidoscope-Hardware-Keyboardio-Model01.h"',
        'TWI_BUFFER_LENGTH=32',
    ]
    clang_opts += ['-D' + _ for _ in defines]

    # Directories to search for libraries to include:
    includes = [
        system_include_dir,
        kaleidoscope_src_dir,
        virtual_arduino_core_dir,
        virtual_model01_dir,
        virtual_keyboardiohid_dir,
    ]
    # Include plugin source dirs for plugins that depend on other plugins:
    includes += glob.glob(os.path.join(kaleidoscope_dir, 'plugins', '*', 'src'))
    # Include dirs specified on the command line, if any:
    if opts.include_dirs:
        includes += [os.path.abspath(_) for _ in opts.include_dirs]
    clang_opts += ['-I' + _ for _ in includes]

    # ----------------------------------------------------------------------
    # Define the `include-what-you-use` command (sans target files)
    iwyu_cmd = [iwyu] + iwyu_opts + clang_opts
    logging.debug("Using IWYU command: %s", ' \\\n\t'.join(iwyu_cmd))

    fix_includes_cmd = [
        fix_includes,
        '--nosafe_headers',
        '--reorder',
        '--separate_project_includes=' + kaleidoscope_src_dir,  # Does this help?
    ]
    if opts.update_comments:
        fix_includes_cmd.append('--update_comments')
    logging.debug("Using `fix_includes` command: %s", ' \\\n\t'.join(fix_includes_cmd))

    # ----------------------------------------------------------------------
    targets = opts.targets
    # If stdin is a pipe, read pathname targets, one per line.  This allows us to connect the
    # output of `find` to our input conveniently:
    if not sys.stdin.isatty():
        logging.debug("Reading targets from STDIN...")
        targets += opts.input_splitter(sys.stdin.read())
        for t in targets:
            logging.debug("    Target path: %s", t)

    # ----------------------------------------------------------------------
    iwyu_ignores_file = os.path.join(kaleidoscope_dir, opts.ignores_file)
    ignores = build_ignores_list(iwyu_ignores_file)

    # ----------------------------------------------------------------------
    regex = re.compile(opts.regex)
    # Process source files first, then header files, because a source file might have been
    # relying on a header included by its associated header, but which that header does not
    # need on its own.  In this case, if we process the header first, IWYU won't be able to
    # parse the source file, and we'll get an error, but if we do them in the other order,
    # the source file will get all the includes it needs before the header removes any of them.
    source_files = []
    header_files = []
    for target_file in (_ for t in targets for _ in build_target_list(t, regex, ignores)):
        if target_file.endswith('.cpp') or target_file.endswith('.ino'):
            source_files.append(target_file)
        else:
            header_files.append(target_file)

    # If there's an error processing any file, return an error code.
    exit_code = 0
    for target_file in source_files + header_files:
        # Run IWYU and fix_headers:
        if not run_iwyu(os.path.relpath(target_file), iwyu_cmd, fix_includes_cmd):
            exit_code = 1

    return exit_code


# ==============================================================================
def build_target_list(target, regex, ignores):
    """Build the list of target files, starting from a file or directory.

    Parameters:
        target (str): The name of the file or directory to search
        regex (Pattern): A compiled regular expression to match target file names
        ignores (list): A list of (absolute) file names to be excluded from the target list
    Returns:
        targets (list): A list of (absolute) file names to be processed

    Given a filename or directory (`path`), returns a list of target filenames to run IWYU
    on.  Target filenames will all be absolute, but `path` can be relative to the current
    directory.  All target filenames will match `regex`, and any items in `ignores` will be
    excluded.  If an element of the `ignores` list is a directory, all files and directories
    beneath it will be excluded from the search.
    """
    logging.debug("Searching target: %s", target)

    # Convert all paths to absolute.
    root = os.path.abspath(target)

    # If `path` is a file, and it matches `regex`, add it to the target list.
    if os.path.isfile(target) and regex.search(target) and root not in ignores:
        return [root]

    # If the specified path is not valid, just return an empty list.
    if not os.path.isdir(target):
        logging.error("Error: File not found: %s", target)
        return []

    # Start with an empty list.
    targets = []

    # The specified path is a directory, so we search recursively for files
    # contained therein that match the specified regular expression.
    for path, dirs, files in os.walk(root):
        logging.debug("Searching dir: %s", os.path.relpath(path))
        # First, ignore all dotfiles (and directories).
        for x in (os.path.basename(_)
                  for _ in ignores + glob.glob(os.path.join(path, '.*'))
                  if os.path.dirname(_) == path):
            if x in dirs:
                logging.info("Skipping ignored dir: %s", os.path.join(path, x))
                dirs.remove(x)
            if x in files:
                logging.info("Skipping ignored file: %s", os.path.join(path, x))
                files.remove(x)

        logging.debug("Files found: %s", ', '.join(files))
        # Add all matching files to the list of source files to be formatted.
        for f in (_ for _ in files if regex.search(_)):
            t = os.path.join(path, f)
            logging.debug("Source file found: %s", t)
            targets.append(t)

    return targets


# ==============================================================================
def build_ignores_list(ignores_file_path):
    """Build a list of files and dirs to exclude from processing by IWYU

    Parameters:
        ignores_file_path (str): The name of the file to read ignores globs from
    Returns:
        ignores_list (list): A list of (absolute) file names to exclude from processing

    Reads `ignores_file` and expands each line as a glob, returning a list of all the target
    file names to be excluded from processing.  Each path in the file is treated as a relative
    pathname (unless it is already absolute), relative to the directory in which `ignores_file`
    is located.
    """
    logging.debug("Searching for ignores file: %s", ignores_file_path)
    # If the ignores file doesn't exist, return an empty list:
    if not os.path.isfile(ignores_file_path):
        logging.debug("Ignores file not found")
        return []

    ignores_list = []
    with open(ignores_file_path) as f:
        for line in f.read().splitlines():
            # Lines starting with `#` are treated as comments.
            if line.startswith('#'):
                continue
            logging.debug("Ignoring files like: %s", line)
            ignores_list += glob.glob(line, recursive=True)

    # Get the dir of the ignores file so we can construct absolute pathnames.
    ignores_file_dir = os.path.dirname(ignores_file_path)
    with cwd(ignores_file_dir):
        ignores_list[:] = [os.path.abspath(_) for _ in ignores_list]

    logging.debug("Ignores list:\n\t%s", "\n\t".join(ignores_list))
    return ignores_list


# ==============================================================================
def run_iwyu(source_file, iwyu_cmd, fix_includes_cmd):
    """Run IWYU and fix_includes on a single source file
    
    Parameters:
        source_file (str): The name of a file to run IWYU on
        iwyu_cmd (list): The command name and options list for `include-what-you-use`
        fix_includes_cmd (list): The command name and options list for `fix_headers.py`
    Returns:
        True on success, False if either IWYU or fix_headers returns an error code
    
    Run `include-what-you-use` on <source_file>, an update that file's header includes by
    sending the output to `fix_includes.py`.  If either command returns an error code, return
    `False`, otherwise return `True`.
    """
    logging.info("Fixing headers in file: %s", source_file)

    # Run IWYU on <source_file>
    iwyu_proc = subprocess.run(iwyu_cmd + [source_file], capture_output=True, check=False)

    # If IWYU returns an error, report on it:
    if iwyu_proc.returncode != 0:
        logging.error("Error: failed to parse file: %s", source_file)
        logging.debug("IWYU returned: %s", iwyu_proc.returncode)
        logging.debug("STDOUT:\n%s", iwyu_proc.stdout.decode('utf-8'))
        logging.debug("STDERR:\n%s", iwyu_proc.stderr.decode('utf-8'))
        # In addition to reporting the error, save the output for analysis:
        with open(source_file + '.iwyu', 'wb') as f:
            f.write(iwyu_proc.stderr)
        # Don't run fix_includes if there was an error (or if we've got an old version of IWYU
        # that returns bogus exit codes):
        return False

    # IWYU reports on the associated header of *.cpp files, but if we want to skip processing
    # that header, we need to use only the part of the output for the *.cpp file.  Fortunately,
    # the header is listed first, so we only need to search for the start of the target file's
    # section of the output.
    n = iwyu_proc.stderr.find(f"\n{source_file} should".encode('utf-8'))
    iwyu_stderr = iwyu_proc.stderr[n:]

    # Run fix_includes.py, using the output (stderr) of IWYU:
    fix_includes_proc = subprocess.run(
        fix_includes_cmd, input=iwyu_stderr, capture_output=True, check=False)

    # Report any errors returned by fix_includes.py:
    if fix_includes_proc.returncode != 0:
        logging.error("Error: failed to fix includes for file: %s", source_file)
        logging.debug("fix_includes.py returned: %s", fix_includes_proc.returncode)
        logging.debug("STDOUT:\n%s", fix_includes_proc.stdout.decode('utf-8'))
        logging.debug("STDERR:\n%s", fix_includes_proc.stderr.decode('utf-8'))
        return False

    # Return true on success, false otherwise:
    return True


# ==============================================================================
if __name__ == "__main__":
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        logging.info("Aborting")
        sys.exit(1)
