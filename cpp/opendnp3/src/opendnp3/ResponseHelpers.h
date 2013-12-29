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
#ifndef __RESPONSE_HELPERS_
#define __RESPONSE_HELPERS_

#include "Database.h"
#include "StaticRange.h"
#include "APDUResponse.h"
#include "ObjectWriter.h"

namespace opendnp3
{

enum class LoadResult
{
	EMPTY,		// nothing was loaded because the response context is empty
	COMPLETED,	// at least 1 event was loaded and the response context is empty
	FULL		// events were loaded and the APDU is full, context is not empty
};

template <class T, class Converter, class IndexType, QualifierCode Qualifier>
LoadResult LoadFixedSizeStartStop(ObjectWriter& writer, StaticRange& range, Database& db, APDUResponse& rsp)
{
	auto values = db.Values<typename T::Target>();			
	auto iter = writer.IterateOverRange<IndexType, T>(Qualifier, static_cast<typename IndexType::Type>(range.start));			
	while(range.IsDefined())
	{
		if(iter.Write(Converter::Apply(values[range.start])))
		{
			range.Advance();
		}
		else 
		{
			iter.Complete();
			return LoadResult::FULL;	
		}
	}
	iter.Complete();	
	return LoadResult::COMPLETED;
}

}

#endif
