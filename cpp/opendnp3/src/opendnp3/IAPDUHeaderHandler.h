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
#ifndef __I_APDU_HANDLER_H_
#define __I_APDU_HANDLER_H_

#include "LazyIterable.h"
#include "GroupVariation.h"

#include <openpal/BufferWrapper.h>

#include <opendnp3/DataTypes.h>
#include <opendnp3/ControlRelayOutputBlock.h>
#include <opendnp3/AnalogOutput.h>
#include <opendnp3/IndexedValue.h>

#include "objects/Group52.h"

namespace opendnp3
{

class IAPDUHeaderHandler 
{
	public:

		virtual void AllObjects(GroupVariation gv, const openpal::ReadOnlyBuffer& header) = 0;

		virtual void OnCountOf(const LazyIterable<Group52Var2>& times) = 0;

		virtual void OnIIN(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<bool>>& meas) = 0;

		virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Binary>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Binary>>& meas) = 0;

		virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<ControlStatus>>& meas) = 0;
		
		virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Counter>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Counter>>& meas) = 0;

		virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Analog>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Analog>>& meas) = 0;

		virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<SetpointStatus>>& meas) = 0;

		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<ControlRelayOutputBlock>>& meas) = 0;

		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputInt16>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputInt32>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputFloat32>>& meas) = 0;
		virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputDouble64>>& meas) = 0;


		virtual void OnRangeOfOctets(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas) = 0;
		virtual void OnIndexPrefixOfOctets(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas) = 0;
						
};

}

#endif
