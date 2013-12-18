/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __DYNAMICICALLY_ALLOCATED_DATABASE_H_
#define __DYNAMICICALLY_ALLOCATED_DATABASE_H_

#include "StaticDataFacade.h"
#include <opendnp3/DatabaseConfiguration.h>

#include <openpal/DynamicArray.h>

namespace opendnp3
{

class DynamicallyAllocatedDatabase
{	
	public:

	DynamicallyAllocatedDatabase(const DatabaseConfiguration& dataTemplate);

	StaticDataFacade GetFacade();
	
	private:

	void Configure(const DatabaseConfiguration& dataTemplate);
	
	openpal::DynamicArray<Binary> binaryValues;
	openpal::DynamicArray<Analog> analogValues;
	openpal::DynamicArray<Counter> counterValues;
	openpal::DynamicArray<ControlStatus> controlStatusValues;
	openpal::DynamicArray<SetpointStatus> setpointStatusValues;

	openpal::DynamicArray<BinaryMetadata> binaryMetadata;
	openpal::DynamicArray<AnalogMetadata> analogMetadata;
	openpal::DynamicArray<CounterMetadata> counterMetadata;
};

}

#endif
