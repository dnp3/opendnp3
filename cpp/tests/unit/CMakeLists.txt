set(unittests_mocks
    ./mocks/MockAPDUHeaderHandler.h
    ./mocks/MockCommandHandler.h
    ./mocks/MockEventWriteHandler.h
    ./mocks/MockFrameSink.h
    ./mocks/MockLinkListener.h
    ./mocks/MockLogHandler.h
    ./mocks/MockLowerLayer.h
    ./mocks/MockMasterApplication.h
    ./mocks/MockOutstationApplication.h
    ./mocks/MockSOEHandler.h
    ./mocks/MockTaskCallback.h
    ./mocks/MockTransportLayer.h
    ./mocks/MockTransportSegment.h

    ./utils/APDUHelpers.h
    ./utils/APDUHexBuilders.h
    ./utils/BufferHelpers.h
    ./utils/CallbackQueue.h
    ./utils/CommandCallbackQueue.h
    ./utils/CopyableBuffer.h
    ./utils/DataSink.h
    ./utils/DNPHelpers.h
    ./utils/LinkHex.h
    ./utils/LinkLayerTest.h
    ./utils/LinkParserTest.h
    ./utils/MasterTestFixture.h
    ./utils/MeasurementComparisons.h
    ./utils/OutstationTestObject.h
)

set(unittests_src
    ./main.cpp

    ./TestAPDUParsing.cpp
    ./TestAPDUWriting.cpp
    ./TestAuthRequestParser.cpp
    ./TestCastLongLongDouble.cpp
    ./TestCollectionTransform.cpp
    ./TestCRC.cpp
#    ./TestDatabase.cpp
    ./TestEventStorage.cpp
    ./TestGroup120Var1.cpp
    ./TestGroup120Var2.cpp
    ./TestGroup120Var5.cpp
    ./TestIndexSearch.cpp
    ./TestLinkAddresses.cpp
    ./TestLinkFrame.cpp
    ./TestLinkLayer.cpp
    ./TestLinkLayerKeepAlive.cpp
    ./TestLinkReceiver.cpp
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
#[[    ./TestShiftableBuffer.cpp
    ./TestTransportLayer.cpp
    ./TestTypedCommandHeader.cpp
    ./TestTypes.cpp
    ./TestUtil.cpp
    ./TestWriteConversions.cpp
]]

    ./mocks/MockFrameSink.cpp
    ./mocks/MockLogHandler.cpp
    ./mocks/MockLowerLayer.cpp
    ./mocks/MockTransportLayer.cpp
    ./mocks/MockTransportSegment.cpp

    ./utils/APDUHelpers.cpp
    ./utils/APDUHexBuilders.cpp
    ./utils/BufferHelpers.cpp
    ./utils/CopyableBuffer.cpp
    ./utils/DataSink.cpp
    ./utils/DNPHelpers.cpp
    ./utils/LinkHex.cpp
    ./utils/LinkLayerTest.cpp
    ./utils/MasterTestFixture.cpp
    ./utils/OutstationTestObject.cpp
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
