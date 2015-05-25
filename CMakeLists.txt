cmake_minimum_required (VERSION 2.8)
project (opendnp3)
set(OPENDNP3_MAJOR_VERSION 2)
set(OPENDNP3_MINOR_VERSION 0)
set(OPENDNP3_MICRO_VERSION 1)
set(OPENDNP3_VERSION ${OPENDNP3_MAJOR_VERSION}.${OPENDNP3_MINOR_VERSION}.${OPENDNP3_MICRO_VERSION})

# options for the project you can enable
option(LIBCPP "link with libc++ (required for clang)" OFF)
option(OSSL "build the openssl crypto wrapper for SA" OFF)
option(WERROR "Set all warnings to errors" OFF)
option(STATICLIBS "Builds static versions of all installed libraries" OFF)
option(FUZZCOV "Builds openpal and opendnp3 with clang coverage flags" OFF)
option(DEMOS "Builds demo applications" OFF)

# only used for opendnp3/openpal when FUZZCOV=ON
set(FUZZ_LINK_FLAGS "-fsanitize=address")
set(FUZZ_COMPILE_FLAGS "${FUZZ_LINK_FLAGS} -fsanitize-coverage=4")

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
include_directories(./cpp/libs/secauthv5/src)
include_directories(./cpp/libs/openpal/src)
include_directories(./cpp/libs/opendnp3/src)
include_directories(./cpp/libs/asiopal/src)
include_directories(./cpp/libs/asiodnp3/src)
include_directories(./cpp/tests/testlib/src)
include_directories(./cpp/tests/dnp3mocks/src)
include_directories(./cpp/tests/catch)
include_directories(./cpp/libs/osslcrypto/src)

# required for ASIO in C++11 only mode
add_definitions(-DASIO_STANDALONE)

# common pattern and exludes for all installed headers
set(INSTALL_ARGS FILES_MATCHING PATTERN "*.h" PATTERN ".deps" EXCLUDE PATTERN ".libs" EXCLUDE)

