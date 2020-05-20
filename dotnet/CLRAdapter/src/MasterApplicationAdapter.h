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
#ifndef OPENDNP3CLR_MASTER_APPLICATION_ADAPTER_H
#define OPENDNP3CLR_MASTER_APPLICATION_ADAPTER_H

#include "Conversions.h"
#include "MasterConversions.h"

#include <opendnp3/master/IMasterApplication.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::ObjectModel;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            private class MasterApplicationAdapter final : public opendnp3::IMasterApplication
            {
            public:

                MasterApplicationAdapter(Automatak::DNP3::Interface::IMasterApplication^ proxy) : proxy(proxy)
                {}

                virtual void OnStateChange(opendnp3::LinkStatus value) override
                {
                    proxy->OnStateChange((LinkStatus)value);
                }

                void OnUnknownDestinationAddress(uint16_t destination)
                {
                    proxy->OnUnknownDestinationAddress(destination);
                }

                void OnUnknownSourceAddress(uint16_t source)
                {
                    proxy->OnUnknownSourceAddress(source);
                }
                
                virtual void OnKeepAliveInitiated() override
                {
                    proxy->OnKeepAliveInitiated();
                }

                virtual void OnKeepAliveFailure() override
                {
                    proxy->OnKeepAliveFailure();
                }

                virtual void OnKeepAliveSuccess() override
                {
                    proxy->OnKeepAliveSuccess();
                }

                virtual opendnp3::UTCTimestamp Now() override final
                {
                    auto milliseconds = proxy->GetMillisecondsSinceEpoch();
                    return opendnp3::UTCTimestamp(milliseconds);
                }

                virtual void OnReceiveIIN(const opendnp3::IINField& iin) override final
                {
                    IINField ^iinField = gcnew IINField((Automatak::DNP3::Interface::LSBMask)iin.LSB, (Automatak::DNP3::Interface::MSBMask)iin.MSB);
                    proxy->OnReceiveIIN(iinField);
                }

                virtual void OnTaskStart(opendnp3::MasterTaskType type, opendnp3::TaskId id) override final
                {
                    proxy->OnTaskStart((MasterTaskType)type, id.GetId());
                }

                virtual void OnTaskComplete(const opendnp3::TaskInfo& info) override final
                {					
                    proxy->OnTaskComplete(MasterConversions::Convert(info));
                }

                virtual void OnOpen() override final
                {
                    proxy->OnOpen();
                }

                virtual void OnClose() override final
                {
                    proxy->OnClose();
                }

                virtual bool AssignClassDuringStartup() override final
                {
                    return proxy->AssignClassDuringStartup();
                }

                virtual void ConfigureAssignClassRequest(const opendnp3::WriteHeaderFunT& writer) override final
                {
                    auto assignments = proxy->GetClassAssignments();
                    for each(auto a in assignments)
                    {
                        if (a.range.IsAllObjects())
                        {
                            writer(opendnp3::Header::AllObjects(a.group, a.variation));
                        }
                        else
                        {
                            writer(opendnp3::Header::Range16(a.group, a.variation, a.range.start, a.range.stop));
                        }
                    }
                }

            private:

                gcroot < Automatak::DNP3::Interface::IMasterApplication^ > proxy;
            };

        }
    }
}

#endif
