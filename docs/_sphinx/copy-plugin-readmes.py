"""Crude Sphinx extension to run a makefile step
"""

import os


def config_inited(app, config):
    os.system('make copy-plugin-readmes')

def setup(app):
    app.connect('config-inited', config_inited)
