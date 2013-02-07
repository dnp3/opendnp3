
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
#ifndef __SLAVE_STACK_CONFIG_H_
#define __SLAVE_STACK_CONFIG_H_

#include "SlaveConfig.h"
#include "DeviceTemplate.h"
#include "AppConfig.h"
#include "LinkConfig.h"
#include "VtoConfig.h"

namespace opendnp3
{

/** A composite configuration struct that contains all the config
	information for a dnp3 slave stack
*/
struct SlaveStackConfig {
	SlaveStackConfig() :
		link(false, false)
	{}

	SlaveConfig slave;		// Slave config
	DeviceTemplate device;		// Device template that specifies database layout
	AppConfig app;			// Application layer config
	LinkConfig link;		// Link layer config
	VtoConfig vto;
};

}

#endif

