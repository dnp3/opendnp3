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
// Copyright 2013-2020 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef OPENDNP3JAVA_OPENDNP3_JNITYPES_H
#define OPENDNP3JAVA_OPENDNP3_JNITYPES_H

#include "opendnp3/util/StaticOnly.h"

#include "JNIAnalogConfig.h"
#include "JNIAnalogInput.h"
#include "JNIAnalogOutputDouble64.h"
#include "JNIAnalogOutputFloat32.h"
#include "JNIAnalogOutputInt16.h"
#include "JNIAnalogOutputInt32.h"
#include "JNIAnalogOutputStatus.h"
#include "JNIAnalogOutputStatusConfig.h"
#include "JNIApplicationIIN.h"
#include "JNIArrayList.h"
#include "JNIAssignClassType.h"
#include "JNIBinaryConfig.h"
#include "JNIBinaryInput.h"
#include "JNIBinaryOutputStatus.h"
#include "JNIBinaryOutputStatusConfig.h"
#include "JNIChannelListener.h"
#include "JNIChannelState.h"
#include "JNIChannelStatistics.h"
#include "JNIClassAssignment.h"
#include "JNIClassField.h"
#include "JNICommandHandler.h"
#include "JNICommandHeaders.h"
#include "JNICommandPointResult.h"
#include "JNICommandPointState.h"
#include "JNICommandProcessor.h"
#include "JNICommandStatus.h"
#include "JNICommandTaskResult.h"
#include "JNICompletableFuture.h"
#include "JNIControlRelayOutputBlock.h"
#include "JNICounter.h"
#include "JNICounterConfig.h"
#include "JNIDNP3Exception.h"
#include "JNIDNPTime.h"
#include "JNIDatabase.h"
#include "JNIDatabaseConfig.h"
#include "JNIDatabaseImpl.h"
#include "JNIDoubleBinaryConfig.h"
#include "JNIDoubleBit.h"
#include "JNIDoubleBitBinaryInput.h"
#include "JNIDuration.h"
#include "JNIEntry.h"
#include "JNIEventAnalogOutputStatusVariation.h"
#include "JNIEventAnalogVariation.h"
#include "JNIEventBinaryOutputStatusVariation.h"
#include "JNIEventBinaryVariation.h"
#include "JNIEventBufferConfig.h"
#include "JNIEventConfig.h"
#include "JNIEventCounterVariation.h"
#include "JNIEventDoubleBinaryVariation.h"
#include "JNIEventFrozenCounterVariation.h"
#include "JNIEventMode.h"
#include "JNIFlags.h"
#include "JNIFrozenCounter.h"
#include "JNIFrozenCounterConfig.h"
#include "JNIGroupVariation.h"
#include "JNIHeader.h"
#include "JNIHeaderInfo.h"
#include "JNIIINField.h"
#include "JNIIPEndpoint.h"
#include "JNIIndexQualifierMode.h"
#include "JNIIndexedValue.h"
#include "JNIInteger.h"
#include "JNIIterable.h"
#include "JNIIterator.h"
#include "JNILinkLayerConfig.h"
#include "JNILinkLayerStatistics.h"
#include "JNILinkStatistics.h"
#include "JNIList.h"
#include "JNILogEntry.h"
#include "JNILogHandler.h"
#include "JNIMap.h"
#include "JNIMasterApplication.h"
#include "JNIMasterConfig.h"
#include "JNIMasterStackConfig.h"
#include "JNIMasterTaskType.h"
#include "JNINumRetries.h"
#include "JNIObject.h"
#include "JNIOperateType.h"
#include "JNIOperationType.h"
#include "JNIOutstationApplication.h"
#include "JNIOutstationConfig.h"
#include "JNIOutstationStackConfig.h"
#include "JNIParserStatistics.h"
#include "JNIPointClass.h"
#include "JNIQualifierCode.h"
#include "JNIRange.h"
#include "JNIResponseInfo.h"
#include "JNIRestartMode.h"
#include "JNISOEHandler.h"
#include "JNISet.h"
#include "JNIStack.h"
#include "JNIStackStatistics.h"
#include "JNIStaticAnalogOutputStatusVariation.h"
#include "JNIStaticAnalogVariation.h"
#include "JNIStaticBinaryOutputStatusVariation.h"
#include "JNIStaticBinaryVariation.h"
#include "JNIStaticCounterVariation.h"
#include "JNIStaticDoubleBinaryVariation.h"
#include "JNIStaticFrozenCounterVariation.h"
#include "JNIStaticTypeBitField.h"
#include "JNITLSConfig.h"
#include "JNITaskCompletion.h"
#include "JNITaskId.h"
#include "JNITaskInfo.h"
#include "JNITimeSyncMode.h"
#include "JNITimestampQuality.h"
#include "JNITransportStatistics.h"
#include "JNITripCloseCode.h"

namespace jni
{
    struct JCache: private opendnp3::StaticOnly
    {
        static bool init(JNIEnv* env);
        static void cleanup(JNIEnv* env);

