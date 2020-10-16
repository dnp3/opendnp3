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
#ifndef OPENDNP3CLR_OUTSTATION_APPLICATION_ADAPTER_H
#define OPENDNP3CLR_OUTSTATION_APPLICATION_ADAPTER_H

#include <opendnp3/outstation/IOutstationApplication.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::ObjectModel;

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            private class OutstationApplicationAdapter final : public opendnp3::IOutstationApplication
            {
            public:
                OutstationApplicationAdapter(Automatak::DNP3::Interface::IOutstationApplication^ proxy);

                virtual void OnStateChange(opendnp3::LinkStatus value) override final;
                
                virtual void OnUnknownDestinationAddress(uint16_t destination) override final;
                
                virtual void OnUnknownSourceAddress(uint16_t source) override final;

                virtual void OnKeepAliveInitiated() override final;

                virtual void OnKeepAliveFailure() override final;

                virtual void OnKeepAliveSuccess() override final;				

                virtual bool WriteAbsoluteTime(const opendnp3::UTCTimestamp& timestamp) override final;

                virtual bool SupportsWriteAbsoluteTime() override final;
                
                virtual bool SupportsWriteTimeAndInterval() override final;
                
                virtual bool WriteTimeAndInterval(const opendnp3::ICollection<opendnp3::Indexed<opendnp3::TimeAndInterval>>& values) override final;

                virtual bool SupportsAssignClass() override final;

                virtual void RecordClassAssignment(opendnp3::AssignClassType type, opendnp3::PointClass clazz, uint16_t start, uint16_t stop) override final;

                virtual opendnp3::ApplicationIIN GetApplicationIIN() const override final;

                virtual opendnp3::RestartMode ColdRestartSupport() const override final;

                virtual opendnp3::RestartMode WarmRestartSupport() const override final;

                virtual uint16_t ColdRestart() override final;

                virtual uint16_t WarmRestart() override final;

                virtual void OnConfirmProcessed(bool is_unsolicited, uint32_t num_class1, uint32_t num_class2, uint32_t num_class3) override final;

            private:

                gcroot < Automatak::DNP3::Interface::IOutstationApplication^ > proxy;
            };

        }
    }
}

#endif
