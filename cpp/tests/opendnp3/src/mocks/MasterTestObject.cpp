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
#include "MasterTestObject.h"

#include <asiodnp3/DefaultMasterApplication.h>

#include <testlib/BufferHelpers.h>

using namespace testlib;

namespace opendnp3
{

MasterParams NoStartupTasks()
{
	MasterParams params;
	params.disableUnsolOnStartup = false;
	params.startupIntegrityClassMask = ClassField::None();
	params.unsolClassMask = ClassField::None();
	return params;
}

MasterTestObject::MasterTestObject(
    const MasterParams& params,
    const std::string& id,
    const std::shared_ptr <openpal::ILogHandler >& log,
    const std::shared_ptr<testlib::MockExecutor>& executor,
    const std::shared_ptr<IMasterScheduler>& scheduler
) :
	log(log),
	exe(executor ? executor : std::make_shared<MockExecutor>()),
	meas(std::make_shared<MockSOEHandler>()),
	lower(std::make_shared<MockLowerLayer>()),
	application(std::make_shared<MockMasterApplication>()),
	scheduler(scheduler ? scheduler : std::make_shared<MasterSchedulerBackend>(exe)),
	context(std::make_shared<MContext>(openpal::Logger(log, id, ~0), exe, lower, meas, application, this->scheduler, params))
{}

void MasterTestObject::SendToMaster(const std::string& hex)
{
	HexSequence hs(hex);
	context->OnReceive(hs.ToRSlice());
}

}

