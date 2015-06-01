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

#include "MasterAuthProvider.h"

#include <opendnp3/master/MasterState.h>


using namespace openpal;
using namespace opendnp3;

namespace secauth
{

MasterAuthProvider::MasterAuthProvider(
	opendnp3::IMasterApplication& application,
	openpal::IExecutor& executor,
	openpal::Logger logger,
	openpal::ICryptoProvider& crypto,
	IMasterUserDatabase& userDB
	) : 
	msstate(application, executor, crypto, userDB),
	sessionKeyTask(application, TimeDuration::Seconds(5), logger, User::Default(), msstate)
{

}

void MasterAuthProvider::GoOnline(opendnp3::MState& mstate)
{
	// add the session key task to the scheduler
	mstate.scheduler.Schedule(openpal::ManagedPtr<IMasterTask>::WrapperOnly(&sessionKeyTask));
}

void MasterAuthProvider::GoOffline(opendnp3::MState& mstate)
{
	// TODO reset the sessions?
}

void MasterAuthProvider::OnReceive(opendnp3::MState& mstate, const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects)
{	
	if (header.function == FunctionCode::AUTH_RESPONSE)
	{
		mstate.ProcessResponse(header, objects);
	}
	else
	{
		mstate.ProcessAPDU(header, objects);
	}
}

}

