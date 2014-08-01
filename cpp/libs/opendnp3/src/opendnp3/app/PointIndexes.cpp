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

#include "PointIndexes.h"

#include <openpal/util/Limits.h>

namespace opendnp3
{
    const uint32_t PointRange::MIN(openpal::MinValue<uint16_t>());
    const uint32_t PointRange::MAX(openpal::MaxValue<uint16_t>());
    const uint32_t PointRange::OUTOFRANGE(openpal::MaxValue<uint32_t>());
    PointRange PointRange::FULLRANGE = { PointRange::MIN, PointRange::MAX, PointRange::MIN };
    PointRange PointRange::EMPTYRANGE = { PointRange::MAX, PointRange::MIN, PointRange::MIN };

    const PointIndexes PointIndexes::FULLINDEXES(openpal::Indexable<PointRange, uint32_t>(&PointRange::FULLRANGE,1));
    const PointIndexes PointIndexes::EMPTYINDEXES(openpal::Indexable<PointRange, uint32_t>(&PointRange::EMPTYRANGE,0));
    
    void PointIndexes::SetRanges(std::initializer_list<Range> points)
    {
        assert(ranges.Size() == CountRanges(points));
        if(ranges.Size() == 0) return;
        
        uint32_t count = 0;
        uint32_t lastPoint = 0;
        for(auto p : points)
        {
            assert(p.start <= p.stop);
            if(count > 0)
            {
                assert(p.start > lastPoint);
                if (p.start > lastPoint + 1)
                {
                    ranges[count].start = p.start;
                    ranges[count].stop = p.stop;
                    ranges[count].offset = p.start - lastPoint + ranges[count-1].offset;
                    count++;
                }
                else
                {
                    ranges[count-1].stop = p.stop;
                }
                lastPoint = p.stop;
            }
            else
            {
                ranges[count].start = p.start;
                ranges[count].stop = p.stop;
                ranges[count].offset = p.start;
                count++;
                lastPoint = p.stop;
            }
        }
    }
    /*
    void PointIndexes::SetRanges(openpal::Indexable<uint32_t, uint32_t>& points)
    {
        assert(ranges.Size() == CountRanges(points));
        if(ranges.Size() == 0) return;
        
        uint32_t count = 0;
        uint32_t lastPoint = 0;
        for(uint32_t p = 0; p < points.Size(); ++p)
        {
            if(count > 0)
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
                count++;
                lastPoint = points[p];
            }
        }
        ranges[count-1].stop = lastPoint;
    }*/
    
    void PointIndexes::SetRanges(openpal::Indexable<Range, uint32_t> pointranges)
    {
        //assert(ranges.Size() == pointranges.Size());
        if(ranges.Size() == 0) return;

        uint32_t count = 0;
        uint32_t lastPoint = 0;
        for(uint32_t p = 0; p < pointranges.Size(); ++p)
        {
            assert(pointranges[p].start <= pointranges[p].stop);
            if(count > 0)
            {
                assert(pointranges[p].start > lastPoint);
                if (pointranges[p].start > lastPoint + 1)
                {
                    ranges[count].start = pointranges[p].start;
                    ranges[count].stop = pointranges[p].stop;
                    ranges[count].offset = pointranges[p].start - lastPoint + ranges[count-1].offset;
                    count++;
                }
                else
                {
                    ranges[count-1].stop = pointranges[p].stop;
                }
                lastPoint = pointranges[p].stop;
            }
            else
            {
                ranges[count].start = pointranges[p].start;
                ranges[count].stop = pointranges[p].stop;
                ranges[count].offset = pointranges[p].start;
                count++;
                lastPoint = pointranges[p].stop;
            }
        }
    }
    
    void PointIndexes::SetRanges(PointIndexes pointranges)
    {
        assert(ranges.Size() == pointranges.ranges.Size());
        if(ranges.Size() == 0) return;
        
        for(uint32_t p = 0; p < pointranges.ranges.Size(); ++p)
        {
            ranges[p].start = pointranges.ranges[p].start;
            ranges[p].stop = pointranges.ranges[p].stop;
            //assert(ranges[p].start <= ranges[p].stop);
            if(p > 0)
            {
                assert(ranges[p].start > ranges[p-1].stop);
                ranges[p].offset = ranges[p-1].offset + (ranges[p].start - ranges[p-1].stop - 1);
            }
            else
            {
                ranges[p].offset = pointranges.ranges[p].start;
            }
        }
    }
    
    // Gives the number of indexes between a start and stop index
    uint32_t PointIndexes::IndexCount(uint32_t start, uint32_t stop) const
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
    
    // Given a point index, returns a position
    uint32_t PointIndexes::GetPosition(uint32_t index) const
    {
        const uint32_t IndexRange = GetRange(index);
        if(IndexRange == PointRange::OUTOFRANGE) return PointRange::OUTOFRANGE;
        if(index < ranges[IndexRange].start) return PointRange::OUTOFRANGE;
        if(index > ranges[IndexRange].stop) return PointRange::OUTOFRANGE;
        return index - ranges[IndexRange].offset;
    }
    
    // Returns the range that the index is in OR
    // the range of the next index
    uint32_t PointIndexes::GetRange(uint32_t index) const
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
    uint32_t PointIndexes::GetNextValid(uint32_t index) const
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
    uint32_t PointIndexes::GetPreviousValid(uint32_t index) const
    {
        const uint32_t IndexRange = GetRange(index);
        if (ranges.Size() == 0) return PointRange::MIN;
        if (IndexRange == PointRange::OUTOFRANGE) return Last();
        // past the range end
        if (index > ranges[IndexRange].stop) return ranges[IndexRange].stop;
        // in the range
        if (index >= ranges[IndexRange].start) return index;
        // before the range start
        if (IndexRange > 0) return ranges[IndexRange-1].stop;
        return PointRange::MIN;
    }
    
    /* Returns true if index is contained in a range */
    bool PointIndexes::Contains(uint32_t index) const
    {
        const uint32_t IndexRange = GetRange(index);
        if (IndexRange == PointRange::OUTOFRANGE) return false;
        if (index < ranges[IndexRange].start) return false;
        if (index > ranges[IndexRange].stop) return false;
        return true;
    }
    
    uint32_t PointIndexes::CountRanges(openpal::Indexable<Range, uint32_t> pointranges)
    {
        /* count the number of contiguous ranges, ensuring points are increasing */
        uint32_t count = 0;
        uint32_t lastPoint = 0;
        for(uint32_t p = 0; p < pointranges.Size(); ++p)
        {
            assert(pointranges[p].start <= pointranges[p].stop);
            if(count)
            {
                assert(pointranges[p].start > lastPoint);
                if (pointranges[p].start > lastPoint + 1) {
                    count++;
                }
                lastPoint = pointranges[p].stop;
            }
            else
            {
                count++;
                lastPoint = pointranges[p].stop;
            }
        }
        return count;
    }
    
    uint32_t PointIndexes::CountRanges(std::initializer_list<Range> points)
    {
        /* count the number of contiguous ranges, ensuring points are increasing */
        uint32_t count = 0;
        uint32_t lastPoint = 0;
        for(auto p : points)
        {
            assert(p.start <= p.stop);
            if(count)
            {
                assert(p.start > lastPoint);
                if (p.start > lastPoint + 1) {
                    count++;
                }
                lastPoint = p.stop;
            }
            else
            {
                count++;
                lastPoint = p.stop;
            }
        }
        return count;
    }
    
    
}