        static cache::AnalogConfig AnalogConfig;
        static cache::AnalogInput AnalogInput;
        static cache::AnalogOutputDouble64 AnalogOutputDouble64;
        static cache::AnalogOutputFloat32 AnalogOutputFloat32;
        static cache::AnalogOutputInt16 AnalogOutputInt16;
        static cache::AnalogOutputInt32 AnalogOutputInt32;
        static cache::AnalogOutputStatus AnalogOutputStatus;
        static cache::AnalogOutputStatusConfig AnalogOutputStatusConfig;
        static cache::ApplicationIIN ApplicationIIN;
        static cache::ArrayList ArrayList;
        static cache::AssignClassType AssignClassType;
        static cache::BinaryConfig BinaryConfig;
        static cache::BinaryInput BinaryInput;
        static cache::BinaryOutputStatus BinaryOutputStatus;
        static cache::BinaryOutputStatusConfig BinaryOutputStatusConfig;
        static cache::ChannelListener ChannelListener;
        static cache::ChannelState ChannelState;
        static cache::ChannelStatistics ChannelStatistics;
        static cache::ClassAssignment ClassAssignment;
        static cache::ClassField ClassField;
        static cache::CommandHandler CommandHandler;
        static cache::CommandHeaders CommandHeaders;
        static cache::CommandPointResult CommandPointResult;
        static cache::CommandPointState CommandPointState;
        static cache::CommandProcessor CommandProcessor;
        static cache::CommandStatus CommandStatus;
        static cache::CommandTaskResult CommandTaskResult;
        static cache::CompletableFuture CompletableFuture;
        static cache::ControlRelayOutputBlock ControlRelayOutputBlock;
        static cache::Counter Counter;
        static cache::CounterConfig CounterConfig;
        static cache::DNP3Exception DNP3Exception;
        static cache::DNPTime DNPTime;
        static cache::Database Database;
        static cache::DatabaseConfig DatabaseConfig;
        static cache::DatabaseImpl DatabaseImpl;
        static cache::DoubleBinaryConfig DoubleBinaryConfig;
        static cache::DoubleBit DoubleBit;
        static cache::DoubleBitBinaryInput DoubleBitBinaryInput;
        static cache::Duration Duration;
        static cache::Entry Entry;
        static cache::EventAnalogOutputStatusVariation EventAnalogOutputStatusVariation;
        static cache::EventAnalogVariation EventAnalogVariation;
        static cache::EventBinaryOutputStatusVariation EventBinaryOutputStatusVariation;
        static cache::EventBinaryVariation EventBinaryVariation;
        static cache::EventBufferConfig EventBufferConfig;
        static cache::EventConfig EventConfig;
        static cache::EventCounterVariation EventCounterVariation;
        static cache::EventDoubleBinaryVariation EventDoubleBinaryVariation;
        static cache::EventFrozenCounterVariation EventFrozenCounterVariation;
        static cache::EventMode EventMode;
        static cache::Flags Flags;
        static cache::FrozenCounter FrozenCounter;
        static cache::FrozenCounterConfig FrozenCounterConfig;
        static cache::GroupVariation GroupVariation;
        static cache::Header Header;
        static cache::HeaderInfo HeaderInfo;
        static cache::IINField IINField;
        static cache::IPEndpoint IPEndpoint;
        static cache::IndexQualifierMode IndexQualifierMode;
        static cache::IndexedValue IndexedValue;
        static cache::Integer Integer;
        static cache::Iterable Iterable;
        static cache::Iterator Iterator;
        static cache::LinkLayerConfig LinkLayerConfig;
        static cache::LinkLayerStatistics LinkLayerStatistics;
        static cache::LinkStatistics LinkStatistics;
        static cache::List List;
        static cache::LogEntry LogEntry;
        static cache::LogHandler LogHandler;
        static cache::Map Map;
        static cache::MasterApplication MasterApplication;
        static cache::MasterConfig MasterConfig;
        static cache::MasterStackConfig MasterStackConfig;
        static cache::MasterTaskType MasterTaskType;
        static cache::NumRetries NumRetries;
        static cache::Object Object;
        static cache::OperateType OperateType;
        static cache::OperationType OperationType;
        static cache::OutstationApplication OutstationApplication;
        static cache::OutstationConfig OutstationConfig;
        static cache::OutstationStackConfig OutstationStackConfig;
        static cache::ParserStatistics ParserStatistics;
        static cache::PointClass PointClass;
        static cache::QualifierCode QualifierCode;
        static cache::Range Range;
        static cache::ResponseInfo ResponseInfo;
        static cache::RestartMode RestartMode;
        static cache::SOEHandler SOEHandler;
        static cache::Set Set;
        static cache::Stack Stack;
        static cache::StackStatistics StackStatistics;
        static cache::StaticAnalogOutputStatusVariation StaticAnalogOutputStatusVariation;
        static cache::StaticAnalogVariation StaticAnalogVariation;
        static cache::StaticBinaryOutputStatusVariation StaticBinaryOutputStatusVariation;
        static cache::StaticBinaryVariation StaticBinaryVariation;
        static cache::StaticCounterVariation StaticCounterVariation;
        static cache::StaticDoubleBinaryVariation StaticDoubleBinaryVariation;
        static cache::StaticFrozenCounterVariation StaticFrozenCounterVariation;
        static cache::StaticTypeBitField StaticTypeBitField;
        static cache::TLSConfig TLSConfig;
        static cache::TaskCompletion TaskCompletion;
        static cache::TaskId TaskId;
        static cache::TaskInfo TaskInfo;
        static cache::TimeSyncMode TimeSyncMode;
        static cache::TimestampQuality TimestampQuality;
        static cache::TransportStatistics TransportStatistics;
        static cache::TripCloseCode TripCloseCode;
    };
}

#endif
