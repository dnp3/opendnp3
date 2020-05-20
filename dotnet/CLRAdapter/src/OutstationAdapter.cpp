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
#include "OutstationAdapter.h"

#include "UpdateAdapter.h"
#include "Conversions.h"

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            OutstationAdapter::OutstationAdapter(const std::shared_ptr<opendnp3::IOutstation>& outstation) : 
                outstation(new std::shared_ptr<opendnp3::IOutstation>(outstation))
            {}

            OutstationAdapter::!OutstationAdapter()
            {
                delete outstation;
            }

            void OutstationAdapter::SetLogFilters(LogFilter filters)
            {
                (*outstation)->SetLogFilters(opendnp3::LogLevel(filters.Flags));
            }

            void OutstationAdapter::Load(IChangeSet^ changes)
            {
                auto adapter = gcnew UpdateAdapter();
                changes->Apply(adapter);
                adapter->Apply(**outstation);
                delete adapter;
            }

            void OutstationAdapter::SetRestartIIN()
            {
                (*outstation)->SetRestartIIN();
            }

            void OutstationAdapter::Shutdown()
            {
                (*outstation)->Shutdown();
            }

            void OutstationAdapter::Enable()
            {
                (*outstation)->Enable();
            }

            void OutstationAdapter::Disable()
            {
                (*outstation)->Disable();
            }

            IStackStatistics^ OutstationAdapter::GetStackStatistics()
            {
                auto stats = (*outstation)->GetStackStatistics();
                return Conversions::ConvertStackStats(stats);
            }

        }
    }
}
