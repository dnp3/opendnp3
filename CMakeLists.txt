cmake_minimum_required (VERSION 3.11)

# Project declaration
set(OPENDNP3_MAJOR_VERSION 3)
set(OPENDNP3_MINOR_VERSION 0)
set(OPENDNP3_MICRO_VERSION 0)
set(OPENDNP3_VERSION ${OPENDNP3_MAJOR_VERSION}.${OPENDNP3_MINOR_VERSION}.${OPENDNP3_MICRO_VERSION})
project(opendnp3 VERSION ${OPENDNP3_VERSION})

# Clang utilities
include(./cmake/ClangFormat.cmake)
include(./cmake/ClangTidy.cmake)

# Compilation options
option(DNP3_TLS "Build TLS client/server support (requires OpenSSL)" OFF)
option(DNP3_EXAMPLES "Build example applications" OFF)

# External dependencies
include(./deps/asio.cmake)
include(./deps/exe4cpp.cmake)
include(./deps/log4cpp.cmake)
include(./deps/ser4cpp.cmake)

if(DNP3_TLS)
    find_package(OpenSSL REQUIRED)
endif()

# Library
add_subdirectory(./cpp/libs/opendnp3)

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

# Define utility targets
define_clang_format()
define_clang_tidy()

# Packaging
set(CPACK_PACKAGE_VERSION_MAJOR ${OPENDNP3_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${OPENDNP3_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${OPENDNP3_MICRO_VERSION})
include(CPack)
