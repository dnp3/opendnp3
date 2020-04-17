include(FetchContent)

FetchContent_Declare(
    catch
    URL                 https://github.com/catchorg/Catch2/releases/download/v2.11.3/catch.hpp
    URL_HASH            SHA1=C127EBB7A4F65C6CEFF7587C8EF18F84A74D6C15
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
