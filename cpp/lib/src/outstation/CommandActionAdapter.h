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
#ifndef OPENDNP3_COMMANDACTIONADAPTER_H
#define OPENDNP3_COMMANDACTIONADAPTER_H

#include "ICommandAction.h"

#include "opendnp3/outstation/ICommandHandler.h"

namespace opendnp3
{

/**
 * Interface used to dispatch an abstract action using a command
 */
class CommandActionAdapter final : public ICommandAction
{

public:
    CommandActionAdapter(ICommandHandler& handler, bool is_select, IUpdateHandler& updates, OperateType op_type);

    ~CommandActionAdapter();

    virtual CommandStatus Action(const ControlRelayOutputBlock& command, uint16_t index) override;

    virtual CommandStatus Action(const AnalogOutputInt16& command, uint16_t index) override;

    virtual CommandStatus Action(const AnalogOutputInt32& command, uint16_t index) override;

    virtual CommandStatus Action(const AnalogOutputFloat32& command, uint16_t index) override;

    virtual CommandStatus Action(const AnalogOutputDouble64& command, uint16_t index) override;

private:
    template<class T> CommandStatus ActionT(const T& command, uint16_t index)
    {
        this->CheckStart();
        return is_select ? this->handler.Select(command, index)
                         : this->handler.Operate(command, index, this->updates, this->op_type);
    }

    void CheckStart();

    bool is_started = false;

    ICommandHandler& handler;
    bool is_select;
    IUpdateHandler& updates;
    OperateType op_type;
};

} // namespace opendnp3

#endif
