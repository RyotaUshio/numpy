import re, sys

signature = re.compile(r'^(\w+)\((.+)\)$', re.MULTILINE)
description = re.compile(r'^\w.*\.$', re.MULTILINE)
section = re.compile(r'^\w+$')

def main(source, target):
    with open(source) as f, open(target, 'w') as g:
        g.write("// https://numpy.org/doc/stable/reference/ufuncs.html\n\n")
        g.write("#pragma once\n#include <algorithm>\n\nnamespace numpy {\n\n")
        
        for line in f:
            sig = signature.match(line)
            des = description.match(line)
            sec = section.match(line)
            
            if sig:
                name, args = sig.groups()
                binary = 'x1, x2' in args
                unary = not binary and 'x' in args

                if unary:
                    g.write(f'  constexpr ufunc_unary {name}();  ')
                elif binary:
                    g.write(f'  constexpr ufunc_binary {name}(); ')
                else:
                    raise Exception(f'{name} is neither unary or binary?')

            elif des:
                comment = '\t// ' + des.group() + '\n'
                g.write(comment)

            elif sec:
                g.write(f'\n\n  /{f" {sec.group()} ":*^80}/\n')

        g.write("\n}\n")

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print(f"usage: python {sys.argv[0]} [source] [target]")
        sys.exit(1)
    main(sys.argv[1], sys.argv[2])
    
