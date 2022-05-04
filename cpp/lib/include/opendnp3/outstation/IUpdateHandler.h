/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
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
#ifndef OPENDNP3_IUPDATEHANDLER_H
#define OPENDNP3_IUPDATEHANDLER_H

#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/app/OctetString.h"
#include "opendnp3/gen/EventMode.h"
#include "opendnp3/gen/FlagsType.h"

namespace opendnp3
{

/**
 * An interface used to update measurement values.
 */
class IUpdateHandler
{
public:
    virtual ~IUpdateHandler() {}

    /**
     * Update a Binary measurement
     * @param meas measurement to be processed
     * @param index index of the measurement
     * @param mode Describes how event generation is handled for this method
     * @return true if the value exists and it was updated
     */
    virtual bool Update(const Binary& meas, uint16_t index, EventMode mode = EventMode::Detect) = 0;

    /**
     * Update a DoubleBitBinary measurement
     * @param meas measurement to be processed
     * @param index index of the measurement
     * @param mode Describes how event generation is handled for this method
     * @return true if the value exists and it was updated
     */
    virtual bool Update(const DoubleBitBinary& meas, uint16_t index, EventMode mode = EventMode::Detect) = 0;

    /**
     * Update an Analog measurement
     * @param meas measurement to be processed
     * @param index index of the measurement
     * @param mode Describes how event generation is handled for this method
     * @return true if the value exists and it was updated
     */
    virtual bool Update(const Analog& meas, uint16_t index, EventMode mode = EventMode::Detect) = 0;

    /**
     * Update a Counter measurement
     * @param meas measurement to be processed
     * @param index index of the measurement
     * @param mode Describes how event generation is handled for this method
     * @return true if the value exists and it was updated
     */
    virtual bool Update(const Counter& meas, uint16_t index, EventMode mode = EventMode::Detect) = 0;

    /**
     * Freeze a Counter measurement
     * @param index index of the measurement
     * @param clear clear the original counter
     * @param mode Describes how event generation is handled for this method
     * @return true if the value exists and it was updated
     */
    virtual bool FreezeCounter(uint16_t index, bool clear = false, EventMode mode = EventMode::Detect) = 0;

    /**
     * Update a BinaryOutputStatus measurement
     * @param meas measurement to be processed
     * @param index index of the measurement
     * @param mode Describes how event generation is handled for this method
     * @return true if the value exists and it was updated
     */
    virtual bool Update(const BinaryOutputStatus& meas, uint16_t index, EventMode mode = EventMode::Detect) = 0;

    /**
     * Update a AnalogOutputStatus measurement
     * @param meas measurement to be processed
     * @param index index of the measurement
     * @param mode Describes how event generation is handled for this method
     * @return true if the value exists and it was updated
     */
    virtual bool Update(const AnalogOutputStatus& meas, uint16_t index, EventMode mode = EventMode::Detect) = 0;

    /**
     * Update an octet string value
     * @param meas measurement to be processed
     * @param index index of the measurement
     * @param mode Describes how event generation is handled for this method
     * @return true if the value exists and it was updated
     */
    virtual bool Update(const OctetString& meas, uint16_t index, EventMode mode = EventMode::Detect) = 0;

    /**
     * Update a TimeAndInterval valueindex
     * @param meas measurement to be processed
     * @param index index of the measurement
     * @return true if the value exists and it was updated
     */
    virtual bool Update(const TimeAndInterval& meas, uint16_t index) = 0;

    /**
     * Update the flags of a measurement without changing it's value
     * @param type enumeration specifiy the type to change
     * @param start the start index at which to begin changing flags
     * @param stop the stop index at which to end changing flags
     * @param flags the new value of the flags
     */
    virtual bool Modify(FlagsType type, uint16_t start, uint16_t stop, uint8_t flags) = 0;
};

} // namespace opendnp3

#endif
