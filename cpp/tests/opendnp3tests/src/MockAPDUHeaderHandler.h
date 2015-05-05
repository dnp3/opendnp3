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
#ifndef __MOCK_APDU_HEADER_HANDLER_H_
#define __MOCK_APDU_HEADER_HANDLER_H_

#include <opendnp3/app/parsing/APDUHandlerBase.h>

#include "LogTester.h"

#include <vector>

namespace opendnp3
{

class MockApduHeaderHandler : public LogTester, public APDUHandlerBase
{
public:

	MockApduHeaderHandler() : LogTester(), APDUHandlerBase(this->GetLogger())
	{}

	virtual void OnHeaderResult(const HeaderRecord& record, const IINField& result) override final
	{
		records.push_back(record);
	}	

	virtual IINField ProcessCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group120Var4& value) override final
	{				
		authStatusRequsts.push_back(value);		
		return IINField::Empty();
	}

	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const IINValue& meas, uint16_t index) override final
	{
		iinBits.push_back(WithIndex(meas, index));

		return IINField::Empty();
	}

	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const Binary& meas, uint16_t index) override final
	{	
		staticBinaries.push_back(WithIndex(meas, index));		
		return IINField::Empty();
	}

	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& meas, uint16_t index) override final
	{				
		staticDoubleBinaries.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const BinaryOutputStatus& meas, uint16_t index) override final
	{		
		staticControlStatii.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const Counter& meas, uint16_t index) override final
	{				
		staticCounters.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const FrozenCounter& meas, uint16_t index) override final
	{				
		staticFrozenCounters.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const Analog& meas, uint16_t index)  override final
	{		
		eventAnalogs.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const AnalogOutputStatus& meas, uint16_t index) override final
	{		
		staticSetpointStatii.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessRange(const HeaderRecord& record, uint32_t count, const OctetString& meas, uint16_t index) override final
	{				
		rangedOctets.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}
	

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Binary& meas, uint16_t index) override final
	{				
		eventBinaries.push_back(WithIndex(meas, index));			
		return IINField::Empty();
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& meas, uint16_t index) override final
	{				
		eventDoubleBinaries.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Counter& meas, uint16_t index)  override final
	{		
		eventCounters.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const FrozenCounter& meas, uint16_t index)  override final
	{		
		eventFrozenCounters.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Analog& meas, uint16_t index)  override final
	{		
		eventAnalogs.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const OctetString& meas, uint16_t index) override final
	{				
		indexPrefixedOctets.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const BinaryCommandEvent& meas, uint16_t index) override final
	{				
		binaryCommandEvents.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const AnalogCommandEvent& meas, uint16_t index) override final
	{				
		analogCommandEvents.push_back(WithIndex(meas, index));
		return IINField::Empty();
	}

	/// --- controls ----

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas) override final
	{				
		meas.foreach([&](const IndexedValue<ControlRelayOutputBlock, uint16_t>& v)
		{
			crobRequests.push_back(v);
		});
		return IINField::Empty();
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas) override final
	{				
		meas.foreach([&](const IndexedValue<AnalogOutputInt16, uint16_t>& v)
		{
			aoInt16Requests.push_back(v);
		});
		return IINField::Empty();
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas) override final
	{				
		meas.foreach([&](const IndexedValue<AnalogOutputInt32, uint16_t>& v)
		{
			aoInt32Requests.push_back(v);
		});
		return IINField::Empty();
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas) override final
	{				
		meas.foreach([&](const IndexedValue<AnalogOutputFloat32, uint16_t>& v)
		{
			aoFloat32Requests.push_back(v);
		});
		return IINField::Empty();
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas) override final
	{				
		meas.foreach([&](const IndexedValue<AnalogOutputDouble64, uint16_t>& v)
		{
			aoDouble64Requests.push_back(v);
		});
		return IINField::Empty();
	}	

	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var1& value) override final
	{	
		authChallenges.push_back(value);
		return IINField::Empty();
	}

	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var2& value) override final
	{	
		authReplys.push_back(value);
		return IINField::Empty();
	}

	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var5& value) override final
	{	
		authKeyStatusResponses.push_back(value);
		return IINField::Empty();
	}

	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var6& value) override final
	{
		authChanges.push_back(value);
		return IINField::Empty();
	}

	std::vector<HeaderRecord> records;

	std::vector<Group120Var1> authChallenges;
	std::vector<Group120Var2> authReplys;
	std::vector<Group120Var4> authStatusRequsts;
	std::vector<Group120Var5> authKeyStatusResponses;
	std::vector<Group120Var6> authChanges;

	std::vector<IndexedValue<IINValue, uint16_t>> iinBits;

	std::vector<IndexedValue<Binary, uint16_t>> eventBinaries;
	std::vector<IndexedValue<Binary, uint16_t>> staticBinaries;

	std::vector<IndexedValue<DoubleBitBinary, uint16_t>> eventDoubleBinaries;
	std::vector<IndexedValue<DoubleBitBinary, uint16_t>> staticDoubleBinaries;

	std::vector<IndexedValue<BinaryOutputStatus, uint16_t>> staticControlStatii;

	std::vector<IndexedValue<Counter, uint16_t>> eventCounters;
	std::vector<IndexedValue<Counter, uint16_t>> staticCounters;

	std::vector<IndexedValue<FrozenCounter, uint16_t>> eventFrozenCounters;
	std::vector<IndexedValue<FrozenCounter, uint16_t>> staticFrozenCounters;

	std::vector<IndexedValue<Analog, uint16_t>> eventAnalogs;
	std::vector<IndexedValue<Analog, uint16_t>> staticAnalogs;

	std::vector<IndexedValue<AnalogOutputStatus, uint16_t>> staticSetpointStatii;

	std::vector<IndexedValue<ControlRelayOutputBlock, uint16_t>> crobRequests;

	std::vector<IndexedValue<AnalogOutputInt16, uint16_t>> aoInt16Requests;
	std::vector<IndexedValue<AnalogOutputInt32, uint16_t>> aoInt32Requests;
	std::vector<IndexedValue<AnalogOutputFloat32, uint16_t>> aoFloat32Requests;
	std::vector<IndexedValue<AnalogOutputDouble64, uint16_t>> aoDouble64Requests;

	std::vector<IndexedValue<OctetString, uint16_t>> indexPrefixedOctets;
	std::vector<IndexedValue<OctetString, uint16_t>> rangedOctets;

	std::vector<IndexedValue<BinaryCommandEvent, uint16_t>> binaryCommandEvents;

	std::vector<IndexedValue<AnalogCommandEvent, uint16_t>> analogCommandEvents;
};

}

#endif
