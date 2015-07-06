cmake_minimum_required (VERSION 2.8)
project (opendnp3)
set(OPENDNP3_MAJOR_VERSION 2)
set(OPENDNP3_MINOR_VERSION 0)
set(OPENDNP3_MICRO_VERSION 1)
set(OPENDNP3_VERSION ${OPENDNP3_MAJOR_VERSION}.${OPENDNP3_MINOR_VERSION}.${OPENDNP3_MICRO_VERSION})

# options off-by-default that you can enable
option(DNP3_LIB_ONLY  "build only the core libraries" OFF)

set(DEFAULT_ENABLE ON)

if(DNP3_LIB_ONLY)
  set(DEFAULT_ENABLE OFF)
endif()

option(DEMO "Build demo applications" ${DEFAULT_ENABLE})
option(TEST "Build tests" ${DEFAULT_ENABLE})

# other options off-by-default that you can enable
option(LIBCPP "manually link with libc++ (required for clang in some configurations)" OFF)
option(WERROR "Set all warnings to errors" OFF)
option(STATICLIBS "Builds static versions of all installed libraries" OFF)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

if(STATICLIBS)
  set(LIB_TYPE STATIC)
else()
  set(LIB_TYPE SHARED)
endif()

if (WERROR)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

if (LIBCPP)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})

# different release and debug flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
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

# common pattern and exludes for all installed headers
set(INSTALL_ARGS FILES_MATCHING PATTERN "*.h" PATTERN ".deps" EXCLUDE PATTERN ".libs" EXCLUDE)

# ---- openpal library ----
file(GLOB_RECURSE openpal_SRC ./cpp/libs/openpal/src/openpal/*.cpp)
add_library(openpal ${LIB_TYPE} ${openpal_SRC})
install(TARGETS openpal DESTINATION lib)
install(DIRECTORY ./cpp/libs/openpal/src/openpal/ DESTINATION include/openpal ${INSTALL_ARGS})

# ---- opendnp3 library ----
file(GLOB_RECURSE opendnp3_SRC ./cpp/libs/opendnp3/src/opendnp3/*.cpp)
add_library(opendnp3 ${LIB_TYPE} ${opendnp3_SRC})
target_link_libraries(opendnp3 openpal)
install(TARGETS opendnp3 DESTINATION lib)
install(DIRECTORY ./cpp/libs/opendnp3/src/opendnp3/ DESTINATION include/opendnp3 ${INSTALL_ARGS})

# ---- asiopal library ----
file(GLOB_RECURSE asiopal_SRC ./cpp/libs/asiopal/src/asiopal/*.cpp)
add_library(asiopal ${LIB_TYPE} ${asiopal_SRC})
target_link_libraries(asiopal openpal)
install(TARGETS asiopal DESTINATION lib)
install(DIRECTORY ./cpp/libs/asiopal/src/asiopal/ DESTINATION include/asiopal ${INSTALL_ARGS})

# ---- asiodnp3 library ----
file(GLOB_RECURSE asiodnp3_SRC ./cpp/libs/asiodnp3/src/asiodnp3/*.cpp)
add_library(asiodnp3 ${LIB_TYPE} ${asiodnp3_SRC})
target_link_libraries(asiodnp3 opendnp3)
target_link_libraries(asiodnp3 asiopal)
install(TARGETS asiodnp3 DESTINATION lib)
install(DIRECTORY ./cpp/libs/asiodnp3/src/asiodnp3/ DESTINATION include/asiodnp3 ${INSTALL_ARGS})

if(DEMO)

  # ----- master demo executable -----
  add_executable (masterdemo ./cpp/examples/master/DemoMain.cpp)
  target_link_libraries (masterdemo LINK_PUBLIC asiodnp3 pthread)

  # ----- outstation demo executable -----
  add_executable (outstationdemo ./cpp/examples/outstation/DemoMain.cpp)
  target_link_libraries (outstationdemo LINK_PUBLIC asiodnp3 pthread)

endif()

if(TEST)  

  # ----- openpal tests -----
  file(GLOB_RECURSE openpal_TESTSRC ./cpp/tests/openpaltests/src/*.cpp)
  add_executable (testopenpal ${openpal_TESTSRC})
  target_link_libraries (testopenpal LINK_PUBLIC openpal pthread)

  # ----- opendnp3 tests -----
  file(GLOB_RECURSE opendnp3_TESTSRC ./cpp/tests/opendnp3tests/src/*.cpp)
  add_executable (testopendnp3 ${opendnp3_TESTSRC})
  target_link_libraries (testopendnp3 LINK_PUBLIC asiodnp3 pthread)  

endif()

