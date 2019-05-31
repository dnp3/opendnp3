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
#include "outstation/DatabaseConfigView.h"

namespace opendnp3
{

DatabaseConfigView::DatabaseConfigView(ser4cpp::ArrayView<Cell<BinarySpec>, uint16_t> binaries,
                                       ser4cpp::ArrayView<Cell<DoubleBitBinarySpec>, uint16_t> doubleBinaries,
                                       ser4cpp::ArrayView<Cell<AnalogSpec>, uint16_t> analogs,
                                       ser4cpp::ArrayView<Cell<CounterSpec>, uint16_t> counters,
                                       ser4cpp::ArrayView<Cell<FrozenCounterSpec>, uint16_t> frozenCounters,
                                       ser4cpp::ArrayView<Cell<BinaryOutputStatusSpec>, uint16_t> binaryOutputStatii,
                                       ser4cpp::ArrayView<Cell<AnalogOutputStatusSpec>, uint16_t> analogOutputStatii,
                                       ser4cpp::ArrayView<Cell<TimeAndIntervalSpec>, uint16_t> timeAndIntervals,
                                       ser4cpp::ArrayView<Cell<OctetStringSpec>, uint16_t> octetStrings)
    : binaries(binaries),
      doubleBinaries(doubleBinaries),
      analogs(analogs),
      counters(counters),
      frozenCounters(frozenCounters),
      binaryOutputStatii(binaryOutputStatii),
      analogOutputStatii(analogOutputStatii),
      timeAndIntervals(timeAndIntervals),
      octetStrings(octetStrings)
{
}

} // namespace opendnp3
