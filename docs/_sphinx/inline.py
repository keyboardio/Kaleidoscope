"""Crude Sphinx extension for inline documentation.

© 2019 Paul Natsuo Kishimoto <mail@paul.kishimoto.name>
  Licensed under the GNU GPLv3: https://www.gnu.org/licenses/gpl-3.0.html
  Star: https://gist.github.com/khaeru/3185679f4dd83b16a0648f6036fb000e

Enable by adding to conf.py::

    sys.path.append('path/to/this/file')
    extensions.append('inline')

    # Root path for inline documentation, relative to conf.py.
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
    # …then inline_source should be like:

    inline_source = '../../my_module'

    # Destination path for copied files, relative to conf.py.
    # For instance, the following:

    inline_dest = 'generated'

    # …results in files being copied to 'BASE/doc/source/generated'

find_inline_docs() performs two replacements:

1. A file like my_module/submodule/doc.rst is copied to
   my_module/submodule.rst.
2. A file like my_module/submodule/doc/index.rst is copied to
   my_module/submodule.rst

All other paths are not modified.

'Crude' means:
- Old files in the inline_dest directory are not removed automatically.
- It's not tested; bad config values will cause strange behaviour.

"""
from os import PathLike
from pathlib import Path
from shutil import copy2

from sphinx.util import status_iterator


def find_inline_docs(src_dir, target_dir, match='*.rst'):
    """Yield tuples of (source, dest) filenames.

    The first element in each tuple is the path to a file in *src_dir* that
    matches '**/{match}'. The second is the same path, below *target_dir*,
    modified as described above.
    """
    for source in src_dir.glob('**/' + match):
        base = source.relative_to(src_dir)

        # Perform modifications. Delete these if you like.
        if base.parent.stem == 'doc' and base.stem == 'index':
            dest = base.parents[1]
        elif base.stem == 'doc':
            dest = base.parent
        else:
            pass   # No change

        yield source, target_dir / dest.with_suffix(source.suffix)


def config_inited(app, config):
    # Handle configuration settings
    source_path = Path(app.srcdir, app.config.inline_source).resolve()
    dest_path = Path(app.srcdir, app.config.inline_dest).resolve()

    def docname(item):
        """Helper for status_iterator()."""
        return str(Path(item[0]).relative_to(source_path).parent)

    # Iterator over inline documentation source files and targets
    docs = find_inline_docs(source_path, dest_path)

    # Wrap iterator for logging
    it = status_iterator(docs, 'Copying inline files... ',
                         color='purple', stringify_func=docname)
    for source, dest in it:
        # Make any parent directory(ies) of dest
        dest.parent.mkdir(parents=True, exist_ok=True)

        # Copy the file
        copy2(source, dest)


def setup(app):
    app.add_config_value('inline_source', None, 'env', (PathLike, str))
    app.add_config_value('inline_dest', None, 'env', (PathLike, str))
    app.connect('config-inited', config_inited)