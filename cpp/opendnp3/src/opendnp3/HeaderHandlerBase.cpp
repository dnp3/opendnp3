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
#include "APDUHandlerBase.h"

#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{	

APDUHandlerBase::APDUHandlerBase() : 	
	ignoredHeaders(0),
	errors(),
	cto(0),
	ctoHeader(-1),
	currentHeader(0)	
{}

void APDUHandlerBase::Reset()
{
	ignoredHeaders = 0;
	errors.Clear();
	cto = 0;
	ctoHeader = -1;
	currentHeader = 0;		
}

IINField APDUHandlerBase::Errors() const
{
	IINField copy(errors);
	if(ignoredHeaders > 0) copy.Set(IINBit::FUNC_NOT_SUPPORTED);
	return copy;
}

void APDUHandlerBase::AllObjects(GroupVariation gv)
{	
	this->_AllObjects(gv);
	++currentHeader;
}

void APDUHandlerBase::OnRangeRequest(GroupVariation gv, const StaticRange& range)
{
	this->_OnRangeRequest(gv, range);
	++currentHeader;
}

void APDUHandlerBase::OnCountRequest(GroupVariation gv, uint32_t count)
{
	this->_OnCountRequest(gv, count);
	++currentHeader;
}

void APDUHandlerBase::OnIIN(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<bool>>& meas)
{	
	this->_OnIIN(meas);
	++currentHeader;
}

void APDUHandlerBase::OnCountOf(const IterableBuffer<Group50Var1>& objects)
{
	this->_OnCountOf(objects);
	++currentHeader;
}

void APDUHandlerBase::OnCountOf(const IterableBuffer<Group52Var2>& objects)
{
	this->_OnCountOf(objects);
	++currentHeader;
}

void APDUHandlerBase::OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary>>& meas)
{	
	this->_OnRange(gv, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary>>& meas)
{
	this->_OnIndexPrefix(gv, meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<BinaryOutputStatus>>& meas)
{	
	this->_OnRange(gv, meas);
	++currentHeader;
}
		
void APDUHandlerBase::OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter>>& meas)
{	
	this->_OnRange(gv, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter>>& meas)
{	
	this->_OnIndexPrefix(gv, meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog>>& meas)
{	
	this->_OnRange(gv, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog>>& meas)
{	
	this->_OnIndexPrefix(gv, meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputStatus>>& meas)
{	
	this->_OnRange(gv, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<ControlRelayOutputBlock>>& meas)
{	
	this->_OnIndexPrefix(qualifier, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(GroupVariation, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt16>>& meas)
{	
	this->_OnIndexPrefix(qualifier, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt32>>& meas)
{	
	this->_OnIndexPrefix(qualifier, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputFloat32>>& meas)
{	
	this->_OnIndexPrefix(qualifier, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputDouble64>>& meas)
{	
	this->_OnIndexPrefix(qualifier, meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString>>& meas)
{	
	this->_OnRange(gv, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString>>& meas)
{	
	this->_OnIndexPrefix(gv, meas);
	++currentHeader;
}

void APDUHandlerBase::_AllObjects(GroupVariation gv)
{	
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRangeRequest(GroupVariation gv, const StaticRange& range)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnCountRequest(GroupVariation gv, uint32_t count)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIIN(const IterableBuffer<IndexedValue<bool>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnCountOf(const IterableBuffer<Group50Var1>& objects)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnCountOf(const IterableBuffer<Group52Var2> &)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Binary>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Binary>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<BinaryOutputStatus>>& meas)
{
	++ignoredHeaders;
}
		
void APDUHandlerBase::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Counter>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Counter>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Analog>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Analog>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<AnalogOutputStatus>>& meas) 
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(QualifierCode qualifier, const IterableBuffer<IndexedValue<ControlRelayOutputBlock>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt16>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt32>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputFloat32>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputDouble64>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<OctetString>>& meas)
{	
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<OctetString>>& meas)
{
	++ignoredHeaders;
}

bool APDUHandlerBase::GetCTO(int64_t& aCTO)
{
	if(ctoHeader >= 0 && (currentHeader == (ctoHeader + 1))) {
		aCTO = cto;
		return true;
	}
	else return false;
}



}


