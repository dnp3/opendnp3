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

#ifndef __TRACKING_HANDLER_H_
#define __TRACKING_HANDLER_H_

#include <opendnp3/IAPDUHeaderHandler.h>

namespace opendnp3 {

class TrackingHandler : public IAPDUHeaderHandler
{
	public:

	uint64_t count;

	TrackingHandler() : count(0) {}

	virtual void AllObjects(GroupVariation gv, const openpal::ReadOnlyBuffer& header) override
	{
		++count;
	}

	virtual void OnCountOf(const IterableBuffer<Group52Var2>& times) override
	{
		times.foreach([this](const Group52Var2& v) { ++count; });
	}

	virtual void OnIIN(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<bool>>& bits) override
	{
		bits.foreach([this](const IndexedValue<bool>& v) { ++count; });
	}

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<Binary>>& meas) override
	{
		meas.foreach([this](const IndexedValue<Binary>& v) { ++count; });
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<Binary>>& meas) override
	{
		meas.foreach([this](const IndexedValue<Binary>& v){ ++count; });
	}

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<ControlStatus>>& meas) override
	{
		meas.foreach([this](const IndexedValue<ControlStatus>& v) { ++count; });
	}

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<Counter>>& meas) override
	{
		meas.foreach([this](const IndexedValue<Counter>& v) { ++count; });
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<Counter>>& meas) override
	{
		meas.foreach([this](const IndexedValue<Counter>& v) { ++count; });
	}

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<Analog>>& meas) override
	{
		meas.foreach([this](const IndexedValue<Analog>& v) { ++count; });
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<Analog>>& meas) override
	{
		meas.foreach([this](const IndexedValue<Analog>& v) { ++count; });
	}

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<SetpointStatus>>& meas) override
	{
		meas.foreach([this](const IndexedValue<SetpointStatus>& v) { ++count; });
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<ControlRelayOutputBlock>>& meas) override
	{
		meas.foreach([this](const IndexedValue<ControlRelayOutputBlock>& v) { ++count; });
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputInt16>>& meas) override
	{
		meas.foreach([this](const IndexedValue<AnalogOutputInt16>& v) { ++count;} );
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputInt32>>& meas) override
	{
		meas.foreach([this](const IndexedValue<AnalogOutputInt32>& v) { ++count; });
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputFloat32>>& meas) override
	{
		meas.foreach([this](const IndexedValue<AnalogOutputFloat32>& v) { ++count; });
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputDouble64>>& meas) override
	{
		meas.foreach([this](const IndexedValue<AnalogOutputDouble64>& v) {++count;} );
	}

	virtual void OnIndexPrefixOfOctets(GroupVariation, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<openpal::ReadOnlyBuffer>>& meas) override
	{
		meas.foreach([this](const IndexedValue<openpal::ReadOnlyBuffer>& v) {++count; });
	}

	virtual void OnRangeOfOctets(GroupVariation, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<openpal::ReadOnlyBuffer>>& meas) override
	{
		meas.foreach([this](const IndexedValue<openpal::ReadOnlyBuffer>& v) { ++count; });
	}

};

}

#endif
