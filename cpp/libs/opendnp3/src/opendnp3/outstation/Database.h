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
#ifndef OPENDNP3_DATABASE_H
#define OPENDNP3_DATABASE_H

#include <openpal/executor/IMutex.h>

#include "opendnp3/app/Range.h"
#include "opendnp3/gen/AssignClassType.h"

#include "opendnp3/outstation/DatabaseTemplate.h"
#include "opendnp3/outstation/StaticBuffers.h"
#include "opendnp3/outstation/StaticSelection.h"
#include "opendnp3/outstation/IStaticSelector.h"
#include "opendnp3/outstation/IDatabase.h"
#include "opendnp3/outstation/IEventReceiver.h"

namespace opendnp3
{

class INewEventDataHandler
{
public:
	// called when a transaction produces new event data
	virtual void OnNewEventData() = 0;
};

/**
The database coordinates all updates of measurement data
*/
class Database : public IDatabase, public IStaticSelector
{
public:

	Database(const DatabaseTemplate&, IEventReceiver& eventReceiver, INewEventDataHandler& handler, openpal::IMutex* pMutex);

	// ------- IDatabase --------------
	virtual bool Update(const Binary& value, uint16_t) override final;
	virtual bool Update(const DoubleBitBinary& value, uint16_t) override final;
	virtual bool Update(const Analog& value, uint16_t) override final;
	virtual bool Update(const Counter& value, uint16_t) override final;
	virtual bool Update(const FrozenCounter& value, uint16_t) override final;
	virtual bool Update(const BinaryOutputStatus& value, uint16_t) override final;
	virtual bool Update(const AnalogOutputStatus& value, uint16_t) override final;
	virtual bool Update(const TimeAndInterval& value, uint16_t) override final;

	// ------- IStaticSelector -------------

	virtual IINField SelectAll(const GroupVariationRecord& record) override final;
	virtual IINField SelectRange(const GroupVariationRecord& record, const Range& range) override final;

	// -------- Misc ---------------

	
	StaticBufferView GetStaticView() { return staticBuffers.GetView(); }	

private:

	template <class T>
	IINField SelectRange(const Range& range);

	// stores the most revent values and event information
	StaticBuffers staticBuffers;
	StaticSelection staticSelection;

	IEventReceiver* pEventReceiver;
	openpal::IMutex* pMutex;
		
	INewEventDataHandler* pEventHandler;
	bool transactionHasEvents;

	Database() = delete;
	Database(const Database&) = delete;

	static bool ConvertToEventClass(PointClass pc, EventClass& ec);	

	template <class T>
	bool UpdateEvent(const T& value, uint16_t index);

	static Range RangeOf(const openpal::HasSize<uint16_t>& sized);

	template <class T>
	IINField GenericSelect(
		const Range& range,
		const openpal::ArrayView<MeasurementCell<T>, uint16_t>& current,
		openpal::ArrayView<BufferedCell<T>, uint16_t>& frozen,	
		bool useDefault,
		typename T::StaticVariation variation
	);

	template <class T>
	IINField SelectAll(		
		bool useDefault = true,
		typename T::StaticVariation variation = T::StaticVariation()
		)
	{
		auto current = staticBuffers.GetArrayView<T>();
		auto frozen = staticSelection.GetArrayView<T>();
		return GenericSelect(RangeOf(current), current, frozen, useDefault, variation);
	}

	// ITransactable  functions, proxies to the given transactable
	virtual void Start() override final;
	virtual void End() override final;
};

template <class T>
bool Database::UpdateEvent(const T& value, uint16_t index)
{	
	auto values = staticBuffers.GetArrayView<T>();

	if (values.Contains(index))
	{
		auto& metadata = values[index].metadata;

		EventClass ec;
		if (ConvertToEventClass(metadata.clazz, ec) && metadata.IsEvent(value))
		{
			metadata.lastEvent = value;

			if (pEventReceiver)
			{
				pEventReceiver->Update(Event<T>(value, index, ec));
				transactionHasEvents = true;
			}
		}

		values[index].value = value;
		return true;
	}
	else
	{
		return false;
	}
}

template <class T>
IINField Database::GenericSelect(
	const Range& range,
	const openpal::ArrayView<MeasurementCell<T>, uint16_t>& current,
	openpal::ArrayView<BufferedCell<T>, uint16_t>& selected,
	bool useDefault,
	typename T::StaticVariation variation)
{
	if (range.IsValid())
	{		
		auto allowed = range.Intersection(RangeOf(current));

		if (allowed.IsValid())
		{
			IINField ret;

			for (uint16_t i = allowed.start; i < allowed.stop; ++i)
			{
				if (selected[i].selected)
				{
					ret |= IINBit::PARAM_ERROR;
				}
				else
				{
					selected[i].selected = true;
					selected[i].value = current[i].value;
					selected[i].variation = useDefault ? current[i].variation : variation;
				}				
			}

			return ret;
		}
		else
		{
			return IINField(IINBit::PARAM_ERROR);
		}
	}
	else
	{
		return IINField();
	}
}

}

#endif

