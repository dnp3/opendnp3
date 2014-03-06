/**
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
#include "MeasurementHandler.h"

#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{

MeasurementHandler::MeasurementHandler(openpal::Logger& aLogger, ISOEHandler* pSOEHandler_) : APDUHandlerBase(aLogger), pSOEHandler(pSOEHandler_)
{}

void MeasurementHandler::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}
		
void MeasurementHandler::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

void MeasurementHandler::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	pSOEHandler->Load(meas);
}

}


