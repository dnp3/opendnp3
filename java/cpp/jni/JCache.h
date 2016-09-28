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

#ifndef OPENDNP3JAVA_OPENDNP3_JNITYPES_H
#define OPENDNP3JAVA_OPENDNP3_JNITYPES_H

#include "openpal/util/Uncopyable.h"

#include "JNIAnalogInput.h"
#include "JNIAnalogOutputDouble64.h"
#include "JNIAnalogOutputFloat32.h"
#include "JNIAnalogOutputInt16.h"
#include "JNIAnalogOutputInt32.h"
#include "JNIAnalogOutputStatus.h"
#include "JNIArrayList.h"
#include "JNIBinaryInput.h"
#include "JNIBinaryOutputStatus.h"
#include "JNIChannelListener.h"
#include "JNIChannelState.h"
#include "JNIClassAssignment.h"
#include "JNIClassField.h"
#include "JNICommandBuilderImpl.h"
#include "JNICommandHeaders.h"
#include "JNICommandPointResult.h"
#include "JNICommandPointState.h"
#include "JNICommandProcessor.h"
#include "JNICommandStatus.h"
#include "JNICommandTaskResult.h"
#include "JNICompletableFuture.h"
#include "JNIControlCode.h"
#include "JNIControlRelayOutputBlock.h"
#include "JNICounter.h"
#include "JNIDoubleBit.h"
#include "JNIDoubleBitBinaryInput.h"
#include "JNIDuration.h"
#include "JNIFrozenCounter.h"
#include "JNIGroupVariation.h"
#include "JNIHeader.h"
#include "JNIHeaderInfo.h"
#include "JNIIINField.h"
#include "JNIIndexedValue.h"
#include "JNIIterable.h"
#include "JNIIterator.h"
#include "JNILinkLayerConfig.h"
#include "JNILogEntry.h"
#include "JNILogHandler.h"
#include "JNIMasterApplication.h"
#include "JNIMasterConfig.h"
#include "JNIMasterStackConfig.h"
#include "JNIMasterTaskType.h"
#include "JNIPointClass.h"
#include "JNIQualifierCode.h"
#include "JNIRange.h"
#include "JNISOEHandler.h"
#include "JNIStack.h"
#include "JNITaskCompletion.h"
#include "JNITaskId.h"
#include "JNITaskInfo.h"
#include "JNITimeSyncMode.h"
#include "JNITimestampMode.h"

namespace jni
{
    struct JCache: private openpal::StaticOnly
    {
        static bool init(JNIEnv* env);
        static void cleanup(JNIEnv* env);

        static jni::AnalogInput AnalogInput;
        static jni::AnalogOutputDouble64 AnalogOutputDouble64;
        static jni::AnalogOutputFloat32 AnalogOutputFloat32;
        static jni::AnalogOutputInt16 AnalogOutputInt16;
        static jni::AnalogOutputInt32 AnalogOutputInt32;
        static jni::AnalogOutputStatus AnalogOutputStatus;
        static jni::ArrayList ArrayList;
        static jni::BinaryInput BinaryInput;
        static jni::BinaryOutputStatus BinaryOutputStatus;
        static jni::ChannelListener ChannelListener;
        static jni::ChannelState ChannelState;
        static jni::ClassAssignment ClassAssignment;
        static jni::ClassField ClassField;
        static jni::CommandBuilderImpl CommandBuilderImpl;
        static jni::CommandHeaders CommandHeaders;
        static jni::CommandPointResult CommandPointResult;
        static jni::CommandPointState CommandPointState;
        static jni::CommandProcessor CommandProcessor;
        static jni::CommandStatus CommandStatus;
        static jni::CommandTaskResult CommandTaskResult;
        static jni::CompletableFuture CompletableFuture;
        static jni::ControlCode ControlCode;
        static jni::ControlRelayOutputBlock ControlRelayOutputBlock;
        static jni::Counter Counter;
        static jni::DoubleBit DoubleBit;
        static jni::DoubleBitBinaryInput DoubleBitBinaryInput;
        static jni::Duration Duration;
        static jni::FrozenCounter FrozenCounter;
        static jni::GroupVariation GroupVariation;
        static jni::Header Header;
        static jni::HeaderInfo HeaderInfo;
        static jni::IINField IINField;
        static jni::IndexedValue IndexedValue;
        static jni::Iterable Iterable;
        static jni::Iterator Iterator;
        static jni::LinkLayerConfig LinkLayerConfig;
        static jni::LogEntry LogEntry;
        static jni::LogHandler LogHandler;
        static jni::MasterApplication MasterApplication;
        static jni::MasterConfig MasterConfig;
        static jni::MasterStackConfig MasterStackConfig;
        static jni::MasterTaskType MasterTaskType;
        static jni::PointClass PointClass;
        static jni::QualifierCode QualifierCode;
        static jni::Range Range;
        static jni::SOEHandler SOEHandler;
        static jni::Stack Stack;
        static jni::TaskCompletion TaskCompletion;
        static jni::TaskId TaskId;
        static jni::TaskInfo TaskInfo;
        static jni::TimeSyncMode TimeSyncMode;
        static jni::TimestampMode TimestampMode;
    };
}

#endif
