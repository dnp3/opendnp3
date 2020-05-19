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
#include "DNP3ManagerAdapter.h"

#include "ChannelAdapter.h"
#include "ChannelListenerAdapter.h"
#include "Conversions.h"
#include "ListenCallbacksAdapter.h"
#include "ListenerAdapter.h"
#include "LogAdapter.h"

#include <opendnp3/DNP3Manager.h>

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            IDNP3Manager^ DNP3ManagerFactory::CreateManager(System::Int32 concurrency, ILogHandler^ logHandler)
            {
                return gcnew DNP3ManagerAdapter(concurrency, logHandler);
            }

            IDNP3Manager^ DNP3ManagerFactory::CreateManager(ILogHandler^ logHandler)
            {
                return gcnew DNP3ManagerAdapter(Environment::ProcessorCount, logHandler);
            }


            DNP3ManagerAdapter::DNP3ManagerAdapter(System::Int32 concurrency, ILogHandler^ logHandler) :
                manager(new opendnp3::DNP3Manager(concurrency, LogAdapter::Create(logHandler)))				
            {

            }

            DNP3ManagerAdapter::!DNP3ManagerAdapter()
            {
                delete manager;
            }

            DNP3ManagerAdapter::~DNP3ManagerAdapter()
            {
                this->!DNP3ManagerAdapter();
            }

            void DNP3ManagerAdapter::Shutdown()
            {
                manager->Shutdown();
            }

            IChannel^ DNP3ManagerAdapter::AddTCPClient(System::String^ id, System::UInt32 filters, Interface::ChannelRetry^ retry, System::Collections::Generic::IList<Interface::IPEndpoint^>^ remotes, Automatak::DNP3::Interface::IChannelListener^ listener)
            {
                std::string stdName = Conversions::ConvertString(id);

                std::vector<opendnp3::IPEndpoint> endpoints;
                for each(auto remote in remotes)
                {
                    endpoints.push_back(Conversions::Convert(remote));
                }
                
                auto listenAdapter = listener
                    ? std::shared_ptr<opendnp3::IChannelListener>(new ChannelListenerAdapter(listener))
                    : nullptr;

                try
                {
                    auto channel = this->manager->AddTCPClient(stdName.c_str(), opendnp3::LogLevel(filters), Convert(retry), endpoints, "", listenAdapter);
                    return gcnew ChannelAdapter(channel);
                }
                catch (opendnp3::DNP3Error e)
                {
                    throw gcnew DNP3Exception(Conversions::ConvertString(e.what()));
                }
            }

            IChannel^ DNP3ManagerAdapter::AddTCPServer(System::String^ id, System::UInt32 filters, Interface::ServerAcceptMode mode, Interface::IPEndpoint^ endpoint, Automatak::DNP3::Interface::IChannelListener^ listener)
            {
                std::string stdName = Conversions::ConvertString(id);
                auto stdEndpoint = Conversions::Convert(endpoint);

                auto listenAdapter = listener
                    ? std::shared_ptr<opendnp3::IChannelListener>(new ChannelListenerAdapter(listener))
                    : nullptr;

                try
                {
                    auto channel = this->manager->AddTCPServer(stdName.c_str(), opendnp3::LogLevel(filters),
                                                      (opendnp3::ServerAcceptMode)mode, stdEndpoint, listenAdapter);
                    return gcnew ChannelAdapter(channel);
                }
                catch (opendnp3::DNP3Error e)
                {
                    throw gcnew DNP3Exception(Conversions::ConvertString(e.what()));
                }
            }

            IChannel^ DNP3ManagerAdapter::AddUDPChannel(System::String^ id, System::UInt32 filters, Interface::ChannelRetry^ retry, Interface::IPEndpoint^ localEndpoint, Interface::IPEndpoint^ remoteEndpoint, Interface::IChannelListener^ listener)
            {
                std::string stdName = Conversions::ConvertString(id);
                auto stdLocalEndpoint = Conversions::Convert(localEndpoint);
                auto stdRemoteEndpoint = Conversions::Convert(remoteEndpoint);

                auto listenAdapter = listener
                    ? std::shared_ptr<opendnp3::IChannelListener>(new ChannelListenerAdapter(listener))
                    : nullptr;

                try
                {
                    auto channel = this->manager->AddUDPChannel(stdName.c_str(), opendnp3::LogLevel(filters), Convert(retry),
                                                       stdLocalEndpoint, stdRemoteEndpoint, listenAdapter);
                    return gcnew ChannelAdapter(channel);
                }
                catch (opendnp3::DNP3Error e)
                {
                    throw gcnew DNP3Exception(Conversions::ConvertString(e.what()));
                }
            }

            IChannel^ DNP3ManagerAdapter::AddTLSClient(System::String^ id, System::UInt32 filters, Interface::ChannelRetry^ retry, System::Collections::Generic::IList<Interface::IPEndpoint^>^ remotes, Automatak::DNP3::Interface::TLSConfig^ config, Automatak::DNP3::Interface::IChannelListener^ listener)
            {
                std::string stdName = Conversions::ConvertString(id);

                std::vector<opendnp3::IPEndpoint> endpoints;
                for each(auto remote in remotes)
                {
                    endpoints.push_back(Conversions::Convert(remote));
                }

                auto listenAdapter = listener
                    ? std::shared_ptr<opendnp3::IChannelListener>(new ChannelListenerAdapter(listener))
                    : nullptr;

                try
                {
                    auto channel = this->manager->AddTLSClient(stdName.c_str(), opendnp3::LogLevel(filters), Convert(retry),
                                                               endpoints, "", Conversions::Convert(config), listenAdapter);
                    return gcnew ChannelAdapter(channel);
                }
                catch (opendnp3::DNP3Error e)
                {
                    throw gcnew DNP3Exception(Conversions::ConvertString(e.what()));
                }
            }
            
            IChannel^ DNP3ManagerAdapter::AddTLSServer(System::String^ id, System::UInt32 filters, Interface::ServerAcceptMode mode, Interface::IPEndpoint^ endpoint, Automatak::DNP3::Interface::TLSConfig^ config, Automatak::DNP3::Interface::IChannelListener^ listener)
            {
                std::string stdName = Conversions::ConvertString(id);
                auto stdEndpoint = Conversions::Convert(endpoint);

                auto listenAdapter = listener
                    ? std::shared_ptr<opendnp3::IChannelListener>(new ChannelListenerAdapter(listener))
                    : nullptr;
                
                try
                {
                    auto channel = this->manager->AddTLSServer(stdName.c_str(), opendnp3::LogLevel(filters), (opendnp3::ServerAcceptMode) mode, stdEndpoint, Conversions::Convert(config), listenAdapter);
                    return gcnew ChannelAdapter(channel);
                }
                catch (opendnp3::DNP3Error e)
                {
                    throw gcnew DNP3Exception(Conversions::ConvertString(e.what()));
                }
            }

            IChannel^ DNP3ManagerAdapter::AddSerial(System::String^ id, System::UInt32 filters, Interface::ChannelRetry^ retry, Automatak::DNP3::Interface::SerialSettings^ settings, Automatak::DNP3::Interface::IChannelListener^ listener)
            {
                std::string stdName = Conversions::ConvertString(id);
                auto s = Conversions::ConvertSerialSettings(settings);				

                auto listenAdapter = listener
                    ? std::shared_ptr<opendnp3::IChannelListener>(new ChannelListenerAdapter(listener))
                    : nullptr;
                
                try
                {
                    auto channel = this->manager->AddSerial(stdName.c_str(), opendnp3::LogLevel(filters),
                                                            Convert(retry), s, listenAdapter);
                    return gcnew ChannelAdapter(channel);
                }
                catch (opendnp3::DNP3Error e)
                {
                    throw gcnew DNP3Exception(Conversions::ConvertString(e.what()));
                }
            }

            Interface::IListener^ DNP3ManagerAdapter::CreateListener(System::String^ loggerid, System::UInt32 filters, Interface::IPEndpoint^ endpoint, IListenCallbacks^ callbacks)
            {
                auto id = Conversions::ConvertString(loggerid);
                auto levels = opendnp3::LogLevel(filters);
                auto ep = Conversions::Convert(endpoint);
                auto cb = std::shared_ptr<opendnp3::IListenCallbacks>(new ListenCallbacksAdapter(callbacks));

                try
                {
                    auto listener = manager->CreateListener(id, levels, ep, cb);
                    return gcnew ListenerAdapter(listener);
                }
                catch (opendnp3::DNP3Error e)
                {
                    throw gcnew DNP3Exception(Conversions::ConvertString(e.what()));
                }
            }

            Interface::IListener^ DNP3ManagerAdapter::CreateListener(System::String^ loggerid, System::UInt32 filters, Interface::IPEndpoint^ endpoint, Interface::TLSConfig^ config, IListenCallbacks^ callbacks)
            {
                auto id = Conversions::ConvertString(loggerid);
                auto levels = opendnp3::LogLevel(filters);
                auto ep = Conversions::Convert(endpoint);
                auto tlsConfig = Conversions::Convert(config);
                auto cb = std::shared_ptr<opendnp3::IListenCallbacks>(new ListenCallbacksAdapter(callbacks));

                try
                {
                    auto listener = manager->CreateListener(id, levels, ep, tlsConfig, cb);
                    return gcnew ListenerAdapter(listener);
                }
                catch (opendnp3::DNP3Error e)
                {
                    throw gcnew DNP3Exception(Conversions::ConvertString(e.what()));
                }
            }

            opendnp3::ChannelRetry DNP3ManagerAdapter::Convert(Interface::ChannelRetry ^ retry)
            {
                return opendnp3::ChannelRetry(Conversions::ConvertTimespan(retry->minRetryDelay),
                                              Conversions::ConvertTimespan(retry->maxRetryDelay),
                                              Conversions::ConvertTimespan(retry->reconnectDelay));
            }
        }
    }
}
