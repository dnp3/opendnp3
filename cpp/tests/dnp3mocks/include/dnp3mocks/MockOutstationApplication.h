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
#ifndef OPENDNP3_UNITTESTS_MOCK_OUTSTATION_APPLICATION_H
#define OPENDNP3_UNITTESTS_MOCK_OUTSTATION_APPLICATION_H

#include <opendnp3/outstation/IOutstationApplication.h>

#include <deque>
#include <tuple>

struct ConfirmResult
{
    bool is_unsolicited;
    uint32_t num_class1;
    uint32_t num_class2;
    uint32_t num_class3;
};

class MockOutstationApplication : public opendnp3::IOutstationApplication
{
public:
    MockOutstationApplication()
        : currentTime(opendnp3::DNPTime(0, opendnp3::TimestampQuality::INVALID)),
          supportsTimeWrite(true),
          supportsAssignClass(false),
          supportsWriteTimeAndInterval(false),
          allowTimeWrite(true),
          warmRestartSupport(opendnp3::RestartMode::UNSUPPORTED),
          coldRestartSupport(opendnp3::RestartMode::UNSUPPORTED),
          warmRestartTimeDelay(0),
          coldRestartTimeDelay(0)
    {
    }

    opendnp3::DNPTime Now() final
    {
        return currentTime;
    }

    void OnStateChange(opendnp3::LinkStatus value) final {}

    bool SupportsWriteAbsoluteTime() final
    {
        return supportsTimeWrite;
    }

    bool WriteAbsoluteTime(const opendnp3::UTCTimestamp& timestamp) final
    {
        if (allowTimeWrite)
        {
            timestamps.push_back(timestamp);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool SupportsWriteTimeAndInterval() final
    {
        return supportsWriteTimeAndInterval;
    }

    bool WriteTimeAndInterval(const opendnp3::ICollection<opendnp3::Indexed<opendnp3::TimeAndInterval>>& meas) final
    {
        auto push = [this](const opendnp3::Indexed<opendnp3::TimeAndInterval>& value) {
            this->timeAndIntervals.push_back(value);
        };

        meas.ForeachItem(push);
        return true;
    }

    bool SupportsAssignClass() final
    {
        return supportsAssignClass;
    }

    void RecordClassAssignment(opendnp3::AssignClassType type,
                               opendnp3::PointClass clazz,
                               uint16_t start,
                               uint16_t stop) final
    {
        this->classAssignments.push_back(std::make_tuple(type, clazz, start, stop));
    }

    opendnp3::ApplicationIIN GetApplicationIIN() const final
    {
        return appIIN;
    }

    opendnp3::RestartMode ColdRestartSupport() const final
    {
        return coldRestartSupport;
    }

    opendnp3::RestartMode WarmRestartSupport() const final
    {
        return warmRestartSupport;
    }

    uint16_t ColdRestart() final
    {
        return coldRestartTimeDelay;
    }

    uint16_t WarmRestart() final
    {
        return warmRestartTimeDelay;
    }

    void OnConfirmProcessed(bool is_unsolicited, uint32_t num_class1, uint32_t num_class2, uint32_t num_class3) final
    {
        ConfirmResult confirm{};
        confirm.is_unsolicited = is_unsolicited;
        confirm.num_class1 = num_class1;
        confirm.num_class2 = num_class2;
        confirm.num_class3 = num_class3;
        this->confirms.push_back(confirm);
    }

    void SetTime(opendnp3::DNPTime time)
    {
        this->currentTime = time;
    }

    opendnp3::DNPTime currentTime;

    bool supportsTimeWrite;
    bool supportsAssignClass;
    bool supportsWriteTimeAndInterval;

    bool allowTimeWrite;

    opendnp3::RestartMode warmRestartSupport;
    opendnp3::RestartMode coldRestartSupport;

    uint16_t warmRestartTimeDelay;
    uint16_t coldRestartTimeDelay;

    opendnp3::ApplicationIIN appIIN;

    std::deque<opendnp3::UTCTimestamp> timestamps;
    std::deque<std::tuple<opendnp3::AssignClassType, opendnp3::PointClass, uint16_t, uint16_t>> classAssignments;
    std::deque<opendnp3::Indexed<opendnp3::TimeAndInterval>> timeAndIntervals;
    std::deque<ConfirmResult> confirms;
};

#endif
