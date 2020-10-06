.. Kaleidoscope documentation master file, created by
   sphinx-quickstart on Fri Jan  3 13:52:38 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Kaleidoscope
============


Flexible firmware for computer keyboards.

This package contains the "core" of Kaleidoscope and a number of [example firmware "Sketches"](https://github.com/keyboardio/Kaleidoscope/tree/master/examples).

If you're just getting started with the Keyboardio Model 01, the [introductory docs are here](https://github.com/keyboardio/Kaleidoscope/wiki/Keyboardio-Model-01-Introduction) and the source for the basic firmware package is here: https://github.com/keyboardio/Model01-Firmware. It's probably a good idea to start there, learn how to modify your keymap and maybe turn some modules on or off, and then come back to the full repository when you have more complex changes in mind.

.. toctree::
   :caption: Installation and setup
   :maxdepth: 2
  
   quick_start  
   setup_toolchain
   build_default_firmware

For users
=========

.. toctree::
   :caption: Customizing your firmware
   :maxdepth: 1
   :glob: 

   layers.md
   keycodes.md
   customization/**
   plugin_list.rst


.. toctree::
   :maxdepth: 2
   :caption: Supported devices
   :glob:

   hardware-devices/**


For developers
==============

.. toctree::
   :caption: Understanding the codebase
   :maxdepth: 1
   :glob: 

   codebase/**

.. toctree::
   :caption: API Design
   :maxdepth: 1
   :glob:

   api-reference/**


.. toctree::
   :caption: Device Drivers
   :glob:

   drivers/**

.. toctree::
   :caption: Development tips
   :glob:

   development/**


.. toctree::
   :maxdepth: 2
   :caption: Changes

   NEWS.md
   UPGRADING.md


.. toctree::
   :maxdepth: 1
   :caption: Code of Conduct

   code_of_conduct.md


.. toctree::
   :caption: Docs that don't work yet 
   :maxdepth: 2

   examples
   about
   api/library_root

   :caption: Contents:


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`



Links
=====

`Source code on GitHub <https://github.com/keyboardio/kaleidoscope>`_
