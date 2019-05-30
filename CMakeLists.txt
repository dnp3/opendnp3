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

# External dependencies
add_subdirectory(./deps)
if(DNP3_TLS)
    find_package(OpenSSL REQUIRED)
endif()

# Libraries
add_subdirectory(./cpp/libs/opendnp3)

# Define utility targets
define_clang_format()
define_clang_tidy()

# Packaging
set(CPACK_PACKAGE_VERSION_MAJOR ${OPENDNP3_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${OPENDNP3_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${OPENDNP3_MICRO_VERSION})
include(CPack)
