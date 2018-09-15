#!/usr/bin/env python
''' Package '''

from setuptools import setup, find_packages
import example

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
    entry_points={
        'console_scripts': [
            'example = example.run:main',                   # creates 'example' command
        ],
    },
    python_requires='>=3.5',
    install_requires=[
        #'somepackage>=1.0',                                # add dependencies here
    ],
    tests_require=['pytest'],
)
