# python名前空間およびnumpy名前空間の関数・クラスを使って書かれたC++コードを、Pythonに翻訳する
# かなり雑なヒューリスティクスに依存したクソコードではある

import sys
import re
mainfunc = re.compile(r'.*int\s+main\s*\(.*\)\s*\{(.*)}', re.DOTALL)

def main(input_fname, output_fname=None):
    if output_fname is None:
       output_fname = input_fname.replace('.cpp', '.py') 
    
    with open(input_fname) as f:
        cpp = mainfunc.match(f.read()).groups()[0].strip(' \n')

    py = cpp.replace('\n  ', '\n').replace('//', '#').replace('::', '.').replace('auto ', '').replace(';', '').replace('const', '').replace('&', '').replace('.T()', '.T').replace('{', '(').replace('}', ')')

    with open(output_fname, 'w') as g:
        g.write('import numpy as np\n\n' + py)
        
if __name__ == '__main__':
    if not (2 <= len(sys.argv) <= 3):
        print("usage: python translator.py [input] [output]")
        sys.exit(1)

    input_fname = sys.argv[1]
        
    if len(sys.argv) == 2:
        output_fname = None
    else:
        output_fname = sys.argv[2]
        
    main(input_fname, output_fname)
