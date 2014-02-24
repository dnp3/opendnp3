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
#include "ResponseHelpers.h"
#include "CountOf.h"
#include "SelectionCriteria.h"

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
	typedef LoadResult (*LoadFun)(GroupVariationID gv, ObjectWriter& writer, StaticRange& range, Database& db);

	class StaticRangeLoader: public StaticRange
	{
		public:
		
		StaticRangeLoader() : pLoadFun(nullptr) {}
		StaticRangeLoader(LoadFun pLoadFun_, GroupVariationID groupVar_, const StaticRange& rng): StaticRange(rng), groupVar(groupVar_), pLoadFun(pLoadFun_) {}

		GroupVariationID groupVar;
		LoadFun pLoadFun;
	};
	
	struct RangeGroupVariation
	{
		RangeGroupVariation(StaticRange range_, GroupVariation groupVariation_) : range(range_), groupVariation(groupVariation_)
		{}

		RangeGroupVariation() : range(), groupVariation(GroupVariation::UNKNOWN)
		{}

		StaticRange range;
		GroupVariation groupVariation;
	};

	public:

	ResponseContext(Database*);

	void Reset();

	QueueResult QueueReadAllObjects(GroupVariation gv);
	QueueResult QueueStaticIntegrity();
	QueueResult QueueReadRange(GroupVariation gv, const StaticRange& range);

	bool IsComplete() const;

	bool IsStaticRequest() const;
		
	LoadResult Load(APDUResponse& response);
	
	private:

	static AppControlField GetAppControl(uint32_t headerCount, LoadResult result);

	LoadResult LoadStaticData(ObjectWriter& writer);
	
	uint32_t fragmentCount;
	bool isStaticRequest;
	Database* pDatabase;
	
	openpal::StaticQueue<StaticRangeLoader, uint8_t, SizeConfiguration::MAX_READ_REQUESTS> staticResponseQueue;

	template <class T, class U>
	QueueResult QueueRange(GroupVariationID gv, const StaticRange& rng);
};

template <class Target, class Serializer>
QueueResult ResponseContext::QueueRange(GroupVariationID gv, const StaticRange& rng)
{		
	if(rng.IsContainedBy(pDatabase->NumValues<Target>()))
	{
		this->isStaticRequest = true;
		
		if(rng.IsContainedByUInt8())
		{
			auto fun = &LoadFixedSizeStartStop<Target, Serializer, openpal::UInt8, QualifierCode::UINT8_START_STOP>;
			StaticRangeLoader loader(fun, gv, rng);
			return staticResponseQueue.Push(loader) ? QueueResult::SUCCESS : QueueResult::FULL;
		}
		else
		{			
			auto fun = &LoadFixedSizeStartStop<Target, Serializer, openpal::UInt16, QualifierCode::UINT16_START_STOP>;
			StaticRangeLoader loader(fun, gv, rng);
			return staticResponseQueue.Push(loader) ? QueueResult::SUCCESS : QueueResult::FULL;
		}
	}
	else
	{
		return QueueResult::OUT_OF_RANGE;		
	}
}

}

#endif
