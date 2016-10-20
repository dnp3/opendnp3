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
#ifndef OPENDNP3_OUTSTATIONSTATES_H
#define OPENDNP3_OUTSTATIONSTATES_H

#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDUWrapper.h"

#include <openpal/util/Uncopyable.h>

namespace opendnp3
{

class OContext;

enum class StateEnum
{
	Idle,
	SolConfirmWait,
	UnsolConfirmWait
};

/**
 * Base class for the outstation states
 */
class OutstationState
{
public:

	virtual StateEnum GetEnum() = 0;

	virtual OutstationState& OnConfirm(OContext&, const APDUHeader& header) = 0;	

	virtual OutstationState& OnConfirmTimeout(OContext&) = 0;

	virtual OutstationState& OnNewReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) = 0;

	virtual OutstationState& OnNewNonReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) = 0;

	virtual OutstationState& OnRepeatNonReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) = 0;

	virtual OutstationState& OnRepeatReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) = 0;



	inline bool IsIdle() { return GetEnum() == StateEnum::Idle; }

};

class StateIdle final : public OutstationState, private openpal::Uncopyable
{

public:

	virtual StateEnum GetEnum() { return StateEnum::Idle; }

	inline static OutstationState& Inst() { return instance; }

	virtual OutstationState& OnConfirm(OContext&, const APDUHeader& header) override;	

	virtual OutstationState& OnConfirmTimeout(OContext&) override;

	virtual OutstationState& OnNewReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

	virtual OutstationState& OnNewNonReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

	virtual OutstationState& OnRepeatNonReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

	virtual OutstationState& OnRepeatReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

private:

	static StateIdle instance;

	StateIdle() {}

};


/*
* waiting for a confirm to a solicited read response
*/
class StateSolicitedConfirmWait : public OutstationState, private openpal::Uncopyable
{

public:

	virtual StateEnum GetEnum() { return StateEnum::SolConfirmWait; }	

	inline static OutstationState& Inst() { return instance; }

	virtual OutstationState& OnConfirm(OContext&, const APDUHeader& header) override;	

	virtual OutstationState& OnConfirmTimeout(OContext&) override;

	virtual OutstationState& OnNewReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

	virtual OutstationState& OnNewNonReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

	virtual OutstationState& OnRepeatNonReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

	virtual OutstationState& OnRepeatReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

private:

	static StateSolicitedConfirmWait instance;

	StateSolicitedConfirmWait() {}
};

/*
* waiting for a confirm to an unsolicited read response
*/
class StateUnsolicitedConfirmWait : public OutstationState, private openpal::Uncopyable
{

public:

	virtual StateEnum GetEnum() { return StateEnum::UnsolConfirmWait; }

	inline static OutstationState& Inst() { return instance; }

	virtual OutstationState& OnConfirm(OContext&, const APDUHeader& header) override;	

	virtual OutstationState& OnConfirmTimeout(OContext&) override;

	virtual OutstationState& OnNewReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

	virtual OutstationState& OnNewNonReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

	virtual OutstationState& OnRepeatNonReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

	virtual OutstationState& OnRepeatReadRequest(OContext&, const APDUHeader& header, const openpal::RSlice& objects) override;

private:

	static StateUnsolicitedConfirmWait instance;

	StateUnsolicitedConfirmWait() {}
};

} //ens ns



#endif

