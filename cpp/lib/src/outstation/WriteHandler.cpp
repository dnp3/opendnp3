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
#include "WriteHandler.h"

#include "logging/LogMacros.h"

namespace opendnp3
{

WriteHandler::WriteHandler(
    IOutstationApplication& application, TimeSyncState& timeSyncState, AppSeqNum seq, Timestamp now, IINField* writeIIN)
    : application(&application), timeSyncState(&timeSyncState), seq(seq), now(now), writeIIN(writeIIN)
{
}

IINField WriteHandler::ProcessHeader(const RangeHeader& /*header*/, const ICollection<Indexed<IINValue>>& values)
{
    Indexed<IINValue> pair;

    if (!values.ReadOnlyValue(pair))
    {
        return IINBit::PARAM_ERROR;
    }

    if (wroteIIN)
    {
        return IINBit::PARAM_ERROR;
    }

    if (pair.index != static_cast<uint16_t>(IINBit::DEVICE_RESTART))
    {
        return IINBit::PARAM_ERROR;
    }

    if (pair.value.value)
    {
        return IINBit::PARAM_ERROR;
    }

    wroteIIN = true;
    writeIIN->ClearBit(IINBit::DEVICE_RESTART);
    return IINField();
}

IINField WriteHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group50Var1>& values)
{
    if (this->wroteTime)
        return IINBit::PARAM_ERROR;

    if (!application->SupportsWriteAbsoluteTime())
        return IINBit::FUNC_NOT_SUPPORTED;

    Group50Var1 value;
    if (!values.ReadOnlyValue(value))
        return IINBit::PARAM_ERROR;

    this->wroteTime = true;
    return application->WriteAbsoluteTime(UTCTimestamp(value.time.value)) ? IINField::Empty() : IINBit::PARAM_ERROR;
}

IINField WriteHandler::ProcessHeader(const CountHeader& /*header*/, const ICollection<Group50Var3>& values)
{
    if (this->wroteTime)
        return IINBit::PARAM_ERROR;

    if (!application->SupportsWriteAbsoluteTime())
        return IINBit::FUNC_NOT_SUPPORTED;

    Group50Var3 value;
    if (!values.ReadOnlyValue(value))
        return IINBit::PARAM_ERROR;

    if (!this->timeSyncState->CalcTimeDifference(this->seq, this->now))
        return IINBit::PARAM_ERROR;

    const UTCTimestamp time(
        value.time.value
        + std::chrono::duration_cast<std::chrono::milliseconds>(this->timeSyncState->GetDifference().value).count());

    this->wroteTime = true;
    return application->WriteAbsoluteTime(time) ? IINField::Empty() : IINBit::PARAM_ERROR;
}

IINField WriteHandler::ProcessHeader(const PrefixHeader& /*header*/,
                                     const ICollection<Indexed<TimeAndInterval>>& values)
{
    if (!application->SupportsWriteTimeAndInterval())
    {
        return IINBit::FUNC_NOT_SUPPORTED;
    }

    return application->WriteTimeAndInterval(values) ? IINField::Empty() : IINBit::PARAM_ERROR;
}

} // namespace opendnp3
