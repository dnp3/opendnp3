/*
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
#ifndef ASIODNP3_GPRSMASTERSTACK_H
#define ASIODNP3_GPRSMASTERSTACK_H

#include "asiodnp3/IGPRSMaster.h"

#include <opendnp3/master/MasterContext.h>
#include <opendnp3/master/MasterStackConfig.h>
#include <opendnp3/transport/TransportStack.h>

namespace asiopal {
	class StrandExecutor;
}

namespace asiodnp3
{

/**
* Interface that represents an ephemeral master session
*/
class GPRSMasterStack : public IGPRSMaster
{
public:

	GPRSMasterStack(		
		openpal::LogRoot& root,
		asiopal::StrandExecutor& executor,
		opendnp3::ISOEHandler& SOEHandler,
		opendnp3::IMasterApplication& application,
		const opendnp3::MasterStackConfig& config		
	);

private:

	asiopal::StrandExecutor* m_executor;
	opendnp3::StackStatistics m_statistics;
	opendnp3::TransportStack m_transport;
	opendnp3::MContext m_context;
};

}

#endif

