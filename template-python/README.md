Example
=======

This is a template to use for small to medium development projects.

Prerequisites
-------------

You need a working python3 interpreter. That's it.

Getting Started
---------------

Simply copy the whole template directory and rename it. Then you can initialize
the git repository for your project:

    git init
    git add .
    git commit -m "Deploy standard project template"

Now, to isolate the project from the rest of your system, we create a python
environment. That environment is local, do not add it to git.

    python3 -m venv .env
    .env/bin.pip install -r requirements-dev.txt

Whenever you want to work with your project, you must activate your environment first
using this command:

    . .env/bin/activate

This will change some variables in your shell so python uses the environment we just
created.

Congratulations, you have a working Python project!


Here is a brief description of how the template works:

* **[`docs`](docs):** documentation sources for [sphinx documentation](http://www.sphinx-doc.org).
  You will document your project here.
* **[`example`](example):** the main module of your project. All your code should be under
  this directory, except for the entry point.
* **[`tests`](tests):** put all your unit tests in this directory.
* **[`LICENSE`](LICENSE):** Your code's license.
* **`README.md`:** This file! Once you have read all this, replace it with the
  README for your project. You can use
  [Markdown](https://guides.github.com/features/mastering-markdown/) in it.
* **[`example.py`](example.py):** This is the main entry point for the program.
* **[`requirements.txt`](requirements.txt):** The list of packages used in your projects,
  so creating a working environment is as simple as `pip install -r requirements.txt`.
* **[`requirements-dev.txt`](requirements-dev.txt):** Same thing, but intended for
  project contributors. Development tools that are not needed for end users go here.
* **[`setup.py`](setup.py):** Allows you to package your project as a python standard
  package for deployment.


Using your project
------------------

Let's run your project. From the project root, run the following commands:

    . .env/bin/activate     # only necessary once per session
    ./example.py
    Hello, World!

Tests should also work:

    pytest

And python linter to check for common mistakes in code style:

    pylint example

Lastly, this will build the documentation into the `html` directory:

    sphinx-build docs html


Customizing your project
------------------------

To fully transform the template into your own project, follow those simple steps:

* Rename directory `example` to your project name.
* Rename `example.py` to your program name, or delete it if your project is
  a library with no entry point.
* Update `setup.py`. It's not hard, just search for “example” in the file and
  put your own name and project name.
* Same in `docs/conf.py`.
* Copy the [license you choose](https://choosealicense.com/) into the `LICENSE`
  file, and make sure to follow the LICENSE's instructions (usually this means
  adding your name and the date).
* Replace this file, `README.md` with your own instructions for your project!

That's it! Don't forget to commit the changes using `git`.

Authors
-------

* **Julien Hartmann** - https://github.com/spectras

License
-------

This template project has a double licensing:

* The template itself is licensed under the MIT License. Mostly, this means you
  must retain copyright information and license, but you are otherwise
  free to modify, copy, redistribute it or sell it. See the [LICENSE](LICENSE)
  file for details.
* Once you have customized your project as described in this README.md, you
  are free to use the resulting project in any way you want. Keeping copyright
  or mentionning the template's author is appreciated, but not required.
