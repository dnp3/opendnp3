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
#include "CommandActionAdapter.h"

namespace opendnp3
{

CommandActionAdapter::CommandActionAdapter(ICommandHandler& handler,
                                           bool is_select,
                                           IUpdateHandler& updates,
                                           OperateType op_type)
    : handler(handler), is_select(is_select), updates(updates), op_type(op_type)
{
}

CommandActionAdapter::~CommandActionAdapter()
{
    if (this->is_started)
    {
        handler.End();
    }
}

void CommandActionAdapter::CheckStart()
{
    if (!this->is_started)
    {
        this->is_started = true;
        handler.Begin();
    }
}

CommandStatus CommandActionAdapter::Action(const ControlRelayOutputBlock& command, uint16_t index)
{
    if (command.IsQUFlagSet())
    {
        return CommandStatus::NOT_SUPPORTED;
    }

    return this->ActionT(command, index);
}

CommandStatus CommandActionAdapter::Action(const AnalogOutputInt16& command, uint16_t index)
{
    return this->ActionT(command, index);
}

CommandStatus CommandActionAdapter::Action(const AnalogOutputInt32& command, uint16_t index)
{
    return this->ActionT(command, index);
}

CommandStatus CommandActionAdapter::Action(const AnalogOutputFloat32& command, uint16_t index)
{
    return this->ActionT(command, index);
}

CommandStatus CommandActionAdapter::Action(const AnalogOutputDouble64& command, uint16_t index)
{
    return this->ActionT(command, index);
}

} // namespace opendnp3
