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
#include "HeaderHandlerBase.h"

#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{	

HeaderHandlerBase::HeaderHandlerBase(Logger& aLogger) : 
	Loggable(aLogger), 
	currentHeader(0),		
	cto(0),
	ctoHeader(-1)
{}

void HeaderHandlerBase::AllObjects(GroupVariation gv, const openpal::ReadOnlyBuffer& header)
{
	++currentHeader;
	this->_AllObjects(gv);
}

void HeaderHandlerBase::OnIIN(GroupVariation, const openpal::ReadOnlyBuffer&, const LazyIterable<IndexedValue<bool>>& meas)
{
	++currentHeader;
	this->_OnIIN(meas);
}

void HeaderHandlerBase::OnCountOf(const opendnp3::LazyIterable<Group52Var2>& objects)
{
	++currentHeader;
	this->_OnCountOf(objects);
}

void HeaderHandlerBase::OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer&, const LazyIterable<IndexedValue<Binary>>& meas)
{
	++currentHeader;
	this->_OnRange(gv, meas);
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer&, const LazyIterable<IndexedValue<Binary>>& meas)
{
	++currentHeader;
	this->_OnIndexPrefix(gv, meas);
}

void HeaderHandlerBase::OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer&, const LazyIterable<IndexedValue<ControlStatus>>& meas)
{
	++currentHeader;
	this->_OnRange(gv, meas);
}
		
void HeaderHandlerBase::OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer&, const LazyIterable<IndexedValue<Counter>>& meas)
{
	++currentHeader;
	this->_OnRange(gv, meas);
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer&, const LazyIterable<IndexedValue<Counter>>& meas)
{
	++currentHeader;
	this->_OnIndexPrefix(gv, meas);
}

void HeaderHandlerBase::OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer&, const LazyIterable<IndexedValue<Analog>>& meas)
{
	++currentHeader;
	this->_OnRange(gv, meas);
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer&, const LazyIterable<IndexedValue<Analog>>& meas)
{
	++currentHeader;
	this->_OnIndexPrefix(gv, meas);
}

void HeaderHandlerBase::OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer&, const LazyIterable<IndexedValue<SetpointStatus>>& meas)
{
	++currentHeader;
	this->_OnRange(gv, meas);
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<ControlRelayOutputBlock>>& meas)
{
	++currentHeader;
	this->_OnIndexPrefix(header, meas);
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputInt16>>& meas)
{
	++currentHeader;
	this->_OnIndexPrefix(header, meas);
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputInt32>>& meas)
{
	++currentHeader;
	this->_OnIndexPrefix(header, meas);
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputFloat32>>& meas)
{
	++currentHeader;
	this->_OnIndexPrefix(header, meas);
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputDouble64>>& meas)
{
	++currentHeader;
	this->_OnIndexPrefix(header, meas);
}

void HeaderHandlerBase::OnRangeOfOctets(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas)
{
	++currentHeader;
	this->_OnRangeOfOctets(gv, meas);
}

void HeaderHandlerBase::OnIndexPrefixOfOctets(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas)
{
	++currentHeader;
	this->_OnIndexPrefixOfOctets(gv, meas);
}

void HeaderHandlerBase::_AllObjects(GroupVariation gv)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring all objects header in: " << this->HandlerName());
}

void HeaderHandlerBase::_OnIIN(const LazyIterable<IndexedValue<bool>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring IIN object header in: " << this->HandlerName());
}

void HeaderHandlerBase::_OnCountOf(const opendnp3::LazyIterable<Group52Var2> &)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring count of objects header in: " << this->HandlerName());
}

void HeaderHandlerBase::_OnRange(GroupVariation gv, const LazyIterable<IndexedValue<Binary>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(GroupVariation gv, const LazyIterable<IndexedValue<Binary>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnRange(GroupVariation gv, const LazyIterable<IndexedValue<ControlStatus>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}
		
void HeaderHandlerBase::_OnRange(GroupVariation gv, const LazyIterable<IndexedValue<Counter>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(GroupVariation gv, const LazyIterable<IndexedValue<Counter>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnRange(GroupVariation gv, const LazyIterable<IndexedValue<Analog>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(GroupVariation gv, const LazyIterable<IndexedValue<Analog>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnRange(GroupVariation gv, const LazyIterable<IndexedValue<SetpointStatus>>& meas) 
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<ControlRelayOutputBlock>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring command objects header in: " << this->HandlerName());
}

void HeaderHandlerBase::_OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputInt16>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring command objects header in: " << this->HandlerName());
}

void HeaderHandlerBase::_OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputInt32>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring command objects header in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputFloat32>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring command objects header in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputDouble64>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring command objects header in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnRangeOfOctets(GroupVariation gv, const LazyIterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas)
{	
	LOG_BLOCK(LogLevel::Warning, "Ignoring octet data in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefixOfOctets(GroupVariation gv, const LazyIterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring octet data in: " << this->HandlerName());	
}

bool HeaderHandlerBase::GetCTO(int64_t& aCTO)
{
	if(ctoHeader >= 0 && (currentHeader == (ctoHeader + 1))) {
		aCTO = cto;
		return true;
	}
	else return false;
}



}


