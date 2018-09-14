''' The classical hello world module.
'''
import sys

def say_hello(*, file=None):
    ''' Say hello to the world.

    :param file: an open file to write to, or `None` to use the standard output. Defaults to `None`
    '''
    if file is None:
        file = sys.stdout
    print("Hello, World!", file=file)
