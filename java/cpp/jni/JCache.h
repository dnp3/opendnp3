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
#include "JNIApplicationIIN.h"
#include "JNIArrayList.h"
#include "JNIAssignClassType.h"
#include "JNIBinaryInput.h"
#include "JNIBinaryOutputStatus.h"
#include "JNIChannelListener.h"
#include "JNIChannelState.h"
#include "JNIClassAssignment.h"
#include "JNIClassField.h"
#include "JNICommandHandler.h"
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
#include "JNIEventBufferConfig.h"
#include "JNIFrozenCounter.h"
#include "JNIGroupVariation.h"
#include "JNIHeader.h"
#include "JNIHeaderInfo.h"
#include "JNIIINField.h"
#include "JNIIndexMode.h"
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
#include "JNIOperateType.h"
#include "JNIOutstationApplication.h"
#include "JNIOutstationConfig.h"
#include "JNIOutstationStackConfig.h"
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

        static cache::AnalogInput AnalogInput;
        static cache::AnalogOutputDouble64 AnalogOutputDouble64;
        static cache::AnalogOutputFloat32 AnalogOutputFloat32;
        static cache::AnalogOutputInt16 AnalogOutputInt16;
        static cache::AnalogOutputInt32 AnalogOutputInt32;
        static cache::AnalogOutputStatus AnalogOutputStatus;
        static cache::ApplicationIIN ApplicationIIN;
        static cache::ArrayList ArrayList;
        static cache::AssignClassType AssignClassType;
        static cache::BinaryInput BinaryInput;
        static cache::BinaryOutputStatus BinaryOutputStatus;
        static cache::ChannelListener ChannelListener;
        static cache::ChannelState ChannelState;
        static cache::ClassAssignment ClassAssignment;
        static cache::ClassField ClassField;
        static cache::CommandHandler CommandHandler;
        static cache::CommandPointResult CommandPointResult;
        static cache::CommandPointState CommandPointState;
        static cache::CommandProcessor CommandProcessor;
        static cache::CommandStatus CommandStatus;
        static cache::CommandTaskResult CommandTaskResult;
        static cache::CompletableFuture CompletableFuture;
        static cache::ControlCode ControlCode;
        static cache::ControlRelayOutputBlock ControlRelayOutputBlock;
        static cache::Counter Counter;
        static cache::DoubleBit DoubleBit;
        static cache::DoubleBitBinaryInput DoubleBitBinaryInput;
        static cache::Duration Duration;
        static cache::EventBufferConfig EventBufferConfig;
        static cache::FrozenCounter FrozenCounter;
        static cache::GroupVariation GroupVariation;
        static cache::Header Header;
        static cache::HeaderInfo HeaderInfo;
        static cache::IINField IINField;
        static cache::IndexMode IndexMode;
        static cache::IndexedValue IndexedValue;
        static cache::Iterable Iterable;
        static cache::Iterator Iterator;
        static cache::LinkLayerConfig LinkLayerConfig;
        static cache::LogEntry LogEntry;
        static cache::LogHandler LogHandler;
        static cache::MasterApplication MasterApplication;
        static cache::MasterConfig MasterConfig;
        static cache::MasterStackConfig MasterStackConfig;
        static cache::MasterTaskType MasterTaskType;
        static cache::OperateType OperateType;
        static cache::OutstationApplication OutstationApplication;
        static cache::OutstationConfig OutstationConfig;
        static cache::OutstationStackConfig OutstationStackConfig;
        static cache::PointClass PointClass;
        static cache::QualifierCode QualifierCode;
        static cache::Range Range;
        static cache::SOEHandler SOEHandler;
        static cache::Stack Stack;
        static cache::TaskCompletion TaskCompletion;
        static cache::TaskId TaskId;
        static cache::TaskInfo TaskInfo;
        static cache::TimeSyncMode TimeSyncMode;
        static cache::TimestampMode TimestampMode;
    };
}

#endif
