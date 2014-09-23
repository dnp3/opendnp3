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

namespace opendnp3
{

class Range
{
public:

	// TODO - Remove this?
	static Range FromCount(uint16_t count)
	{
		assert(count > 0);
		return Range(0, count - 1);
	}
    
	Range(uint16_t index_) :
        start(index_),
        stop(index_)
	{}

	Range(uint16_t start_, uint16_t stop_) :
		start(start_),
		stop(stop_)
	{}

	uint32_t Count() const
	{
		return (stop - start) + 1;
	}

	Range() : start(0), stop(0)
	{}

	uint16_t start;
	uint16_t stop;
};

}

#endif
