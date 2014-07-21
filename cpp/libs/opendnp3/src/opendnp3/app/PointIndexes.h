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

#ifndef __POINT_INDEXES_H_
#define __POINT_INDEXES_H_

#include <cstdint>
#include <initializer_list>

#include <openpal/container/Indexable.h>
#include <openpal/container/StaticArray.h>

#include <openpal/container/DynamicArray.h>

#include "Range.h"

namespace opendnp3
{

    class PointRange
    {
    public:
        PointRange(uint32_t count=0)
        {
            if (count == 0)
            {
                start = MAX;
                stop = MIN;
                offset = MIN;
            }
            else
            {
                start = 0;
                stop = count - 1;
                offset = 0;
            }
        }
        
        PointRange(uint32_t start_, uint32_t stop_, uint32_t offset_) :
        start(start_),
        stop(stop_),
        offset(offset_)
        { }
        
        uint32_t start;
        uint32_t stop;
        uint32_t offset;
        
        static const uint32_t MIN;
        static const uint32_t MAX;
        static PointRange FULLRANGE;
        static PointRange EMPTYRANGE;
        static const uint32_t OUTOFRANGE;
    };

    class PointIndexes
    {
    public:
        static const PointIndexes FULLINDEXES;
        static const PointIndexes EMPTYINDEXES;
        
        PointIndexes(openpal::Indexable<PointRange, uint32_t> base) :
        ranges(base)
        { }
        
        inline const PointRange ToRange() const
        {
            return {First(), Last(), First()};
        }
        
        void operator=(const PointIndexes& base)
        {
            ranges = base.ranges;
        }
        
        const PointRange& operator[](uint32_t index) const
        {
            assert(index < IndexCount());
            return ranges[index];
        }
        
        inline uint32_t First() const
        {
            if(ranges.Size() == 0) return PointRange::MAX;
            return ranges[0].start;
        }
        
        inline uint32_t Last() const
        {
            if(ranges.Size() == 0) return PointRange::MIN;
            return ranges[LastRange()].stop;
        }
        
        inline uint32_t IndexCount() const
        {
            return IndexCount(First(), Last());
        }
        
        inline uint32_t IndexCount(uint32_t start, uint32_t stop) const
        {
            if(ranges.Size() == 0) return 0;
            if(First() > Last()) return 0;
            
            uint32_t count = 1 + stop - start;
            
            /* delete all the gaps between start and stop */
            for(auto i = 1; i < ranges.Size(); i++)
            {
                if ((start <= ranges[i-1].stop) && (stop >= ranges[i].start))
                {
                    count -= ranges[i].start - ranges[i-1].stop - 1;
                }
            }
            return count;
        }
        
        /* Given a point index, returns a position */
        inline uint32_t GetPosition(uint32_t index) const
        {
            const uint32_t IndexRange = GetRange(index);
            assert(IndexRange != PointRange::OUTOFRANGE);
            assert(index >= ranges[IndexRange].start);
            assert(index <= ranges[IndexRange].stop);
            return index - ranges[IndexRange].offset;
        }
        
        // Returns the range that the index is in OR
        // the range of the next index
        uint32_t GetRange(uint32_t index) const
        {
            uint32_t nRanges = ranges.Size();
            if (nRanges == 0) return PointRange::OUTOFRANGE;
            if (index < ranges[0].start) return 0;
            if (index > ranges[nRanges-1].stop) return PointRange::OUTOFRANGE;
            
            /* Simple binary search */
            uint32_t imin = 0;
            uint32_t imax = ranges.Size() - 1;
            
            while (imax > imin)
            {
                const uint32_t imid = imin + (imax - imin)/2;
                if (index < ranges[imid].start) imax = imid - 1;
                else if (index > ranges[imid].stop) imin = imid + 1;
                else return imid;
            }
            return imin;
        }

        /* Given an index, returns the next valid index (inclusive of the index provided) */
        inline uint32_t GetNextValid(uint32_t index) const
        {
            const uint32_t IndexRange = GetRange(index);
            if (IndexRange == PointRange::OUTOFRANGE) return PointRange::OUTOFRANGE;
            // before the range start
            if (index < ranges[IndexRange].start) return ranges[IndexRange].start;
            // in the range
            if (index <= ranges[IndexRange].stop) return index;
            // past the range end
            assert(IndexRange < LastRange());
            return ranges[IndexRange+1].start;
        }
        
        /* Given an index, returns the previous valid index (inclusive of the index provided) */
        inline uint32_t GetPreviousValid(uint32_t index) const
        {
            const uint32_t IndexRange = GetRange(index);
            if (ranges.Size() == 0) return PointRange::MIN;
            if (IndexRange == PointRange::OUTOFRANGE) return GetStop();
            // past the range end
            if (index > ranges[IndexRange].stop) return ranges[IndexRange].stop;
            // in the range
            if (index >= ranges[IndexRange].start) return index;
            // before the range start
            if (IndexRange > 0) return ranges[IndexRange-1].stop;
            return PointRange::MIN;
        }
        
        /* Returns true if index is contained in a range */
        inline bool Contains(uint32_t index) const
        {
            const uint32_t IndexRange = GetRange(index);
            if (IndexRange == PointRange::OUTOFRANGE) return false;
            if (index < ranges[IndexRange].start) return false;
            if (index > ranges[IndexRange].stop) return false;
            return true;
        }
        
