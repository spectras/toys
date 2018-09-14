#!/usr/bin/env python
''' Package '''

import os.path
from setuptools import setup, find_packages
import example

ROOT = os.path.dirname(__file__)

# Load requirements from the file, keeping them in one place
with open(os.path.join(ROOT, 'requirements.txt')) as fd:
    REQUIREMENTS = [line.strip() for line in fd if line]

setup(
    name='example',
    version='%d.%d.%d' % example.__VERSION__,
    author='Julien Hartmann',
    url='https://github.com/spectras/toys/example-python',
    description='Python project template',
    classifiers=[
        'Development Status :: 1 - Planning',
        'License :: OSI Approved :: MIT License',           # replace with your license or delete
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',            # add/remove versions as needed
        'Programming Language :: Python :: 3.7',
    ],
    packages=find_packages(exclude=('tests', 'tests.*')),
    scripts=['example.py'],                                 # program entry point, remove if none
    python_requires='>=3.5',
    install_requires=REQUIREMENTS,
    tests_require=['pytest'],
)
