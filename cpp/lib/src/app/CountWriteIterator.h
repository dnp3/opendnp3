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
#ifndef OPENDNP3_COUNTWRITEITERATOR_H
#define OPENDNP3_COUNTWRITEITERATOR_H

#include "app/Serializer.h"

namespace opendnp3
{

// A facade for writing APDUs to an external buffer
template<class CountType, class WriteType> class CountWriteIterator
{
public:
    static CountWriteIterator Null()
    {
        return CountWriteIterator();
    }

    CountWriteIterator() : count(0), isValid(false), pPosition(nullptr) {}

    CountWriteIterator(const Serializer<WriteType>& serializer, ser4cpp::wseq_t& position)
        : count(0),
          serializer(serializer),
          isValid(position.length() >= CountType::size),
          countPosition(position),
          pPosition(&position)
    {
        if (isValid)
        {
            position.advance(CountType::size);
        }
    }

    ~CountWriteIterator()
    {
        if (isValid)
        {
            CountType::write_to(countPosition, count);
        }
    }

    bool Write(const WriteType& value)
    {
        if (isValid && (serializer.get_size() <= pPosition->length()) && (count < CountType::max_value))
        {
            serializer.write(value, *this->pPosition);
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
    typename CountType::type_t count;
    Serializer<WriteType> serializer;

    bool isValid;

    ser4cpp::wseq_t countPosition; // make a copy to record where we write the count
    ser4cpp::wseq_t* pPosition;
};

} // namespace opendnp3

#endif
