enable_language(CSharp)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/AssemblyInfo.cs "[assembly: System.Reflection.AssemblyVersion(\"${opendnp3_VERSION}\")]")

add_library(DNP3CLRInterface SHARED
    ${CMAKE_CURRENT_BINARY_DIR}/AssemblyInfo.cs

    ./src/ApplicationIIN.cs
    ./src/BaseMeasurementTypes.cs
    ./src/ChangeSet.cs
    ./src/ChannelRetry.cs
    ./src/ChannelStatistics.cs
    ./src/ClassAssignment.cs
    ./src/ClassField.cs
    ./src/CommandResultTypes.cs
    ./src/CommandSet.cs
    ./src/CommandTypes.cs
    ./src/DefaultListenCallbacks.cs
    ./src/DNPTime.cs
    ./src/DNP3Exception.cs
    ./src/Extensions.cs
    ./src/Flags.cs
    ./src/Header.cs
    ./src/IChannel.cs
    ./src/IChannelListener.cs
    ./src/IChannelStatistics.cs
    ./src/ICommandHandler.cs
    ./src/ICommandProcessor.cs
    ./src/IDNP3Manager.cs
    ./src/IDnpTimeSource.cs
    ./src/IDatabase.cs
    ./src/ILinkListener.cs
    ./src/IListenCallbacks.cs
    ./src/IListener.cs
    ./src/ILogHandler.cs
    ./src/IMaster.cs
    ./src/IMasterApplication.cs
    ./src/IMasterOperations.cs
    ./src/IMasterScan.cs
    ./src/IMasterSession.cs
    ./src/IOutstation.cs
    ./src/IOutstationApplication.cs
    ./src/IPEndpoint.cs
    ./src/IQualityBitInfo.cs
    ./src/ISOEHandler.cs
    ./src/ISessionAcceptor.cs
    ./src/IStack.cs
    ./src/IStackStatistics.cs
    ./src/ITaskCallback.cs
    ./src/LinkHeader.cs
    ./src/LogLevels.cs
    ./src/MeasurementTypes.cs
    ./src/NamespaceDoc.cs
    ./src/NumRetries.cs
    ./src/PrintingSOEHandler.cs
    ./src/RestartResultType.cs
    ./src/SimpleCommandHandler.cs
    ./src/StackStatistics.cs
    ./src/TLSConfig.cs
    ./src/TaskCompletionProxy.cs
    ./src/TaskConfig.cs
    ./src/TaskId.cs
    ./src/TaskInfo.cs
    ./src/X509Info.cs

    ./src/config/DatabaseTemplate.cs
    ./src/config/EventBufferConfig.cs
    ./src/config/LinkConfig.cs
    ./src/config/MasterConfig.cs
    ./src/config/MasterStackConfig.cs
    ./src/config/OutstationConfig.cs
    ./src/config/OutstationParams.cs
    ./src/config/OutstationStackConfig.cs
    ./src/config/PointRecords.cs
    ./src/config/SerialSettings.cs
    ./src/config/StaticTypeBitfield.cs
    ./src/gen/AnalogOutputStatusQuality.cs
    ./src/gen/AnalogQuality.cs
    ./src/gen/AssignClassType.cs
    ./src/gen/BinaryOutputStatusQuality.cs
    ./src/gen/BinaryQuality.cs
    ./src/gen/ChannelState.cs
    ./src/gen/CommandPointState.cs
    ./src/gen/CommandStatus.cs
    ./src/gen/CounterQuality.cs
    ./src/gen/DoubleBit.cs
    ./src/gen/DoubleBitBinaryQuality.cs
    ./src/gen/EventAnalogOutputStatusVariation.cs
    ./src/gen/EventAnalogVariation.cs
    ./src/gen/EventBinaryOutputStatusVariation.cs
    ./src/gen/EventBinaryVariation.cs
    ./src/gen/EventCounterVariation.cs
    ./src/gen/EventDoubleBinaryVariation.cs
    ./src/gen/EventFrozenCounterVariation.cs
    ./src/gen/EventMode.cs
    ./src/gen/EventOctetStringVariation.cs
    ./src/gen/EventSecurityStatVariation.cs
    ./src/gen/FlagsType.cs
    ./src/gen/FlowControl.cs
    ./src/gen/FrozenCounterQuality.cs
    ./src/gen/FunctionCode.cs
    ./src/gen/GroupVariation.cs
    ./src/gen/IINField.cs
    ./src/gen/ILinkStatusListener.cs
    ./src/gen/IndexQualifierMode.cs
    ./src/gen/IntervalUnits.cs    
    ./src/gen/LinkFunction.cs
    ./src/gen/LinkStatus.cs
    ./src/gen/MasterTaskType.cs
    ./src/gen/OperateType.cs
    ./src/gen/OperationType.cs
    ./src/gen/Parity.cs
    ./src/gen/PointClass.cs
    ./src/gen/QualifierCode.cs
    ./src/gen/RestartMode.cs
    ./src/gen/RestartType.cs
    ./src/gen/ServerAcceptMode.cs
    ./src/gen/StaticAnalogOutputStatusVariation.cs
    ./src/gen/StaticAnalogVariation.cs
    ./src/gen/StaticBinaryOutputStatusVariation.cs
    ./src/gen/StaticBinaryVariation.cs
    ./src/gen/StaticCounterVariation.cs
    ./src/gen/StaticDoubleBinaryVariation.cs
    ./src/gen/StaticFrozenCounterVariation.cs
    ./src/gen/StaticOctetStringVariation.cs
    ./src/gen/StaticSecurityStatVariation.cs
    ./src/gen/StaticTimeAndIntervalVariation.cs
    ./src/gen/StaticTypeBitmask.cs
    ./src/gen/StopBits.cs
    ./src/gen/TaskCompletion.cs
    ./src/gen/TimeSyncMode.cs
    ./src/gen/TimestampQuality.cs
    ./src/gen/TripCloseCode.cs    
)

set_target_properties(DNP3CLRInterface PROPERTIES FOLDER "dotnet/libs")
set_property(TARGET DNP3CLRInterface PROPERTY VS_DOTNET_REFERENCES "System.Xml")
set_property(TARGET DNP3CLRInterface PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION ${DNP3_DOTNET_FRAMEWORK_VERSION})
target_compile_options(DNP3CLRInterface PRIVATE "/langversion:6")
install(TARGETS DNP3CLRInterface
    RUNTIME DESTINATION lib/${framework_folder_name}
    LIBRARY DESTINATION lib/${framework_folder_name}
)
