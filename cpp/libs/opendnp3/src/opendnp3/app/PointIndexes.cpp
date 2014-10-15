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

    const PointIndexes PointIndexes::EMPTYINDEXES(0);
    
    void PointIndexes::SetRanges(const openpal::Indexable<Range, uint32_t>& pointranges)
    {
        if(ranges.Size() == 0) return;

        uint32_t count = 0;
        uint32_t lastPoint = 0;
        for(uint32_t p = 0; p < pointranges.Size(); ++p)
        {
            const Range range = pointranges[p];
            assert(range.start <= range.stop);
            if(count > 0)
            {
                assert(range.start > lastPoint);
                if (range.start > lastPoint + 1)
                {
                    assert(count < ranges.Size());
                    ranges[count].start = range.start;
                    ranges[count].stop = range.stop;
                    ranges[count].offset = range.start + (ranges[count-1].offset - lastPoint - 1);
                    count++;
                }
                else
                {
                    ranges[count-1].stop = range.stop;
                }
                lastPoint = range.stop;
            }
            else
            {
                ranges[count].start = range.start;
                ranges[count].stop = range.stop;
                ranges[count].offset = range.start;
                count++;
                lastPoint = range.stop;
            }
        }
    }
    
    void PointIndexes::SetRanges(const openpal::Indexable<uint32_t, uint32_t>& points)
    {
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
                    assert(count < ranges.Size());
                    ranges[count-1].stop = lastPoint-1;
                    ranges[count].start = points[p];
                    ranges[count].offset = points[p] + (ranges[count-1].offset - lastPoint);
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
    }
    
    void PointIndexes::SetRanges(const PointIndexes& pointranges)
    {
        assert(ranges.Size() == pointranges.ranges.Size());        
        for(uint32_t p = 0; p < pointranges.ranges.Size(); ++p)
        {
            ranges[p] = pointranges.ranges[p];
        }
    }
    
    // Gives the number of indexes between a start and stop index
    uint32_t PointIndexes::IndexCount(uint32_t start, uint32_t stop) const
    {
        if(ranges.Size() == 0) return 0;
        if(First() > Last()) return 0;
        return 1 + GetPosition(stop) - GetPosition(start);
    }
    
    
    /* Returns true if index is contained in a range */
    bool PointIndexes::Contains(uint32_t index) const
    {
        if(IsEmpty()) return false;
        const uint32_t IndexRange = GetRange(index);
        if (index < ranges[IndexRange].start) return false;
        if (index > ranges[IndexRange].stop) return false;
        return true;
    }
    
    // Returns true if there are no dis-continuities in the range of point indexes
    bool PointIndexes::IsContiguous(uint32_t start, uint32_t stop) const
    {
        if(IsEmpty()) return false;
        if (start > stop) return false;
        const uint32_t IndexRange = GetRange(start);
        if (start < ranges[IndexRange].start) return false;
        if (stop > ranges[IndexRange].stop) return false;
        return true;
    }
    
    // Given point index, returns database array position
    uint32_t PointIndexes::GetPosition(uint32_t index) const
    {
        if(IsEmpty()) return PointRange::OUTOFRANGE;
        const uint32_t IndexRange = GetRange(index);
        if(index < ranges[IndexRange].start) return PointRange::OUTOFRANGE;
        if(index > ranges[IndexRange].stop) return PointRange::OUTOFRANGE;
        return index - ranges[IndexRange].offset;
    }
    
    // Given database array position, returns point index
    uint32_t PointIndexes::GetIndex(uint32_t position) const
    {
        const uint32_t nRanges = ranges.Size();
        if (nRanges == 0) return PointRange::OUTOFRANGE;

        /* Simple binary search */
        uint32_t imin = 0;
        uint32_t imax = nRanges - 1;
        uint32_t index = 0;

        if (position + ranges[imax].offset > ranges[imax].stop) return PointRange::OUTOFRANGE;
        
        while (imax > imin)
        {
            const uint32_t imid = imin + (imax - imin)/2;
            index = position + ranges[imid].offset;
            if (index < ranges[imid].start) imax = imid - 1;
            else if (index > ranges[imid].stop) imin = imid + 1;
            else return index;
        }
        return index;
    }
    
    // Returns the range that the index is in OR
    // the next range
    uint32_t PointIndexes::GetRange(uint32_t index) const
    {
        const uint32_t nRanges = ranges.Size();
        if (nRanges == 0) return PointRange::OUTOFRANGE;

        /* Simple binary search */
        uint32_t imin = 0;
        uint32_t imax = nRanges - 1;
        
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
        if (IsEmpty()) return PointRange::OUTOFRANGE;
        if (index < First()) return First();
        if (index > Last()) return PointRange::OUTOFRANGE;
        
        const uint32_t IndexRange = GetRange(index);
        if (index <= ranges[IndexRange].stop) return index;
        return ranges[IndexRange+1].start; // past the range end
    }
    
    /* Given an index, returns the previous valid index (inclusive of the index provided) */
    uint32_t PointIndexes::GetPreviousValid(uint32_t index) const
    {
        if (IsEmpty()) return PointRange::OUTOFRANGE;
        if (index < First()) return PointRange::OUTOFRANGE;
        if (index > Last()) return Last();

        const uint32_t IndexRange = GetRange(index);
        // past the range end
        if (index > ranges[IndexRange].stop) return ranges[IndexRange].stop;
        // in the range
        if (index >= ranges[IndexRange].start) return index;
        // before the range start
        if (IndexRange > 0) return ranges[IndexRange-1].stop;
        return PointRange::OUTOFRANGE;
    }
    
    uint32_t PointIndexes::CountRanges(const openpal::Indexable<Range, uint32_t>& pointranges)
    {
        /* count the number of contiguous ranges, ensuring points are increasing */
        uint32_t count = 0;
        uint32_t lastPoint = 0;
        for(uint32_t p = 0; p < pointranges.Size(); ++p)
        {
            const Range range = pointranges[p];
            assert(range.start <= range.stop);
            if(count)
            {
                assert(range.start > lastPoint);
                if (range.start > lastPoint + 1) {
                    count++;
                }
                lastPoint = range.stop;
            }
            else
            {
                count++;
                lastPoint = range.stop;
            }
        }
        return count;
    }
    
    uint32_t PointIndexes::CountRanges(const openpal::Indexable<uint32_t, uint32_t>& points)
    {
        /* count the number of contiguous ranges, ensuring points are increasing */
        uint32_t count = 0;
        uint32_t lastPoint = 0;
        for(uint32_t p = 0; p < points.Size(); ++p)
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
    
}
