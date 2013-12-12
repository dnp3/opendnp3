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

#include <opendnp3/IAPDUHeaderHandler.h>

#include <vector>

namespace opendnp3
{

class MockApduHeaderHandler : public IAPDUHeaderHandler
{
	public:

		MockApduHeaderHandler()
		{}

		virtual void AllObjects(GroupVariation gv, const openpal::ReadOnlyBuffer& header) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
		}

		virtual void OnIIN(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<bool>>& bits) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			bits.Foreach([&](const IndexedValue<bool>& v) { iinBits.push_back(v); });
		}
				
		virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Binary>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<Binary>& v) {  
				staticBinaries.push_back(v);
			});						
		}		

		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Binary>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<Binary>& v) {  
				eventBinaries.push_back(v);
			});			
		}

		virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<ControlStatus>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<ControlStatus>& v) {  
				staticControlStatii.push_back(v);
			});	
		}

		virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Counter>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<Counter>& v) {  
				eventCounters.push_back(v);
			});	
		}

		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Counter>>& meas)  override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<Counter>& v) {  
				eventCounters.push_back(v);
			});	
		}

		virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Analog>>& meas)  override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<Analog>& v) {  
				eventAnalogs.push_back(v);
			});	
		}
		
		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Analog>>& meas)  override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<Analog>& v) {  
				eventAnalogs.push_back(v);
			});	
		}

		virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<SetpointStatus>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<SetpointStatus>& v) {  
				staticSetpointStatii.push_back(v);
			});	
		}

		void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<ControlRelayOutputBlock>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<ControlRelayOutputBlock>& v) {
				crobRequests.push_back(v);
			});			
		}

		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputInt16>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<AnalogOutputInt16>& v) {
				aoInt16Requests.push_back(v);
			});	
		}
		
		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputInt32>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<AnalogOutputInt32>& v) {
				aoInt32Requests.push_back(v);
			});	
		}
		
		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputFloat32>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<AnalogOutputFloat32>& v) {
				aoFloat32Requests.push_back(v);
			});	
		}
		
		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputDouble64>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<AnalogOutputDouble64>& v) {
				aoDouble64Requests.push_back(v);
			});	
		}

		virtual void OnRangeOfOctets(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<openpal::ReadOnlyBuffer>& v) {
				rangedOctets.push_back(v);
			});
		}
		
		virtual void OnIndexPrefixOfOctets(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas) override
		{
			groupVariations.push_back(gv);
			headers.push_back(header);
			meas.Foreach([&](const IndexedValue<openpal::ReadOnlyBuffer>& v) {
				indexPrefixedOctets.push_back(v);
			});
		}
		
		std::vector<openpal::ReadOnlyBuffer> headers;
		std::vector<GroupVariation> groupVariations;

		std::vector<IndexedValue<bool>> iinBits;

		std::vector<IndexedValue<Binary>> eventBinaries;
		std::vector<IndexedValue<Binary>> staticBinaries;

		std::vector<IndexedValue<ControlStatus>> staticControlStatii;

		std::vector<IndexedValue<Counter>> eventCounters;
		std::vector<IndexedValue<Counter>> staticCounters;

		std::vector<IndexedValue<Analog>> eventAnalogs;
		std::vector<IndexedValue<Analog>> staticAnalogs;

		std::vector<IndexedValue<SetpointStatus>> staticSetpointStatii;

		std::vector<IndexedValue<ControlRelayOutputBlock>> crobRequests;

		std::vector<IndexedValue<AnalogOutputInt16>> aoInt16Requests;
		std::vector<IndexedValue<AnalogOutputInt32>> aoInt32Requests;
		std::vector<IndexedValue<AnalogOutputFloat32>> aoFloat32Requests;
		std::vector<IndexedValue<AnalogOutputDouble64>> aoDouble64Requests;

		std::vector<IndexedValue<openpal::ReadOnlyBuffer>> indexPrefixedOctets;
		std::vector<IndexedValue<openpal::ReadOnlyBuffer>> rangedOctets;
};

}

#endif
