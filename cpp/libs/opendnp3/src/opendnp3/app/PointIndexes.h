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
        
        void SetRanges(std::initializer_list<Range> points);
        void SetRanges(openpal::Indexable<uint32_t, uint32_t>& points);
        void SetRanges(openpal::Indexable<Range, uint32_t> pointranges);
        void SetRanges(PointIndexes pointranges);
        
        inline const PointRange ToRange() const
        {
            return {First(), Last(), First()};
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
        
        inline bool IsEmpty() const
        {
            return (ranges.Size() == 0);
        }
        
        inline bool IsFull() const
        {
            if(ranges.Size() != 1) return false;
            if(ranges[0].start != PointRange::MIN) return false;
            if(ranges[0].stop != PointRange::MAX) return false;
            return true;
        }
        
        inline uint32_t IndexCount() const
        {
            return IndexCount(First(), Last());
        }
        
        // Gives the number of indexes between a start and stop index
        uint32_t IndexCount(uint32_t start, uint32_t stop) const;
        
        // Given a point index, returns a position
        uint32_t GetPosition(uint32_t index) const;
        
        // Returns the range that the index is in OR
        // the range of the next index
        uint32_t GetRange(uint32_t index) const;
        
        /* Given an index, returns the next valid index (inclusive of the index provided) */
        uint32_t GetNextValid(uint32_t index) const;
        
        /* Given an index, returns the previous valid index (inclusive of the index provided) */
        uint32_t GetPreviousValid(uint32_t index) const;
        
        /* Returns true if index is contained in a range */;
        bool Contains(uint32_t index) const;
        
        static uint32_t CountRanges(std::initializer_list<Range> points);
        static uint32_t CountRanges(openpal::Indexable<uint32_t, uint32_t> points);
        static uint32_t CountRanges(openpal::Indexable<Range, uint32_t> points);
        
        openpal::Indexable<PointRange, uint32_t> ranges;
        
    private:
        inline uint32_t LastRange() const
        {
            return ranges.Size()-1;
        }
    };
    
}

#endif
