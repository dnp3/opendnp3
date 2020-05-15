/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef OPENDNP3_RANGEWRITEITERATOR_H
#define OPENDNP3_RANGEWRITEITERATOR_H

#include "app/Serializer.h"

namespace opendnp3
{

// A facade for writing APDUs to an external buffer
template<class IndexType, class WriteType> class RangeWriteIterator
{
public:
    static RangeWriteIterator Null()
    {
        return RangeWriteIterator();
    }

    RangeWriteIterator() : start(0), count(0), isValid(false), pPosition(nullptr) {}

    RangeWriteIterator(typename IndexType::type_t start_,
                       const Serializer<WriteType>& serializer,
                       ser4cpp::wseq_t& position)
        : start(start_),
          serializer(serializer),
          count(0),
          isValid(position.length() >= 2 * IndexType::size),
          range(position),
          pPosition(&position)
    {
        if (isValid)
        {
            IndexType::write_to(range, start);
            pPosition->advance(2 * IndexType::size);
        }
    }

    ~RangeWriteIterator()
    {
        if (isValid && count > 0)
        {
            auto stop = start + count - 1;
            IndexType::write_to(range, static_cast<typename IndexType::type_t>(stop));
        }
    }

    bool Write(const WriteType& value)
    {
        if (isValid && (pPosition->length() >= serializer.get_size()) && (count <= IndexType::max_value))
        {
            serializer.write(value, *pPosition);
            ++count;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool IsValid() const
    {
        return isValid;
    }

private:
    typename IndexType::type_t start;
    Serializer<WriteType> serializer;
    uint32_t count;

    bool isValid;

    ser4cpp::wseq_t range; // make a copy to record where we write the range
    ser4cpp::wseq_t* pPosition;
};

} // namespace opendnp3

#endif
