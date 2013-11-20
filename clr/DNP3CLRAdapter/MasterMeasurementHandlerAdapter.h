
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#ifndef __MASTER_MEASUREMENT_HANDLER_ADAPTER_H_
#define __MASTER_MEASUREMENT_HANDLER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/IMeasurementHandler.h>
#include <vcclr.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private class MasterMeasurementHandlerAdapter : public opendnp3::IMeasurementHandler
{
public:

	MasterMeasurementHandlerAdapter(DNP3::Interface::IMeasurementHandler^ proxy);

	void Load(const opendnp3::IMeasurementUpdate& arUpdate);

private:

	gcroot < DNP3::Interface::IMeasurementHandler ^ > proxy;	
};

private ref class MasterMeasurementHandlerWrapper
{
public:

	MasterMeasurementHandlerWrapper(DNP3::Interface::IMeasurementHandler^ proxy) :
		mpAdapter(new MasterMeasurementHandlerAdapter(proxy))
	{}

	~MasterMeasurementHandlerWrapper() {
		delete mpAdapter;
	}

	opendnp3::IMeasurementHandler* Get() {
		return mpAdapter;
	}

private:
	MasterMeasurementHandlerAdapter* mpAdapter;
};

}
}

#endif
