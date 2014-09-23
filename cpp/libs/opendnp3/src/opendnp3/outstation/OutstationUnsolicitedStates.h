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

#ifndef OPENDNP3_OUTSTATIONUNSOLICITEDSTATES_H
#define OPENDNP3_OUTSTATIONUNSOLICITEDSTATES_H

#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDUWrapper.h"

#include <openpal/util/Uncopyable.h>

namespace opendnp3
{

class OutstationContext;

class OutstationUnsolicitedStateBase : openpal::Uncopyable
{

public:		

	virtual OutstationUnsolicitedStateBase* OnConfirm(OutstationContext*, const APDUHeader& header) = 0;

	virtual OutstationUnsolicitedStateBase* OnSendResult(OutstationContext*, bool isSucccess) = 0;

	virtual OutstationUnsolicitedStateBase* OnConfirmTimeout(OutstationContext*) = 0;

};

/**
 * Idle state does nothing but log unexpected events
 */
class OutstationUnsolicitedStateIdle : public OutstationUnsolicitedStateBase
{

public:

	static OutstationUnsolicitedStateBase& Inst() { return instance;  }

	virtual OutstationUnsolicitedStateBase* OnConfirm(OutstationContext*, const APDUHeader& header) override;

	virtual OutstationUnsolicitedStateBase* OnSendResult(OutstationContext*, bool isSucccess) override;

	virtual OutstationUnsolicitedStateBase* OnConfirmTimeout(OutstationContext*) override;

protected:

	OutstationUnsolicitedStateIdle() {}

private:

	static OutstationUnsolicitedStateIdle instance;

};

class OutstationUnsolicitedStateConfirmWait : public OutstationUnsolicitedStateIdle
{

public:

	static OutstationUnsolicitedStateBase& Inst() { return instance; }

	virtual OutstationUnsolicitedStateBase* OnConfirm(OutstationContext*, const APDUHeader& header) override;

	virtual OutstationUnsolicitedStateBase* OnConfirmTimeout(OutstationContext*) override;

private:

	OutstationUnsolicitedStateConfirmWait() {}

	static OutstationUnsolicitedStateConfirmWait instance;

};


} //ens ns

#endif
