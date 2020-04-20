set(dnp3mocks_public_headers
    ./include/dnp3mocks/DatabaseHelpers.h
    ./include/dnp3mocks/DataSink.h
    ./include/dnp3mocks/MockAPDUHeaderHandler.h
    ./include/dnp3mocks/MockCommandHandler.h
    ./include/dnp3mocks/MockEventWriteHandler.h
    ./include/dnp3mocks/MockFrameSink.h
    ./include/dnp3mocks/MockLinkLayer.h
    ./include/dnp3mocks/MockLinkListener.h
    ./include/dnp3mocks/MockLogHandler.h
    ./include/dnp3mocks/MockLowerLayer.h
    ./include/dnp3mocks/MockMasterApplication.h
    ./include/dnp3mocks/MockOutstationApplication.h
    ./include/dnp3mocks/MockSOEHandler.h
    ./include/dnp3mocks/MockTaskCallback.h
    ./include/dnp3mocks/MockTransportLayer.h
    ./include/dnp3mocks/MockUpperLayer.h
)

set(dnp3mocks_src
    ./src/DatabaseHelpers.cpp
    ./src/DataSink.cpp
    ./src/MockFrameSink.cpp
    ./src/MockLogHandler.cpp
    ./src/MockLowerLayer.cpp
    ./src/MockTransportLayer.cpp
    ./src/MockUpperLayer.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${dnp3mocks_public_headers} ${dnp3mocks_src}
)

add_library(dnp3mocks
    ${dnp3mocks_public_headers} ${dnp3mocks_src}
)
target_link_libraries(dnp3mocks PUBLIC opendnp3 ser4cpp exe4cpp asio)
target_include_directories(dnp3mocks PUBLIC ./include ../../lib/src)
set_target_properties(dnp3mocks PROPERTIES FOLDER cpp/tests)

clang_format(dnp3mocks)
clang_tidy(dnp3mocks)
