#SET(CMAKE_VERBOSE_MAKEFILE TRUE)

ENABLE_TESTING()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -Wall -Wextra -DIN_GCC")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage -Wall -Wextra")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage -Wall -Wextra -DIN_GCC")
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ftest-coverage -Wall -Wextra")
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

TARGET_LINK_LIBRARIES(core boost_filesystem)
TARGET_LINK_LIBRARIES(gnocchi boost_program_options)

ADD_LIBRARY(test_input
  test_input.c
  before.cpp
  refactored.cpp
)

ADD_EXECUTABLE(system_test_1 system_test_1.cpp)
ADD_TEST(system_test_1 system_test_1)

ADD_EXECUTABLE(test_mccabe test_mccabe.cpp)
TARGET_LINK_LIBRARIES(test_mccabe bgl-viz)

ADD_EXECUTABLE(test_npath_counter test_npath_counter.cpp)
ADD_TEST(test_npath_counter test_npath_counter)

