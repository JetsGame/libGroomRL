# This file is part of GroomRL by S. Carrazza and F. A. Dreyer
import cffi
ffibuilder = cffi.FFI()

with open('groomrl/groomrl.h') as f:
    ffibuilder.embedding_api(f.read())

ffibuilder.set_source('cgroomrl', r'''
    #include "groomrl/groomrl.h"
''', source_extension='.cc')

with open('wrapper.py') as f:
    ffibuilder.embedding_init_code(f.read())

ffibuilder.compile(target='libgroomrl.*', verbose=True)
