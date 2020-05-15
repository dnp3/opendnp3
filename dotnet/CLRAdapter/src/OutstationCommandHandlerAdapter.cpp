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
#include "OutstationCommandHandlerAdapter.h"


#include "DatabaseAdapter.h"
#include "Conversions.h"

namespace Automatak
{
namespace DNP3
{
    namespace Adapter
    {

        void OutstationCommandHandlerAdapter::Begin()
        {
            proxy->Begin();
        }

        void OutstationCommandHandlerAdapter::End()
        {
            proxy->End();
        }

        OutstationCommandHandlerAdapter::OutstationCommandHandlerAdapter(Automatak::DNP3::Interface::ICommandHandler
                                                                         ^ proxy)
            : proxy(proxy)
        {
        }

        opendnp3::CommandStatus OutstationCommandHandlerAdapter::Select(
            const opendnp3::ControlRelayOutputBlock& command, uint16_t index)
        {
            return Conversions::ConvertCommandStatus(proxy->Select(Conversions::ConvertCommand(command), index));
        }

        opendnp3::CommandStatus OutstationCommandHandlerAdapter::Select(const opendnp3::AnalogOutputInt32& command,
                                                                        uint16_t index)
        {
            return Conversions::ConvertCommandStatus(proxy->Select(Conversions::ConvertCommand(command), index));
        }

        opendnp3::CommandStatus OutstationCommandHandlerAdapter::Select(const opendnp3::AnalogOutputInt16& command,
                                                                        uint16_t index)
        {
            return Conversions::ConvertCommandStatus(proxy->Select(Conversions::ConvertCommand(command), index));
        }

        opendnp3::CommandStatus OutstationCommandHandlerAdapter::Select(const opendnp3::AnalogOutputFloat32& command,
                                                                        uint16_t index)
        {
            return Conversions::ConvertCommandStatus(proxy->Select(Conversions::ConvertCommand(command), index));
        }

        opendnp3::CommandStatus OutstationCommandHandlerAdapter::Select(const opendnp3::AnalogOutputDouble64& command,
                                                                        uint16_t index)
        {
            return Conversions::ConvertCommandStatus(proxy->Select(Conversions::ConvertCommand(command), index));
        }

        opendnp3::CommandStatus OutstationCommandHandlerAdapter::Operate(
            const opendnp3::ControlRelayOutputBlock& command,
            uint16_t index,
            opendnp3::IUpdateHandler& handler,
            opendnp3::OperateType opType)
        {
            const auto adapter = gcnew DatabaseAdapter<opendnp3::IUpdateHandler>(&handler);

            const auto result = proxy->Operate(Conversions::ConvertCommand(command), index, adapter, (OperateType)opType);

            return Conversions::ConvertCommandStatus(result);                
        }

        opendnp3::CommandStatus OutstationCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputInt32& command,
                                                                         uint16_t index,
                                                                         opendnp3::IUpdateHandler& handler,
                                                                         opendnp3::OperateType opType)
        {
            const auto adapter = gcnew DatabaseAdapter<opendnp3::IUpdateHandler>(&handler);

            const auto result = proxy->Operate(Conversions::ConvertCommand(command), index, adapter, (OperateType)opType);

            return Conversions::ConvertCommandStatus(result);
        }

        opendnp3::CommandStatus OutstationCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputInt16& command,
                                                                         uint16_t index,
                                                                         opendnp3::IUpdateHandler& handler,
                                                                         opendnp3::OperateType opType)
        {
            const auto adapter = gcnew DatabaseAdapter<opendnp3::IUpdateHandler>(&handler);

            const auto result = proxy->Operate(Conversions::ConvertCommand(command), index, adapter, (OperateType)opType);

            return Conversions::ConvertCommandStatus(result);
        }

        opendnp3::CommandStatus OutstationCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputFloat32& command,
                                                                         uint16_t index,
                                                                         opendnp3::IUpdateHandler& handler,
                                                                         opendnp3::OperateType opType)
        {            
            const auto adapter = gcnew DatabaseAdapter<opendnp3::IUpdateHandler>(&handler);

            const auto result = proxy->Operate(Conversions::ConvertCommand(command), index, adapter, (OperateType)opType);

            return Conversions::ConvertCommandStatus(result);
        }

        opendnp3::CommandStatus OutstationCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputDouble64& command,
                                                                         uint16_t index,
                                                                         opendnp3::IUpdateHandler& handler,
                                                                         opendnp3::OperateType opType)
        {
            const auto adapter = gcnew DatabaseAdapter<opendnp3::IUpdateHandler>(&handler);

            const auto result = proxy->Operate(Conversions::ConvertCommand(command), index, adapter, (OperateType)opType);

            return Conversions::ConvertCommandStatus(result);
        }

    } // namespace Adapter
} // namespace DNP3
} // namespace Automatak
