import sys
import pytest

from example import hello


def test_hello_default(capsys):
    ''' Calling the function writes to standard output '''
    hello.say_hello()

    captured = capsys.readouterr()
    assert captured.out == 'Hello, World!\n'
    assert captured.err == ''


def test_hello_redirects(capsys, tmpdir):
    ''' Passing an open file as file argument writes to that file '''

    tempfile = tmpdir.join('sample_file')

    with tempfile.open('w') as fd:
        hello.say_hello(file=fd)

    captured = capsys.readouterr()
    assert captured.out == ''
    assert tempfile.read() == 'Hello, World!\n'


def test_hello_rejects_invalid_args():
    ''' Invalid arguments get rejected with correct exception '''

    # Passing option as positional argument should be rejected
    # pylint: disable=too-many-function-args
    with pytest.raises(TypeError):
        hello.say_hello(sys.stdout)

    # Passing something that does not have a write method should be rejected
    with pytest.raises(AttributeError):
        hello.say_hello(file='foo')
