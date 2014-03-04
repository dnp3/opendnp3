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
#ifndef __SLAVE_STACK_CONFIG_H_
#define __SLAVE_STACK_CONFIG_H_

#include "opendnp3/outstation/SlaveConfig.h"
#include "opendnp3/outstation/DatabaseConfiguration.h"
#include "opendnp3/app/AppConfig.h"
#include "opendnp3/link/LinkConfig.h"

namespace opendnp3
{

/** A composite configuration struct that contains all the config
	information for a dnp3 slave stack
*/
struct SlaveStackConfig {
	
	SlaveStackConfig(const DatabaseConfiguration& dbConfig) :
		database(dbConfig),
		link(false, false),		
		app(false)
	{
	
	}

	SlaveStackConfig() :
		database(),
		link(false, false),		
		app(false)
	{}

	//Configuration of the database
	DatabaseConfiguration database;
	/// Slave config
	SlaveConfig slave;	
	/// Application layer config
	AppConfig app;
	/// Link layer config
	LinkConfig link;
		
};

}

#endif

