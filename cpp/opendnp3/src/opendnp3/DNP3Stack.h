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
#ifndef __DNP3_STACK_H_
#define __DNP3_STACK_H_

#include "IStack.h"
#include "StackActionHandler.h"

namespace opendnp3
{

/**
* Base class for masters or outstations. Can be used to bind a vto endpoint or shutdown.
*/
class DNP3Stack : public IStack
{
public:	

	DNP3Stack(const StackActionHandler& handler_);

	virtual ~DNP3Stack() {}
	
	virtual openpal::IExecutor* GetExecutor() override final;

	/**
	* Enable communications
	*/
	virtual void Enable() override final;

	/**
	* Enable communications
	*/
	virtual void Disable() override final;

	/**
	* External Shutdown function
	*/
	virtual void BeginShutdown() override final;

private:

	StackActionHandler handler;
};

}

#endif
