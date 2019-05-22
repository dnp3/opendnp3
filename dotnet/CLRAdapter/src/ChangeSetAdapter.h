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
#ifndef OPENDNP3CLR_CHANGESET_ADAPTER_H
#define OPENDNP3CLR_CHANGESET_ADAPTER_H

#include <asiodnp3/IOutstation.h>
#include <asiodnp3/UpdateBuilder.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private ref class ChangeSetAdapter : public Automatak::DNP3::Interface::IDatabase
			{
			public:

				ChangeSetAdapter();

				~ChangeSetAdapter();
				!ChangeSetAdapter();
				
				virtual void Update(Binary^ update, System::UInt16 index, EventMode mode);
				virtual void Update(DoubleBitBinary^ update, System::UInt16 index, EventMode mode);				
				virtual void Update(Analog^ update, System::UInt16 index, EventMode mode);
				virtual void Update(Counter^ update, System::UInt16 index, EventMode mode);
				virtual void Update(FrozenCounter^ update, System::UInt16 index, EventMode mode);
				virtual void Update(BinaryOutputStatus^ update, System::UInt16 index, EventMode mode);
				virtual void Update(AnalogOutputStatus^ update, System::UInt16 index, EventMode mode);
				virtual void Update(OctetString^ update, System::UInt16 index, EventMode mode);
				virtual void Update(TimeAndInterval^ update, System::UInt16 index);				

				void Apply(asiodnp3::IOutstation& proxy);
				
			private:
				
				asiodnp3::UpdateBuilder* builder;
			};

		}
	}
}

#endif
