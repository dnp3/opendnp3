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

#include <opendnp3/IAPDUHandler.h>

#include <vector>

namespace opendnp3
{

class MockApduHeaderHandler : public IAPDUHandler
{
	public:

		MockApduHeaderHandler()
		{}

		virtual void AllObjects(GroupVariation gv) override
		{
			groupVariations.push_back(gv);			
		}

		virtual void OnRangeRequest(GroupVariation gv, const StaticRange& range) override
		{
			groupVariations.push_back(gv);			
		}

		virtual void OnCountRequest(GroupVariation gv, uint32_t count) override
		{
			groupVariations.push_back(gv);			
		}

		virtual void OnIIN(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<bool>>& bits) override
		{
			groupVariations.push_back(gv);			
			bits.foreach([&](const IndexedValue<bool>& v) { iinBits.push_back(v); });
		}

		virtual void OnCountOf(const IterableBuffer<Group50Var1>& times) override
		{
			groupVariations.push_back(GroupVariation::Group50Var1);  // TODO - normalize this record keeping?			
		}

		virtual void OnCountOf(const IterableBuffer<Group52Var2>& times) override
		{
			groupVariations.push_back(GroupVariation::Group52Var2);  // TODO - normalize this record keeping?			
		}
				
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary>>& meas) override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<Binary>& v) { staticBinaries.push_back(v); });						
		}		

		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary>>& meas) override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<Binary>& v) { eventBinaries.push_back(v); });			
		}

		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<BinaryOutputStatus>>& meas) override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<BinaryOutputStatus>& v) { staticControlStatii.push_back(v); });	
		}

		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter>>& meas) override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<Counter>& v) { staticCounters.push_back(v); });	
		}

		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter>>& meas)  override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<Counter>& v) { eventCounters.push_back(v); });	
		}

		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog>>& meas)  override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<Analog>& v) { eventAnalogs.push_back(v); });	
		}
		
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog>>& meas)  override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<Analog>& v) { eventAnalogs.push_back(v); });	
		}

		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputStatus>>& meas) override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<AnalogOutputStatus>& v) { staticSetpointStatii.push_back(v); });	
		}

		void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<ControlRelayOutputBlock>>& meas) override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<ControlRelayOutputBlock>& v) { crobRequests.push_back(v); });			
		}

		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt16>>& meas) override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<AnalogOutputInt16>& v) { aoInt16Requests.push_back(v); });	
		}
		
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt32>>& meas) override
		{
			groupVariations.push_back(gv);		
			meas.foreach([&](const IndexedValue<AnalogOutputInt32>& v) { aoInt32Requests.push_back(v); });	
		}
		
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputFloat32>>& meas) override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<AnalogOutputFloat32>& v) { aoFloat32Requests.push_back(v); });	
		}
		
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputDouble64>>& meas) override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<AnalogOutputDouble64>& v) { aoDouble64Requests.push_back(v); });	
		}

		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString>>& meas) override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<OctetString>& v) { rangedOctets.push_back(v); });
		}
		
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString>>& meas) override
		{
			groupVariations.push_back(gv);			
			meas.foreach([&](const IndexedValue<OctetString>& v) { indexPrefixedOctets.push_back(v); });
		}
				
		std::vector<GroupVariation> groupVariations;

		std::vector<IndexedValue<bool>> iinBits;

		std::vector<IndexedValue<Binary>> eventBinaries;
		std::vector<IndexedValue<Binary>> staticBinaries;

		std::vector<IndexedValue<BinaryOutputStatus>> staticControlStatii;

		std::vector<IndexedValue<Counter>> eventCounters;
		std::vector<IndexedValue<Counter>> staticCounters;

		std::vector<IndexedValue<Analog>> eventAnalogs;
		std::vector<IndexedValue<Analog>> staticAnalogs;

		std::vector<IndexedValue<AnalogOutputStatus>> staticSetpointStatii;

		std::vector<IndexedValue<ControlRelayOutputBlock>> crobRequests;

		std::vector<IndexedValue<AnalogOutputInt16>> aoInt16Requests;
		std::vector<IndexedValue<AnalogOutputInt32>> aoInt32Requests;
		std::vector<IndexedValue<AnalogOutputFloat32>> aoFloat32Requests;
		std::vector<IndexedValue<AnalogOutputDouble64>> aoDouble64Requests;

		std::vector<IndexedValue<OctetString>> indexPrefixedOctets;
		std::vector<IndexedValue<OctetString>> rangedOctets;
};

}

#endif
