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

#ifndef __DYNAMIC_POINT_INDEXES_H_
#define __DYNAMIC_POINT_INDEXES_H_

#include <cstdint>
#include <initializer_list>

#include <openpal/container/Indexable.h>
#include <openpal/container/DynamicArray.h>

#include "PointIndexes.h"

namespace opendnp3
{
    // A dynamically sized array of point indexes
    class DynamicPointIndexes
    {
    public:
        DynamicPointIndexes(std::initializer_list<Range> points) :
        ranges(PointIndexes::CountRanges(points))
        {
            PointIndexes pi(ranges.ToIndexable());
            pi.SetRanges(points);
        }
        
        DynamicPointIndexes(openpal::Indexable<uint32_t, uint32_t> points) :
        ranges(PointIndexes::CountRanges(points))
        {
            PointIndexes pi(ranges.ToIndexable());
            pi.SetRanges(points);
        }
        
        DynamicPointIndexes(openpal::Indexable<Range, uint32_t> pointranges) :
        ranges(pointranges.Size())
        {
            PointIndexes pi(ranges.ToIndexable());
            pi.SetRanges(pointranges);
        }
        
        DynamicPointIndexes(PointIndexes pointranges) :
        ranges(pointranges.ranges.Size())
        {
            PointIndexes pi(ranges.ToIndexable());
            pi.SetRanges(pointranges);
        }
        
        operator PointIndexes()
        {
            return ranges.ToIndexable();
        }
        
    private:
        openpal::DynamicArray<PointRange, uint32_t> ranges;
    };
    
}

#endif
