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
#ifndef __STATIC_RESPONSE_CONTEXT_H_
#define __STATIC_RESPONSE_CONTEXT_H_

#include <openpal/container/StaticQueue.h>
#include <openpal/serialization/Serialization.h>

#include <openpal/util/Uncopyable.h>
#include "opendnp3/Configure.h"

#include "opendnp3/app/StaticRange.h"
#include "opendnp3/app/APDUResponse.h"

#include "opendnp3/outstation/Database.h"
#include "opendnp3/outstation/StaticLoader.h"
#include "opendnp3/outstation/StaticResponseTypes.h"

namespace opendnp3
{

/**
 * Builds and tracks the state of multi-fragmented static responses to READ requests,
 * coordinating with the database.
 */
class StaticResponseContext : private openpal::Uncopyable
{
	class StaticRangeLoader: public StaticRange
	{
	public:

		StaticRangeLoader() : pLoadFun(nullptr) {}
		StaticRangeLoader(StaticLoadFun pLoadFun_, const StaticRange& rng): StaticRange(rng), pLoadFun(pLoadFun_) {}

		StaticLoadFun pLoadFun;
	};


public:

	StaticResponseContext(Database*, const StaticResponseTypes& rspTypes = StaticResponseTypes());

	void Reset();

	IINField ReadAll(const GroupVariationRecord& record);
	IINField ReadRange(const GroupVariationRecord& record, const StaticRange& range);

	bool IsComplete() const;

	bool HasRequests() const;

	bool Load(ObjectWriter& writer);

private:

	IINField QueueReadRange(const StaticRangeLoader& loader);

	IINField QueueStaticIntegrity();

	template <class T>
	StaticRangeLoader GetFullRangeWithDefaultLoader();

	template <class T>
	StaticRangeLoader GetClippedRangeWithDefaultLoader(const StaticRange& range);

	template <class Serializer>
	StaticRangeLoader GetFullRange();

	template <class Serializer>
	StaticRangeLoader GetClippedRange(const StaticRange& range);

	StaticLoadResult LoadStaticData(ObjectWriter& writer);

	Database* pDatabase;
	StaticResponseTypes rspTypes;

	openpal::StaticQueue<StaticRangeLoader, uint8_t, sizes::MAX_READ_REQUESTS> staticResponseQueue;

};

template <class T>
StaticResponseContext::StaticRangeLoader StaticResponseContext::GetClippedRangeWithDefaultLoader(const StaticRange& range)
{
	StaticRange copy(range);
	copy.ClipTo(pDatabase->FullRange<T>());
	return StaticRangeLoader(rspTypes.GetLoader<T>(), copy);
}


template <class T>
StaticResponseContext::StaticRangeLoader StaticResponseContext::GetFullRangeWithDefaultLoader()
{
	return StaticRangeLoader(rspTypes.GetLoader<T>(), pDatabase->FullRange<T>());
}

template <class Serializer>
StaticResponseContext::StaticRangeLoader StaticResponseContext::GetFullRange()
{
	return StaticRangeLoader(StaticLoader::GetLoadFunction<Serializer>(), pDatabase->FullRange<typename Serializer::Target>());
}

template <class Serializer>
StaticResponseContext::StaticRangeLoader StaticResponseContext::GetClippedRange(const StaticRange& range)
{
	StaticRange copy(range);
	copy.ClipTo(pDatabase->FullRange<typename Serializer::Target>());
	return StaticRangeLoader(StaticLoader::GetLoadFunction<Serializer>(), copy);
}

}

#endif
