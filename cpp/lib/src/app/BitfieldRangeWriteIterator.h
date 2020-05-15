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
#ifndef OPENDNP3_BITFIELDRANGEWRITEITERATOR_H
#define OPENDNP3_BITFIELDRANGEWRITEITERATOR_H

#include <ser4cpp/serialization/LittleEndian.h>

namespace opendnp3
{

// A facade for writing APDUs to an external buffer
template<class IndexType> class BitfieldRangeWriteIterator
{
public:
    static BitfieldRangeWriteIterator Null()
    {
        auto buffer = ser4cpp::wseq_t::empty();
        return BitfieldRangeWriteIterator(0, buffer);
    }

    BitfieldRangeWriteIterator(typename IndexType::type_t start_, ser4cpp::wseq_t& position_)
        : start(start_),
          count(0),
          maxCount(0),
          isValid(position_.length() >= (2 * IndexType::size)),
          range(position_),
          pPosition(&position_)
    {
        if (isValid)
        {
            IndexType::write_to(range, start_);
            pPosition->advance(2 * IndexType::size);
            maxCount = pPosition->length() * 8;
        }
    }

    ~BitfieldRangeWriteIterator()
    {
        if (isValid && count > 0)
        {
            typename IndexType::type_t stop = start + count - 1;
            IndexType::write_to(range, stop);

            auto num = count / 8;

            if ((count % 8) > 0)
            {
                ++num;
            }

            pPosition->advance(num);
        }
    }

    bool Write(bool value)
    {
        if (isValid && count < maxCount)
        {
            auto byte = count / 8;
            auto bit = count % 8;

            if (bit == 0)
            {
                (*pPosition)[byte] = 0; // initialize byte to 0
            }

            if (value)
            {
                (*pPosition)[byte] = ((*pPosition)[byte] | (1 << bit));
            }

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
    typename IndexType::type_t count;

    size_t maxCount;

    bool isValid;

    ser4cpp::wseq_t range; // make a copy to record where we write the range
    ser4cpp::wseq_t* pPosition;
};

} // namespace opendnp3

#endif
