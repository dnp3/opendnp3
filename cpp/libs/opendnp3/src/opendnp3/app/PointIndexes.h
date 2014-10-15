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

#include <openpal/container/DynamicArray.h>

#include "Range.h"

namespace opendnp3
{
    struct PointRange
    {
    public:
        PointRange(uint32_t count=0)
        {
            if (count == 0)
            {
                start = Max();
                stop = Min();
                offset = Min();
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
        
		static uint32_t Min();
        static uint32_t Max();
		static uint32_t OutOfRange();
    };
    
    class PointIndexes
    {
    public:
        static const PointIndexes EMPTYINDEXES;

        PointIndexes(uint32_t npoints) :
        ranges(npoints>0)
        {
            if(npoints>0)
            {
                ranges[0].start = 0;
                ranges[0].stop = npoints-1;
            }
        }
        
        PointIndexes(openpal::Indexable<uint32_t, uint32_t> points) :
        ranges(CountRanges(points))
        {
            SetRanges(points);
        }
        
        PointIndexes(openpal::Indexable<Range, uint32_t> pointranges) :
        ranges(pointranges.Size())
        {
            SetRanges(pointranges);
        }
        
        PointIndexes(PointIndexes const &pointranges) :
        ranges(pointranges.ranges.Size())
        {
            SetRanges(pointranges);
        }
        
        PointIndexes& operator=(PointIndexes const &pointranges)
        {
            ranges.resize(pointranges.ranges.Size());
            SetRanges(pointranges);
            return *this;
        }
        
        inline const PointRange ToRange() const
        {
            return {First(), Last(), First()};
        }
        
        inline uint32_t First() const
        {
            if(IsEmpty()) return PointRange::Max();
            return ranges[0].start;
        }
        
        inline uint32_t Last() const
        {
            if(IsEmpty()) return PointRange::Min();
            return ranges[LastRange()].stop;
        }
        
        inline bool IsEmpty() const
        {
            return (ranges.Size() == 0);
        }
        
        inline bool IsFull() const
        {
            if(ranges.Size() != 1) return false;
            if(ranges[0].start != PointRange::Min()) return false;
            if(ranges[0].stop != PointRange::Max()) return false;
            return true;
        }
        
        // Returns the total number of defined points
        inline uint32_t IndexCount() const
        {
            if(IsEmpty()) return 0;
            const uint32_t lr = LastRange();
            return 1 + ranges[lr].stop - ranges[lr].offset;
        }
        
        // Returns the number of points between a start and stop index
        uint32_t IndexCount(uint32_t start, uint32_t stop) const;
        
        // Given point index, returns database array position
        uint32_t GetPosition(uint32_t index) const;

        // Given database array position, returns point index
        uint32_t GetIndex(uint32_t position) const;
        
        // Given an index, returns the next valid index (inclusive of the index provided)
        uint32_t GetNextValid(uint32_t index) const;
        
        // Given an index, returns the previous valid index (inclusive of the index provided)
        uint32_t GetPreviousValid(uint32_t index) const;
        
        // Returns true if index is contained in the point indexes
        bool Contains(uint32_t index) const;
        
        // Returns true if there are no discontinuities betweent the start and stop indexes
        bool IsContiguous(uint32_t start, uint32_t stop) const;
    
        // Sets PointIndexes from indexable list of Ranges (array of <start, stop> pairs
        void SetRanges(const openpal::Indexable<Range, uint32_t>& pointranges);

        // Sets PointIndexes from indexable list of increasing point indexes
        void SetRanges(const openpal::Indexable<uint32_t, uint32_t>& points);
        
        // Sets PointIndexes from existing PointIndexes reference
        void SetRanges(const PointIndexes& pointranges);
        
        // Static functions for counting the number of dis-contiguous ranges in a collection of points
        static uint32_t CountRanges(const openpal::Indexable<uint32_t, uint32_t>& points);
        static uint32_t CountRanges(const openpal::Indexable<Range, uint32_t>& points);
        
        uint32_t GetRange(uint32_t index) const;
        openpal::DynamicArray<PointRange, uint32_t> ranges;
        
    private:
        inline uint32_t LastRange() const
        {
            return ranges.Size()-1;
        }
    };
    
}

#endif
