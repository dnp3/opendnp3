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
#include "ChannelAdapter.h"

#include "Conversions.h"
#include "EventConverter.h"
#include "SOEHandlerAdapter.h"
#include "OutstationAdapter.h"
#include "OutstationApplicationAdapter.h"
#include "OutstationCommandHandlerAdapter.h"
#include "MasterAdapter.h"
#include "MasterApplicationAdapter.h"

#include <functional>

using namespace System::Collections::Generic;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            ChannelAdapter::ChannelAdapter(const std::shared_ptr<opendnp3::IChannel>& channel)
                : channel(new std::shared_ptr<opendnp3::IChannel>(channel))
            {}

            ChannelAdapter::~ChannelAdapter()
            {
                this->!ChannelAdapter();
            }

            ChannelAdapter::!ChannelAdapter()
            {
                delete channel;
            }

            LogFilter ChannelAdapter::GetLogFilters()
            {
                return LogFilter((*channel)->GetLogFilters().get_value());
            }

            IChannelStatistics^ ChannelAdapter::GetChannelStatistics()
            {
                auto stats = (*channel)->GetStatistics();
                return Conversions::ConvertChannelStats(stats);
            }

            void ChannelAdapter::SetLogFilters(LogFilter filters)
            {				
                (*channel)->SetLogFilters(opendnp3::LogLevel(filters.Flags));
            }

            void CallbackListener(gcroot < System::Action<ChannelState> ^ >* listener, opendnp3::ChannelState aState)
            {
                ChannelState state = Conversions::ConvertChannelState(aState);
                (*listener)->Invoke(state);
            }

            IMaster^ ChannelAdapter::AddMaster(System::String^ loggerId, ISOEHandler^ handler, IMasterApplication^ application, MasterStackConfig^ config)
            {
                std::string stdLoggerId = Conversions::ConvertString(loggerId);

                auto SOEAdapter = std::shared_ptr<opendnp3::ISOEHandler>(new SOEHandlerAdapter(handler));
                auto appAdapter = std::shared_ptr<opendnp3::IMasterApplication>(new MasterApplicationAdapter(application));

                auto master = (*channel)->AddMaster(stdLoggerId.c_str(), SOEAdapter, appAdapter, Conversions::ConvertConfig(config));
                return master ? gcnew MasterAdapter(master) : nullptr;
            }

            IOutstation^ ChannelAdapter::AddOutstation(System::String^ loggerId, ICommandHandler^ cmdHandler, IOutstationApplication^ application, OutstationStackConfig^ config)
            {
                std::string stdLoggerId = Conversions::ConvertString(loggerId);

                auto commandAdapter = std::shared_ptr<opendnp3::ICommandHandler>(new OutstationCommandHandlerAdapter(cmdHandler));
                auto appAdapter = std::shared_ptr<opendnp3::IOutstationApplication>(new OutstationApplicationAdapter(application));

                auto outstation = (*channel)->AddOutstation(stdLoggerId.c_str(), commandAdapter, appAdapter, Conversions::ConvertConfig(config));
                return outstation ? gcnew OutstationAdapter(outstation) : nullptr;
            }

            void ChannelAdapter::Shutdown()
            {
                (*channel)->Shutdown();
            }

        }
    }
}
