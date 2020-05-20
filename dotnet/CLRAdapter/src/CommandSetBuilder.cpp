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
#include "CommandSetBuilder.h"

#include "Conversions.h"

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            CommandSetBuilder::CommandSetBuilder(opendnp3::CommandSet& commands) : commands(&commands)
            {}

            void CommandSetBuilder::Add(IEnumerable<IndexedValue<ControlRelayOutputBlock^>^>^ commands)
            {
                auto& header = this->commands->StartHeader<opendnp3::ControlRelayOutputBlock>();
                for each(auto pair in commands)
                {
                    header.Add(Conversions::ConvertCommand(pair->Value), pair->Index);
                }
            }

            void CommandSetBuilder::Add(IEnumerable<IndexedValue<AnalogOutputInt16^>^>^ commands)
            {
                auto& header = this->commands->StartHeader<opendnp3::AnalogOutputInt16>();
                for each(auto pair in commands)
                {
                    header.Add(Conversions::ConvertCommand(pair->Value), pair->Index);
                }
            }

            void CommandSetBuilder::Add(IEnumerable<IndexedValue<AnalogOutputInt32^>^>^ commands)
            {
                auto& header = this->commands->StartHeader<opendnp3::AnalogOutputInt32>();
                for each(auto pair in commands)
                {
                    header.Add(Conversions::ConvertCommand(pair->Value), pair->Index);
                }
            }

            void CommandSetBuilder::Add(IEnumerable<IndexedValue<AnalogOutputFloat32^>^>^ commands)
            {
                auto& header = this->commands->StartHeader<opendnp3::AnalogOutputFloat32>();
                for each(auto pair in commands)
                {
                    header.Add(Conversions::ConvertCommand(pair->Value), pair->Index);
                }
            }

            void CommandSetBuilder::Add(IEnumerable<IndexedValue<AnalogOutputDouble64^>^>^ commands)
            {
                auto& header = this->commands->StartHeader<opendnp3::AnalogOutputDouble64>();
                for each(auto pair in commands)
                {
                    header.Add(Conversions::ConvertCommand(pair->Value), pair->Index);
                }
            }

        }
    }
}
