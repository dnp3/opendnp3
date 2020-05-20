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
#include "SOEHandlerAdapter.h"

#include "Conversions.h"

namespace Automatak
{
    namespace DNP3
    {
        namespace Adapter
        {

            HeaderInfo ^ ConvertHeaderInfo(const opendnp3::HeaderInfo& info)
            {
                return gcnew HeaderInfo((GroupVariation)info.gv, (QualifierCode)info.qualifier,
                                        (TimestampQuality)info.tsquality, info.isEventVariation, info.flagsValid,
                                        info.headerIndex);
            }

            ResponseInfo ^ ConvertResponseInfo(const opendnp3::ResponseInfo& info)
            {
                return gcnew ResponseInfo(info.unsolicited, info.fir, info.fin);
            }

            SOEHandlerAdapter::SOEHandlerAdapter(Automatak::DNP3::Interface::ISOEHandler^ aProxy) : proxy(aProxy)
            {}
                        
            void SOEHandlerAdapter::BeginFragment(const opendnp3::ResponseInfo& info)
            {
                proxy->BeginFragment(ConvertResponseInfo(info));
            }

            void SOEHandlerAdapter::EndFragment(const opendnp3::ResponseInfo& info)
            {
                proxy->EndFragment(ConvertResponseInfo(info));
            }
        
            void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Binary>>& values)
            {
                auto enumerable = Conversions::ToIndexedEnumerable<Binary^>(values);
                proxy->Process(ConvertHeaderInfo(info), enumerable);
            }

            void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::DoubleBitBinary>>& values)
            {
                auto enumerable = Conversions::ToIndexedEnumerable<DoubleBitBinary^>(values);
                proxy->Process(ConvertHeaderInfo(info), enumerable);
            }

            void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Analog>>& values)
            {
                auto enumerable = Conversions::ToIndexedEnumerable<Analog^>(values);
                proxy->Process(ConvertHeaderInfo(info), enumerable);
            }

            void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Counter>>& values)
            {
                auto enumerable = Conversions::ToIndexedEnumerable<Counter^>(values);
                proxy->Process(ConvertHeaderInfo(info), enumerable);
            }

            void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::FrozenCounter>>& values)
            {
                auto enumerable = Conversions::ToIndexedEnumerable<FrozenCounter^>(values);
                proxy->Process(ConvertHeaderInfo(info), enumerable);
            }

            void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryOutputStatus>>& values)
            {
                auto enumerable = Conversions::ToIndexedEnumerable<BinaryOutputStatus^>(values);
                proxy->Process(ConvertHeaderInfo(info), enumerable);
            }

            void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputStatus>>& values)
            {
                auto enumerable = Conversions::ToIndexedEnumerable<AnalogOutputStatus^>(values);
                proxy->Process(ConvertHeaderInfo(info), enumerable);
            }

            void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::OctetString>>& values)
            {
                auto enumerable = Conversions::ToIndexedEnumerable<OctetString^>(values);
                proxy->Process(ConvertHeaderInfo(info), enumerable);
            }

            void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::TimeAndInterval>>& values)
            {
                auto enumerable = Conversions::ToIndexedEnumerable<TimeAndInterval^>(values);
                proxy->Process(ConvertHeaderInfo(info), enumerable);
            }

            void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryCommandEvent>>& values)
            {
                auto enumerable = Conversions::ToIndexedEnumerable<BinaryCommandEvent^>(values);
                proxy->Process(ConvertHeaderInfo(info), enumerable);
            }

            void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogCommandEvent>>& values)
            {
                auto enumerable = Conversions::ToIndexedEnumerable<AnalogCommandEvent^>(values);
                proxy->Process(ConvertHeaderInfo(info), enumerable);
            }
        
        }
    }
}
