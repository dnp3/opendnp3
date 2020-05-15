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
#ifndef OPENDNP3_UNITTESTS_MOCK_COMMAND_HANDLER_H
#define OPENDNP3_UNITTESTS_MOCK_COMMAND_HANDLER_H

#include <opendnp3/app/Indexed.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>

#include <vector>

template<class T> class Operation : public opendnp3::Indexed<T>
{
public:
    Operation(T value, uint16_t index, opendnp3::OperateType opType_)
        : opendnp3::Indexed<T>(value, index), opType(opType_)
    {
    }

    opendnp3::OperateType opType;
};

class MockCommandHandler final : public opendnp3::SimpleCommandHandler
{
public:
    MockCommandHandler(opendnp3::CommandStatus status = opendnp3::CommandStatus::SUCCESS)
        : opendnp3::SimpleCommandHandler(status)
    {
    }

    void SetResponse(opendnp3::CommandStatus status_)
    {
        status = status_;
    }

    uint32_t NumInvocations() const
    {
        return numSelect + numOperate;
    }

    uint32_t NumSelect() const
    {
        return numSelect;
    }

    uint32_t NumOperate() const
    {
        return numOperate;
    }

protected:
    virtual void DoOperate(const opendnp3::ControlRelayOutputBlock& command,
                           uint16_t index,
                           opendnp3::OperateType opType) override
    {
        this->crobOps.push_back(Operation<opendnp3::ControlRelayOutputBlock>(command, index, opType));
    }

    virtual void DoOperate(const opendnp3::AnalogOutputInt16& command,
                           uint16_t index,
                           opendnp3::OperateType opType) override
    {
        this->aoInt16Ops.push_back(Operation<opendnp3::AnalogOutputInt16>(command, index, opType));
    }

    virtual void DoOperate(const opendnp3::AnalogOutputInt32& command,
                           uint16_t index,
                           opendnp3::OperateType opType) override
    {
        this->aoInt32Ops.push_back(Operation<opendnp3::AnalogOutputInt32>(command, index, opType));
    }

    virtual void DoOperate(const opendnp3::AnalogOutputFloat32& command,
                           uint16_t index,
                           opendnp3::OperateType opType) override
    {
        this->aoFloat32Ops.push_back(Operation<opendnp3::AnalogOutputFloat32>(command, index, opType));
    }

    virtual void DoOperate(const opendnp3::AnalogOutputDouble64& command,
                           uint16_t index,
                           opendnp3::OperateType opType) override
    {
        this->aoDouble64Ops.push_back(Operation<opendnp3::AnalogOutputDouble64>(command, index, opType));
    }

public:
    std::vector<Operation<opendnp3::ControlRelayOutputBlock>> crobOps;
    std::vector<Operation<opendnp3::AnalogOutputInt16>> aoInt16Ops;
    std::vector<Operation<opendnp3::AnalogOutputInt32>> aoInt32Ops;
    std::vector<Operation<opendnp3::AnalogOutputFloat32>> aoFloat32Ops;
    std::vector<Operation<opendnp3::AnalogOutputDouble64>> aoDouble64Ops;
};

#endif
