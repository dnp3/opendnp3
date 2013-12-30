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
#ifndef __RESPONSE_CONTEXT_H_
#define __RESPONSE_CONTEXT_H_

#include <opendnp3/Uncopyable.h>

#include <openpal/QueueAdapter.h>

#include "StaticSizeConfiguration.h"
#include "StaticRange.h"
#include "APDUResponse.h"
#include "ObjectWriter.h"
#include "Database.h"
#include "ResponseHelpers.h"

namespace opendnp3
{

enum class QueueResult
{
	SUCCESS,
	FULL,
	OBJECT_UNDEFINED,
	OUT_OF_RANGE
};

/**
 * Builds and tracks the state of multi-fragmented responses to READ requests,
 * coordinating the static database and event buffer
 */
class ResponseContext : private Uncopyable
{			
	typedef LoadResult (*LoadFun)(ObjectWriter& writer, StaticRange& range, Database& db);

	class FunctionalStaticRange: public StaticRange
	{
		public:

		FunctionalStaticRange(const StaticRange& sr) : StaticRange(sr), pLoadFun(nullptr) {}
		FunctionalStaticRange() : StaticRange(), pLoadFun(nullptr) {}
		FunctionalStaticRange(LoadFun pLoadFun_): StaticRange(), pLoadFun(pLoadFun_) {}
		LoadFun pLoadFun;
	};

	public:

	ResponseContext(Database*);

	void Reset();

	QueueResult QueueReadAllObjects(GroupVariation gv);
	QueueResult QueueReadRange(GroupVariation gv, const StaticRange& range);
	
	
	LoadResult Load(ObjectWriter& writer);
	
	private:

	LoadResult LoadStaticData(ObjectWriter& writer);
	
	bool first;
	Database* pDatabase;

	openpal::StaticArray<FunctionalStaticRange, uint8_t, SizeConfiguration::MAX_READ_REQUESTS> staticRangeArray;
	openpal::QueueAdapter<FunctionalStaticRange, uint8_t> staticResponseQueue;	

	template <class T, class U>
	QueueResult QueueRange(const StaticRange& rng);
};

template <class T, class U>
QueueResult ResponseContext::QueueRange(const StaticRange& rng)
{	
	if(rng.IsContainedBy(pDatabase->NumValues<typename T::Target>()))
	{
		FunctionalStaticRange range(rng);
		if(range.IsContainedByUInt8())
		{
			range.pLoadFun = &LoadFixedSizeStartStop<T, U, UInt8, QualifierCode::UINT8_START_STOP>;
			return staticResponseQueue.Push(range) ? QueueResult::SUCCESS : QueueResult::FULL;
		}
		else
		{			
			range.pLoadFun = &LoadFixedSizeStartStop<T, U, UInt16, QualifierCode::UINT16_START_STOP>;
			return staticResponseQueue.Push(range) ? QueueResult::SUCCESS : QueueResult::FULL;
		}
	}
	else
	{
		return QueueResult::OUT_OF_RANGE;		
	}
}

}

#endif
