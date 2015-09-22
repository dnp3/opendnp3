cmake_minimum_required (VERSION 2.8)
project (opendnp3)
set(OPENDNP3_MAJOR_VERSION 2)
set(OPENDNP3_MINOR_VERSION 1)
set(OPENDNP3_MICRO_VERSION 0)
set(OPENDNP3_VERSION ${OPENDNP3_MAJOR_VERSION}.${OPENDNP3_MINOR_VERSION}.${OPENDNP3_MICRO_VERSION})

# various optional libraries and projects
option(DEMO "Build demo applications" OFF)
option(TEST "Build tests" OFF)
option(FULL "Build all optional projects (secauth, demos, tests)" OFF)
option(SECAUTH "build the secure authentication module and openssl crypto wrapper" OFF)

if(FULL)
	set(SECAUTH ON)
	set(DEMO ON)
	set(TEST ON)
endif()

if(SECAUTH)
	 add_definitions(-DOPENDNP3_USE_SECAUTH)
endif()
 
# other options off-by-default that you can enable
option(LIBCPP "manually link with libc++ (required for clang in some configurations)" OFF)
option(WERROR "Set all warnings to errors" OFF)
option(STATICLIBS "Builds static versions of all installed libraries" OFF)
option(COVERAGE "Builds the libraries with coverage info for gcov" OFF)

if(SECAUTH)
   find_package(OpenSSL REQUIRED)
   message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")
endif()

if(WIN32)

  set_property(GLOBAL PROPERTY USE_FOLDERS ON) #allows the creation of solution folders

   # for ASIO
  add_definitions(-D_WIN32_WINNT=0x0501)
  add_definitions(-DASIO_HAS_STD_SYSTEM_ERROR)  

  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /MP")

  set(LIB_TYPE STATIC) #default to static on MSVC

  if(SECAUTH)
    include_directories(${OPENSSL_INCLUDE_DIR})
  endif() 

else()

  set(PTHREAD pthread)   

  if(STATICLIBS)
	set(LIB_TYPE STATIC)
  else()
	set(LIB_TYPE SHARED)
  endif()

  set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

  # different release and debug flags
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
  
  if(COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -g -O0")
  endif()

  if (WERROR)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif()

endif()

# There's problem with detecting usage of pthread with gcc 4.8.x
# http://ubuntuforums.org/showthread.php?t=2183408
if(CMAKE_COMPILER_IS_GNUCXX)      
  if(CMAKE_CXX_COMPILER_VERSION MATCHES 4.8.*)
    message("Adding pthread workaround for GCC version: ${CMAKE_CXX_COMPILER_VERSION}")
    SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed" )            
  endif()
endif()

set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})

if (DEFINED ENV{ASIO_HOME})
  message("ASIO_HOME defined in environment: $ENV{ASIO_HOME}")
  message("Adding ASIO_HOME To your include path")
  include_directories($ENV{ASIO_HOME})  
else()
  if(ASIO_HOME)
    message("ASIO_HOME defined in cache: ${ASIO_HOME}")
    include_directories(${ASIO_HOME})    
  else()
    message("ASIO_HOME not found. ASIO expected to be on include path")  
  endif()
endif()



# include paths for all the local libraries
include_directories(./cpp/libs/src)
include_directories(./cpp/libs/include)
include_directories(./cpp/tests/libs/src)

# required for ASIO in C++11 only mode
add_definitions(-DASIO_STANDALONE)

