/*
 * Copyright 2013-2019 Automatak, LLC
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
#ifndef ASIODNP3_DATABASECONFIG_H
#define ASIODNP3_DATABASECONFIG_H

#include "openpal/container/Array.h"

#include "opendnp3/outstation/DatabaseSizes.h"
#include "opendnp3/outstation/MeasurementConfig.h"

namespace asiodnp3
{

class DatabaseConfig
{
public:
    DatabaseConfig(const opendnp3::DatabaseSizes& sizes)
        : sizes(sizes),
          binary(sizes.numBinary),
          doubleBinary(sizes.numDoubleBinary),
          analog(sizes.numAnalog),
          counter(sizes.numCounter),
          frozenCounter(sizes.numFrozenCounter),
          boStatus(sizes.numBinaryOutputStatus),
          aoStatus(sizes.numAnalogOutputStatus),
          timeAndInterval(sizes.numTimeAndInterval),
          octetString(sizes.numOctetString)
    {
        InitIndices(binary);
        InitIndices(doubleBinary);
        InitIndices(analog);
        InitIndices(counter);
        InitIndices(frozenCounter);
        InitIndices(boStatus);
        InitIndices(aoStatus);
        InitIndices(timeAndInterval);
        InitIndices(octetString);
    }

    const opendnp3::DatabaseSizes sizes;

    openpal::Array<opendnp3::BinaryConfig, uint16_t> binary;
    openpal::Array<opendnp3::DoubleBitBinaryConfig, uint16_t> doubleBinary;
    openpal::Array<opendnp3::AnalogConfig, uint16_t> analog;
    openpal::Array<opendnp3::CounterConfig, uint16_t> counter;
    openpal::Array<opendnp3::FrozenCounterConfig, uint16_t> frozenCounter;
    openpal::Array<opendnp3::BOStatusConfig, uint16_t> boStatus;
    openpal::Array<opendnp3::AOStatusConfig, uint16_t> aoStatus;
    openpal::Array<opendnp3::TimeAndIntervalConfig, uint16_t> timeAndInterval;
    openpal::Array<opendnp3::OctetStringConfig, uint16_t> octetString;

private:
    template<class T> void InitIndices(T& values)
    {
        for (auto i = 0; i < values.Size(); ++i)
        {
            values[i].vIndex = i;
        }
    }
};

} // namespace asiodnp3

#endif
