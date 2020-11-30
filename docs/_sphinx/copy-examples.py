"""Crude Sphinx extension for examples documentation.

© 2019 Paul Natsuo Kishimoto <mail@paul.kishimoto.name>
  Licensed under the GNU GPLv3: https://www.gnu.org/licenses/gpl-3.0.html
  Star: https://gist.github.com/khaeru/3185679f4dd83b16a0648f6036fb000e

Enable by adding to conf.py::

    sys.path.append('path/to/this/file')
    extensions.append('examples')

    # Root path for examples documentation, relative to conf.py.
    # If your source code is laid out like:
    #
    # - BASE/
    #   - doc/
    #     - source/
    #       - conf.py
    #   - my_module/
    #     - __init__.py
    #     - doc.rst
    #
    # …then examples_source should be like:

    examples_source = '../../my_module'

    # Destination path for copied files, relative to conf.py.
    # For instance, the following:

    examples_dest = 'generated'

    # …results in files being copied to 'BASE/doc/source/generated'

find_examples_docs() performs two replacements:

1. A file like my_module/submodule/doc.rst is copied to
   my_module/submodule.rst.
2. A file like my_module/submodule/doc/index.rst is copied to
   my_module/submodule.rst

All other paths are not modified.

'Crude' means:
- Old files in the examples_dest directory are not removed automatically.
- It's not tested; bad config values will cause strange behaviour.

"""

import glob
import os
from os import PathLike
from pathlib import Path
from shutil import copy2, copytree, rmtree

from sphinx.util import status_iterator

def find_examples_docs(src_dir, target_dir):
    """Yield tuples of (source, dest) filenames.

    The first element in each tuple is the path to a file in *src_dir* that
    matches '**/{match}'. The second is the same path, below *target_dir*,
    modified as described above.
    """
    for source in glob.glob(os.path.join(src_dir,'**'), recursive=True):
        print(source)
        if not os.path.isdir(source):
            dest = source
            print(dest)
            yield source, target_dir / dest


def config_inited(app, config):
    os.system('make examples')

def setup(app):
    app.connect('config-inited', config_inited)
