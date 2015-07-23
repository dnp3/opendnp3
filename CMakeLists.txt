cmake_minimum_required (VERSION 3.0)
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

option(OSSL "build the openssl crypto wrapper for SA" ${DEFAULT_ENABLE})
option(DEMO "Build demo applications" ${DEFAULT_ENABLE})
option(TEST "Build tests" ${DEFAULT_ENABLE})

# other options off-by-default that you can enable
option(LIBCPP "manually link with libc++ (required for clang in some configurations)" OFF)
option(WERROR "Set all warnings to errors" OFF)
option(STATICLIBS "Builds static versions of all installed libraries" OFF)
option(DOTNET "ADDS the .NET bindings to the generated solution file")


if(WIN32)

  set_property(GLOBAL PROPERTY USE_FOLDERS ON) #allows the creation of solution folders

  add_definitions(-D_WIN32_WINNT=0x0501) # for ASIO
  add_definitions(-DASIO_HAS_STD_SYSTEM_ERROR)  

  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /MP")

  set(LIB_TYPE STATIC) #default to static on MSVC

  find_package(OpenSSL REQUIRED)

  include_directories(${OPENSSL_INCLUDE_DIR})

  message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")

  #if (DEFINED ENV{OSSL_HOME})
	#message("OSSL_HOME defined: $ENV{OSSL_HOME}")
    #include_directories($ENV{OSSL_HOME})
    #set (CMAKE_REQUIRED_INCLUDES $ENV{ASIO_HOME})
  #else()
    #message(FATAL_ERROR "You must define OSSL_HOME to point to your openssl include subdirectory")
  #endif() 

else()

  set(PTHREAD pthread)
  
  find_package(OpenSSL REQUIRED)
  message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")

  if(STATICLIBS)
	set(LIB_TYPE STATIC)
  else()
	set(LIB_TYPE SHARED)
  endif()

  set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

  # different release and debug flags
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

  if (WERROR)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif()

endif()



set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})

if (DEFINED ENV{ASIO_HOME})
  message("ASIO_HOME defined: $ENV{ASIO_HOME}")
  include_directories($ENV{ASIO_HOME})
  set (CMAKE_REQUIRED_INCLUDES $ENV{ASIO_HOME})
else()
  message(FATAL_ERROR "You must define ASIO_HOME to point to your ASIO include subdirectory")
endif()

