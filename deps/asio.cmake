include(FetchContent)

FetchContent_Declare(
    asio
    URL                 https://github.com/chriskohlhoff/asio/archive/asio-1-12-2.zip
    URL_HASH            SHA1=23F74B271D984A7B2BF00640DE856F5BBA410A50
)

FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
    FetchContent_Populate(asio)

    find_package(Threads)

    add_library(asio INTERFACE)
    target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
    target_compile_definitions(asio INTERFACE ASIO_STANDALONE)
    target_compile_features(asio INTERFACE cxx_std_11)
    target_link_libraries(asio INTERFACE Threads::Threads)

    if(WIN32)
        target_link_libraries(asio INTERFACE ws2_32 wsock32) # Link to Winsock
        target_compile_definitions(asio INTERFACE _WIN32_WINNT=0x0601) # Windows 7 and up
    endif()
endif()
