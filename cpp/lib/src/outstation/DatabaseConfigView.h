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
#ifndef OPENDNP3_DATABASECONFIGVIEW_H
#define OPENDNP3_DATABASECONFIGVIEW_H

#include "opendnp3/app/MeasurementTypeSpecs.h"
#include "opendnp3/outstation/Cell.h"

#include <ser4cpp/container/ArrayView.h>

namespace opendnp3
{

/**
 * DatabaseConfigView provides abstracted access to the raw buffers in outstation database.
 *
 * Use this object to congfigure:
 *
 *  1) Inital values if you want something besides false/zero with 0x02 restart quality
 *  2) Default static/event reporting variations for each point
 *  3) Class assignments (0,1,2,3) for each point
 *  4) deadbands for analogs / counters / etc
 *
 */
class DatabaseConfigView
{

public:
    DatabaseConfigView(ser4cpp::ArrayView<Cell<BinarySpec>, uint16_t> binaries,
                       ser4cpp::ArrayView<Cell<DoubleBitBinarySpec>, uint16_t> doubleBinaries,
                       ser4cpp::ArrayView<Cell<AnalogSpec>, uint16_t> analogs,
                       ser4cpp::ArrayView<Cell<CounterSpec>, uint16_t> counters,
                       ser4cpp::ArrayView<Cell<FrozenCounterSpec>, uint16_t> frozenCounters,
                       ser4cpp::ArrayView<Cell<BinaryOutputStatusSpec>, uint16_t> binaryOutputStatii,
                       ser4cpp::ArrayView<Cell<AnalogOutputStatusSpec>, uint16_t> analogOutputStatii,
                       ser4cpp::ArrayView<Cell<TimeAndIntervalSpec>, uint16_t> timeAndIntervals,
                       ser4cpp::ArrayView<Cell<OctetStringSpec>, uint16_t> octetStrings);

    //  ----------- Views of the underlying storage ---------

    ser4cpp::ArrayView<Cell<BinarySpec>, uint16_t> binaries;
    ser4cpp::ArrayView<Cell<DoubleBitBinarySpec>, uint16_t> doubleBinaries;
    ser4cpp::ArrayView<Cell<AnalogSpec>, uint16_t> analogs;
    ser4cpp::ArrayView<Cell<CounterSpec>, uint16_t> counters;
    ser4cpp::ArrayView<Cell<FrozenCounterSpec>, uint16_t> frozenCounters;
    ser4cpp::ArrayView<Cell<BinaryOutputStatusSpec>, uint16_t> binaryOutputStatii;
    ser4cpp::ArrayView<Cell<AnalogOutputStatusSpec>, uint16_t> analogOutputStatii;
    ser4cpp::ArrayView<Cell<TimeAndIntervalSpec>, uint16_t> timeAndIntervals;
    ser4cpp::ArrayView<Cell<OctetStringSpec>, uint16_t> octetStrings;
};

} // namespace opendnp3

#endif