# include paths for all the local libraries
include_directories(./cpp/libs/secauth/src)
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
file(GLOB_RECURSE openpal_SRC ./cpp/libs/openpal/src/openpal/*.cpp ./cpp/libs/openpal/src/openpal/*.h)
add_library(openpal ${LIB_TYPE} ${openpal_SRC})
install(TARGETS openpal DESTINATION lib)
install(DIRECTORY ./cpp/libs/openpal/src/openpal/ DESTINATION include/openpal ${INSTALL_ARGS})
set_target_properties(openpal PROPERTIES FOLDER libs)

# ---- opendnp3 library ----
file(GLOB_RECURSE opendnp3_SRC ./cpp/libs/opendnp3/src/opendnp3/*.cpp ./cpp/libs/opendnp3/src/opendnp3/*.h)
add_library(opendnp3 ${LIB_TYPE} ${opendnp3_SRC})
target_link_libraries(openpal)
install(TARGETS opendnp3 DESTINATION lib)
install(DIRECTORY ./cpp/libs/opendnp3/src/opendnp3/ DESTINATION include/opendnp3 ${INSTALL_ARGS})
set_target_properties(opendnp3 PROPERTIES FOLDER libs)

# ---- asiopal library ----
file(GLOB_RECURSE asiopal_SRC ./cpp/libs/asiopal/src/asiopal/*.cpp ./cpp/libs/asiopal/src/asiopal/*.h)
add_library(asiopal ${LIB_TYPE} ${asiopal_SRC})
target_link_libraries(asiopal openpal)
install(TARGETS asiopal DESTINATION lib)
install(DIRECTORY ./cpp/libs/asiopal/src/asiopal/ DESTINATION include/asiopal ${INSTALL_ARGS})
set_target_properties(asiopal PROPERTIES FOLDER libs)

# ---- asiodnp3 library ----
file(GLOB_RECURSE asiodnp3_SRC ./cpp/libs/asiodnp3/src/asiodnp3/*.cpp ./cpp/libs/asiodnp3/src/asiodnp3/*.h)
add_library(asiodnp3 ${LIB_TYPE} ${asiodnp3_SRC})
target_link_libraries(asiodnp3 opendnp3)
target_link_libraries(asiodnp3 secauth)
target_link_libraries(asiodnp3 asiopal)
install(TARGETS asiodnp3 DESTINATION lib)
install(DIRECTORY ./cpp/libs/asiodnp3/src/asiodnp3/ DESTINATION include/asiodnp3 ${INSTALL_ARGS})
set_target_properties(asiodnp3 PROPERTIES FOLDER libs)

# ---- secauth library ----
file(GLOB_RECURSE secauth_SRC ./cpp/libs/secauth/src/secauth/*.cpp ./cpp/libs/secauth/src/secauth/*.h)
add_library(secauth ${LIB_TYPE} ${secauth_SRC})
target_link_libraries(secauth opendnp3)
install(TARGETS secauth DESTINATION lib)
install(DIRECTORY ./cpp/libs/secauth/src/secauth/ DESTINATION include/secauth ${INSTALL_ARGS})
set_target_properties(secauth PROPERTIES FOLDER libs)

if(DEMO)

  # ----- master demo executable -----
  add_executable (masterdemo ./cpp/examples/master/DemoMain.cpp)
  target_link_libraries (masterdemo LINK_PUBLIC asiodnp3 ${PTHREAD})
  set_target_properties(masterdemo PROPERTIES FOLDER demos)

  # ----- outstation demo executable -----
  add_executable (outstationdemo ./cpp/examples/outstation/DemoMain.cpp)
  target_link_libraries (outstationdemo LINK_PUBLIC asiodnp3 ${PTHREAD})
  set_target_properties(outstationdemo PROPERTIES FOLDER demos)

endif()

if(OSSL)

  # ---- osslcrypto library ----
  file(GLOB_RECURSE osslcrypto_SRC ./cpp/libs/osslcrypto/src/osslcrypto/*.cpp ./cpp/libs/osslcrypto/src/osslcrypto/*.h)
  add_library(osslcrypto ${LIB_TYPE} ${osslcrypto_SRC})
  target_link_libraries(osslcrypto openpal)
  target_link_libraries(osslcrypto ${OPENSSL_LIBRARIES})  
  install(TARGETS osslcrypto DESTINATION lib)
  install(DIRECTORY ./cpp/libs/osslcrypto/src/osslcrypto/ DESTINATION include/osslcrypto ${INSTALL_ARGS})
  set_target_properties(osslcrypto PROPERTIES FOLDER libs)

  if(DEMO)

  # ----- outstation SAv5 demo executable -----
  add_executable (outstationdemoSA ./cpp/examples/secauth/outstation/DemoMain.cpp)
  target_link_libraries (outstationdemoSA LINK_PUBLIC asiodnp3 secauth osslcrypto ${PTHREAD})
  set_target_properties(outstationdemoSA PROPERTIES FOLDER demos/secauth)

  # ----- master SAv5 demo executable -----
  add_executable (masterdemoSA ./cpp/examples/secauth/master/DemoMain.cpp)
  target_link_libraries (masterdemoSA LINK_PUBLIC asiodnp3 secauth osslcrypto ${PTHREAD})
  set_target_properties(masterdemoSA PROPERTIES FOLDER demos/secauth)

  endif()

endif()

if(TEST)

  # ----- testlib library ------
  file(GLOB_RECURSE testlib_SRC ./cpp/tests/testlib/src/testlib/*.cpp ./cpp/tests/testlib/src/testlib/*.h)
  add_library(testlib ${testlib_SRC})
  target_link_libraries(testlib openpal)
  set_target_properties(testlib PROPERTIES FOLDER tests/mocks)

  # ----- dnp3mocks library ------
  file(GLOB_RECURSE dnp3mocks_SRC ./cpp/tests/dnp3mocks/src/dnp3mocks/*.cpp ./cpp/tests/dnp3mocks/src/dnp3mocks/*.h)
  add_library(dnp3mocks ${dnp3mocks_SRC})
  target_link_libraries(dnp3mocks opendnp3 testlib)
  set_target_properties(dnp3mocks PROPERTIES FOLDER tests/mocks)

  # ----- openpal tests -----
  file(GLOB_RECURSE openpal_TESTSRC ./cpp/tests/openpaltests/src/*.cpp ./cpp/tests/openpaltests/src/*.h)
  add_executable (testopenpal ${openpal_TESTSRC})
  target_link_libraries (testopenpal LINK_PUBLIC testlib ${PTHREAD})
  set_target_properties(testopenpal PROPERTIES FOLDER tests)

  # ----- opendnp3 tests -----
  file(GLOB_RECURSE opendnp3_TESTSRC ./cpp/tests/opendnp3tests/src/*.cpp ./cpp/tests/opendnp3tests/src/*.h)
  add_executable (testopendnp3 ${opendnp3_TESTSRC})
  target_link_libraries (testopendnp3 LINK_PUBLIC asiodnp3 secauth dnp3mocks ${PTHREAD})
  set_target_properties(testopendnp3 PROPERTIES FOLDER tests)

  # ----- sa tests -----
  file(GLOB_RECURSE secauth_TESTSRC ./cpp/tests/secauthtests/src/*.cpp ./cpp/tests/secauthtests/src/*.h)
  add_executable (testsecauth ${secauth_TESTSRC})
  target_link_libraries (testsecauth LINK_PUBLIC secauth dnp3mocks)
  set_target_properties(testsecauth PROPERTIES FOLDER tests)  

  #[[
  if(NOT WIN32)
    # ------ afl fuzzer exe -----
    add_executable (aflfuzzer ./cpp/tests/afl-fuzzer/main.cpp)
    target_link_libraries (aflfuzzer LINK_PUBLIC opendnp3)
    set_target_properties(aflfuzzer PROPERTIES FOLDER tests)
  endif()
  #]]

  # ----- opensslcrypto tests -----
  if(OSSL)
    file(GLOB_RECURSE osslcrypto_TESTSRC ./cpp/tests/osslcryptotests/*.cpp ./cpp/tests/osslcryptotests/*.h)
    add_executable (testosslcrypto ${osslcrypto_TESTSRC})
    target_link_libraries (testosslcrypto LINK_PUBLIC testlib osslcrypto ${PTHREAD})
    set_target_properties(testosslcrypto PROPERTIES FOLDER tests)
  endif()

endif()

