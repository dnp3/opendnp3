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

#ifndef OPENDNP3JAVA_OPENDNP3_JNIWRAPPERS_H_H
#define OPENDNP3JAVA_OPENDNP3_JNIWRAPPERS_H_H

#include <jni.h>

namespace jni
{
    struct JAnalogConfig
    {
        JAnalogConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JAnalogInput
    {
        JAnalogInput(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JAnalogOutputDouble64
    {
        JAnalogOutputDouble64(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JAnalogOutputFloat32
    {
        JAnalogOutputFloat32(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JAnalogOutputInt16
    {
        JAnalogOutputInt16(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JAnalogOutputInt32
    {
        JAnalogOutputInt32(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JAnalogOutputStatus
    {
        JAnalogOutputStatus(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JAnalogOutputStatusConfig
    {
        JAnalogOutputStatusConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JApplicationIIN
    {
        JApplicationIIN(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JArrayList
    {
        JArrayList(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JAssignClassType
    {
        JAssignClassType(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JBinaryConfig
    {
        JBinaryConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JBinaryInput
    {
        JBinaryInput(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JBinaryOutputStatus
    {
        JBinaryOutputStatus(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JBinaryOutputStatusConfig
    {
        JBinaryOutputStatusConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JChannelListener
    {
        JChannelListener(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JChannelState
    {
        JChannelState(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JChannelStatistics
    {
        JChannelStatistics(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JClassAssignment
    {
        JClassAssignment(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JClassField
    {
        JClassField(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JCommandHandler
    {
        JCommandHandler(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JCommandHeaders
    {
        JCommandHeaders(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JCommandPointResult
    {
        JCommandPointResult(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JCommandPointState
    {
        JCommandPointState(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JCommandProcessor
    {
        JCommandProcessor(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JCommandStatus
    {
        JCommandStatus(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JCommandTaskResult
    {
        JCommandTaskResult(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JCompletableFuture
    {
        JCompletableFuture(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JControlRelayOutputBlock
    {
        JControlRelayOutputBlock(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JCounter
    {
        JCounter(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JCounterConfig
    {
        JCounterConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JDNP3Exception
    {
        JDNP3Exception(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JDNPTime
    {
        JDNPTime(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JDatabase
    {
        JDatabase(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JDatabaseConfig
    {
        JDatabaseConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JDatabaseImpl
    {
        JDatabaseImpl(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JDoubleBinaryConfig
    {
        JDoubleBinaryConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JDoubleBit
    {
        JDoubleBit(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JDoubleBitBinaryInput
    {
        JDoubleBitBinaryInput(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JDuration
    {
        JDuration(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JEntry
    {
        JEntry(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JEventAnalogOutputStatusVariation
    {
        JEventAnalogOutputStatusVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JEventAnalogVariation
    {
        JEventAnalogVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JEventBinaryOutputStatusVariation
    {
        JEventBinaryOutputStatusVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JEventBinaryVariation
    {
        JEventBinaryVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JEventBufferConfig
    {
        JEventBufferConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JEventConfig
    {
        JEventConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JEventCounterVariation
    {
        JEventCounterVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JEventDoubleBinaryVariation
    {
        JEventDoubleBinaryVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JEventFrozenCounterVariation
    {
        JEventFrozenCounterVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JEventMode
    {
        JEventMode(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JFlags
    {
        JFlags(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JFrozenCounter
    {
        JFrozenCounter(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JFrozenCounterConfig
    {
        JFrozenCounterConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JGroupVariation
    {
        JGroupVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JHeader
    {
        JHeader(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JHeaderInfo
    {
        JHeaderInfo(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JIINField
    {
        JIINField(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JIPEndpoint
    {
        JIPEndpoint(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JIndexQualifierMode
    {
        JIndexQualifierMode(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JIndexedValue
    {
        JIndexedValue(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JInteger
    {
        JInteger(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JIterable
    {
        JIterable(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JIterator
    {
        JIterator(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JLinkLayerConfig
    {
        JLinkLayerConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JLinkLayerStatistics
    {
        JLinkLayerStatistics(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JLinkStatistics
    {
        JLinkStatistics(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JList
    {
        JList(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JLogEntry
    {
        JLogEntry(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JLogHandler
    {
        JLogHandler(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JMap
    {
        JMap(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JMasterApplication
    {
        JMasterApplication(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JMasterConfig
    {
        JMasterConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JMasterStackConfig
    {
        JMasterStackConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JMasterTaskType
    {
        JMasterTaskType(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JNumRetries
    {
        JNumRetries(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JObject
    {
        JObject(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JOperateType
    {
        JOperateType(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JOperationType
    {
        JOperationType(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JOutstationApplication
    {
        JOutstationApplication(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JOutstationConfig
    {
        JOutstationConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JOutstationStackConfig
    {
        JOutstationStackConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JParserStatistics
    {
        JParserStatistics(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JPointClass
    {
        JPointClass(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JQualifierCode
    {
        JQualifierCode(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JRange
    {
        JRange(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JResponseInfo
    {
        JResponseInfo(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JRestartMode
    {
        JRestartMode(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JSOEHandler
    {
        JSOEHandler(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JSet
    {
        JSet(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JStack
    {
        JStack(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JStackStatistics
    {
        JStackStatistics(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JStaticAnalogOutputStatusVariation
    {
        JStaticAnalogOutputStatusVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JStaticAnalogVariation
    {
        JStaticAnalogVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JStaticBinaryOutputStatusVariation
    {
        JStaticBinaryOutputStatusVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JStaticBinaryVariation
    {
        JStaticBinaryVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JStaticCounterVariation
    {
        JStaticCounterVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JStaticDoubleBinaryVariation
    {
        JStaticDoubleBinaryVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JStaticFrozenCounterVariation
    {
        JStaticFrozenCounterVariation(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JStaticTypeBitField
    {
        JStaticTypeBitField(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JTLSConfig
    {
        JTLSConfig(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JTaskCompletion
    {
        JTaskCompletion(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JTaskId
    {
        JTaskId(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JTaskInfo
    {
        JTaskInfo(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JTimeSyncMode
    {
        JTimeSyncMode(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JTimestampQuality
    {
        JTimestampQuality(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JTransportStatistics
    {
        JTransportStatistics(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

    struct JTripCloseCode
    {
        JTripCloseCode(jobject value) : value(value) {}

        operator jobject() const { return value; }

        jobject value;
    };

}

#endif
