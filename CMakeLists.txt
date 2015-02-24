cmake_minimum_required (VERSION 2.8)
project (opendnp3)
set(OPENDNP3_MAJOR_VERSION 2)
set(OPENDNP3_MINOR_VERSION 0)
set(OPENDNP3_MICRO_VERSION 1)
set(OPENDNP3_VERSION ${OPENDNP3_MAJOR_VERSION}.${OPENDNP3_MINOR_VERSION}.${OPENDNP3_MICRO_VERSION})

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

if (DEFINED ENV{ASIO_HOME})
  message("ASIO_HOME defined: $ENV{ASIO_HOME}")
  include_directories($ENV{ASIO_HOME})
  set (CMAKE_REQUIRED_INCLUDES $ENV{ASIO_HOME})
else()
  message(FATAL_ERROR "You must define ASIO_HOME to point to your ASIO include subdirectory")
endif()

# include paths for all the local libraries
include_directories(./cpp/libs/openpal/src)
include_directories(./cpp/libs/opendnp3/src)
include_directories(./cpp/libs/asiopal/src)
include_directories(./cpp/libs/asiodnp3/src)
include_directories(./cpp/tests/catch)

# required for ASIO in C++11 only mode
add_definitions(-DASIO_STANDALONE)


# ---- openpal library ----
file(GLOB_RECURSE openpal_SRC 
       ./cpp/libs/openpal/src/openpal/*.cpp)

add_library(openpal 
 ${openpal_SRC}
)

# ---- opendnp3 library ----
file(GLOB_RECURSE opendnp3_SRC 
       ./cpp/libs/opendnp3/src/opendnp3/*.cpp)

add_library(opendnp3
 ${opendnp3_SRC}
)

# ---- asiopal library ----

file(GLOB_RECURSE asiopal_SRC 
       ./cpp/libs/asiopal/src/asiopal/*.cpp)

add_library(asiopal
 ${asiopal_SRC}
)

# ---- asiodnp3 library ----
file(GLOB_RECURSE asiodnp3_SRC 
       ./cpp/libs/asiodnp3/src/asiodnp3/*.cpp)

add_library(asiodnp3
 ${asiodnp3_SRC}
)

# ---- declare linking dependencies, i.e. if A then B ----

target_link_libraries(opendnp3 openpal)
target_link_libraries(asiopal openpal)
target_link_libraries(asiodnp3 opendnp3)
target_link_libraries(asiodnp3 asiopal)

# ----- master demo executable -----

add_executable (masterdemo ./cpp/examples/master/DemoMain.cpp)
target_link_libraries (masterdemo LINK_PUBLIC asiodnp3 pthread)

# ----- outstation demo executable -----
add_executable (outstationdemo ./cpp/examples/outstation/DemoMain.cpp)
target_link_libraries (outstationdemo LINK_PUBLIC asiodnp3 pthread)

if(0)
file(GLOB_RECURSE openpal_TESTSRC
       ./cpp/tests/openpaltests/src/*.cpp)

file(GLOB_RECURSE opendnp3_TESTSRC
       ./cpp/tests/opendnp3tests/src/*.cpp)

add_executable (testopenpal ${openpal_TESTSRC})
target_link_libraries (testopenpal LINK_PUBLIC asiodnp3 pthread)

add_executable (testopendnp3 ${opendnp3_TESTSRC})
target_link_libraries (testopendnp3 LINK_PUBLIC asiodnp3 pthread)
endif(0)