# ---- openpal library ----
file(GLOB_RECURSE openpal_SRC ./cpp/libs/src/openpal/*.cpp ./cpp/libs/src/openpal/*.h ./cpp/libs/include/openpal/*.h)
add_library(openpal ${LIB_TYPE} ${openpal_SRC})
install(TARGETS openpal DESTINATION lib)
set_target_properties(openpal PROPERTIES FOLDER libs)

# ---- opendnp3 library ----
file(GLOB_RECURSE opendnp3_SRC ./cpp/libs/src/opendnp3/*.cpp ./cpp/libs/src/opendnp3/*.h ./cpp/libs/include/opendnp3/*.h)
add_library(opendnp3 ${LIB_TYPE} ${opendnp3_SRC})
target_link_libraries(openpal)
install(TARGETS opendnp3 DESTINATION lib)
set_target_properties(opendnp3 PROPERTIES FOLDER libs)

# ---- asiopal library ----
file(GLOB_RECURSE asiopal_SRC ./cpp/libs/src/asiopal/*.cpp ./cpp/libs/src/asiopal/*.h ./cpp/libs/include/asiopal/*.h)
add_library(asiopal ${LIB_TYPE} ${asiopal_SRC})
target_link_libraries(asiopal openpal)
install(TARGETS asiopal DESTINATION lib)
set_target_properties(asiopal PROPERTIES FOLDER libs)

# ---- asiotls library ----
file(GLOB_RECURSE asiotls_SRC ./cpp/libs/src/asiotls/*.cpp ./cpp/libs/src/asiotls/*.h ./cpp/libs/include/asiotls/*.h)
add_library(asiotls ${LIB_TYPE} ${asiotls_SRC})
target_link_libraries(asiotls asiopal)
install(TARGETS asiotls DESTINATION lib)
set_target_properties(asiotls PROPERTIES FOLDER libs)

# ---- asiodnp3 library ----
if(SECAUTH)
	file(GLOB_RECURSE asiodnp3_SRC ./cpp/libs/src/asiodnp3/*.cpp ./cpp/libs/src/asiodnp3/*.h ./cpp/libs/include/asiodnp3/*.h)
else()
	file(GLOB asiodnp3_SRC ./cpp/libs/src/asiodnp3/*.cpp ./cpp/libs/src/asiodnp3/*.h ./cpp/libs/include/asiopal/*.h)
endif()

add_library(asiodnp3 ${LIB_TYPE} ${asiodnp3_SRC})

if(SECAUTH)
  target_link_libraries(asiodnp3 secauth opendnp3 asiotls ${OPENSSL_LIBRARIES})
else()
  target_link_libraries(asiodnp3 opendnp3 asiotls ${OPENSSL_LIBRARIES})  
endif()

install(TARGETS asiodnp3 DESTINATION lib)
set_target_properties(asiodnp3 PROPERTIES FOLDER libs)

if(SECAUTH)
  
  # ---- secauth library ----
  file(GLOB_RECURSE secauth_SRC ./cpp/libs/src/secauth/*.cpp ./cpp/libs/src/secauth/*.h ./cpp/libs/include/secauth/*.h)
  add_library(secauth ${LIB_TYPE} ${secauth_SRC})
  target_link_libraries(secauth opendnp3)
  install(TARGETS secauth DESTINATION lib)  
  set_target_properties(secauth PROPERTIES FOLDER libs)

  # ---- osslcrypto library ----
  file(GLOB_RECURSE osslcrypto_SRC ./cpp/libs/src/osslcrypto/*.cpp ./cpp/libs/src/osslcrypto/*.h ./cpp/libs/include/osslcrypto/*.h)
  add_library(osslcrypto ${LIB_TYPE} ${osslcrypto_SRC})
  target_link_libraries(osslcrypto openpal)
  target_link_libraries(osslcrypto ${OPENSSL_LIBRARIES})  
  install(TARGETS osslcrypto DESTINATION lib)  
  set_target_properties(osslcrypto PROPERTIES FOLDER libs)  

endif()


# ----- install -----

# common pattern and exludes for all installed headers
set(INSTALL_ARGS FILES_MATCHING PATTERN "*.h" PATTERN ".deps" EXCLUDE PATTERN ".libs" EXCLUDE)
install(DIRECTORY ./cpp/libs/include/ DESTINATION include ${INSTALL_ARGS})



if(DEMO)

  # ----- master demo executable -----
  add_executable (masterdemo ./cpp/examples/master/DemoMain.cpp)
  target_link_libraries (masterdemo LINK_PUBLIC asiodnp3 ${PTHREAD})
  set_target_properties(masterdemo PROPERTIES FOLDER demos)

  # ----- outstation demo executable -----
  add_executable (outstationdemo ./cpp/examples/outstation/DemoMain.cpp)
  target_link_libraries (outstationdemo LINK_PUBLIC asiodnp3 ${PTHREAD})
  set_target_properties(outstationdemo PROPERTIES FOLDER demos)

  # ----- master tls executable -----  
  add_executable (master-tls-demo ./cpp/examples/tls/master/main.cpp)
  target_link_libraries (master-tls-demo LINK_PUBLIC asiodnp3 ${PTHREAD})
  set_target_properties(master-tls-demo PROPERTIES FOLDER demos/tls)

  if(SECAUTH)

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
  file(GLOB_RECURSE testlib_SRC ./cpp/tests/libs/src/testlib/*.cpp ./cpp/tests/libs/src/testlib/*.h)
  add_library(testlib ${testlib_SRC})
  target_link_libraries(testlib openpal)
  set_target_properties(testlib PROPERTIES FOLDER tests/mocks)

  # ----- dnp3mocks library ------
  file(GLOB_RECURSE dnp3mocks_SRC ./cpp/tests/libs/src/dnp3mocks/*.cpp ./cpp/tests/libs/src/dnp3mocks/*.h)
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
  target_link_libraries (testopendnp3 LINK_PUBLIC dnp3mocks ${PTHREAD})  
  set_target_properties(testopendnp3 PROPERTIES FOLDER tests)

  # ----- asiodnp3 tests -----
  file(GLOB_RECURSE asiodnp3_TESTSRC ./cpp/tests/asiodnp3tests/src/*.cpp ./cpp/tests/asiodnp3tests/src/*.h)
  add_executable (testasiodnp3 ${asiodnp3_TESTSRC})
  if(SECAUTH)
   target_link_libraries (testasiodnp3 LINK_PUBLIC asiodnp3 secauth dnp3mocks ${PTHREAD})
  else()
   target_link_libraries (testasiodnp3 LINK_PUBLIC asiodnp3 dnp3mocks ${PTHREAD})
  endif()
  set_target_properties(testasiodnp3 PROPERTIES FOLDER tests)

  if(SECAUTH)
  
    # ----- sa tests -----
    file(GLOB_RECURSE secauth_TESTSRC ./cpp/tests/secauthtests/src/*.cpp ./cpp/tests/secauthtests/src/*.h)
    add_executable (testsecauth ${secauth_TESTSRC})
    target_link_libraries (testsecauth LINK_PUBLIC secauth dnp3mocks)
    set_target_properties(testsecauth PROPERTIES FOLDER tests)   

    # ----- opensslcrypto tests -----
    file(GLOB_RECURSE osslcrypto_TESTSRC ./cpp/tests/osslcryptotests/*.cpp ./cpp/tests/osslcryptotests/*.h)
    add_executable (testosslcrypto ${osslcrypto_TESTSRC})
    target_link_libraries (testosslcrypto LINK_PUBLIC testlib osslcrypto ${PTHREAD})
    set_target_properties(testosslcrypto PROPERTIES FOLDER tests)          
  
  endif()

endif()

