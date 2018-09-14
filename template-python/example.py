#!/usr/bin/env python3
''' Entry point for example project '''

import argparse
from example.hello import say_hello

def parse_arguments(args=None):
    ''' Convert command-line arguments into usable options '''
    parser = argparse.ArgumentParser(description="Say hello")
    parser.add_argument('--out', '-o', type=argparse.FileType('w'),
                        help='output file')
    return parser.parse_args(args)

if __name__ == '__main__':
    options = parse_arguments()
    say_hello(file=options.out)
