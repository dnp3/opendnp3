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
#ifndef OPENDNP3_COMMANDHANDLERADAPTER_H
#define OPENDNP3_COMMANDHANDLERADAPTER_H

#include "GlobalRef.h"
#include "LocalRef.h"
#include "../jni/JNIWrappers.h"

#include <opendnp3/outstation/ICommandHandler.h>

class CommandHandlerAdapter final : public opendnp3::ICommandHandler
{
public:
    CommandHandlerAdapter(jni::JCommandHandler proxy) : proxy(proxy) {}

    virtual void Begin() override;
    virtual void End() override;

    opendnp3::CommandStatus Select(const opendnp3::ControlRelayOutputBlock& command, uint16_t index) override;

    opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt16& command, uint16_t index) override;

    opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt32& command, uint16_t index) override;

    opendnp3::CommandStatus Select(const opendnp3::AnalogOutputFloat32& command, uint16_t index) override;

    opendnp3::CommandStatus Select(const opendnp3::AnalogOutputDouble64& command, uint16_t index) override;

    opendnp3::CommandStatus Operate(const opendnp3::ControlRelayOutputBlock& command, 
                                    uint16_t index,
                                    opendnp3::IUpdateHandler& database,
                                    opendnp3::OperateType opType) override;

    opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt16& command,
                                    uint16_t index,
                                    opendnp3::IUpdateHandler& database,
                                    opendnp3::OperateType opType) override;

    opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt32& command,
                                    uint16_t index,
                                    opendnp3::IUpdateHandler& database,
                                    opendnp3::OperateType opType) override;

    opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputFloat32& command,
                                    uint16_t index,
                                    opendnp3::IUpdateHandler& database,
                                    opendnp3::OperateType opType) override;

    opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputDouble64& command,
                                    uint16_t index,
                                    opendnp3::IUpdateHandler& database,
                                    opendnp3::OperateType opType) override;

private:

    template<class T>
    opendnp3::CommandStatus SelectAny(const T& command, uint16_t index);

    template<class T>
    opendnp3::CommandStatus OperateAny(const T& command,
                             uint16_t index,
                             opendnp3::IUpdateHandler& database,
                             opendnp3::OperateType opType);

    
    static LocalRef<jni::JControlRelayOutputBlock> Convert(JNIEnv * env, const opendnp3::ControlRelayOutputBlock& command);
    static LocalRef<jni::JAnalogOutputInt16> Convert(JNIEnv * env, const opendnp3::AnalogOutputInt16& command);
    static LocalRef<jni::JAnalogOutputInt32> Convert(JNIEnv * env, const opendnp3::AnalogOutputInt32& command);
    static LocalRef<jni::JAnalogOutputFloat32> Convert(JNIEnv * env, const opendnp3::AnalogOutputFloat32& command);
    static LocalRef<jni::JAnalogOutputDouble64> Convert(JNIEnv * env, const opendnp3::AnalogOutputDouble64& command);

    GlobalRef<jni::JCommandHandler> proxy;
};

#endif
