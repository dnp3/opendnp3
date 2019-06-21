cmake_minimum_required (VERSION 3.8)

set(OPENDNP3_MAJOR_VERSION 2)
set(OPENDNP3_MINOR_VERSION 3)
set(OPENDNP3_MICRO_VERSION 2)
set(OPENDNP3_VERSION ${OPENDNP3_MAJOR_VERSION}.${OPENDNP3_MINOR_VERSION}.${OPENDNP3_MICRO_VERSION})
project (opendnp3 VERSION ${OPENDNP3_VERSION})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif()

message("CMake build is: ${CMAKE_BUILD_TYPE}")

include(./cmake/settings.cmake)
include(./cmake/ClangFormat.cmake)
include(./cmake/ClangTidy.cmake)

# various optional libraries and projects

option(DNP3_ALL "Build all optional projects (secauth, demos, tests)" OFF)
option(DNP3_TEST "Build tests" OFF)
option(DNP3_DEMO "Build demo applications" OFF)
option(DNP3_DECODER "Build the decoder library" OFF)
option(DNP3_TLS "Build TLS client/server support" OFF)
option(DNP3_JAVA "Build the Java bindings" OFF)
option(DNP3_FUZZING "Build the OSSFuzz integration" OFF)
if(WIN32)
  option(DNP3_DOTNET "Build the .NET bindings" OFF)
  set(DNP3_DOTNET_FRAMEWORK_VERSION "v4.5" CACHE STRING "The target .NET framework version for the .NET components")
endif()

# other options off-by-default that you can enable
option(WERROR "Set all warnings to errors" OFF)
option(STATICLIBS "Builds static versions of all installed libraries" OFF)
option(COVERAGE "Builds the libraries with coverage info for gcov (gcc only)" OFF)
option(PROFILE "Builds the libraries with profiling support (gcc only)" OFF)
option(FLOCK "Builds the libraries with lock serial port (flock, Linux and BSD)" OFF)

if(DNP3_ALL)
  message("enabling all optional components")
  set(DNP3_DEMO ON)
  set(DNP3_TEST ON)
  set(DNP3_TLS ON)
  set(DNP3_DECODER ON)
  set(DNP3_JAVA ON)
  set(DNP3_FUZZING ON)
  if(WIN32)
    set(DNP3_DOTNET ON)
  endif()
endif()

if(DNP3_FUZZING)
  # Fuzzing depends on test lib and the decoder library
  set(DNP3_TEST ON)
  set(DNP3_DECODER ON)
endif()

if(DNP3_JAVA)
  find_package(Java COMPONENTS Development)
  find_package(JNI REQUIRED)
  include_directories(${JNI_INCLUDE_DIRS})
endif()

SET(ASIO_SUBMODULE_DIR "${PROJECT_SOURCE_DIR}/deps/asio/asio/include")

# detection stuff for ASIO
if (EXISTS "${ASIO_SUBMODULE_DIR}/asio.hpp")
    message("ASIO has been checked out as a git submodule: ${ASIO_SUBMODULE_DIR}")
  include_directories(${ASIO_SUBMODULE_DIR})
else()
  message("ASIO has NOT been checked out as a git submodule...")
  if (ASIO_HOME)
    message("ASIO_HOME defined in cache: ${ASIO_HOME}")
    include_directories(${ASIO_HOME})  
  else()
    if(DEFINED ENV{ASIO_HOME})
      message("ASIO_HOME defined in environment: $ENV{ASIO_HOME}")  
      include_directories($ENV{ASIO_HOME})    
    else()
      message("ASIO_HOME was not defined. ASIO expected to be on include path")  
    endif()
  endif()
endif()

# required for ASIO in C++11 only mode
add_definitions(-DASIO_STANDALONE)

if(DNP3_TLS)
  add_definitions(-DOPENDNP3_USE_TLS)

  find_package(OpenSSL REQUIRED)
  message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")
  include_directories(${OPENSSL_INCLUDE_DIR})
endif()

# include paths for all the local libraries
include_directories(./cpp/libs/src)
include_directories(./cpp/libs/include)
include_directories(./cpp/tests/libs/src)

