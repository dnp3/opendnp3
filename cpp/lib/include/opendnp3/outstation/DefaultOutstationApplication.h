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
#ifndef OPENDNP3_DEFAULTOUTSTATIONAPPLICATION_H
#define OPENDNP3_DEFAULTOUTSTATIONAPPLICATION_H

#include "opendnp3/outstation/IOutstationApplication.h"
#include "opendnp3/util/TimeDuration.h"

#include <chrono>
#include <memory>

namespace opendnp3
{

/**
 * A singleton with default setting useful for examples
 */
class DefaultOutstationApplication : public IOutstationApplication
{
public:
    static std::shared_ptr<IOutstationApplication> Create(TimeDuration timeSyncRefreshRate = TimeDuration::Minutes(1))
    {
        return std::make_shared<DefaultOutstationApplication>(timeSyncRefreshRate);
    }

    DefaultOutstationApplication(TimeDuration timeSyncRefreshRate = TimeDuration::Minutes(1));
    virtual ~DefaultOutstationApplication() = default;

    // IDnpTimeSource
    virtual DNPTime Now() override;

    // IOutstationApplication
    virtual bool SupportsWriteAbsoluteTime() override
    {
        return true;
    }
    virtual bool WriteAbsoluteTime(const UTCTimestamp& timestamp) override;

    virtual bool SupportsWriteTimeAndInterval() override
    {
        return false;
    }
    virtual bool WriteTimeAndInterval(const ICollection<Indexed<TimeAndInterval>>& values) override
    {
        return false;
    }

    virtual bool SupportsAssignClass() override
    {
        return true;
    }
    virtual void RecordClassAssignment(AssignClassType type, PointClass clazz, uint16_t start, uint16_t stop) override
    {
    }

    virtual ApplicationIIN GetApplicationIIN() const override;

    virtual RestartMode ColdRestartSupport() const override
    {
        return RestartMode::UNSUPPORTED;
    }
    virtual RestartMode WarmRestartSupport() const override
    {
        return RestartMode::UNSUPPORTED;
    }
    virtual uint16_t ColdRestart() override
    {
        return 65535;
    }
    virtual uint16_t WarmRestart() override
    {
        return 65535;
    }

private:
    bool IsTimeValid() const;
    bool NeedsTime() const;

    TimeDuration refresh_rate;
    UTCTimestamp last_timestamp = UTCTimestamp();
    std::chrono::system_clock::time_point last_update
        = std::chrono::system_clock::time_point(std::chrono::milliseconds(0));
};

} // namespace opendnp3

#endif
