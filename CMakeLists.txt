cmake_minimum_required (VERSION 2.8)
project (opendnp3)
set(OPENDNP3_MAJOR_VERSION 2)
set(OPENDNP3_MINOR_VERSION 0)
set(OPENDNP3_MICRO_VERSION 1)
set(OPENDNP3_VERSION ${OPENDNP3_MAJOR_VERSION}.${OPENDNP3_MINOR_VERSION}.${OPENDNP3_MICRO_VERSION})

# options for the project you can enable
option(LIBCPP "link with libc++ (required for clang)" OFF)
option(OSSL "build the openssl crypto wrapper for SA" OFF)

if (LIBCPP)
  message("Setting the stdlib to libc++")
  set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -stdlib=libc++")
else()
  set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
endif()

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
include_directories(./cpp/libs/secauthv5/src)
include_directories(./cpp/libs/openpal/src)
include_directories(./cpp/libs/opendnp3/src)
include_directories(./cpp/libs/asiopal/src)
include_directories(./cpp/libs/asiodnp3/src)
include_directories(./cpp/tests/testlib/src)
include_directories(./cpp/tests/catch)
include_directories(./cpp/libs/osslcrypto/src)


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

# ---- secauthv5 library ----
file(GLOB_RECURSE secauthv5_SRC 
       ./cpp/libs/secauthv5/src/secauthv5/*.cpp)

add_library(secauthv5
 ${secauthv5_SRC}
)

if(OSSL)
# ---- osslcrypto library ----
file(GLOB_RECURSE osslcrypto_SRC 
       ./cpp/libs/osslcrypto/src/osslcrypto/*.cpp)

add_library(osslcrypto
 ${osslcrypto_SRC}
)
endif()



# ---- testlib library ----
file(GLOB_RECURSE testlib_SRC 
       ./cpp/tests/testlib/src/testlib/*.cpp)

add_library(testlib
 ${testlib_SRC}
)

# ---- declare linking dependencies, i.e. if A then B ----

target_link_libraries(secauthv5 opendnp3)
target_link_libraries(secauthv5 openpal)
target_link_libraries(opendnp3 openpal)
target_link_libraries(testlib openpal)
target_link_libraries(asiopal openpal)
target_link_libraries(asiodnp3 opendnp3)
target_link_libraries(asiodnp3 asiopal)


# ----- master demo executable -----
add_executable (masterdemo ./cpp/examples/master/DemoMain.cpp)
target_link_libraries (masterdemo LINK_PUBLIC asiodnp3 pthread)

# ----- outstation demo executable -----
add_executable (outstationdemo ./cpp/examples/outstation/DemoMain.cpp)
target_link_libraries (outstationdemo LINK_PUBLIC asiodnp3 pthread)

if(OSSL)
# ----- outstation SAv5 demo executable -----
add_executable (outstationdemoSAv5 ./cpp/examples/sav5/outstation/DemoMain.cpp)
target_link_libraries (outstationdemoSAv5 LINK_PUBLIC asiodnp3 secauthv5 osslcrypto crypto pthread)
endif()

# ----- openpal tests -----
file(GLOB_RECURSE openpal_TESTSRC
       ./cpp/tests/openpaltests/src/*.cpp)

add_executable (testopenpal ${openpal_TESTSRC})
target_link_libraries (testopenpal LINK_PUBLIC openpal pthread)

# ----- opendnp3 tests -----

file(GLOB_RECURSE opendnp3_TESTSRC
       ./cpp/tests/opendnp3tests/src/*.cpp)

add_executable (testopendnp3 ${opendnp3_TESTSRC})
target_link_libraries (testopendnp3 LINK_PUBLIC asiodnp3 secauthv5 testlib pthread)