        inline uint32_t GetStart() const
        {
            if (ranges.Size() == 0) return PointRange::MAX;
            return ranges[0].start;
        }
        
        inline uint32_t GetStop() const
        {
            if (ranges.Size() == 0) return PointRange::MIN;
            return ranges[LastRange()].stop;
        }
        
        openpal::Indexable<PointRange, uint32_t> ranges;
        
    private:
        inline uint32_t LastRange() const
        {
            return ranges.Size()-1;
        }
    };
   
    // A statically sized array of point indexes
    template <uint32_t... args>
    class StaticPointIndexes
    {
    public:
        static_assert(sizeof...(args) % 2 == 0, "StaticPointIndexes must have even number of arguments");
        static const uint32_t nranges = sizeof...(args)/2;
        
        StaticPointIndexes()
        {
            const uint32_t startstop[sizeof...(args)] = {args...};
            
            for(uint32_t count = 0; 1 + count < 1 + sizeof...(args) / 2; count++)
            {
                ranges[count].start = startstop[count*2];
                ranges[count].stop = startstop[count*2+1];
                assert(ranges[count].start <= ranges[count].stop);
                if(count>0)
                {
                    assert(ranges[count].start > ranges[count-1].stop);
                    ranges[count].offset = ranges[count-1].offset + (ranges[count].start - ranges[count-1].stop - 1);
                }
                else
                {
                    ranges[count].offset = startstop[count*2];
                }
            }
        }
        
        operator const PointIndexes()
        {
            return ranges.ToIndexable();
        }
        
        openpal::StaticArray<PointRange, uint32_t, nranges> ranges;
    };

    // A dynamically sized array of point indexes
    // All ranges must be pushed to the config before it can be converted to a PointIndexes
    // Once a range is defined, it can not be changes
    class DynamicPointIndexes
    {
    public:
        DynamicPointIndexes(const DynamicPointIndexes& copy) :
        count(copy.count),
        ranges(copy.ranges)
        {
            if(ranges.Size() == 0) return;
        }
        
        DynamicPointIndexes(uint32_t nranges_ = 0) :
        ranges(nranges_),
        count(0)
        {
            
        }
        
        void push(uint32_t start, uint32_t stop)
        {
            assert(count < ranges.Size());
            assert(start <= stop);
            
            if(count>0)
            {
                assert(ranges[count-1].stop < start);
                ranges[count].offset = ranges[count-1].offset + (start - ranges[count-1].stop - 1);
            }
            else
            {
                ranges[count].offset = start;
            }
            
            ranges[count].start = start;
            ranges[count].stop = stop;
            
            count++;
        }
        
        DynamicPointIndexes(openpal::Indexable<uint32_t, uint32_t> points) :
        count(CountRanges(points)),
        ranges(count)
        {
            if(ranges.Size() == 0) return;
            
            uint32_t count = 0;
            uint32_t lastPoint = 0;
            for(uint32_t p = 0; p < points.Size(); ++p)
            {
                if(count)
                {
                    assert(points[p] > lastPoint);
                    lastPoint++;
                    if (points[p] > lastPoint)
                    {
                        ranges[count-1].stop = lastPoint-1;
                        ranges[count].start = points[p];
                        ranges[count].offset = ranges[count-1].offset + (points[p] - lastPoint);
                        count++;
                        lastPoint = points[p];
                    }
                }
                else
                {
                    ranges[count].start = points[p];
                    ranges[count].offset = points[p];
                    lastPoint = points[p];
                    count++;
                }
            }
            ranges[count-1].stop = lastPoint;
        }
        
        DynamicPointIndexes(openpal::Indexable<Range, uint32_t> pointranges) :
        count(pointranges.Size()),
        ranges(count)
        {
            if(ranges.Size() == 0) return;
            
            uint32_t count = 0;
            for(uint32_t p = 0; p < pointranges.Size(); ++p)
            {
                ranges[count].start = pointranges[p].start;
                ranges[count].stop = pointranges[p].stop;
                assert(ranges[count].start <= ranges[count].stop);
                if(count)
                {
                    assert(ranges[count].start > ranges[count-1].stop);
                    ranges[count].offset = ranges[count-1].offset + (ranges[count].start - ranges[count-1].stop - 1);
                }
                else
                {
                    ranges[count].offset = pointranges[p].start;
                }
                count++;
            }
        }
        
        operator PointIndexes()
        {
            assert(count == ranges.Size()); // all ranges not defined
            return ranges.ToIndexable();
        }
        
    private:
        uint32_t CountRanges(openpal::Indexable<uint32_t, uint32_t> points)
        {
            /* count the number of contiguous ranges, ensuring points are increasing */
            uint32_t count = 0;
            uint32_t lastPoint = 0;
            for(uint32_t p = 0; p< points.Size(); ++p)
            {
                if(count)
                {
                    assert(points[p] > lastPoint);
                    lastPoint++;
                    if (points[p] > lastPoint) {
                        lastPoint = points[p];
                        count++;
                    }
                }
                else
                {
                    lastPoint = points[p];
                    count++;
                }
            }
            return count;
        }
        
        uint32_t count;
        openpal::DynamicArray<PointRange, uint32_t> ranges;
    };
    
}

#endif
