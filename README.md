# Installation

In order to install proceed with the usual cmake steps:
```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=<your install prefix>
make
make install
```
We provide the cmake options `-DENABLE_TEST=ON/OFF` in order to compile the
test example located in the `test` folder.


# Usage

The compiler flags to include this library in your package can be
retreived with:
```bash
pkg-config groomrl --cflags
pkg-config groomrl --libs
```

Sample programs using this library are provided in the examples/ directory.