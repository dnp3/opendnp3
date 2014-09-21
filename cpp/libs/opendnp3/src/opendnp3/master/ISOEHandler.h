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
#include "opendnp3/app/IterableBuffer.h"
#include "opendnp3/app/IndexedValue.h"
#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/app/OctetString.h"
#include "opendnp3/app/GroupVariationRecord.h"
#include "opendnp3/gen/TimestampMode.h"

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

	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) = 0;
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) = 0;
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) = 0;
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) = 0;
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) = 0;
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) = 0;
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) = 0;
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) = 0;

	virtual ~ISOEHandler() {}
};


class NullSOEHandler : public ISOEHandler
{

public:

	static ISOEHandler& Instance()
	{
		return instance;
	}

	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final {}
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final {}
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final {}
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)  override final {}
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)  override final {}
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)  override final {}
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)  override final {}
	virtual void OnReceiveHeader(const HeaderRecord& header, TimestampMode tsmode, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)  override final {}

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
