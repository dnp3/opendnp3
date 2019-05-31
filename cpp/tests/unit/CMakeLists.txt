set(unittests_mocks
    ./mocks/MeasurementComparisons.h
    ./mocks/MockAPDUHeaderHandler.h
    ./mocks/MockLogHandler.h

    ./utils/BufferHelpers.h
    ./utils/CopyableBuffer.h
)

set(unittests_src
    ./main.cpp

    ./TestAPDUParsing.cpp
#[[    ./TestAPDUWriting.cpp
    ./TestAuthRequestParser.cpp
    ./TestCastLongLongDouble.cpp
    ./TestCollectionTransform.cpp
    ./TestCRC.cpp
    ./TestDatabase.cpp
    ./TestEventStorage.cpp
    ./TestGroup120Var1.cpp
    ./TestGroup120Var2.cpp
    ./TestGroup120Var5.cpp
    ./TestIndexSearch.cpp
    ./TestLinkFrame.cpp
    ./TestLinkLayer.cpp
    ./TestLinkLayerKeepAlive.cpp
    ./TestLinkReceiver.cpp
    ./TestLinkRoute.cpp
    ./TestList.cpp
    ./TestLog.cpp
    ./TestMaster.cpp
    ./TestMasterAssignClass.cpp
    ./TestMasterCommandRequests.cpp
    ./TestMasterMultiCommandRequests.cpp
    ./TestMasterMultidrop.cpp
    ./TestMasterUnsolBehaviors.cpp
    ./TestMeasurementHandler.cpp
    ./TestOutstation.cpp
    ./TestOutstationAssignClass.cpp
    ./TestOutstationCommandResponses.cpp
    ./TestOutstationDiscontiguousIndices.cpp
    ./TestOutstationEventResponses.cpp
    ./TestOutstationStateMachine.cpp
    ./TestOutstationUnsolicitedResponses.cpp
    ./TestShiftableBuffer.cpp
    ./TestTransportLayer.cpp
    ./TestTypedCommandHeader.cpp
    ./TestTypes.cpp
    ./TestUtil.cpp
    ./TestWriteConversions.cpp
]]

    ./mocks/MockLogHandler.cpp

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
