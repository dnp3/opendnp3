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
#ifndef OPENDNP3_TIMESYNCSTATE_H
#define OPENDNP3_TIMESYNCSTATE_H

#include "app/AppSeqNum.h"

#include "opendnp3/util/Timestamp.h"

namespace opendnp3
{

///
/// Represent all of the mutable state for time synchornization
///
class TimeSyncState
{

public:
    TimeSyncState() {}

    void RecordCurrentTime(const AppSeqNum& seq, const Timestamp& now)
    {
        valid = true;
        time = now;
        expectedSeqNum = seq.Next();
    }

    bool CalcTimeDifference(const AppSeqNum& seq, const Timestamp& now)
    {
        if (!valid)
            return false;
        if (!expectedSeqNum.Equals(seq))
            return false;
        if (now < time)
            return false;

        this->difference = now - time;
        this->valid = false;

        return true;
    }

    TimeDuration GetDifference() const
    {
        return this->difference;
    }

private:
    bool valid = false;
    Timestamp time;
    TimeDuration difference;
    AppSeqNum expectedSeqNum;
};

} // namespace opendnp3

#endif
