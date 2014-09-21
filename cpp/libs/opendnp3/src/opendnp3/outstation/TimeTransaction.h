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
#ifndef OPENDNP3_TIMETRANSACTION_H
#define OPENDNP3_TIMETRANSACTION_H

#include "opendnp3/app/ITransactable.h"

#include <openpal/executor/UTCTimestamp.h>

#include "opendnp3/outstation/IDatabase.h"

namespace opendnp3
{

class TimeTransaction : private Transaction
{
public:
	TimeTransaction(IDatabase& database, openpal::UTCTimestamp timestamp_);

	void Update(const Binary& meas, uint16_t index);
	void Update(const DoubleBitBinary& meas, uint16_t index);
	void Update(const Analog& meas, uint16_t index);
	void Update(const Counter& meas, uint16_t index);
	void Update(const FrozenCounter& meas, uint16_t index);
	void Update(const BinaryOutputStatus& meas, uint16_t index);
	void Update(const AnalogOutputStatus& meas, uint16_t index);

private:

	template <class T>
	void Load(T meas, uint16_t index)
	{
		meas.time = timestamp.msSinceEpoch;
		pDatabase->Update(meas, index);
	}

	IDatabase* pDatabase;
	openpal::UTCTimestamp timestamp;
};

}

#endif
