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
#ifndef ASIODNP3_IOUTSTATION_H
#define ASIODNP3_IOUTSTATION_H

#include "IStack.h"


namespace opendnp3
{
	class Database;
}

namespace asiodnp3
{

/**
* Interface representing a running outstation.
* To get a data observer interface to load measurements on the outstation:-
\code
	IMeasurementLoader* pDataObserver = pOutstation->GetDataObserver()
\endcode
*/
class IOutstation : public IStack
{
	public:	
		
	virtual ~IOutstation() {}

	/**
	* Sets the restart IIN bit. Normally applications should not
	* touch this bit, but it is provided for simulating restarts.
	*/
	virtual void SetRestartIIN() = 0;

	/**
	* @return stack statistics counters
	*/
	virtual opendnp3::StackStatistics GetStackStatistics() = 0;

	/**
	* Get a the database interface to load measurements into the outstation
	* @return Database inteface used to load measurements into the outstation
	*/
	virtual opendnp3::Database& GetDatabase() = 0;
};

}

#endif
