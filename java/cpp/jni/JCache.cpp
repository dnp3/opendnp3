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
    jni::MasterStackConfig JCache::MasterStackConfig;
    jni::MasterConfig JCache::MasterConfig;
    jni::LinkLayerConfig JCache::LinkLayerConfig;
    jni::LogEntry JCache::LogEntry;
    jni::LogHandler JCache::LogHandler;
    jni::ClassField JCache::ClassField;
    jni::SOEHandler JCache::SOEHandler;
    jni::HeaderInfo JCache::HeaderInfo;
    jni::GroupVariation JCache::GroupVariation;
    jni::QualifierCode JCache::QualifierCode;
    jni::TimestampMode JCache::TimestampMode;
    jni::IndexedValue JCache::IndexedValue;
    jni::BinaryInput JCache::BinaryInput;
    jni::DoubleBitBinaryInput JCache::DoubleBitBinaryInput;
    jni::AnalogInput JCache::AnalogInput;
    jni::Counter JCache::Counter;
    jni::FrozenCounter JCache::FrozenCounter;
    jni::BinaryOutputStatus JCache::BinaryOutputStatus;
    jni::AnalogOutputStatus JCache::AnalogOutputStatus;
    jni::Duration JCache::Duration;
    jni::ArrayList JCache::ArrayList;
    jni::DoubleBit JCache::DoubleBit;
    jni::TimeSyncMode JCache::TimeSyncMode;
    jni::MasterApplication JCache::MasterApplication;
    jni::IINField JCache::IINField;
    jni::MasterTaskType JCache::MasterTaskType;
    jni::TaskId JCache::TaskId;
    jni::TaskInfo JCache::TaskInfo;
    jni::TaskCompletion JCache::TaskCompletion;
    jni::Iterable JCache::Iterable;
    jni::Iterator JCache::Iterator;
    jni::ClassAssignment JCache::ClassAssignment;
    jni::Range JCache::Range;
    jni::CommandProcessor JCache::CommandProcessor;
    jni::Stack JCache::Stack;
    jni::CommandBuilderImpl JCache::CommandBuilderImpl;
    jni::CommandHeaders JCache::CommandHeaders;
    jni::CommandTaskResult JCache::CommandTaskResult;
    jni::CommandPointResult JCache::CommandPointResult;
    jni::CommandPointState JCache::CommandPointState;
    jni::CommandStatus JCache::CommandStatus;
    jni::CompletableFuture JCache::CompletableFuture;
    jni::ControlRelayOutputBlock JCache::ControlRelayOutputBlock;
    jni::AnalogOutputInt16 JCache::AnalogOutputInt16;
    jni::AnalogOutputInt32 JCache::AnalogOutputInt32;
    jni::AnalogOutputFloat32 JCache::AnalogOutputFloat32;
    jni::AnalogOutputDouble64 JCache::AnalogOutputDouble64;
    jni::ControlCode JCache::ControlCode;

    bool JCache::init(JNIEnv* env)
    {
        auto success = true;

        success = MasterStackConfig.init(env);
        if(!success) return false;
        success = MasterConfig.init(env);
        if(!success) return false;
        success = LinkLayerConfig.init(env);
        if(!success) return false;
        success = LogEntry.init(env);
        if(!success) return false;
        success = LogHandler.init(env);
        if(!success) return false;
        success = ClassField.init(env);
        if(!success) return false;
        success = SOEHandler.init(env);
        if(!success) return false;
        success = HeaderInfo.init(env);
        if(!success) return false;
        success = GroupVariation.init(env);
        if(!success) return false;
        success = QualifierCode.init(env);
        if(!success) return false;
        success = TimestampMode.init(env);
        if(!success) return false;
        success = IndexedValue.init(env);
        if(!success) return false;
        success = BinaryInput.init(env);
        if(!success) return false;
        success = DoubleBitBinaryInput.init(env);
        if(!success) return false;
        success = AnalogInput.init(env);
        if(!success) return false;
        success = Counter.init(env);
        if(!success) return false;
        success = FrozenCounter.init(env);
        if(!success) return false;
        success = BinaryOutputStatus.init(env);
        if(!success) return false;
        success = AnalogOutputStatus.init(env);
        if(!success) return false;
        success = Duration.init(env);
        if(!success) return false;
        success = ArrayList.init(env);
        if(!success) return false;
        success = DoubleBit.init(env);
        if(!success) return false;
        success = TimeSyncMode.init(env);
        if(!success) return false;
        success = MasterApplication.init(env);
        if(!success) return false;
        success = IINField.init(env);
        if(!success) return false;
        success = MasterTaskType.init(env);
        if(!success) return false;
        success = TaskId.init(env);
        if(!success) return false;
        success = TaskInfo.init(env);
        if(!success) return false;
        success = TaskCompletion.init(env);
        if(!success) return false;
        success = Iterable.init(env);
        if(!success) return false;
        success = Iterator.init(env);
        if(!success) return false;
        success = ClassAssignment.init(env);
        if(!success) return false;
        success = Range.init(env);
        if(!success) return false;
        success = CommandProcessor.init(env);
        if(!success) return false;
        success = Stack.init(env);
        if(!success) return false;
        success = CommandBuilderImpl.init(env);
        if(!success) return false;
        success = CommandHeaders.init(env);
        if(!success) return false;
        success = CommandTaskResult.init(env);
        if(!success) return false;
        success = CommandPointResult.init(env);
        if(!success) return false;
        success = CommandPointState.init(env);
        if(!success) return false;
        success = CommandStatus.init(env);
        if(!success) return false;
        success = CompletableFuture.init(env);
        if(!success) return false;
        success = ControlRelayOutputBlock.init(env);
        if(!success) return false;
        success = AnalogOutputInt16.init(env);
        if(!success) return false;
        success = AnalogOutputInt32.init(env);
        if(!success) return false;
        success = AnalogOutputFloat32.init(env);
        if(!success) return false;
        success = AnalogOutputDouble64.init(env);
        if(!success) return false;
        success = ControlCode.init(env);
        if(!success) return false;
        return true;
    }

    void JCache::cleanup(JNIEnv* env)
    {
        MasterStackConfig.cleanup(env);
        MasterConfig.cleanup(env);
        LinkLayerConfig.cleanup(env);
        LogEntry.cleanup(env);
        LogHandler.cleanup(env);
        ClassField.cleanup(env);
        SOEHandler.cleanup(env);
        HeaderInfo.cleanup(env);
        GroupVariation.cleanup(env);
        QualifierCode.cleanup(env);
        TimestampMode.cleanup(env);
        IndexedValue.cleanup(env);
        BinaryInput.cleanup(env);
        DoubleBitBinaryInput.cleanup(env);
        AnalogInput.cleanup(env);
        Counter.cleanup(env);
        FrozenCounter.cleanup(env);
        BinaryOutputStatus.cleanup(env);
        AnalogOutputStatus.cleanup(env);
        Duration.cleanup(env);
        ArrayList.cleanup(env);
        DoubleBit.cleanup(env);
        TimeSyncMode.cleanup(env);
        MasterApplication.cleanup(env);
        IINField.cleanup(env);
        MasterTaskType.cleanup(env);
        TaskId.cleanup(env);
        TaskInfo.cleanup(env);
        TaskCompletion.cleanup(env);
        Iterable.cleanup(env);
        Iterator.cleanup(env);
        ClassAssignment.cleanup(env);
        Range.cleanup(env);
        CommandProcessor.cleanup(env);
        Stack.cleanup(env);
        CommandBuilderImpl.cleanup(env);
        CommandHeaders.cleanup(env);
        CommandTaskResult.cleanup(env);
        CommandPointResult.cleanup(env);
        CommandPointState.cleanup(env);
        CommandStatus.cleanup(env);
        CompletableFuture.cleanup(env);
        ControlRelayOutputBlock.cleanup(env);
        AnalogOutputInt16.cleanup(env);
        AnalogOutputInt32.cleanup(env);
        AnalogOutputFloat32.cleanup(env);
        AnalogOutputDouble64.cleanup(env);
        ControlCode.cleanup(env);
    }
}
