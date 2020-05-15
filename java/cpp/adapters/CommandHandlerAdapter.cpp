/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "CommandHandlerAdapter.h"

#include "../jni/JCache.h"

using namespace opendnp3;
using namespace jni;

void CommandHandlerAdapter::Begin()
{
    const auto env = JNI::GetEnv();
    JCache::CommandHandler.begin(env, proxy);
}

void CommandHandlerAdapter::End()
{
    const auto env = JNI::GetEnv();
    JCache::CommandHandler.end(env, proxy);
}

CommandStatus CommandHandlerAdapter::Select(const ControlRelayOutputBlock& command, uint16_t index)
{
    return this->SelectAny(command, index);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputInt16& command, uint16_t index)
{
    return this->SelectAny(command, index);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputInt32& command, uint16_t index)
{
    return this->SelectAny(command, index);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputFloat32& command, uint16_t index)
{
    return this->SelectAny(command, index);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputDouble64& command, uint16_t index)
{
    return this->SelectAny(command, index);
}

CommandStatus CommandHandlerAdapter::Operate(const ControlRelayOutputBlock& command,
                                             uint16_t index,
                                             opendnp3::IUpdateHandler& database,
                                             OperateType opType)
{
    return OperateAny(command, index, database, opType);
}

CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputInt16& command,
                                             uint16_t index,
                                             opendnp3::IUpdateHandler& database,
                                             OperateType opType)
{
    return OperateAny(command, index, database, opType);
}

CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputInt32& command,
                                             uint16_t index,
                                             opendnp3::IUpdateHandler& database,
                                             OperateType opType)
{
    return OperateAny(command, index, database, opType);
}

CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputFloat32& command,
                                             uint16_t index,
                                             opendnp3::IUpdateHandler& database,
                                             OperateType opType)
{
    return OperateAny(command, index, database, opType);
}

CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputDouble64& command,
                                             uint16_t index,
                                             opendnp3::IUpdateHandler& database,
                                             OperateType opType)
{
    return OperateAny(command, index, database, opType);
}

template<class T> CommandStatus CommandHandlerAdapter::SelectAny(const T& command, uint16_t index)
{
    const auto env = JNI::GetEnv();
    auto jcommand = Convert(env, command);
    auto jstatus = JCache::CommandHandler.select(env, proxy, jcommand, index);
    return CommandStatusSpec::from_type(static_cast<uint8_t>(JCache::CommandStatus.toType(env, jstatus)));
}

template<class T>
CommandStatus CommandHandlerAdapter::OperateAny(const T& command,
                                                uint16_t index,
                                                IUpdateHandler& database,
                                                OperateType opType)
{
    const auto env = JNI::GetEnv();
    auto jcommand = Convert(env, command);
    auto jopType = JCache::OperateType.fromType(env, static_cast<jint>(opType));
    auto jdatabase = JCache::DatabaseImpl.construct(env, (jlong)&database);
    auto jstatus = JCache::CommandHandler.operate(env, proxy.get(), jcommand, static_cast<jint>(index),
                                                  jdatabase.as<jni::JDatabase>(), jopType);
    return CommandStatusSpec::from_type(static_cast<uint8_t>(JCache::CommandStatus.toType(env, jstatus)));
}


LocalRef<jni::JControlRelayOutputBlock> CommandHandlerAdapter::Convert(JNIEnv* env,
                                                                       const ControlRelayOutputBlock& command)
{
    auto joperationtype = JCache::OperationType.fromType(env, static_cast<jint>(command.opType));
    auto jtcc = JCache::TripCloseCode.fromType(env, static_cast<jint>(command.tcc));
    auto jcommandstatus = JCache::CommandStatus.fromType(env, CommandStatusSpec::to_type(command.status));
    return JCache::ControlRelayOutputBlock.construct(env, joperationtype, jtcc, command.clear, command.count,
                                                     command.onTimeMS, command.offTimeMS, jcommandstatus);
}

LocalRef<jni::JAnalogOutputInt16> CommandHandlerAdapter::Convert(JNIEnv* env, const AnalogOutputInt16& command)
{
    auto jcommandstatus = JCache::CommandStatus.fromType(env, CommandStatusSpec::to_type(command.status));
    return JCache::AnalogOutputInt16.construct(env, command.value, jcommandstatus);
}

LocalRef<jni::JAnalogOutputInt32> CommandHandlerAdapter::Convert(JNIEnv* env, const AnalogOutputInt32& command)
{
    auto jcommandstatus = JCache::CommandStatus.fromType(env, CommandStatusSpec::to_type(command.status));
    return JCache::AnalogOutputInt32.construct(env, command.value, jcommandstatus);
}

LocalRef<jni::JAnalogOutputFloat32> CommandHandlerAdapter::Convert(JNIEnv* env, const AnalogOutputFloat32& command)
{
    auto jcommandstatus = JCache::CommandStatus.fromType(env, CommandStatusSpec::to_type(command.status));
    return JCache::AnalogOutputFloat32.construct(env, command.value, jcommandstatus);
}

LocalRef<jni::JAnalogOutputDouble64> CommandHandlerAdapter::Convert(JNIEnv* env, const AnalogOutputDouble64& command)
{
    auto jcommandstatus = JCache::CommandStatus.fromType(env, CommandStatusSpec::to_type(command.status));
    return JCache::AnalogOutputDouble64.construct(env, command.value, jcommandstatus);
}
