SET(CMAKE_VERBOSE_MAKEFILE TRUE)

ENABLE_TESTING()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -Wall -Wextra -DIN_GCC")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage -Wall -Wextra")
ADD_DEFINITIONS('-DBASEVER="4.1.2"' '-DDATESTAMP=""' '-DDEVPHASE=""')

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/gcc
)

ADD_LIBRARY(core
  gcov_reader.cpp
  analyser.cpp
  errors.c
)

LINK_LIBRARIES(core)

ADD_EXECUTABLE(gnocchi
  main.cpp
  gcov_reader.cpp
  version.c
  )

ADD_LIBRARY(test_input test_input.c)

ADD_EXECUTABLE(system_test_1 system_test_1.cpp)

ADD_TEST(system_test_1 system_test_1)

