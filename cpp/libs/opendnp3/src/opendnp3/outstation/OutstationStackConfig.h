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
#ifndef OPENDNP3_OUTSTATIONSTACKCONFIG_H
#define OPENDNP3_OUTSTATIONSTACKCONFIG_H

#include "opendnp3/outstation/OutstationConfig.h"
#include "opendnp3/outstation/EventBufferConfig.h"
#include "opendnp3/outstation/DatabaseTemplate.h"
#include "opendnp3/link/LinkConfig.h"

namespace opendnp3
{

/** A composite configuration struct that contains all the config
	information for a dnp3 outstation stack
*/
struct OutstationStackConfig
{

	OutstationStackConfig(const DatabaseTemplate& dbTemplate_) :
		dbTemplate(dbTemplate_),		
		link(false, false)		
	{

	}

	OutstationStackConfig() : link(false, false)		
	{}

	// Configuration of the database
	DatabaseTemplate dbTemplate;	
	
	/// Outstation config
	OutstationConfig outstation;
	
	/// Link layer config
	LinkConfig link;

};

}

#endif

