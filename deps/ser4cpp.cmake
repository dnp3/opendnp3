include(FetchContent)

FetchContent_Declare(
    ser4cpp
    GIT_REPOSITORY https://github.com/automatak/ser4cpp.git
    GIT_TAG        master
    GIT_SHALLOW    ON
)

FetchContent_GetProperties(ser4cpp)
if(NOT ser4cpp_POPULATED)
    message(STATUS "Fetching ser4cpp")
    FetchContent_Populate(ser4cpp)

    add_library(ser4cpp INTERFACE)
    target_include_directories(ser4cpp INTERFACE ${ser4cpp_SOURCE_DIR}/src)
    target_compile_features(ser4cpp INTERFACE cxx_std_14)
endif()
