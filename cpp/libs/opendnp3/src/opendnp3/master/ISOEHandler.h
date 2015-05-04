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
#ifndef OPENDNP3_ISOEHANDLER_H
#define OPENDNP3_ISOEHANDLER_H

#include "opendnp3/app/ITransactable.h"

#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/app/TimeAndInterval.h"
#include "opendnp3/app/BinaryCommandEvent.h"
#include "opendnp3/app/AnalogCommandEvent.h"
#include "opendnp3/app/OctetString.h"
#include "opendnp3/app/SecurityStat.h"

#include "opendnp3/master/HeaderInfo.h"

namespace opendnp3
{

/**
* An interface for Sequence-Of-Events (SOE) callbacks from a master stack to
* the application layer.
*
* A call is made to the appropriate member method for every header in a APDU
*
*/
class ISOEHandler : public ITransactable
{
public:
	
	virtual void OnValue(const HeaderInfo& info, const Binary& meas, uint16_t index) = 0;
	virtual void OnValue(const HeaderInfo& info, const DoubleBitBinary& meas, uint16_t index) = 0;
	virtual void OnValue(const HeaderInfo& info, const Analog& meas, uint16_t index) = 0;
	virtual void OnValue(const HeaderInfo& info, const Counter& meas, uint16_t index) = 0;
	virtual void OnValue(const HeaderInfo& info, const FrozenCounter& meas, uint16_t index) = 0;
	virtual void OnValue(const HeaderInfo& info, const BinaryOutputStatus& meas, uint16_t index) = 0;
	virtual void OnValue(const HeaderInfo& info, const AnalogOutputStatus& meas, uint16_t index) = 0;
	virtual void OnValue(const HeaderInfo& info, const OctetString& meas, uint16_t index) = 0;
	virtual void OnValue(const HeaderInfo& info, const TimeAndInterval& meas, uint16_t index) = 0;
	virtual void OnValue(const HeaderInfo& info, const BinaryCommandEvent& meas, uint16_t index) = 0;
	virtual void OnValue(const HeaderInfo& info, const AnalogCommandEvent& meas, uint16_t index) = 0;
	virtual void OnValue(const HeaderInfo& info, const SecurityStat& meas, uint16_t index) = 0;

	virtual ~ISOEHandler() {}
};


class NullSOEHandler : public ISOEHandler
{

public:

	static ISOEHandler& Instance()
	{
		return instance;
	}

	virtual void OnValue(const HeaderInfo& info, const Binary& meas, uint16_t index) override final {}
	virtual void OnValue(const HeaderInfo& info, const DoubleBitBinary& meas, uint16_t index) override final {}
	virtual void OnValue(const HeaderInfo& info, const Analog& meas, uint16_t index) override final {}
	virtual void OnValue(const HeaderInfo& info, const Counter& meas, uint16_t index) override final {}
	virtual void OnValue(const HeaderInfo& info, const FrozenCounter& meas, uint16_t index) override final {}
	virtual void OnValue(const HeaderInfo& info, const BinaryOutputStatus& meas, uint16_t index) override final {}
	virtual void OnValue(const HeaderInfo& info, const AnalogOutputStatus& meas, uint16_t index) override final {}
	virtual void OnValue(const HeaderInfo& info, const OctetString& meas, uint16_t index) override final {}
	virtual void OnValue(const HeaderInfo& info, const TimeAndInterval& meas, uint16_t index) override final {}
	virtual void OnValue(const HeaderInfo& info, const BinaryCommandEvent& meas, uint16_t index) override final {}
	virtual void OnValue(const HeaderInfo& info, const AnalogCommandEvent& meas, uint16_t index) override final {}
	virtual void OnValue(const HeaderInfo& info, const SecurityStat& meas, uint16_t index) override final {}

protected:

	void Start() override final {}
	void End() override final {}

private:
	NullSOEHandler()
	{}

	static NullSOEHandler instance;
};

}

#endif
