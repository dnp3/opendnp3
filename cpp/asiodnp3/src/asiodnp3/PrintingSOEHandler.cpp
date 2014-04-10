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

#include "PrintingSOEHandler.h"

using namespace std;

namespace opendnp3
{

PrintingSOEHandler PrintingSOEHandler::instance;

void PrintingSOEHandler::LoadStatic(const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	Print(meas, "Static - Binary");
}

void PrintingSOEHandler::LoadStatic(const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	Print(meas, "Static - DoubleBitBinary");
}

void PrintingSOEHandler::LoadStatic(const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	Print(meas, "Static - Analog");
}

void PrintingSOEHandler::LoadStatic(const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	Print(meas, "Static - Counter");
}

void PrintingSOEHandler::LoadStatic(const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	Print(meas, "Static - FrozenCounter");
}

void PrintingSOEHandler::LoadStatic(const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)
{
	Print(meas, "Static - BinaryOutputStatus");
}

void PrintingSOEHandler::LoadStatic(const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)
{
	Print(meas, "Static - AnalogOutputStatus");
}

void PrintingSOEHandler::LoadStatic(const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	meas.foreach([&](const IndexedValue<OctetString, uint16_t>& pair)
	{
		std::cout << "Static - OctetString " <<  " [" << pair.index << "] : Size : " << pair.value.ToReadOnly().Size() << std::endl;
	});
}

void PrintingSOEHandler::LoadEvent(const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	Print(meas, "Event - Binary");
}

void PrintingSOEHandler::LoadEvent(const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	Print(meas, "Event - DoubleBitBinary");
}

void PrintingSOEHandler::LoadEvent(const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	Print(meas, "Event - Analog");
}

void PrintingSOEHandler::LoadEvent(const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	Print(meas, "Event - Counter");
}

void PrintingSOEHandler::LoadEvent(const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	Print(meas, "Event - FrozenCounter");
}

void PrintingSOEHandler::LoadEvent(const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)
{
	Print(meas, "Event - BinaryOutputStatus");
}

void PrintingSOEHandler::LoadEvent(const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)
{
	Print(meas, "Event - AnalogOutputStatus");
}

void PrintingSOEHandler::LoadEvent(const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	meas.foreach([&](const IndexedValue<OctetString, uint16_t>& pair)
	{
		std::cout << "Event - OctetString " << " [" << pair.index << "] : Size : " << pair.value.ToReadOnly().Size() << std::endl;
	});
}

}
