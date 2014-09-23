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
#ifndef OPENDNP3_STATICRESPONSECONTEXT_H
#define OPENDNP3_STATICRESPONSECONTEXT_H

#include <openpal/container/Queue.h>
#include <openpal/serialization/Serialization.h>

#include <openpal/util/Uncopyable.h>

#include "opendnp3/app/StaticRange.h"
#include "opendnp3/app/APDUResponse.h"

#include "opendnp3/outstation/Database.h"
#include "opendnp3/outstation/StaticLoader.h"
#include "opendnp3/outstation/StaticResponseConfig.h"
#include "opendnp3/outstation/StaticLoadFunctions.h"

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

	StaticResponseContext(Database& database, const StaticResponseConfig& config);

	void Reset();

	IINField ReadAll(const GroupVariationRecord& record);
	IINField ReadRange(const GroupVariationRecord& record, const Range& range);

	bool IsComplete() const;

	bool HasRequests() const;

	bool Load(HeaderWriter& writer);

private:

	IINField QueueLoader(const StaticRangeLoader& loader);	

	StaticLoadResult LoadStaticData(HeaderWriter& writer);

	Database* pDatabase;
	StaticResponseConfig defaults;

	openpal::Queue<StaticRangeLoader, uint8_t> staticResponseQueue;

	template <class Target>
	IINField QueueRange(const Range& range, typename Target::StaticResponseEnum enumeration)
	{
        StaticRange copy(pDatabase->FullRange<Target>());
        copy.ClipTo(range);
		return QueueLoader(StaticRangeLoader(StaticLoadFunctions::Get(enumeration), copy));
	}	

	template <class Target>
	IINField QueueRange(typename Target::StaticResponseEnum enumeration)
	{		
		return QueueLoader(StaticRangeLoader(StaticLoadFunctions::Get(enumeration), pDatabase->FullRange<Target>()));
	}
};

}

#endif
