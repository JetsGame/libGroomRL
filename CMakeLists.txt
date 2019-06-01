cmake_minimum_required (VERSION 3.0.2)

# Use rpaths for now, previously there were issues with osx
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

find_package(PythonInterp 3 REQUIRED)

project(libgroomrl)

set(VERSION "\"1.0.0\"")

option(ENABLE_TEST "Enable test" OFF)

# fastjet
find_program(FASTJET_CONFIG fastjet-config REQUIRED)
if (FASTJET_CONFIG)
  exec_program(${FASTJET_CONFIG}
    ARGS --cxxflags
    OUTPUT_VARIABLE FASTJET_CXX_FLAGS
  )
  set(FASTJET_CXX_FLAGS ${FASTJET_CXX_FLAGS} CACHE STRING INTERNAL)
  exec_program(${FASTJET_CONFIG}
    ARGS --libs
    OUTPUT_VARIABLE FASTJET_LDFLAGS
  )
  set(FASTJET_LDFLAGS ${FASTJET_LDFLAGS} CACHE STRING INTERNAL)
endif(FASTJET_CONFIG)

# running the cffi builder
if (NOT EXISTS ${PROJECT_SOURCE_DIR/src/cgroomrl.cc})
  execute_process(COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/src/build.py WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
endif()

find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs   3 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FASTJET_CXX_FLAGS}")
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(src)
add_library(groomrl SHARED src/groomrl.cc src/cgroomrl.cc)

# pkg-config
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix "${prefix}")
set(includedir "${prefix}/include")
set(extraincludirs "-I${PYTHON_INCLUDE_DIR} ${FASTJET_CXX_FLAGS}")
set(libdir "${prefix}/lib")
set(fastjetlibs "${FASTJET_LDFLAGS}")
set(pythonlibs "${PYTHON_LIBRARIES}")

configure_file(
  "${PROJECT_SOURCE_DIR}/src/groomrl.pc.in"
  "${PROJECT_SOURCE_DIR}/src/groomrl.pc"
  )

install(FILES ${PROJECT_SOURCE_DIR}/src/groomrl.pc DESTINATION lib/pkgconfig)
install(DIRECTORY src/groomrl DESTINATION include)
install(TARGETS groomrl LIBRARY DESTINATION lib)