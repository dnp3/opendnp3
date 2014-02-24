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

#include <openpal/StaticQueue.h>
#include <openpal/Serialization.h>

#include "StaticSizeConfiguration.h"
#include "StaticRange.h"
#include "APDUResponse.h"
#include "ObjectWriter.h"
#include "Database.h"
#include "StaticLoader.h"
#include "CountOf.h"
#include "SelectionCriteria.h"
#include "StaticResponseTypes.h"

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
	class StaticRangeLoader: public StaticRange
	{
		public:
		
		StaticRangeLoader() : pLoadFun(nullptr) {}
		StaticRangeLoader(StaticLoadFun pLoadFun_, const StaticRange& rng): StaticRange(rng), pLoadFun(pLoadFun_) {}
		
		StaticLoadFun pLoadFun;
	};
	

	public:

	ResponseContext(Database*, const StaticResponseTypes& rspTypes = StaticResponseTypes());

	void Reset();

	QueueResult QueueReadAllObjects(GroupVariation gv);
	QueueResult QueueReadRange(GroupVariation gv, const StaticRange& range);

	bool IsComplete() const;	
		
	StaticLoadResult Load(APDUResponse& response);
	
	private:

	QueueResult QueueReadRange(const StaticRangeLoader& loader);

	QueueResult QueueStaticIntegrity();

	static AppControlField GetAppControl(uint32_t headerCount, StaticLoadResult result);
	
	template <class T>
	StaticRangeLoader GetFullRangeWithDefaultLoader();

	template <class Serializer>
	StaticRangeLoader GetFullRange();

	StaticLoadResult LoadStaticData(ObjectWriter& writer);
	
	uint32_t fragmentCount;
	Database* pDatabase;
	StaticResponseTypes rspTypes;
	
	openpal::StaticQueue<StaticRangeLoader, uint8_t, SizeConfiguration::MAX_READ_REQUESTS> staticResponseQueue;

};

template <class T>
ResponseContext::StaticRangeLoader ResponseContext::GetFullRangeWithDefaultLoader()
{
	return StaticRangeLoader(rspTypes.GetLoader<T>(), pDatabase->FullRange<T>());
}

template <class Serializer>
ResponseContext::StaticRangeLoader ResponseContext::GetFullRange()
{
	return StaticRangeLoader(StaticLoader::GetLoadFunction<Serializer>(), pDatabase->FullRange<typename Serializer::Target>());
}

}

#endif
