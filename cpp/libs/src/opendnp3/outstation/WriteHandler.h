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
#ifndef OPENDNP3_WRITEHANDLER_H
#define OPENDNP3_WRITEHANDLER_H

#include "opendnp3/app/parsing/IAPDUHandler.h"
#include "opendnp3/app/IINField.h"

#include "opendnp3/outstation/IOutstationApplication.h"
#include "opendnp3/outstation/TimeSyncState.h"

#include "openpal/logging/Logger.h"
#include "openpal/executor/MonotonicTimestamp.h"

namespace opendnp3
{

class WriteHandler final : public IAPDUHandler
{
public:

	WriteHandler(IOutstationApplication& application, TimeSyncState& timeSyncState, AppSeqNum seq, openpal::MonotonicTimestamp now, IINField* pWriteIIN);

	virtual bool IsAllowed(uint32_t headerCount, GroupVariation gv, QualifierCode qc) override
	{
		return true;
	}

private:

	virtual IINField ProcessHeader(const RangeHeader& header, const ICollection<Indexed<IINValue>>& values) override;

	virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group50Var1>& times) override;

	virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group50Var3>& times) override;

	virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<TimeAndInterval>>& values) override;

	IOutstationApplication* application;
	TimeSyncState* timeSyncState;
	AppSeqNum seq;
	openpal::MonotonicTimestamp now;
	IINField* writeIIN;

	bool wroteTime = false;
	bool wroteIIN = false;
};

}



#endif

