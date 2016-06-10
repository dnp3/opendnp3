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
#include "OutstationTestObject.h"
#include <testlib/BufferHelpers.h>

using namespace openpal;
using namespace testlib;

namespace opendnp3
{

OutstationTestObject::OutstationTestObject(
    const OutstationConfig& config,
    const DatabaseTemplate& dbTemplate
) :
	log(),
	exe(),
	lower(),
	cmdHandler(CommandStatus::SUCCESS),
	application(),
	context(config, dbTemplate, log.root.logger, exe, lower, cmdHandler, application)
{
	lower.SetUpperLayer(context);
}

uint32_t OutstationTestObject::LowerLayerUp()
{
	context.OnLowerLayerUp();
	return exe.RunMany();
}

uint32_t OutstationTestObject::LowerLayerDown()
{
	context.OnLowerLayerDown();
	return exe.RunMany();
}

uint32_t OutstationTestObject::OnSendResult(bool isSuccess)
{
	context.OnSendResult(isSuccess);
	return exe.RunMany();
}

uint32_t OutstationTestObject::SendToOutstation(const std::string& hex)
{
	HexSequence hs(hex);
	context.OnReceive(hs.ToRSlice());
	return exe.RunMany();
}

size_t OutstationTestObject::NumPendingTimers() const
{
	return exe.NumPendingTimers();
}

bool OutstationTestObject::AdvanceToNextTimer()
{
	if (exe.AdvanceToNextTimer())
	{
		return exe.RunMany() > 0;
	}
	else
	{
		return false;
	}
}

uint32_t OutstationTestObject::AdvanceTime(const openpal::TimeDuration& td)
{
	exe.AdvanceTime(td);
	return exe.RunMany();
}

}

