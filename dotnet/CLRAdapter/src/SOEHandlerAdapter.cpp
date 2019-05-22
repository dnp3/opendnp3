/*
 * Copyright 2013-2019 Automatak, LLC
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

			SOEHandlerAdapter::SOEHandlerAdapter(Automatak::DNP3::Interface::ISOEHandler^ aProxy) : proxy(aProxy)
			{}

			void SOEHandlerAdapter::Start()
			{
				proxy->Start();
			}

			void SOEHandlerAdapter::End()
			{
				proxy->End();
			}

			HeaderInfo^ SOEHandlerAdapter::GetInfo(const opendnp3::HeaderInfo& info)
			{
				return gcnew HeaderInfo(
					(GroupVariation)info.gv, 
					(QualifierCode)info.qualifier, 
					(TimestampMode)info.tsmode,
					info.isEventVariation,
					info.flagsValid,
					info.headerIndex
				);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Binary>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<Binary^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::DoubleBitBinary>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<DoubleBitBinary^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Analog>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<Analog^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::Counter>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<Counter^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::FrozenCounter>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<FrozenCounter^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryOutputStatus>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<BinaryOutputStatus^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogOutputStatus>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<AnalogOutputStatus^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::OctetString>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<OctetString^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::TimeAndInterval>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<TimeAndInterval^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::BinaryCommandEvent>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<BinaryCommandEvent^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::AnalogCommandEvent>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<AnalogCommandEvent^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

			void SOEHandlerAdapter::Process(const opendnp3::HeaderInfo& info, const opendnp3::ICollection<opendnp3::Indexed<opendnp3::SecurityStat>>& values)
			{
				auto enumerable = Conversions::ToIndexedEnumerable<SecurityStat^>(values);
				proxy->Process(GetInfo(info), enumerable);
			}

		}
	}
}
