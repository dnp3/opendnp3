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
#ifndef OPENDNP3CLR_SLAVE_COMMAND_HANDLER_ADAPTER_H
#define OPENDNP3CLR_SLAVE_COMMAND_HANDLER_ADAPTER_H

#include <opendnp3/outstation/ICommandHandler.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::ObjectModel;

namespace Automatak
{
namespace DNP3
{
    namespace Adapter
    {

        // this object goes into the stack
        private class OutstationCommandHandlerAdapter : public opendnp3::ICommandHandler
        {
        public:
            OutstationCommandHandlerAdapter(Automatak::DNP3::Interface::ICommandHandler ^ proxy);

            void Begin() override final;
            void End() override final;

            opendnp3::CommandStatus Select(const opendnp3::ControlRelayOutputBlock& command,
                                           uint16_t index) override final;
            opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt32& command, uint16_t index) override final;
            opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt16& command, uint16_t index) override final;
            opendnp3::CommandStatus Select(const opendnp3::AnalogOutputFloat32& command, uint16_t index) override final;
            opendnp3::CommandStatus Select(const opendnp3::AnalogOutputDouble64& command,
                                           uint16_t index) override final;

            opendnp3::CommandStatus Operate(const opendnp3::ControlRelayOutputBlock& command,
                                            uint16_t index,
                                            opendnp3::IUpdateHandler& handler,
                                            opendnp3::OperateType opType) override final;

            opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt32& command,
                                            uint16_t index,
                                            opendnp3::IUpdateHandler& handler,
                                            opendnp3::OperateType opType) override final;

            opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt16& command,
                                            uint16_t index,
                                            opendnp3::IUpdateHandler& handler,
                                            opendnp3::OperateType opType) override final;

            opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputFloat32& command,
                                            uint16_t index,
                                            opendnp3::IUpdateHandler& handler,
                                            opendnp3::OperateType opType) override final;

            opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputDouble64& command,
                                            uint16_t index,
                                            opendnp3::IUpdateHandler& handler,
                                            opendnp3::OperateType opType) override final;

        private:
            gcroot<Automatak::DNP3::Interface::ICommandHandler ^> proxy;
        };

    } // namespace Adapter
} // namespace DNP3
} // namespace Automatak

#endif
