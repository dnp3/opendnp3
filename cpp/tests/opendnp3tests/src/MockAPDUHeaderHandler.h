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

#include <testlib/MockLogHandler.h>

#include <vector>

namespace opendnp3
{

class MockApduHeaderHandler : public testlib::MockLogHandler, public APDUHandlerBase
{
public:

	MockApduHeaderHandler() : MockLogHandler(), APDUHandlerBase(this->GetLogger())
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

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<ControlRelayOutputBlock>>& meas) override final
	{			
		return this->ProcessAny(record, meas, crobRequests);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt16>>& meas) override final
	{				
		return this->ProcessAny(record, meas, aoInt16Requests);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt32>>& meas) override final
	{				
		return this->ProcessAny(record, meas, aoInt32Requests);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputFloat32>>& meas) override final
	{				
		return this->ProcessAny(record, meas, aoFloat32Requests);
	}

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputDouble64>>& meas) override final
	{				
		return this->ProcessAny(record, meas, aoDouble64Requests);
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

	std::vector<Indexed<IINValue>> iinBits;

	std::vector<Indexed<Binary>> eventBinaries;
	std::vector<Indexed<Binary>> staticBinaries;

	std::vector<Indexed<DoubleBitBinary>> eventDoubleBinaries;
	std::vector<Indexed<DoubleBitBinary>> staticDoubleBinaries;

	std::vector<Indexed<BinaryOutputStatus>> staticControlStatii;

	std::vector<Indexed<Counter>> eventCounters;
	std::vector<Indexed<Counter>> staticCounters;

	std::vector<Indexed<FrozenCounter>> eventFrozenCounters;
	std::vector<Indexed<FrozenCounter>> staticFrozenCounters;

	std::vector<Indexed<Analog>> eventAnalogs;
	std::vector<Indexed<Analog>> staticAnalogs;

	std::vector<Indexed<AnalogOutputStatus>> staticSetpointStatii;

	std::vector<Indexed<ControlRelayOutputBlock>> crobRequests;

	std::vector<Indexed<AnalogOutputInt16>> aoInt16Requests;
	std::vector<Indexed<AnalogOutputInt32>> aoInt32Requests;
	std::vector<Indexed<AnalogOutputFloat32>> aoFloat32Requests;
	std::vector<Indexed<AnalogOutputDouble64>> aoDouble64Requests;

	std::vector<Indexed<OctetString>> indexPrefixedOctets;
	std::vector<Indexed<OctetString>> rangedOctets;

	std::vector<Indexed<BinaryCommandEvent>> binaryCommandEvents;

	std::vector<Indexed<AnalogCommandEvent>> analogCommandEvents;

	private:

	template <class T>
	IINField ProcessAny(const HeaderRecord& record, const ICollection<Indexed<T>>& meas, std::vector<Indexed<T>>& items)
	{
		auto add = [&items](const Indexed<T>& v) { items.push_back(v); };
		meas.ForeachItem(add);
		return IINField::Empty();
	}
};

}

#endif
