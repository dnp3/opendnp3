include(FetchContent)

FetchContent_Declare(
    catch
    URL                 https://github.com/catchorg/Catch2/releases/download/v2.9.2/catch.hpp
    URL_HASH            SHA1=DD87ABE730D130DDF2B0EEF37F9D849DFAD9FCB2
    DOWNLOAD_NO_EXTRACT TRUE
    DOWNLOAD_DIR        ${CMAKE_CURRENT_BINARY_DIR}/catch-src
)

FetchContent_GetProperties(catch)
if(NOT catch_POPULATED)
    FetchContent_Populate(catch)

    find_package(Threads)

    add_library(catch INTERFACE)
    target_include_directories(catch INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/catch-src)
    target_compile_features(catch INTERFACE cxx_std_11)
endif()
