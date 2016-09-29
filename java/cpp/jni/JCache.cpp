//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2016 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "JCache.h"

namespace jni
{
    cache::AnalogInput JCache::AnalogInput;
    cache::AnalogOutputDouble64 JCache::AnalogOutputDouble64;
    cache::AnalogOutputFloat32 JCache::AnalogOutputFloat32;
    cache::AnalogOutputInt16 JCache::AnalogOutputInt16;
    cache::AnalogOutputInt32 JCache::AnalogOutputInt32;
    cache::AnalogOutputStatus JCache::AnalogOutputStatus;
    cache::ApplicationIIN JCache::ApplicationIIN;
    cache::ArrayList JCache::ArrayList;
    cache::AssignClassType JCache::AssignClassType;
    cache::BinaryInput JCache::BinaryInput;
    cache::BinaryOutputStatus JCache::BinaryOutputStatus;
    cache::ChannelListener JCache::ChannelListener;
    cache::ChannelState JCache::ChannelState;
    cache::ClassAssignment JCache::ClassAssignment;
    cache::ClassField JCache::ClassField;
    cache::CommandHandler JCache::CommandHandler;
    cache::CommandPointResult JCache::CommandPointResult;
    cache::CommandPointState JCache::CommandPointState;
    cache::CommandProcessor JCache::CommandProcessor;
    cache::CommandStatus JCache::CommandStatus;
    cache::CommandTaskResult JCache::CommandTaskResult;
    cache::CompletableFuture JCache::CompletableFuture;
    cache::ControlCode JCache::ControlCode;
    cache::ControlRelayOutputBlock JCache::ControlRelayOutputBlock;
    cache::Counter JCache::Counter;
    cache::DoubleBit JCache::DoubleBit;
    cache::DoubleBitBinaryInput JCache::DoubleBitBinaryInput;
    cache::Duration JCache::Duration;
    cache::EventBufferConfig JCache::EventBufferConfig;
    cache::FrozenCounter JCache::FrozenCounter;
    cache::GroupVariation JCache::GroupVariation;
    cache::Header JCache::Header;
    cache::HeaderInfo JCache::HeaderInfo;
    cache::IINField JCache::IINField;
    cache::IndexMode JCache::IndexMode;
    cache::IndexedValue JCache::IndexedValue;
    cache::Iterable JCache::Iterable;
    cache::Iterator JCache::Iterator;
    cache::LinkLayerConfig JCache::LinkLayerConfig;
    cache::LogEntry JCache::LogEntry;
    cache::LogHandler JCache::LogHandler;
    cache::MasterApplication JCache::MasterApplication;
    cache::MasterConfig JCache::MasterConfig;
    cache::MasterStackConfig JCache::MasterStackConfig;
    cache::MasterTaskType JCache::MasterTaskType;
    cache::OperateType JCache::OperateType;
    cache::OutstationApplication JCache::OutstationApplication;
    cache::OutstationConfig JCache::OutstationConfig;
    cache::OutstationStackConfig JCache::OutstationStackConfig;
    cache::PointClass JCache::PointClass;
    cache::QualifierCode JCache::QualifierCode;
    cache::Range JCache::Range;
    cache::SOEHandler JCache::SOEHandler;
    cache::Stack JCache::Stack;
    cache::TaskCompletion JCache::TaskCompletion;
    cache::TaskId JCache::TaskId;
    cache::TaskInfo JCache::TaskInfo;
    cache::TimeSyncMode JCache::TimeSyncMode;
    cache::TimestampMode JCache::TimestampMode;

