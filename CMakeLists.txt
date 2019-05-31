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

# build custom library
add_custom_target(groomrl_target ALL
                  COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/src/build.py
                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/src
)
add_library(cgroomrl SHARED IMPORTED GLOBAL)
add_dependencies(cgroomrl groomrl_target)
set_target_properties(cgroomrl PROPERTIES
                      IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/src/libcgroomrl.so
                      INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/src/groomrl)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${FASTJET_CXX_FLAGS})
add_library(groomrl SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/groomrl.cc)

# pkg-config
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix "${prefix}")
set(includedir "${prefix}/include")
set(libdir "${prefix}/lib")

configure_file(
  "${PROJECT_SOURCE_DIR}/src/groomrl.pc.in"
  "${PROJECT_SOURCE_DIR}/src/groomrl.pc"
  )


install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/libgroomrl.so DESTINATION lib PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES ${PROJECT_SOURCE_DIR}/src/groomrl.pc DESTINATION lib/pkgconfig)
install(DIRECTORY src/groomrl DESTINATION include)


# nnpdfcpp configuration
if(ENABLE_TEST)
  include_directories(BEFORE src/groomrl src)
  add_subdirectory(example)
endif(ENABLE_TEST)
