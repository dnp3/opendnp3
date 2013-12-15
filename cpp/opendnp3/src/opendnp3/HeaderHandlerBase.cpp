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

void HeaderHandlerBase::Reset()
{
	currentHeader = 0;	
	cto = 0;
	ctoHeader = -1;
}

void HeaderHandlerBase::AllObjects(GroupVariation gv, const openpal::ReadOnlyBuffer& header)
{	
	this->_AllObjects(gv);
	++currentHeader;
}

void HeaderHandlerBase::OnIIN(GroupVariation, const openpal::ReadOnlyBuffer&, const IterableBuffer<IndexedValue<bool>>& meas)
{	
	this->_OnIIN(meas);
	++currentHeader;
}

void HeaderHandlerBase::OnCountOf(const IterableBuffer<Group52Var2>& objects)
{
	this->_OnCountOf(objects);
	++currentHeader;
}

void HeaderHandlerBase::OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer&, const IterableBuffer<IndexedValue<Binary>>& meas)
{	
	this->_OnRange(gv, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer&, const IterableBuffer<IndexedValue<Binary>>& meas)
{
	this->_OnIndexPrefix(gv, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer&, const IterableBuffer<IndexedValue<ControlStatus>>& meas)
{	
	this->_OnRange(gv, meas);
	++currentHeader;
}
		
void HeaderHandlerBase::OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer&, const IterableBuffer<IndexedValue<Counter>>& meas)
{	
	this->_OnRange(gv, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer&, const IterableBuffer<IndexedValue<Counter>>& meas)
{	
	this->_OnIndexPrefix(gv, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer&, const IterableBuffer<IndexedValue<Analog>>& meas)
{	
	this->_OnRange(gv, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer&, const IterableBuffer<IndexedValue<Analog>>& meas)
{	
	this->_OnIndexPrefix(gv, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer&, const IterableBuffer<IndexedValue<SetpointStatus>>& meas)
{	
	this->_OnRange(gv, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<ControlRelayOutputBlock>>& meas)
{	
	this->_OnIndexPrefix(header, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputInt16>>& meas)
{	
	this->_OnIndexPrefix(header, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputInt32>>& meas)
{	
	this->_OnIndexPrefix(header, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputFloat32>>& meas)
{	
	this->_OnIndexPrefix(header, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputDouble64>>& meas)
{	
	this->_OnIndexPrefix(header, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnRangeOfOctets(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<openpal::ReadOnlyBuffer>>& meas)
{	
	this->_OnRangeOfOctets(gv, meas);
	++currentHeader;
}

void HeaderHandlerBase::OnIndexPrefixOfOctets(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<openpal::ReadOnlyBuffer>>& meas)
{	
	this->_OnIndexPrefixOfOctets(gv, meas);
	++currentHeader;
}

void HeaderHandlerBase::_AllObjects(GroupVariation gv)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring all objects header in: " << this->HandlerName());
}

void HeaderHandlerBase::_OnIIN(const IterableBuffer<IndexedValue<bool>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring IIN object header in: " << this->HandlerName());
}

void HeaderHandlerBase::_OnCountOf(const IterableBuffer<Group52Var2> &)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring count of objects header in: " << this->HandlerName());
}

void HeaderHandlerBase::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Binary>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Binary>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<ControlStatus>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}
		
void HeaderHandlerBase::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Counter>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Counter>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Analog>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Analog>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<SetpointStatus>>& meas) 
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring measurements in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<ControlRelayOutputBlock>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring command objects header in: " << this->HandlerName());
}

void HeaderHandlerBase::_OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputInt16>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring command objects header in: " << this->HandlerName());
}

void HeaderHandlerBase::_OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputInt32>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring command objects header in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputFloat32>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring command objects header in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const IterableBuffer<IndexedValue<AnalogOutputDouble64>>& meas)
{
	LOG_BLOCK(LogLevel::Warning, "Ignoring command objects header in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnRangeOfOctets(GroupVariation gv, const IterableBuffer<IndexedValue<openpal::ReadOnlyBuffer>>& meas)
{	
	LOG_BLOCK(LogLevel::Warning, "Ignoring octet data in: " << this->HandlerName());	
}

void HeaderHandlerBase::_OnIndexPrefixOfOctets(GroupVariation gv, const IterableBuffer<IndexedValue<openpal::ReadOnlyBuffer>>& meas)
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


