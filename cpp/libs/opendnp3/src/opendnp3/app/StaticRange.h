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

#ifndef OPENDNP3_STATICRANGE_H
#define OPENDNP3_STATICRANGE_H

#include <cstdint>
#include "GroupVariationRecord.h"
#include "opendnp3/app/PointIndexes.h"

namespace opendnp3
{

class StaticRange
{
public:

	StaticRange();
    StaticRange(const PointIndexes* points_);
    
	void ClipTo(const Range& borders);

	bool IsClipped() const
	{
		return clipped;
	}

	bool IsContainedBy(uint16_t size) const;

	bool IsContainedByUInt8() const;

	inline bool IsDefined() const
	{
		return start <= stop;
	}

	bool Advance();
    
    inline Range ToRange() const
    {
        return Range(start, stop);
    }

	uint16_t start;
    uint16_t position;

private:

    const PointIndexes* indexes;
	uint16_t stop;
    uint16_t range;
	bool clipped;

	static const uint16_t MIN;
	static const uint16_t MAX;
};

}

#endif
