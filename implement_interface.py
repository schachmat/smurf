#!/bin/env python

import sys
from pycparser import parse_file

sys.path.extend(['.', '..'])


def impl_struct():
    return True


def impl_func():
    return True


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(1)

    ast = parse_file(sys.argv[1], use_cpp=True, cpp_args=[r'-Ifake', r'-I.'])
    ast.show(showcoord=True)
