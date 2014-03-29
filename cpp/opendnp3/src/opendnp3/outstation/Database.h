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
#ifndef __DATABASE_H_
#define __DATABASE_H_

#include <openpal/StaticList.h>
#include <openpal/IMutex.h>

#include "opendnp3/app/StaticRange.h"
#include "opendnp3/outstation/IEventBuffer.h"
#include "opendnp3/outstation/IMeasurementLoader.h"
#include "opendnp3/outstation/StaticDataFacade.h"

namespace opendnp3
{

struct DeviceTemplate;

/**
Manages the static data model of a DNP3 slave. Dual-interface to update data points and read current values.

Passes data updates to an associated event buffer for event generation/management.
*/
class Database : public IMeasurementLoader
{
public:

	Database(const StaticDataFacade&, openpal::IMutex* pMutex = nullptr);

	/* Functions for obtaining iterators */

	void SetEventBuffer(IEventBuffer& eventBuffer);

	void DoubleBuffer();

	// IMeasurementLoader*functions
	void Update(const Binary& arPoint, uint16_t) final;
	void Update(const Analog& arPoint, uint16_t) final;
	void Update(const Counter& arPoint, uint16_t) final;
	void Update(const FrozenCounter& arPoint, uint16_t) final;
	void Update(const BinaryOutputStatus& arPoint, uint16_t) final;
	void Update(const AnalogOutputStatus& arPoint, uint16_t) final;

	openpal::Indexable<DualValue<Binary>, uint16_t> Binaries();
	openpal::Indexable<DualValue<Analog>, uint16_t> Analogs();
	openpal::Indexable<DualValue<Counter>, uint16_t> Counters();
	openpal::Indexable<DualValue<FrozenCounter>, uint16_t> FrozenCounters();
	openpal::Indexable<DualValue<BinaryOutputStatus>, uint16_t> ControlStatii();
	openpal::Indexable<DualValue<AnalogOutputStatus>, uint16_t> SetpointStatii();

	template <class T>
	openpal::Indexable<DualValue<T>, uint16_t> Values();

	template <class T>
	uint16_t NumValues() const;

	template <class T>
	StaticRange FullRange() const
	{
		uint16_t num = NumValues<T>();
		if(num > 0) return StaticRange(0, num - 1);
		else return StaticRange();
	}

	StaticDataFacade staticData;

private:

	template <class T>
	static void FreezeCollection(openpal::Indexable<DualValue<T>, uint16_t>& collection)
	{
		collection.foreach([](DualValue<T>& value)
		{
			value.Freeze();
		});
	}

	Database();
	Database(const Database&);	

	template <class T, class U>
	inline void UpdateEvent(const T& value, uint32_t index, U& collection)
	{
		if(collection.values.Contains(index))
		{
			auto& metadata = collection.metadata[index];
			EventClass eventClass;
			if (metadata.GetEventClass(eventClass) && metadata.CheckForEvent(value))
			{
				if (pEventBuffer)
				{
					pEventBuffer->Update(Event<T>(value, index, eventClass));
				}				
			}
			collection.values[index].Update(value);
		}
	}

	IEventBuffer* pEventBuffer;

	openpal::IMutex* pMutex;

	// ITransactable  functions, proxies to the given transactable

	virtual void Start() override final;
	virtual void End() override final;
};

}

#endif

