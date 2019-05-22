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
#include "ChangeSetAdapter.h"

#include "Conversions.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			ChangeSetAdapter::ChangeSetAdapter() : builder(new asiodnp3::UpdateBuilder())
			{}

			ChangeSetAdapter::~ChangeSetAdapter()
			{
				this->!ChangeSetAdapter();
			}

			ChangeSetAdapter::!ChangeSetAdapter()
			{
				delete builder;
			}
		
			void ChangeSetAdapter::Apply(asiodnp3::IOutstation& proxy)
			{
				proxy.Apply(builder->Build());				
			}

			void ChangeSetAdapter::Update(Binary^ update, System::UInt16 index, EventMode mode)
			{
				builder->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}
			
			void ChangeSetAdapter::Update(DoubleBitBinary^ update, System::UInt16 index, EventMode mode)
			{
				builder->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(Analog^ update, System::UInt16 index, EventMode mode)
			{
				builder->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(Counter^ update, System::UInt16 index, EventMode mode)
			{
				builder->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(FrozenCounter^ update, System::UInt16 index, EventMode mode)
			{
				builder->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}
			
			void ChangeSetAdapter::Update(BinaryOutputStatus^ update, System::UInt16 index, EventMode mode)
			{
				builder->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(AnalogOutputStatus^ update, System::UInt16 index, EventMode mode)
			{
				builder->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(OctetString^ update, System::UInt16 index, EventMode mode)
			{				
				builder->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(TimeAndInterval^ update, System::UInt16 index)
			{
				builder->Update(Conversions::ConvertMeas(update), index);
			}

		}
	}
}
