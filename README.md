libGroomRL
==========

## About

libGroomRL is a C++ library allowing an easy access in C++ analyses to grooming strategies based on neural network models trained by 
[GroomRL](https://github.com/JetsGame/GroomRL "GroomRL github").

## Installation

In order to install proceed with the usual cmake steps:
```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=<your install prefix>
make
make install
```
We provide the cmake options `-DENABLE_WRAPPERONLY=ON/OFF` in order to compile a light wrapper 
`libgroomrl-fastjet` which only includes a wrapper for the python call evaluating the neural network output.


## Usage

The compiler flags to include this library in your package can be
retreived with:
```bash
pkg-config groomrl --cflags
pkg-config groomrl --libs
```

Sample programs using this library are provided in the examples/ directory.
