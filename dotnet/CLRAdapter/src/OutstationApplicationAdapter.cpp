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
#include "OutstationApplicationAdapter.h"

#include "Conversions.h"

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            OutstationApplicationAdapter::OutstationApplicationAdapter(Automatak::DNP3::Interface::IOutstationApplication^ proxy_) :
                proxy(proxy_)
            {

            }

            void OutstationApplicationAdapter::OnStateChange(opendnp3::LinkStatus value)
            {
                proxy->OnStateChange((LinkStatus)value);
            }

            void OutstationApplicationAdapter::OnUnknownDestinationAddress(uint16_t destination)
            {
                proxy->OnUnknownDestinationAddress(destination);
            }

            void OutstationApplicationAdapter::OnUnknownSourceAddress(uint16_t source)
            {
                proxy->OnUnknownSourceAddress(source);
            }

            void OutstationApplicationAdapter::OnKeepAliveInitiated()
            {
                proxy->OnKeepAliveInitiated();
            }

            void OutstationApplicationAdapter::OnKeepAliveFailure()
            {
                proxy->OnKeepAliveFailure();
            }

            void OutstationApplicationAdapter::OnKeepAliveSuccess()
            {
                proxy->OnKeepAliveSuccess();
            }

            bool OutstationApplicationAdapter::WriteAbsoluteTime(const opendnp3::UTCTimestamp& timestamp)
            {
                return proxy->WriteAbsoluteTime(timestamp.msSinceEpoch);
            }

            bool OutstationApplicationAdapter::SupportsWriteAbsoluteTime()
            {
                return proxy->SupportsWriteAbsoluteTime;
            }

            bool OutstationApplicationAdapter::SupportsWriteTimeAndInterval()
            {
                return proxy->SupportsWriteTimeAndInterval();
            }

            bool OutstationApplicationAdapter::WriteTimeAndInterval(const opendnp3::ICollection<opendnp3::Indexed<opendnp3::TimeAndInterval>>& values)
            {
                auto list = Conversions::ToIndexedEnumerable<TimeAndInterval^>(values);

                return proxy->WriteTimeAndInterval(list);
            }

            bool OutstationApplicationAdapter::SupportsAssignClass()
            {
                return proxy->SupportsAssignClass();
            }

            void OutstationApplicationAdapter::RecordClassAssignment(opendnp3::AssignClassType type, opendnp3::PointClass clazz, uint16_t start, uint16_t stop)
            {
                proxy->RecordClassAssignment((AssignClassType) type, (PointClass) clazz, start, stop);
            }

            opendnp3::ApplicationIIN OutstationApplicationAdapter::GetApplicationIIN() const
            {
                ApplicationIIN indications = proxy->ApplicationIndications;

                opendnp3::ApplicationIIN iin;
                iin.configCorrupt = indications.configCorrupt;
                iin.deviceTrouble = indications.deviceTrouble;
                iin.localControl = indications.localControl;
                iin.needTime = indications.needTime;
                iin.eventBufferOverflow = indications.eventBufferOverflow;
                return iin;
            }

            opendnp3::RestartMode OutstationApplicationAdapter::ColdRestartSupport() const
            {
                return (opendnp3::RestartMode) proxy->ColdRestartSupport;
            }

            opendnp3::RestartMode OutstationApplicationAdapter::WarmRestartSupport() const
            {
                return (opendnp3::RestartMode) proxy->WarmRestartSupport;
            }

            uint16_t OutstationApplicationAdapter::ColdRestart()
            {
                return proxy->ColdRestart();
            }

            uint16_t OutstationApplicationAdapter::WarmRestart()
            {
                return proxy->WarmRestart();
            }

            void OutstationApplicationAdapter::OnConfirmProcessed(bool is_unsolicited, uint32_t num_class1, uint32_t num_class2, uint32_t num_class3)
            {
                proxy->OnConfirmProcessed(is_unsolicited, num_class1, num_class2, num_class3);
            }
        }
    }
}
