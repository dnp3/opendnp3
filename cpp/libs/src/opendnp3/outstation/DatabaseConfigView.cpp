/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include "opendnp3/outstation/DatabaseConfigView.h"

namespace opendnp3
{

DatabaseConfigView::DatabaseConfigView(
    openpal::ArrayView<Cell<Binary>, uint16_t> binaries_,
    openpal::ArrayView<Cell<DoubleBitBinary>, uint16_t> doubleBinaries_,
    openpal::ArrayView<Cell<Analog>, uint16_t> analogs_,
    openpal::ArrayView<Cell<Counter>, uint16_t> counters_,
    openpal::ArrayView<Cell<FrozenCounter>, uint16_t> frozenCounters_,
    openpal::ArrayView<Cell<BinaryOutputStatus>, uint16_t> binaryOutputStatii_,
    openpal::ArrayView<Cell<AnalogOutputStatus>, uint16_t> analogOutputStatii_,
    openpal::ArrayView<Cell<TimeAndInterval>, uint16_t> timeAndIntervals_
) :
	binaries(binaries_),
	doubleBinaries(doubleBinaries_),
	analogs(analogs_),
	counters(counters_),
	frozenCounters(frozenCounters_),
	binaryOutputStatii(binaryOutputStatii_),
	analogOutputStatii(analogOutputStatii_),
	timeAndIntervals(timeAndIntervals_)
{}

void DatabaseConfigView::SetInitialValue(const Binary& meas, uint16_t index)
{
	binaries[index].value = meas;
	binaries[index].metadata.lastEvent = meas;
}

void DatabaseConfigView::SetInitialValue(const DoubleBitBinary& meas, uint16_t index)
{
	doubleBinaries[index].value = meas;
	doubleBinaries[index].metadata.lastEvent = meas;
}

void DatabaseConfigView::SetInitialValue(const Analog& meas, uint16_t index)
{
	analogs[index].value = meas;
	analogs[index].metadata.lastEvent = meas;
}

void DatabaseConfigView::SetInitialValue(const Counter& meas, uint16_t index)
{
	counters[index].value = meas;
	counters[index].metadata.lastEvent = meas;
}

void DatabaseConfigView::SetInitialValue(const FrozenCounter& meas, uint16_t index)
{
	frozenCounters[index].value = meas;
	frozenCounters[index].metadata.lastEvent = meas;
}

void DatabaseConfigView::SetInitialValue(const BinaryOutputStatus& meas, uint16_t index)
{
	binaryOutputStatii[index].value = meas;
	binaryOutputStatii[index].metadata.lastEvent = meas;
}

void DatabaseConfigView::SetInitialValue(const AnalogOutputStatus& meas, uint16_t index)
{
	analogOutputStatii[index].value = meas;
	analogOutputStatii[index].metadata.lastEvent = meas;
}

void DatabaseConfigView::SetInitialValue(const TimeAndInterval& meas, uint16_t index)
{
	timeAndIntervals[index].value = meas;
}

}


