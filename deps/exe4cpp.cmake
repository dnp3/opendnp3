include(FetchContent)

FetchContent_Declare(
    exe4cpp
    GIT_REPOSITORY https://github.com/automatak/exe4cpp.git
    GIT_TAG        asio-1-12-2
    GIT_SHALLOW    ON
)

FetchContent_GetProperties(exe4cpp)
if(NOT exe4cpp_POPULATED)
    message(STATUS "Fetching exe4cpp")
    FetchContent_Populate(exe4cpp)

    add_library(exe4cpp INTERFACE)
    target_include_directories(exe4cpp INTERFACE ${exe4cpp_SOURCE_DIR}/src)
    target_compile_features(exe4cpp INTERFACE cxx_std_14)
endif()
