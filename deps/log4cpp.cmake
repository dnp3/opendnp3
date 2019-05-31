include(FetchContent)

FetchContent_Declare(
    log4cpp
    GIT_REPOSITORY https://github.com/automatak/log4cpp.git
    GIT_TAG        master
    GIT_SHALLOW    ON
)

FetchContent_GetProperties(log4cpp)
if(NOT log4cpp_POPULATED)
    FetchContent_Populate(log4cpp)

    add_library(log4cpp INTERFACE)
    target_include_directories(log4cpp INTERFACE ${log4cpp_SOURCE_DIR}/include)
    target_compile_features(log4cpp INTERFACE cxx_std_11)
endif()
