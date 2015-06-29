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

#include "OutstationUnsolicitedStates.h"

#include "opendnp3/LogLevels.h"
#include "opendnp3/outstation/OutstationActions.h"

#include <openpal/logging/LogMacros.h>

namespace opendnp3
{

// --------------------- OutstationUnsolicitedStateIdle ----------------------

OutstationUnsolicitedStateIdle OutstationUnsolicitedStateIdle::instance;

OutstationUnsolicitedStateBase* OutstationUnsolicitedStateIdle::OnConfirm(OContext& ocontext, const APDUHeader& header)
{
	FORMAT_LOG_BLOCK(ocontext.logger, flags::WARN, "Unexpected unsolicted confirm with sequence: %u", header.control.SEQ);
	return this;
}

OutstationUnsolicitedStateBase* OutstationUnsolicitedStateIdle::OnSendResult(OContext& ocontext, bool isSucccess)
{
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::WARN, "Unexpected unsolcitied send result callback");
	return this;
}

OutstationUnsolicitedStateBase* OutstationUnsolicitedStateIdle::OnConfirmTimeout(OContext& ocontext)
{
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::WARN, "Unexpected unsolicited confirm timeout");
	return this;
}

// --------------------- OutstationUnsolicitedStateConfirmWait ----------------

OutstationUnsolicitedStateConfirmWait OutstationUnsolicitedStateConfirmWait::instance;

OutstationUnsolicitedStateBase* OutstationUnsolicitedStateConfirmWait::OnConfirm(OContext& ocontext, const APDUHeader& header)
{
	if (ocontext.unsol.seq.confirmNum.Equals(header.control.SEQ))
	{
		ocontext.confirmTimer.Cancel();

		if (ocontext.unsol.completedNull)
		{			
			ocontext.eventBuffer.ClearWritten();
		}
		else
		{
			ocontext.unsol.completedNull = true;
		}

		return &OutstationUnsolicitedStateIdle::Inst();
	}
	else
	{
		return this;
	}
}

OutstationUnsolicitedStateBase* OutstationUnsolicitedStateConfirmWait::OnConfirmTimeout(OContext& ocontext)
{
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::WARN, "Unsolicited confirm timeout");
	ocontext.eventBuffer.Unselect();
	return &OutstationUnsolicitedStateIdle::Inst();
}

} //ens ns

