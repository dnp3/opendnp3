set(unittests_mocks
    ./utils/BufferHelpers.h
    ./utils/CopyableBuffer.h
)

set(unittests_src
    ./main.cpp

    ./TestCRC.cpp

    ./utils/BufferHelpers.cpp
    ./utils/CopyableBuffer.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${unittests_headers} ${unittests_src}
)

add_executable(unittests
    ${unittests_headers} ${unittests_src}
)
target_compile_features(unittests PRIVATE cxx_std_14)
target_link_libraries(unittests PRIVATE catch opendnp3 log4cpp ser4cpp exe4cpp asio)
target_include_directories(unittests PRIVATE ./ ../../lib/src)
set_target_properties(unittests PROPERTIES FOLDER cpp/tests)
add_test(unittests unittests)

clang_format(unittests)
clang_tidy(unittests)
