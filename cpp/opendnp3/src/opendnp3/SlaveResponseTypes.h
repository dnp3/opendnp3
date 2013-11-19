
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __SLAVE_RESPONSE_TYPES_H_
#define __SLAVE_RESPONSE_TYPES_H_

#include <opendnp3/ObjectInterfaces.h>
#include <opendnp3/DataTypes.h>
#include <openpal/Visibility.h>
#include <opendnp3/OutstationResponses.h>

namespace opendnp3
{

struct SlaveConfig;

/**
 * Reads a slave config object and and translates the configuration to
 * singletons.
 */
class DLL_LOCAL SlaveResponseTypes
{
public:

	SlaveResponseTypes(const SlaveConfig& arCfg);

	StreamObject<Binary>* mpStaticBinary;
	StreamObject<Analog>* mpStaticAnalog;
	StreamObject<Counter>* mpStaticCounter;
	StreamObject<ControlStatus>* mpStaticControlStatus;
	StreamObject<SetpointStatus>* mpStaticSetpointStatus;

	StreamObject<Binary>* mpEventBinary;
	StreamObject<Analog>* mpEventAnalog;
	StreamObject<Counter>* mpEventCounter;

	SizeByVariationObject* mpEventVto;

private:

	static StreamObject<Binary>* GetStaticBinary(StaticBinaryResponse);
	static StreamObject<Analog>* GetStaticAnalog(StaticAnalogResponse);
	static StreamObject<Counter>* GetStaticCounter(StaticCounterResponse);
	static StreamObject<SetpointStatus>* GetStaticSetpointStatus(StaticSetpointStatusResponse);

	static StreamObject<Binary>* GetEventBinary(EventBinaryResponse);
	static StreamObject<Analog>* GetEventAnalog(EventAnalogResponse);
	static StreamObject<Counter>* GetEventCounter(EventCounterResponse);

};

}

#endif

