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
#ifndef OPENDNP3_RANGE_H
#define OPENDNP3_RANGE_H

#include <openpal/Configure.h>
#include <cstdint>
#include <assert.h>

#include <openpal/util/Comparisons.h>

namespace opendnp3
{

class Range
{
public:

	static Range From(uint16_t start, uint16_t stop)
	{
		return Range(start, stop);
	}

	static Range Invalid()
	{
		return Range(1, 0);
	}

	Range() : start(1), stop(0)
	{}

	Range Intersection(const Range& other) const
	{
		if (other.IsValid() && this->IsValid())
		{			
			return Range(
				openpal::Max<uint16_t>(other.start, start), 
				openpal::Min<uint16_t>(other.stop, stop)
			);
		}
		else
		{
			return Range::Invalid();
		}
	}

	bool Equals(const Range& other) const
	{
		return (other.start == start) && (other.stop == stop);
	}
   	
	uint32_t Count() const
	{
		return (stop - start) + 1;
	}

	bool IsValid() const
	{
		return start <= stop;
	}

	bool IsOneByte() const
	{
		return IsValid() && (start <= 255) && (stop <= 255);
	}
	
	uint16_t start;
	uint16_t stop;

private:

	Range(uint16_t index_) :
		start(index_),
		stop(index_)
	{}

	Range(uint16_t start_, uint16_t stop_) :
		start(start_),
		stop(stop_)
	{}
};

}

#endif
