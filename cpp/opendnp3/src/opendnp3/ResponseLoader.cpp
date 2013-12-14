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
#include "ResponseLoader.h"

#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{

MeasurementHandler::MeasurementHandler(openpal::Logger& arLogger) : HeaderHandlerBase(arLogger)	
{}

void MeasurementHandler::_OnRange(GroupVariation gv, const LazyIterable<IndexedValue<Binary>>& meas)
{
	meas.Foreach([this](const IndexedValue<Binary>& v) {updates.Add(v.value, v.index); });
}

void MeasurementHandler::_OnIndexPrefix(GroupVariation gv, const LazyIterable<IndexedValue<Binary>>& meas)
{
	// todo - handle CTO
	meas.Foreach([this](const IndexedValue<Binary>& v) { updates.Add(v.value, v.index); });
}

void MeasurementHandler::_OnRange(GroupVariation gv, const LazyIterable<IndexedValue<ControlStatus>>& meas)
{
	meas.Foreach([this](const IndexedValue<ControlStatus>& v) { updates.Add(v.value, v.index); });
}
		
void MeasurementHandler::_OnRange(GroupVariation gv, const LazyIterable<IndexedValue<Counter>>& meas)
{
	meas.Foreach([this](const IndexedValue<Counter>& v) { updates.Add(v.value, v.index); });
}

void MeasurementHandler::_OnIndexPrefix(GroupVariation gv, const LazyIterable<IndexedValue<Counter>>& meas)
{
	meas.Foreach([this](const IndexedValue<Counter>& v) { updates.Add(v.value, v.index); });
}

void MeasurementHandler::_OnRange(GroupVariation gv, const LazyIterable<IndexedValue<Analog>>& meas)
{
	meas.Foreach([this](const IndexedValue<Analog>& v) { updates.Add(v.value, v.index); });
}

void MeasurementHandler::_OnIndexPrefix(GroupVariation gv, const LazyIterable<IndexedValue<Analog>>& meas)
{
	meas.Foreach([this](const IndexedValue<Analog>& v) { updates.Add(v.value, v.index); });
}

void MeasurementHandler::_OnRange(GroupVariation gv, const LazyIterable<IndexedValue<SetpointStatus>>& meas) 
{
	meas.Foreach([this](const IndexedValue<SetpointStatus>& v) { updates.Add(v.value, v.index); });
}

}

/* vim: set ts=4 sw=4: */
