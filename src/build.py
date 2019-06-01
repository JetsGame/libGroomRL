# This file is part of GroomRL by S. Carrazza and F. A. Dreyer
import cffi
ffibuilder = cffi.FFI()

with open('groomrl/cgroomrl.h') as f:
    ffibuilder.embedding_api(f.read())

ffibuilder.set_source('cgroomrl', r'''
    #include "groomrl/cgroomrl.h"
''', source_extension='.cc')

with open('wrapper.py') as f:
    ffibuilder.embedding_init_code(f.read())

ffibuilder.emit_c_code('cgroomrl.cc')
