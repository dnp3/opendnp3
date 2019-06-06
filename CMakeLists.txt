cmake_minimum_required (VERSION 3.11)

# Project declaration
set(OPENDNP3_MAJOR_VERSION 3)
set(OPENDNP3_MINOR_VERSION 0)
set(OPENDNP3_MICRO_VERSION 0)
set(OPENDNP3_VERSION ${OPENDNP3_MAJOR_VERSION}.${OPENDNP3_MINOR_VERSION}.${OPENDNP3_MICRO_VERSION})
project(opendnp3 VERSION ${OPENDNP3_VERSION})

# Minimum compiler version
if(MSVC_VERSION LESS 1900)
    message(FATAL_ERROR "Visual Studio earlier than 2015 does not implement std::chrono::steady_clock properly. Use a modern compiler.")
endif()

# Clang utilities
include(./cmake/ClangFormat.cmake)
include(./cmake/ClangTidy.cmake)

# Compilation options
option(DNP3_TLS "Build TLS client/server support (requires OpenSSL)" OFF)
option(DNP3_TESTS "Build unit and integration tests" OFF)
option(DNP3_EXAMPLES "Build example applications" OFF)
option(DNP3_FUZZING "Build Google OSS-Fuzz targets" OFF)
option(DNP3_EVERYTHING "Build all optional targets" OFF)

if(DNP3_EVERYTHING)    
	set(DNP3_TLS ON)
	set(DNP3_TESTS ON)
	set(DNP3_EXAMPLES ON)
	set(DNP3_FUZZING ON)
endif()

# External dependencies
include(./deps/asio.cmake)
include(./deps/exe4cpp.cmake)
include(./deps/log4cpp.cmake)
include(./deps/ser4cpp.cmake)

if(DNP3_TLS)
    find_package(OpenSSL REQUIRED)
endif()

if(DNP3_TESTS)
    include(./deps/catch.cmake)
endif()

# Library
add_subdirectory(./cpp/lib)

# Tests
if(DNP3_TESTS)
    enable_testing()

    add_subdirectory(./cpp/tests/dnp3mocks)
    add_subdirectory(./cpp/tests/unit)
    add_subdirectory(./cpp/tests/asiotests)
    add_subdirectory(./cpp/tests/integration)
endif()

# Examples
if(DNP3_EXAMPLES)
    add_subdirectory(./cpp/examples/decoder)
    add_subdirectory(./cpp/examples/master)
    add_subdirectory(./cpp/examples/master-gprs)
    add_subdirectory(./cpp/examples/outstation)

    if(DNP3_TLS)
        add_subdirectory(./cpp/examples/tls/master)
        add_subdirectory(./cpp/examples/tls/master-gprs)
        add_subdirectory(./cpp/examples/tls/outstation)
    endif()
endif()

# Fuzzing
if(DNP3_FUZZING)
    if(NOT DNP3_TESTS)
        # DNP3 mocks is needed for the fuzzing
        add_subdirectory(./cpp/tests/dnp3mocks)
    endif()
    add_subdirectory(./cpp/tests/fuzz)
endif()

# Define utility targets
define_clang_format()
define_clang_tidy()

# Packaging
set(CPACK_PACKAGE_VERSION_MAJOR ${OPENDNP3_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${OPENDNP3_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${OPENDNP3_MICRO_VERSION})
include(CPack)
