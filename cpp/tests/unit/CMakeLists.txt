set(unittests_headers    

    ./utils/APDUHelpers.h
    ./utils/APDUHexBuilders.h
    ./utils/BufferHelpers.h
    ./utils/CallbackQueue.h
    ./utils/CommandCallbackQueue.h
    ./utils/CopyableBuffer.h	
    ./utils/DNPHelpers.h
    ./utils/LinkHex.h
    ./utils/LinkLayerTest.h
    ./utils/LinkParserTest.h
    ./utils/MasterTestFixture.h
    ./utils/MeasurementComparisons.h
    ./utils/MockTransportSegment.h
    ./utils/OutstationTestObject.h
    ./utils/ProtocolUtil.h
    ./utils/TransportTestObject.h
)

set(unittests_src
    ./main.cpp

    ./TestAPDUParsing.cpp
    ./TestAPDUWriting.cpp    
    ./TestCollectionTransform.cpp
    ./TestControlRelayOutputBlock.cpp
    ./TestCRC.cpp
    ./TestEventStorage.cpp
    ./TestFlags.cpp    
    ./TestIPEndpointsList.cpp
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
    ./TestOutstationBroadcast.cpp
    ./TestOutstationAssignClass.cpp
    ./TestOutstationCommandResponses.cpp
    ./TestOutstationDiscontiguousIndices.cpp
    ./TestOutstationEventResponses.cpp
    ./TestOutstationFrozenCounters.cpp
    ./TestOutstationStateMachine.cpp
    ./TestOutstationUnsolicitedResponses.cpp
    ./TestShiftableBuffer.cpp
	./TestStaticDataMap.cpp
    ./TestTimeDuration.cpp
    ./TestTransportLayer.cpp
    ./TestTypedCommandHeader.cpp
    ./TestUpdateBuilder.cpp
    ./TestWriteConversions.cpp

    ./utils/APDUHelpers.cpp
    ./utils/APDUHexBuilders.cpp
    ./utils/BufferHelpers.cpp
    ./utils/CopyableBuffer.cpp	
    ./utils/DNPHelpers.cpp
    ./utils/LinkHex.cpp
    ./utils/LinkLayerTest.cpp
    ./utils/MasterTestFixture.cpp
    ./utils/MockTransportSegment.cpp
    ./utils/OutstationTestObject.cpp
    ./utils/ProtocolUtil.cpp
    ./utils/TransportTestObject.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${unittests_headers} ${unittests_src}
)

add_executable(unittests
    ${unittests_headers} ${unittests_src}
)
target_compile_features(unittests PRIVATE cxx_std_14)
target_link_libraries(unittests PRIVATE catch dnp3mocks)
target_include_directories(unittests PRIVATE ./ ../../lib/src)
set_target_properties(unittests PROPERTIES FOLDER cpp/tests)
add_test(NAME unittests COMMAND unittests)

clang_format(unittests)
clang_tidy(unittests)
