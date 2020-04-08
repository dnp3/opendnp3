include(FetchContent)

FetchContent_Declare(
    ser4cpp
    URL      https://github.com/automatak/ser4cpp/archive/3c449734dc530a8f465eb0982de29165cc4e23d5.zip
    URL_HASH SHA1=937B759B7CC80180DA26B47037E796B59798A672
)

FetchContent_GetProperties(ser4cpp)
if(NOT ser4cpp_POPULATED)
    FetchContent_Populate(ser4cpp)

    add_library(ser4cpp INTERFACE)
    target_include_directories(ser4cpp INTERFACE ${ser4cpp_SOURCE_DIR}/src)
    target_compile_features(ser4cpp INTERFACE cxx_std_14)
endif()
