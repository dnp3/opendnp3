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
#ifndef OPENDNP3CLR_CHANNEL_ADAPTER_H
#define OPENDNP3CLR_CHANNEL_ADAPTER_H

#include <opendnp3/channel/IChannel.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            ref class ChannelAdapter : IChannel
            {
            public:

                ChannelAdapter(const std::shared_ptr<opendnp3::IChannel>& channel);

                ~ChannelAdapter();
                !ChannelAdapter();

                virtual LogFilter GetLogFilters() sealed;

                virtual IChannelStatistics^ GetChannelStatistics() sealed;

                virtual void SetLogFilters(LogFilter filters) sealed;

                virtual IMaster^ AddMaster(System::String^ loggerId, ISOEHandler^ publisher, IMasterApplication^ application, MasterStackConfig^ config) sealed;

                virtual IOutstation^ AddOutstation(System::String^ loggerId, ICommandHandler^ cmdHandler, IOutstationApplication^ application, OutstationStackConfig^ config) sealed;

                virtual void Shutdown() sealed;

            private:

                std::shared_ptr<opendnp3::IChannel>* channel;
            };

        }
    }
}

#endif
