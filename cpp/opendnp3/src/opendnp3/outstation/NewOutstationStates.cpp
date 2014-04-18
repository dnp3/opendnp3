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

#include "NewOutstationStates.h"

#include "opendnp3/LogLevels.h"

#include <openpal/LogMacros.h>

namespace opendnp3
{

// --------------------- OutstationStateBase ----------------------

void OutstationStateBase::OnSolConfirm(OutstationContext& context, const APDURecord& frag)
{
	FORMAT_LOG_BLOCK(context.logger, flags::WARN, "Unexpected sol confirm with sequence: %u", frag.control.SEQ);
}

void OutstationStateBase::OnUnsolConfirm(OutstationContext& context, const APDURecord& frag)
{
	FORMAT_LOG_BLOCK(context.logger, flags::WARN, "Unexpected unsol confirm with sequence: %u", frag.control.SEQ);
}

void OutstationStateBase::OnSendResult(OutstationContext& context, bool isSucccess)
{
	SIMPLE_LOG_BLOCK(context.logger, flags::WARN, "Unexpected send result callback");
}

void OutstationStateBase::OnConfirmTimeout(OutstationContext& context)
{
	SIMPLE_LOG_BLOCK(context.logger, flags::WARN, "Unexpected confirm timeout");
}

// --------------------- OutstationStateIdle ----------------------


OutstationStateIdle OutstationStateIdle::instance;

OutstationStateBase& OutstationStateIdle::Inst()
{
	return instance;
}

void OutstationStateIdle::OnNewRequest(OutstationContext&, const APDURecord& frag)
{

}

void OutstationStateIdle::OnRepeatRequest(OutstationContext&, const APDURecord& frag)
{

}

void OutstationStateIdle::OnSendResult(OutstationContext&, bool isSucccess)
{

}

}