# ---- openpal library ----
file(GLOB_RECURSE openpal_SRC ./cpp/libs/openpal/src/openpal/*.cpp)
add_library(openpal ${LIB_TYPE} ${openpal_SRC})
install(TARGETS openpal DESTINATION lib)
install(DIRECTORY ./cpp/libs/openpal/src/openpal/ DESTINATION include/openpal ${INSTALL_ARGS})
if(FUZZCOV)
  set_target_properties(openpal PROPERTIES COMPILE_FLAGS ${FUZZ_COMPILE_FLAGS})
endif()

# ---- opendnp3 library ----
file(GLOB_RECURSE opendnp3_SRC ./cpp/libs/opendnp3/src/opendnp3/*.cpp)
add_library(opendnp3 ${LIB_TYPE} ${opendnp3_SRC})
target_link_libraries(opendnp3 openpal)
install(TARGETS opendnp3 DESTINATION lib)
install(DIRECTORY ./cpp/libs/opendnp3/src/opendnp3/ DESTINATION include/opendnp3 ${INSTALL_ARGS})
if(FUZZCOV)
  set_target_properties(openpal PROPERTIES COMPILE_FLAGS ${FUZZ_COMPILE_FLAGS})
endif()

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
target_link_libraries(asiodnp3 secauthv5)
target_link_libraries(asiodnp3 asiopal)
install(TARGETS asiodnp3 DESTINATION lib)
install(DIRECTORY ./cpp/libs/asiodnp3/src/asiodnp3/ DESTINATION include/asiodnp3 ${INSTALL_ARGS})

# ---- secauthv5 library ----
file(GLOB_RECURSE secauthv5_SRC ./cpp/libs/secauthv5/src/secauthv5/*.cpp)
add_library(secauthv5 ${LIB_TYPE} ${secauthv5_SRC})
target_link_libraries(secauthv5 opendnp3)
install(TARGETS secauthv5 DESTINATION lib)
install(DIRECTORY ./cpp/libs/secauthv5/src/secauthv5/ DESTINATION include/secauthv5 ${INSTALL_ARGS})

if(DEMOS)

  # ----- master demo executable -----
  add_executable (masterdemo ./cpp/examples/master/DemoMain.cpp)
  target_link_libraries (masterdemo LINK_PUBLIC asiodnp3 pthread)

  # ----- outstation demo executable -----
  add_executable (outstationdemo ./cpp/examples/outstation/DemoMain.cpp)
  target_link_libraries (outstationdemo LINK_PUBLIC asiodnp3 pthread)

endif()

if(OSSL)

  # ---- osslcrypto library ----
  file(GLOB_RECURSE osslcrypto_SRC ./cpp/libs/osslcrypto/src/osslcrypto/*.cpp)
  add_library(osslcrypto ${LIB_TYPE} ${osslcrypto_SRC})
  target_link_libraries(osslcrypto openpal)
  target_link_libraries(osslcrypto crypto)
  target_link_libraries(osslcrypto pthread)
  install(TARGETS osslcrypto DESTINATION lib)
  install(DIRECTORY ./cpp/libs/osslcrypto/src/osslcrypto/ DESTINATION include/osslcrypto ${INSTALL_ARGS})

  if(DEMOS)

  # ----- outstation SAv5 demo executable -----
  add_executable (outstationdemoSAv5 ./cpp/examples/sav5/outstation/DemoMain.cpp)
  target_link_libraries (outstationdemoSAv5 LINK_PUBLIC asiodnp3 secauthv5 osslcrypto)

  endif()

endif()

if(TESTS)

  # ----- testlib library ------
  file(GLOB_RECURSE testlib_SRC ./cpp/tests/testlib/src/testlib/*.cpp)
  add_library(testlib ${testlib_SRC})
  target_link_libraries(testlib openpal)

  # ----- dnp3mocks library ------
  file(GLOB_RECURSE dnp3mocks_SRC ./cpp/tests/dnp3mocks/src/dnp3mocks/*.cpp)
  add_library(dnp3mocks ${dnp3mocks_SRC})
  target_link_libraries(dnp3mocks opendnp3 testlib)

  # ----- openpal tests -----
  file(GLOB_RECURSE openpal_TESTSRC ./cpp/tests/openpaltests/src/*.cpp)
  add_executable (testopenpal ${openpal_TESTSRC})
  target_link_libraries (testopenpal LINK_PUBLIC testlib pthread)

  # ----- opendnp3 tests -----
  file(GLOB_RECURSE opendnp3_TESTSRC ./cpp/tests/opendnp3tests/src/*.cpp)
  add_executable (testopendnp3 ${opendnp3_TESTSRC})
  target_link_libraries (testopendnp3 LINK_PUBLIC asiodnp3 secauthv5 dnp3mocks pthread)

  # ----- sav5 tests -----
  file(GLOB_RECURSE sav5_TESTSRC ./cpp/tests/sav5tests/src/*.cpp)
  add_executable (testsav5 ${sav5_TESTSRC})
  target_link_libraries (testsav5 LINK_PUBLIC secauthv5 dnp3mocks)

  # ----- opensslcrypto tests -----
  if(OSSL)
    file(GLOB_RECURSE osslcrypto_TESTSRC ./cpp/tests/osslcryptotests/*.cpp)
    add_executable (testosslcrypto ${osslcrypto_TESTSRC})
    target_link_libraries (testosslcrypto LINK_PUBLIC testlib osslcrypto)
  endif()

endif()

if(FUZZCOV)

  # ------ libfuzzer coverage -------
  file(GLOB fuzzer_TESTSRC ../Fuzzer/*.cpp)
  add_library (clangfuzzer ${fuzzer_TESTSRC})		

  # ------ fuzzer executable -------
  file(GLOB bindings_TESTSRC ./cpp/tests/fuzzer/Bindings.cpp)
  add_executable (dnp3fuzzer ${bindings_TESTSRC})
  target_link_libraries (dnp3fuzzer LINK_PUBLIC clangfuzzer opendnp3 openpal)
  set_target_properties(dnp3fuzzer PROPERTIES COMPILE_FLAGS ${FUZZ_COMPILE_FLAGS})
  set_target_properties(dnp3fuzzer PROPERTIES LINK_FLAGS ${FUZZ_LINK_FLAGS})

endif()




