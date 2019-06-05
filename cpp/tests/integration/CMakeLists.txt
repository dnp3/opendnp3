set(integrationtests_headers
    ./mocks/CountingSOEHandler.h
    ./mocks/ExpectedValue.h
    ./mocks/NullSOEHandler.h
    ./mocks/PerformanceStackPair.h
    ./mocks/QueuedChannelListener.h
    ./mocks/QueuingSOEHandler.h
    ./mocks/StackPair.h
    ./mocks/SynchronizedQueue.h
)

set(integrationtests_src
    ./main.cpp

    ./TestDeadlock.cpp
    ./TestDNP3Manager.cpp
    ./TestEventIntegration.cpp
    ./TestMasterServerSmoke.cpp
    ./TestPerformance.cpp

    ./mocks/NullSOEHandler.cpp
    ./mocks/PerformanceStackPair.cpp
    ./mocks/StackPair.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${integrationtests_headers} ${integrationtests_src}
)

add_executable(integrationtests
    ${integrationtests_headers} ${integrationtests_src}
)
target_compile_features(integrationtests PRIVATE cxx_std_14)
target_link_libraries(integrationtests PRIVATE catch opendnp3 dnp3mocks)
target_include_directories(integrationtests PRIVATE ./)
set_target_properties(integrationtests PROPERTIES FOLDER cpp/tests)
add_test(NAME integrationtests COMMAND integrationtests)

clang_format(integrationtests)
clang_tidy(integrationtests)
