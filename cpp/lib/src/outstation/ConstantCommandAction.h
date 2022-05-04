/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
#ifndef OPENDNP3_CONSTANTCOMMANDACTION_H
#define OPENDNP3_CONSTANTCOMMANDACTION_H

#include "ICommandAction.h"

namespace opendnp3
{

/**
 * Interface used to dispatch an abstract action using a command
 */
class ConstantCommandAction : public ICommandAction
{

public:
    ConstantCommandAction(CommandStatus status_) : status(status_) {}

    virtual CommandStatus Action(const ControlRelayOutputBlock& command, uint16_t aIndex) final
    {
        return status;
    }

    virtual CommandStatus Action(const AnalogOutputInt16& command, uint16_t aIndex) final
    {
        return status;
    }

    virtual CommandStatus Action(const AnalogOutputInt32& command, uint16_t aIndex) final
    {
        return status;
    }

    virtual CommandStatus Action(const AnalogOutputFloat32& command, uint16_t aIndex) final
    {
        return status;
    }

    virtual CommandStatus Action(const AnalogOutputDouble64& command, uint16_t aIndex) final
    {
        return status;
    }

private:
    CommandStatus status;
};

} // namespace opendnp3

#endif
