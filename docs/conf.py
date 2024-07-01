import os
import sys
from sphinx.ext.autodoc import ModuleDocumenter, between

# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Loxoc'
copyright = '2024, William A. Lim'
author = 'William A. Lim'
release = '1.0.0.dev26'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinx.ext.autodoc',
    'autoapi.extension'
]

autoapi_ignore = []

autoapi_dirs = ['../Loxoc']
templates_path = ['_templates']
exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'alabaster'
html_static_path = ['_static']


engine_name = 'Loxoc'
rst_epilog = '.. |engine_name| replace:: {}'.format(engine_name)
