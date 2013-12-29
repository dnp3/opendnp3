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

#include <openpal/Loggable.h>
#include <openpal/StaticList.h>
#include <opendnp3/IDataObserver.h>

#include "IEventBuffer.h"
#include "StaticDataFacade.h"
#include "StaticSizeConfiguration.h"

namespace opendnp3
{

struct DeviceTemplate;

/**
Manages the static data model of a DNP3 slave. Dual-interface to update data points and read current values.

Passes data updates to an associated event buffer for event generation/management.
*/
class Database : public openpal::Loggable, public IDataObserver
{
public:

	Database(openpal::Logger, const StaticDataFacade&);	
	
	/* Functions for obtaining iterators */

	bool AddEventBuffer(IEventBuffer* apEventBuffer);	

	// IDataObserver functions
	void Update(const Binary& arPoint, uint16_t) final;
	void Update(const Analog& arPoint, uint16_t) final;
	void Update(const Counter& arPoint, uint16_t) final;
	void Update(const ControlStatus& arPoint, uint16_t) final;
	void Update(const SetpointStatus& arPoint, uint16_t) final;

	openpal::Indexable<Binary, uint16_t> Binaries();
	openpal::Indexable<Analog, uint16_t> Analogs();
	openpal::Indexable<Counter, uint16_t> Counters();
	openpal::Indexable<ControlStatus, uint16_t> ControlStatii();
	openpal::Indexable<SetpointStatus, uint16_t> SetpointStatii();

	template <class T> 
	openpal::Indexable<T, uint16_t> Values();

	StaticDataFacade staticData;

private:

	Database();
	Database(const Database&);

	template <class T>
	void UpdateEventBuffer(const T& value, uint32_t index, EventClass clazz)
	{
		eventBuffers.foreach([&](IEventBuffer* pBuffer) { pBuffer->Update(Event<T>(value, index, clazz)); });
	}

	template <class T, class U>
	inline void UpdateEvent(const T& value, uint32_t index, U& collection)
	{		
		if(collection.values.Contains(index))
		{	
			auto& metadata = collection.metadata[index];
			EventClass eventClass;
			if(metadata.CheckForEvent(value) && metadata.GetEventClass(eventClass))
			{			
				this->UpdateEventBuffer(value, index, eventClass);
			}
			collection.values[index] = value;
		}	
	}

	openpal::StaticList<IEventBuffer*, uint16_t, SizeConfiguration::MAX_EVENT_BUFFERS> eventBuffers;

	// ITransactable  functions, no lock on this structure.
	void Start() final {}
	void End() final {}	
};

}

#endif

