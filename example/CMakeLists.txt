add_executable(example_wrapper test test.cc)
target_link_libraries(example_wrapper cgroomrl)

add_executable(example example.cc)
target_link_libraries(example groomrl cgroomrl ${FASTJET_LDFLAGS})
