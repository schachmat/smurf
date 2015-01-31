#!/bin/env python

import sys
from pycparser import parse_file, c_generator

sys.path.extend(['.', '..'])


def impl_struct():
    return True


def impl_func():
    return True


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(1)

    fname = sys.argv[1]
    ast = parse_file("gen.c", use_cpp=True, cpp_args=[r'-Ifake', r'-I.'])
    for e in ast.ext:
        if not fname in str(e.coord):
            continue

        generator = c_generator.CGenerator()
        print(generator.visit(e))

#        e.show()
