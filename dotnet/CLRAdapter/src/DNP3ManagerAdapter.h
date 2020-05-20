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
#ifndef OPENDNP3CLR_DNP3_MANAGER_ADAPTER_H
#define OPENDNP3CLR_DNP3_MANAGER_ADAPTER_H

#include <opendnp3/channel/ChannelRetry.h>
#include <opendnp3/channel/TLSConfig.h>

using namespace Automatak::DNP3::Interface;

using namespace System;

namespace opendnp3
{
    class DNP3Manager;
}

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            /// <summary>
            /// Factory class used to get the root DNP3 object
            /// </summary>
            public ref class DNP3ManagerFactory
            {
            public:
                /// <summary>
                /// Create a new IDNP3Manager using the specified concurrency
                /// </summary>
                /// <param name="concurrency">how many threads are allocated to the thread pool</param>
                
                /// <returns>IDNP3Manager interface</returns>
                static IDNP3Manager^ CreateManager(System::Int32 concurrency, ILogHandler^ logHandler);

                /// <summary>
                /// Create a new IDNP3Manager using the default concurrency
                /// </summary>
                /// <param name="logHandler">Callback interface for log messages</param>
                /// <returns>IDNP3Manager interface</returns>
                static IDNP3Manager^ CreateManager(ILogHandler^ logHandler);

            private:
                DNP3ManagerFactory() {}
            };

            ref class DNP3ManagerAdapter : public Automatak::DNP3::Interface::IDNP3Manager
            {
            public:

                DNP3ManagerAdapter(System::Int32 concurrency, ILogHandler^ logHandler);
                !DNP3ManagerAdapter();
                ~DNP3ManagerAdapter();

                virtual void Shutdown() sealed;

                virtual IChannel^ AddTCPClient(System::String^ id, System::UInt32 filters, Interface::ChannelRetry^ retry, System::Collections::Generic::IList<Interface::IPEndpoint^>^ remotes, Interface::IChannelListener^ listener)  sealed;
                virtual IChannel^ AddTCPServer(System::String^ id, System::UInt32 filters, Interface::ServerAcceptMode mode, Interface::IPEndpoint^ endpoint, Interface::IChannelListener^ listener) sealed;
                virtual IChannel^ AddUDPChannel(System::String^ id, System::UInt32 filters, Interface::ChannelRetry^ retry, Interface::IPEndpoint^ localEndpoint, Interface::IPEndpoint^ remoteEndpoint, Interface::IChannelListener^ listener);

                virtual IChannel^ AddTLSClient(System::String^ id, System::UInt32 filters, Interface::ChannelRetry^ retry, System::Collections::Generic::IList<Interface::IPEndpoint^>^ remotes, Interface::TLSConfig^ config, Interface::IChannelListener^ listener)  sealed;
                virtual IChannel^ AddTLSServer(System::String^ id, System::UInt32 filters, Interface::ServerAcceptMode mode, Interface::IPEndpoint^ endpoint, Interface::TLSConfig^ config, Interface::IChannelListener^ listener) sealed;
                
                virtual IChannel^ AddSerial(System::String^ id, System::UInt32 filters, Interface::ChannelRetry^ retry, Automatak::DNP3::Interface::SerialSettings^ settings, Interface::IChannelListener^ listener) sealed;

                virtual Interface::IListener^ CreateListener(System::String^ loggerid, System::UInt32 filters, Interface::IPEndpoint^ endpoint, IListenCallbacks^ callbacks) sealed;

                virtual Interface::IListener^ CreateListener(System::String^ loggerid, System::UInt32 filters, Interface::IPEndpoint^ endpoint, Interface::TLSConfig^ config, IListenCallbacks^ callbacks) sealed;

            private:

                opendnp3::DNP3Manager* manager;

                static opendnp3::ChannelRetry Convert(Interface::ChannelRetry^ retry);
            };

        }
    }
}

#endif
