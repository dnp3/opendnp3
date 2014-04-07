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
#ifndef __MASTER_STACK_IMPL_H_
#define __MASTER_STACK_IMPL_H_

#include "IMaster.h"

#include <opendnp3/master/MasterStackConfig.h>
#include <opendnp3/master/Master.h>
#include <opendnp3/app/ApplicationStack.h>

namespace asiodnp3
{

class ILinkContext;

/** @section desc A stack object for a master */
class MasterStackImpl : public IMaster
{
public:

	MasterStackImpl(
		openpal::LogRoot& root,
	    openpal::IExecutor* apExecutor,
	    opendnp3::ISOEHandler* apPublisher,
	    IUTCTimeSource* apTimeSource,
		opendnp3::AsyncTaskGroup* apTaskGroup,
		const opendnp3::MasterStackConfig& config,
		const StackActionHandler& handler);

	opendnp3::ICommandProcessor* GetCommandProcessor();

	opendnp3::MasterScan GetIntegrityScan();

	opendnp3::MasterScan AddClassScan(int aClassMask, openpal::TimeDuration aScanRate, openpal::TimeDuration aRetryRate);

private:
	opendnp3::Master master;
};

}

#endif

