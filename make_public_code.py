# encoding: utf-8
import sys
import os


def generate_from_file(filename, fpo):
    with open(filename, 'r') as fpi:
        output = True
        for line in fpi:
            if line.strip().startswith('// filter start'):
                output = False
            elif line.strip().startswith('// filter end'):
                output = True
            elif output:
                fpo.write(line)


def generate_public(filename):
    curr_dir = os.path.dirname(filename)
    if os.path.basename(filename) == 'main.cpp':
        with open(filename, 'r') as fpi:
            with open(os.path.join(curr_dir, 'main_public.cpp'), 'w') as fpo:
                output = True
                for line in fpi:
                    if line.strip().startswith('#include "'):
                        # project header need to expand it
                        subfile = os.path.join(curr_dir, line.strip().replace('#include', '').replace('"', '').strip())
                        generate_from_file(subfile, fpo)
                    elif line.strip().startswith('// filter start'):
                        output = False
                    elif line.strip().startswith('// filter end'):
                        output = True
                    elif output:
                        fpo.write(line)


def main():
    if len(sys.argv) > 1:
        generate_public(sys.argv[1])


if __name__ == '__main__':
    main()
