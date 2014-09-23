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

#ifndef OPENDNP3_IMASTERSTATE_H
#define OPENDNP3_IMASTERSTATE_H

#include <openpal/util/Uncopyable.h>
#include <openpal/container/ReadOnlyBuffer.h>

#include <opendnp3/app/APDUHeader.h>

namespace opendnp3
{

class MasterContext;

class IMasterState : private openpal::Uncopyable
{
	public:

	virtual IMasterState* OnStart(MasterContext*pContext);
	
	virtual IMasterState* OnResponse(MasterContext*pContext, const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects);

	virtual IMasterState* OnResponseTimeout(MasterContext*pContext);
};

class MasterStateIdle : public IMasterState
{
	public:

	static IMasterState& Instance() { return instance; }

	virtual IMasterState* OnStart(MasterContext*pContext);

	private:

	MasterStateIdle() {}

	static MasterStateIdle instance;
};

class MasterStateTaskReady : public IMasterState
{

public:

	static IMasterState& Instance() { return instance; }

	virtual IMasterState* OnStart(MasterContext*pContext) override final;	

private:

	MasterStateTaskReady() {}

	static MasterStateTaskReady instance;
};

class MasterStateWaitForResponse : public IMasterState
{

public:

	static IMasterState& Instance() { return instance; }	

	virtual IMasterState* OnResponse(MasterContext*pContext, const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects) override final;

	virtual IMasterState* OnResponseTimeout(MasterContext*pContext)  override final;

private:

	MasterStateWaitForResponse() {}

	static MasterStateWaitForResponse instance;
};


}

#endif

