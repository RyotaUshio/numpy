# python名前空間およびnumpy名前空間の関数・クラスを使って書かれたC++コードを、Pythonに翻訳する
# かなり雑なヒューリスティクスに依存する

import sys
import re
import os

mainfunc = re.compile(r'.*int\s+main\s*\(.*\)\s*\{(.*)}', re.DOTALL)

def replace(cpp_source : str, replace_dict : dict) -> str:
    for key, val in replace_dict.items():
        cpp_source = cpp_source.replace(key, val)
    return cpp_source

def translate(input_fname, output_fname):
    with open(input_fname) as f:
        cpp = mainfunc.match(f.read()).groups()[0].strip(' \n')

    py = replace(re.sub(r'print\((.*)\);', 'print(repr(\\1));', cpp),
                 {'\n  '   : '\n',
                  '//'     : '#',
                  '::'     : '.',
                  'auto '  : '',
                  ';'      : '',
                  'const'  : '',
                  '&'      : '',
                  '.T()'   : '.T',
                  '{'      : '(',
                  '}'      : ')',
                  'int '   : ''
                 })

    with open(output_fname, 'w') as g:
        g.write('import numpy as np\n\n' + py)

def run(input_fname, output_fname):
    input_basename = re.match(r'(.*/)?(.*).cpp', input_fname).groups()[1]
    os.system(f"./py++ {input_basename} -f");
    os.system(f"python {output_fname}");

def main():
    if not (2 <= len(sys.argv) <= 3):
        print("usage: python translator.py [input] [output]")
        sys.exit(1)

    input_fname = sys.argv[1]
    
    if len(sys.argv) == 2:
        output_fname = input_fname.replace('.cpp', '.py') 

    else:
        output_fname = sys.argv[2]

    print(input_fname, output_fname)
    translate(input_fname, output_fname)
    run(input_fname, output_fname)
        
if __name__ == '__main__':
    main()