    bool JCache::init(JNIEnv* env)
    {
        auto success = true;

        success = AnalogInput.init(env);
        if(!success) return false;
        success = AnalogOutputDouble64.init(env);
        if(!success) return false;
        success = AnalogOutputFloat32.init(env);
        if(!success) return false;
        success = AnalogOutputInt16.init(env);
        if(!success) return false;
        success = AnalogOutputInt32.init(env);
        if(!success) return false;
        success = AnalogOutputStatus.init(env);
        if(!success) return false;
        success = ApplicationIIN.init(env);
        if(!success) return false;
        success = ArrayList.init(env);
        if(!success) return false;
        success = AssignClassType.init(env);
        if(!success) return false;
        success = BinaryInput.init(env);
        if(!success) return false;
        success = BinaryOutputStatus.init(env);
        if(!success) return false;
        success = ChannelListener.init(env);
        if(!success) return false;
        success = ChannelState.init(env);
        if(!success) return false;
        success = ClassAssignment.init(env);
        if(!success) return false;
        success = ClassField.init(env);
        if(!success) return false;
        success = CommandHandler.init(env);
        if(!success) return false;
        success = CommandPointResult.init(env);
        if(!success) return false;
        success = CommandPointState.init(env);
        if(!success) return false;
        success = CommandProcessor.init(env);
        if(!success) return false;
        success = CommandStatus.init(env);
        if(!success) return false;
        success = CommandTaskResult.init(env);
        if(!success) return false;
        success = CompletableFuture.init(env);
        if(!success) return false;
        success = ControlCode.init(env);
        if(!success) return false;
        success = ControlRelayOutputBlock.init(env);
        if(!success) return false;
        success = Counter.init(env);
        if(!success) return false;
        success = DoubleBit.init(env);
        if(!success) return false;
        success = DoubleBitBinaryInput.init(env);
        if(!success) return false;
        success = Duration.init(env);
        if(!success) return false;
        success = EventBufferConfig.init(env);
        if(!success) return false;
        success = FrozenCounter.init(env);
        if(!success) return false;
        success = GroupVariation.init(env);
        if(!success) return false;
        success = Header.init(env);
        if(!success) return false;
        success = HeaderInfo.init(env);
        if(!success) return false;
        success = IINField.init(env);
        if(!success) return false;
        success = IndexMode.init(env);
        if(!success) return false;
        success = IndexedValue.init(env);
        if(!success) return false;
        success = Iterable.init(env);
        if(!success) return false;
        success = Iterator.init(env);
        if(!success) return false;
        success = LinkLayerConfig.init(env);
        if(!success) return false;
        success = LogEntry.init(env);
        if(!success) return false;
        success = LogHandler.init(env);
        if(!success) return false;
        success = MasterApplication.init(env);
        if(!success) return false;
        success = MasterConfig.init(env);
        if(!success) return false;
        success = MasterStackConfig.init(env);
        if(!success) return false;
        success = MasterTaskType.init(env);
        if(!success) return false;
        success = OperateType.init(env);
        if(!success) return false;
        success = OutstationApplication.init(env);
        if(!success) return false;
        success = OutstationConfig.init(env);
        if(!success) return false;
        success = OutstationStackConfig.init(env);
        if(!success) return false;
        success = PointClass.init(env);
        if(!success) return false;
        success = QualifierCode.init(env);
        if(!success) return false;
        success = Range.init(env);
        if(!success) return false;
        success = SOEHandler.init(env);
        if(!success) return false;
        success = Stack.init(env);
        if(!success) return false;
        success = TaskCompletion.init(env);
        if(!success) return false;
        success = TaskId.init(env);
        if(!success) return false;
        success = TaskInfo.init(env);
        if(!success) return false;
        success = TimeSyncMode.init(env);
        if(!success) return false;
        success = TimestampMode.init(env);
        if(!success) return false;
        return true;
    }

    void JCache::cleanup(JNIEnv* env)
    {
        AnalogInput.cleanup(env);
        AnalogOutputDouble64.cleanup(env);
        AnalogOutputFloat32.cleanup(env);
        AnalogOutputInt16.cleanup(env);
        AnalogOutputInt32.cleanup(env);
        AnalogOutputStatus.cleanup(env);
        ApplicationIIN.cleanup(env);
        ArrayList.cleanup(env);
        AssignClassType.cleanup(env);
        BinaryInput.cleanup(env);
        BinaryOutputStatus.cleanup(env);
        ChannelListener.cleanup(env);
        ChannelState.cleanup(env);
        ClassAssignment.cleanup(env);
        ClassField.cleanup(env);
        CommandHandler.cleanup(env);
        CommandPointResult.cleanup(env);
        CommandPointState.cleanup(env);
        CommandProcessor.cleanup(env);
        CommandStatus.cleanup(env);
        CommandTaskResult.cleanup(env);
        CompletableFuture.cleanup(env);
        ControlCode.cleanup(env);
        ControlRelayOutputBlock.cleanup(env);
        Counter.cleanup(env);
        DoubleBit.cleanup(env);
        DoubleBitBinaryInput.cleanup(env);
        Duration.cleanup(env);
        EventBufferConfig.cleanup(env);
        FrozenCounter.cleanup(env);
        GroupVariation.cleanup(env);
        Header.cleanup(env);
        HeaderInfo.cleanup(env);
        IINField.cleanup(env);
        IndexMode.cleanup(env);
        IndexedValue.cleanup(env);
        Iterable.cleanup(env);
        Iterator.cleanup(env);
        LinkLayerConfig.cleanup(env);
        LogEntry.cleanup(env);
        LogHandler.cleanup(env);
        MasterApplication.cleanup(env);
        MasterConfig.cleanup(env);
        MasterStackConfig.cleanup(env);
        MasterTaskType.cleanup(env);
        OperateType.cleanup(env);
        OutstationApplication.cleanup(env);
        OutstationConfig.cleanup(env);
        OutstationStackConfig.cleanup(env);
        PointClass.cleanup(env);
        QualifierCode.cleanup(env);
        Range.cleanup(env);
        SOEHandler.cleanup(env);
        Stack.cleanup(env);
        TaskCompletion.cleanup(env);
        TaskId.cleanup(env);
        TaskInfo.cleanup(env);
        TimeSyncMode.cleanup(env);
        TimestampMode.cleanup(env);
    }
}
