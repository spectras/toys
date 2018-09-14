# -*- coding: utf-8 -*-
#
# Configuration file for the Sphinx documentation builder.
# http://www.sphinx-doc.org/en/master/config

# -- Path setup --------------------------------------------------------------

import os
import sys
sys.path.insert(0, os.path.abspath('..'))


# -- Project information -----------------------------------------------------

project = 'example'
copyright = '2018, Julien Hartmann'
author = 'Julien Hartmann'

version = '0.1'
release = '0.1.0'

# -- General configuration ---------------------------------------------------

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
]

templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'

language = None
exclude_patterns = []

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'


# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinxdoc'
html_static_path = ['_static']

# -- Options for autodoc extension -------------------------------------------

autodoc_default_flags = ['members', 'inherited-members',
                         'private-members', 'show-inheritance']
autodoc_member_order = 'bysource'

# -- Options for intersphinx extension ---------------------------------------

intersphinx_mapping = {'https://docs.python.org/3': None}

