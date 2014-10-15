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

#include "StaticRange.h"

#include <openpal/util/Limits.h>

namespace opendnp3
{

const uint16_t StaticRange::MIN(openpal::MinValue<uint16_t>());
const uint16_t StaticRange::MAX(openpal::MaxValue<uint16_t>());

StaticRange::StaticRange() :
    start(MAX),
    stop(MIN),
	position(0),
    indexes(&PointIndexes::EMPTYINDEXES),    
    range(0),
    clipped(false)
{}

StaticRange::StaticRange(const PointIndexes* points_) :    
    start(points_->First()),
    stop(points_->Last()),
    position(0),
	indexes(points_),
    range(0),
    clipped(false)
{}
    
bool StaticRange::IsContainedByUInt8() const
{
	return (stop <= 255) && (start <= 255);
}

void StaticRange::ClipTo(const Range& borders)
{
	auto maxStart = indexes->GetNextValid(borders.start);
	auto minStop = indexes->GetPreviousValid(borders.stop);

	if(!clipped)
	{
		clipped = !(indexes->IsContiguous(borders.start, borders.stop));
	}

	start = maxStart;
	stop = minStop;
    if(start <= stop)
    {
        range = indexes->GetRange(start);
        position = indexes->GetPosition(start);
    }
}

bool StaticRange::IsContainedBy(uint16_t size) const
{
	return IsDefined() && (start < size) && (stop < size);
}

bool StaticRange::Advance()
{
	if(start < stop)
	{
		++start;
		++position;
        if(start > indexes->ranges[range].stop)
        {
            // end of contiguous range of points
            if(++range < indexes->ranges.Size())
            {
                // move to next range of points
                start = indexes->ranges[range].start;
                return false;
            }
            else
            {
                // at the end of the points
                position = MAX;
                start = MAX;
                stop = MIN;
                return true;
            }
        }
	}
	else
	{
        position = MAX;
		start = MAX;
		stop = MIN;
	}
    return true;
}

}
