.. Kaleidoscope documentation master file, created by
   sphinx-quickstart on Fri Jan  3 13:52:38 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Kaleidoscope's documentation!
========================================

.. toctree::
   :maxdepth: 2

   about
   api/library_root

   :caption: Contents:


For users
=========


Supported input devices
-----------------------
.. toctree::
   :maxdepth: 2
   :glob:

   hardware-devices/*

Plugins 
-------
.. toctree::
   :maxdepth: 1
   :glob: 

   plugins/*


For developers
==============

Understanding the codebase
--------------------------

.. toctree::
   :maxdepth: 1

   codebase/glossary.md
   codebase/code-style.md

API Design docs
---------------

.. toctree::
   maxdepth: 1
   :glob:

   api-reference/*


Device drivers
--------------
.. toctree::
   :maxdepth: 1
   :glob:

   drivers/**



Upgrading from old versions
---------------------------
   
.. toctree::
   :maxdepth: 1

   UPGRADING.md


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
