cmake_minimum_required (VERSION 2.8)
project (opendnp3)
set(OPENDNP3_MAJOR_VERSION 2)
set(OPENDNP3_MINOR_VERSION 0)
set(OPENDNP3_MICRO_VERSION 0)
set(OPENDNP3_VERSION ${OPENDNP3_MAJOR_VERSION}.${OPENDNP3_MINOR_VERSION}.${OPENDNP3_MICRO_VERSION})

# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

if (DEFINED ENV{ASIO_HOME})
  message("ASIO_HOME defined: $ENV{ASIO_HOME}")
  include_directories($ENV{ASIO_HOME})
  set (CMAKE_REQUIRED_INCLUDES $ENV{ASIO_HOME})
endif()

include(CheckIncludeFileCXX)

# Look for and configure asio standalone
CHECK_INCLUDE_FILE_CXX(asio.hpp HAVE_ASIO)
message("Result: ${HAVE_ASIO}")
IF(NOT HAVE_ASIO)
  message(FATAL_ERROR "asio.h not found. You must define ASIO_HOME to point to your ASIO include subdirectory")
ENDIF()
add_definitions(-DASIO_STANDALONE)

# openpal
include_directories(./cpp/libs/openpal/src)
file(GLOB_RECURSE openpal_SRC 
       ./cpp/libs/openpal/src/openpal/*.cpp)
file(GLOB openpal_HDR 
       ./cpp/libs/openpal/src/openpal/*.h)
file(GLOB openpal_channel_HDR 
       ./cpp/libs/openpal/src/openpal/channel/*.h)
file(GLOB openpal_container_HDR 
       ./cpp/libs/openpal/src/openpal/container/*.h)
file(GLOB openpal_executor_HDR 
       ./cpp/libs/openpal/src/openpal/executor/*.h)
file(GLOB openpal_logging_HDR 
       ./cpp/libs/openpal/src/openpal/logging/*.h)
file(GLOB openpal_serialization_HDR 
       ./cpp/libs/openpal/src/openpal/serialization/*.h)
file(GLOB openpal_util_HDR 
       ./cpp/libs/openpal/src/openpal/util/*.h)
add_library(openpal SHARED
 ${openpal_SRC}
)

install (TARGETS openpal LIBRARY DESTINATION lib)
install (FILES ${openpal_HDR} DESTINATION include/openpal)
install (FILES ${openpal_channel_HDR} DESTINATION include/openpal/channel)
install (FILES ${openpal_container_HDR} DESTINATION include/openpal/container)
install (FILES ${openpal_executor_HDR} DESTINATION include/openpal/executor)
install (FILES ${openpal_logging_HDR} DESTINATION include/openpal/logging)
install (FILES ${openpal_serialization_HDR} DESTINATION include/openpal/serialization)
install (FILES ${openpal_util_HDR} DESTINATION include/openpal/util)

# opendnp3
include_directories(./cpp/libs/opendnp3/src)
file(GLOB_RECURSE opendnp3_SRC 
       ./cpp/libs/opendnp3/src/opendnp3/*.cpp)
file(GLOB opendnp3_HDR
       ./cpp/libs/opendnp3/src/opendnp3/*.h)
file(GLOB opendnp3_app_HDR
       ./cpp/libs/opendnp3/src/opendnp3/app/*.h)
file(GLOB opendnp3_gen_HDR
       ./cpp/libs/opendnp3/src/opendnp3/gen/*.h)
file(GLOB opendnp3_link_HDR
       ./cpp/libs/opendnp3/src/opendnp3/link/*.h)
file(GLOB opendnp3_master_HDR
       ./cpp/libs/opendnp3/src/opendnp3/master/*.h)
file(GLOB opendnp3_objects_HDR
       ./cpp/libs/opendnp3/src/opendnp3/objects/*.h)
file(GLOB opendnp3_outstation_HDR
       ./cpp/libs/opendnp3/src/opendnp3/outstation/*.h)
file(GLOB opendnp3_transport_HDR
       ./cpp/libs/opendnp3/src/opendnp3/transport/*.h)
add_library(opendnp3 SHARED
 ${opendnp3_SRC}
)
target_link_libraries(opendnp3 openpal)

install (TARGETS opendnp3 LIBRARY DESTINATION lib)
install (FILES ${opendnp3_HDR} DESTINATION include/opendnp3)
install (FILES ${opendnp3_app_HDR} DESTINATION include/opendnp3/app)
install (FILES ${opendnp3_gen_HDR} DESTINATION include/opendnp3/gen)
install (FILES ${opendnp3_link_HDR} DESTINATION include/opendnp3/link)
install (FILES ${opendnp3_master_HDR} DESTINATION include/opendnp3/master)
install (FILES ${opendnp3_objects_HDR} DESTINATION include/opendnp3/objects)
install (FILES ${opendnp3_outstation_HDR} DESTINATION include/opendnp3/outstation)
install (FILES ${opendnp3_transport_HDR} DESTINATION include/opendnp3/transport)

# asiopal
include_directories(./cpp/libs/asiopal/src)
file(GLOB_RECURSE asiopal_SRC 
       ./cpp/libs/asiopal/src/asiopal/*.cpp)
file(GLOB asiopal_HDR
       ./cpp/libs/asiopal/src/asiopal/*.h)
add_library(asiopal SHARED
 ${asiopal_SRC}
)
target_link_libraries(asiopal openpal)
install (TARGETS asiopal LIBRARY DESTINATION lib)
install (FILES ${asiopal_HDR} DESTINATION include/asiopal)

# asiodnp3
include_directories(./cpp/libs/asiodnp3/src)
file(GLOB_RECURSE asiodnp3_SRC 
       ./cpp/libs/asiodnp3/src/asiodnp3/*.cpp)
file(GLOB asiodnp3_HDR 
       ./cpp/libs/asiodnp3/src/asiodnp3/*.h)
file(GLOB asiodnp3_impl_HDR 
       ./cpp/libs/asiodnp3/src/asiodnp3/impl/*.h)
add_library(asiodnp3 SHARED
 ${asiodnp3_SRC}
)
target_link_libraries(asiodnp3 opendnp3 asiopal)
install (TARGETS asiodnp3 LIBRARY DESTINATION lib)
install (FILES ${asiodnp3_HDR} DESTINATION include/asiodnp3)
install (FILES ${asiodnp3_impl_HDR} DESTINATION include/asiodnp3/impl)

# demos
file(GLOB_RECURSE masterdemo_SRC 
       ./cpp/examples/master/*.cpp)
add_executable (masterdemo ${masterdemo_SRC})
target_link_libraries (masterdemo LINK_PUBLIC asiodnp3 pthread)

file(GLOB_RECURSE outstationdemo_SRC 
       ./cpp/examples/outstation/*.cpp)
add_executable (outstationdemo ${outstationdemo_SRC})
target_link_libraries (outstationdemo LINK_PUBLIC asiodnp3 pthread)

# tests
include_directories(./cpp/tests/catch)
file(GLOB_RECURSE openpal_TESTSRC
       ./cpp/tests/openpaltests/src/*.cpp)

add_executable (testopenpal ${openpal_TESTSRC})
target_link_libraries (testopenpal LINK_PUBLIC asiodnp3 pthread)

file(GLOB_RECURSE opendnp3_TESTSRC
       ./cpp/tests/opendnp3tests/src/*.cpp)

add_executable (testopendnp3 ${opendnp3_TESTSRC})
target_link_libraries (testopendnp3 LINK_PUBLIC asiodnp3 pthread)