# ---- openpal library ----
file(GLOB_RECURSE openpal_SRC ./cpp/libs/src/openpal/*.cpp ./cpp/libs/src/openpal/*.h ./cpp/libs/include/openpal/*.h)
add_library(openpal ${LIB_TYPE} ${openpal_SRC})
install(TARGETS openpal DESTINATION lib)
set_target_properties(openpal PROPERTIES FOLDER cpp/libs VERSION ${OPENDNP3_VERSION} SOVERSION ${OPENDNP3_MAJOR_VERSION})
clang_format(openpal)
clang_tidy(openpal)

# ---- opendnp3 library ----
file(GLOB_RECURSE opendnp3_SRC ./cpp/libs/src/opendnp3/*.cpp ./cpp/libs/src/opendnp3/*.h ./cpp/libs/include/opendnp3/*.h)
add_library(opendnp3 ${LIB_TYPE} ${opendnp3_SRC})
target_link_libraries(opendnp3 openpal)
install(TARGETS opendnp3 DESTINATION lib)
set_target_properties(opendnp3 PROPERTIES FOLDER cpp/libs VERSION ${OPENDNP3_VERSION} SOVERSION ${OPENDNP3_MAJOR_VERSION})
clang_format(opendnp3 EXCLUDES ".*/gen/.*" ".*/objects/.*")
clang_tidy(opendnp3)

if(DNP3_DECODER)
  file(GLOB_RECURSE dnp3decode_SRC ./cpp/libs/src/dnp3decode/*.cpp ./cpp/libs/src/dnp3decode/*.h ./cpp/libs/include/dnp3decode/*.h)
  add_library(dnp3decode ${LIB_TYPE} ${dnp3decode_SRC})
  target_link_libraries(dnp3decode opendnp3)
  install(TARGETS dnp3decode DESTINATION lib)
  set_target_properties(dnp3decode PROPERTIES FOLDER cpp/libs VERSION ${OPENDNP3_VERSION} SOVERSION ${OPENDNP3_MAJOR_VERSION})
  clang_format(dnp3decode)
  clang_tidy(dnp3decode)
endif()

# ---- asiopal library ----
file(GLOB_RECURSE asiopal_HPP ./cpp/libs/src/asiopal/*.h ./cpp/libs/include/asiopal/*.h)

if(DNP3_TLS)
  file(GLOB_RECURSE asiopal_CPP ./cpp/libs/src/asiopal/*.cpp)  
else()
  file(GLOB asiopal_CPP ./cpp/libs/src/asiopal/*.cpp)
endif()

add_library(asiopal ${LIB_TYPE} ${asiopal_HPP} ${asiopal_CPP})

set(asiopal_link_libraries "openpal")
if(DNP3_TLS)
    set(asiopal_link_libraries "${asiopal_link_libraries};${OPENSSL_LIBRARIES}")
endif()

target_link_libraries(asiopal ${asiopal_link_libraries})
install(TARGETS asiopal DESTINATION lib)
set_target_properties(asiopal PROPERTIES FOLDER cpp/libs VERSION ${OPENDNP3_VERSION} SOVERSION ${OPENDNP3_MAJOR_VERSION})
if(FLOCK)
  add_definitions(-DUSE_FLOCK)
endif()
clang_format(asiopal)
clang_tidy(asiopal)

# ---- asiodnp3 library ----
file(GLOB_RECURSE asiodnp3_HPP ./cpp/libs/src/asiodnp3/*.h ./cpp/libs/include/asiodnp3/*.h)
if(DNP3_TLS)
  file(GLOB_RECURSE asiodnp3_CPP ./cpp/libs/src/asiodnp3/*.cpp)
else()
  file(GLOB asiodnp3_CPP ./cpp/libs/src/asiodnp3/*.cpp)
endif()
add_library(asiodnp3 ${LIB_TYPE} ${asiodnp3_HPP} ${asiodnp3_CPP})
target_link_libraries(asiodnp3 asiopal opendnp3)
install(TARGETS asiodnp3 DESTINATION lib)
set_target_properties(asiodnp3 PROPERTIES FOLDER cpp/libs VERSION ${OPENDNP3_VERSION} SOVERSION ${OPENDNP3_MAJOR_VERSION})
clang_format(asiodnp3)
clang_tidy(asiodnp3)

if(DNP3_JAVA)
  file(GLOB_RECURSE opendnp3java_SRC ./java/cpp/*.h ./java/cpp/*.cpp)
  add_library(opendnp3java SHARED ${opendnp3java_SRC})
  target_link_libraries(opendnp3java asiodnp3)
  install(TARGETS opendnp3java DESTINATION lib)
  set_target_properties(opendnp3java PROPERTIES FOLDER java VERSION ${OPENDNP3_VERSION} SOVERSION ${OPENDNP3_MAJOR_VERSION})
  clang_format(opendnp3java EXCLUDES ".*/jni/.*" "com_automatak_.*h$")
  clang_tidy(opendnp3java)
endif()

# ----- install -----

# common pattern and exludes for all installed headers
set(INSTALL_ARGS FILES_MATCHING PATTERN "*.h" PATTERN ".deps" EXCLUDE PATTERN ".libs" EXCLUDE)
install(DIRECTORY ./cpp/libs/include/ DESTINATION include ${INSTALL_ARGS})

if(DNP3_DEMO)

  # ----- master demo executable -----
  add_executable(master-demo ./cpp/examples/master/main.cpp)
  target_link_libraries (master-demo LINK_PUBLIC asiodnp3 ${PTHREAD})
  set_target_properties(master-demo PROPERTIES FOLDER cpp/examples)
  clang_format(master-demo)
  clang_tidy(master-demo)

  # ----- master demo executable -----
  add_executable(master-gprs-demo ./cpp/examples/master-gprs/main.cpp ./cpp/examples/master-gprs/ExampleListenCallbacks.cpp ./cpp/examples/master-gprs/ExampleListenCallbacks.h)
  target_link_libraries (master-gprs-demo LINK_PUBLIC asiodnp3 ${PTHREAD})
  set_target_properties(master-gprs-demo PROPERTIES FOLDER cpp/examples)
  clang_format(master-gprs-demo)
  clang_tidy(master-gprs-demo)

  # ----- outstation demo executable -----
  add_executable(outstation-demo ./cpp/examples/outstation/main.cpp)
  target_link_libraries (outstation-demo LINK_PUBLIC asiodnp3 ${PTHREAD})
  set_target_properties(outstation-demo PROPERTIES FOLDER cpp/examples)
  clang_format(outstation-demo)
  clang_tidy(outstation-demo)

  if(DNP3_TLS)

    # ----- master tls executable -----
    add_executable(master-tls-demo ./cpp/examples/tls/master/main.cpp)
    target_link_libraries (master-tls-demo LINK_PUBLIC asiodnp3 ${PTHREAD})
    set_target_properties(master-tls-demo PROPERTIES FOLDER cpp/examples/tls)
    clang_format(master-tls-demo)
    clang_tidy(master-tls-demo)

    # ----- outstation tls executable -----
    add_executable(outstation-tls-demo ./cpp/examples/tls/outstation/main.cpp)
    target_link_libraries (outstation-tls-demo LINK_PUBLIC asiodnp3 ${PTHREAD})
    set_target_properties(outstation-tls-demo PROPERTIES FOLDER cpp/examples/tls)
    clang_format(outstation-tls-demo)
    clang_tidy(outstation-tls-demo)

    # ----- master-gprs tls executable -----
    add_executable(master-gprs-tls-demo ./cpp/examples/tls/master-gprs/main.cpp)
    target_link_libraries (master-gprs-tls-demo LINK_PUBLIC asiodnp3 ${PTHREAD})
    set_target_properties(master-gprs-tls-demo PROPERTIES FOLDER cpp/examples/tls)
    clang_format(master-gprs-tls-demo)
    clang_tidy(master-gprs-tls-demo)

  endif()

endif()

if(DNP3_DECODER)

  # ----- decoder executable -----
  add_executable(decoder ./cpp/examples/decoder/main.cpp)
  target_link_libraries (decoder asiodnp3 dnp3decode ${PTHREAD})
  set_target_properties(decoder PROPERTIES FOLDER cpp/examples)
  clang_format(decoder)
  clang_tidy(decoder)

endif()

if(DNP3_TEST)

  enable_testing()

  # ----- testlib library ------
  file(GLOB_RECURSE testlib_SRC ./cpp/tests/libs/src/testlib/*.cpp ./cpp/tests/libs/src/testlib/*.h)
  add_library(testlib ${testlib_SRC})
  target_link_libraries(testlib openpal)
  set_target_properties(testlib PROPERTIES FOLDER cpp/tests/unit/mocks)
  clang_format(testlib)
  clang_tidy(testlib)

  # ----- dnp3mocks library ------
  file(GLOB_RECURSE dnp3mocks_SRC ./cpp/tests/libs/src/dnp3mocks/*.cpp ./cpp/tests/libs/src/dnp3mocks/*.h)
  add_library(dnp3mocks ${dnp3mocks_SRC})
  target_link_libraries(dnp3mocks opendnp3 testlib)
  set_target_properties(dnp3mocks PROPERTIES FOLDER cpp/tests/unit/mocks)
  clang_format(dnp3mocks)
  clang_tidy(dnp3mocks)

  # ----- openpal tests -----
  file(GLOB_RECURSE openpal_TESTSRC ./cpp/tests/openpal/src/*.cpp ./cpp/tests/openpal/src/*.h)
  add_executable (testopenpal ${openpal_TESTSRC})
  target_link_libraries (testopenpal LINK_PUBLIC testlib ${PTHREAD})
  set_target_properties(testopenpal PROPERTIES FOLDER cpp/tests/unit)
  clang_format(testopenpal)
  clang_tidy(testopenpal)
  add_test(testopenpal testopenpal)

  # ----- opendnp3 tests -----
  file(GLOB_RECURSE opendnp3_TESTSRC ./cpp/tests/opendnp3/src/*.cpp ./cpp/tests/opendnp3/src/*.h)
  add_executable (testopendnp3 ${opendnp3_TESTSRC})
  target_link_libraries (testopendnp3 LINK_PUBLIC dnp3mocks ${PTHREAD})
  set_target_properties(testopendnp3 PROPERTIES FOLDER cpp/tests/unit)
  clang_format(testopendnp3)
  clang_tidy(testopendnp3)
  add_test(testopendnp3 testopendnp3)

  # ----- asiopal tests -----
  if(DNP3_TLS)
    file(GLOB_RECURSE asiopal_TESTSRC ./cpp/tests/asiopal/src/*.cpp ./cpp/tests/asiopal/src/*.h)
  else()
    file(GLOB asiopal_TESTSRC ./cpp/tests/asiopal/src/*.cpp ./cpp/tests/asiopal/src/*.h ./cpp/tests/asiopal/src/mocks/*.cpp ./cpp/tests/asiopal/src/mocks/*.h)
  endif()
  add_executable (testasiopal ${asiopal_TESTSRC})
  target_link_libraries (testasiopal LINK_PUBLIC asiopal testlib ${PTHREAD})
  set_target_properties(testasiopal PROPERTIES FOLDER cpp/tests/integration)
  clang_format(testasiopal)
  clang_tidy(testasiopal)
  add_test(testasiopal testasiopal)

  # ----- asiodnp3 tests -----
  file(GLOB_RECURSE asiodnp3_TESTSRC ./cpp/tests/asiodnp3/src/*.cpp ./cpp/tests/asiodnp3/src/*.h)
  add_executable (testasiodnp3 ${asiodnp3_TESTSRC})
  target_link_libraries (testasiodnp3 LINK_PUBLIC asiodnp3 dnp3mocks ${PTHREAD})
  set_target_properties(testasiodnp3 PROPERTIES FOLDER cpp/tests/integration)
  clang_format(testasiodnp3)
  clang_tidy(testasiodnp3)
  add_test(testasiodnp3 testasiodnp3)

endif()

# ----- fuzz tests -----
if(DNP3_FUZZING)
  add_executable (fuzzdecoder ./cpp/tests/fuzz/onefile.cpp ./cpp/tests/fuzz/fuzzdecoder.cpp)
  target_link_libraries (fuzzdecoder asiodnp3 dnp3decode ${PTHREAD})
  set_target_properties(fuzzdecoder PROPERTIES FOLDER cpp/tests/fuzzing)
  clang_format(fuzzdecoder)
  clang_tidy(fuzzdecoder)

  add_executable (fuzzoutstation ./cpp/tests/fuzz/onefile.cpp ./cpp/tests/fuzz/fuzzoutstation.cpp)
  target_link_libraries (fuzzoutstation dnp3mocks ${PTHREAD})
  set_target_properties(fuzzoutstation PROPERTIES FOLDER cpp/tests/fuzzing)
  clang_format(fuzzoutstation)
  clang_tidy(fuzzoutstation)

  add_executable (fuzzmaster ./cpp/tests/fuzz/onefile.cpp ./cpp/tests/fuzz/fuzzmaster.cpp)
  target_link_libraries (fuzzmaster dnp3mocks ${PTHREAD})
  set_target_properties(fuzzmaster PROPERTIES FOLDER cpp/tests/fuzzing)
  clang_format(fuzzmaster)
  clang_tidy(fuzzmaster)
endif()

if(WIN32 AND DNP3_DOTNET)

  message("The .NET framework version is: ${DNP3_DOTNET_FRAMEWORK_VERSION}")

  add_subdirectory(./dotnet/CLRInterface)
  add_subdirectory(./dotnet/CLRAdapter)

  if(DNP3_DEMO)
    add_subdirectory(./dotnet/examples/master)
    add_subdirectory(./dotnet/examples/master-gprs)
    add_subdirectory(./dotnet/examples/master-gprs-tls)
    add_subdirectory(./dotnet/examples/outstation)
  endif()

endif()

# ----- Formating -----
#add_custom_target(
#	format
#	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#	COMMAND astyle -R ./cpp/*.h ./cpp/*.cpp --options=./config/astyle.cfg --exclude=./cpp/libs/include/opendnp3/gen --exclude=./cpp/libs/src/opendnp3/gen --exclude=./cpp/libs/src/opendnp3/objects
#	COMMAND astyle -R ./java/cpp/adapters/*.h ./java/cpp/adapters/*.cpp --options=./config/astyle.cfg
#)

define_clang_format()
define_clang_tidy()

# ----- Packaging -----
set(CPACK_PACKAGE_VERSION_MAJOR ${OPENDNP3_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${OPENDNP3_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${OPENDNP3_MICRO_VERSION})
include(CPack)
