/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
#ifndef OPENDNP3_SEQUENCENUM_H
#define OPENDNP3_SEQUENCENUM_H

namespace opendnp3
{

/** represents a sequence number
 */
template<class T, T Modulus> class SequenceNum
{
    inline static T Next(T seq)
    {
        return (seq + 1) % Modulus;
    }

public:
    uint8_t Get() const
    {
        return this->seq;
    }

    operator uint8_t() const
    {
        return this->seq;
    }

    SequenceNum() : seq(0) {}

    SequenceNum(T value) : seq(value) {}

    bool Equals(T other) const
    {
        return other == this->seq;
    }

    void Increment()
    {
        this->seq = Next(this->seq);
    }

    void Reset()
    {
        this->seq = 0;
    }

    SequenceNum Next() const
    {
        return SequenceNum(Next(seq));
    }

protected:
    T seq;
};

} // namespace opendnp3

#endif
