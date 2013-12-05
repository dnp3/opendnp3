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
#ifndef __MEASUREMENT_UPDATE_H_
#define __MEASUREMENT_UPDATE_H_

#include "IMeasurementUpdate.h"

#include <vector>

namespace opendnp3 
{  
  
  class MeasurementUpdate : public IMeasurementUpdate
  {
	public:
		MeasurementUpdate();

		MeasurementUpdate(MeasurementUpdate&& arOther);

		const BinaryContainer& BinaryUpdates() const override;
		const AnalogContainer& AnalogUpdates() const override;
	    const CounterContainer& CounterUpdates() const override;
	    const ControlStatusContainer& ControlStatusUpdates() const override;
	    const SetpointStatusContainer& SetpointStatusUpdates() const override;
		const OctetStringContainer& OctetStringUpdates() const override;

		bool HasUpdates();

		// functions for building an update
		void Add(const Binary& arValue, uint32_t aIndex);
		void Add(const Analog& arValue, uint32_t aIndex);
		void Add(const Counter& arValue, uint32_t aIndex);
		void Add(const ControlStatus& arValue, uint32_t aIndex);
		void Add(const SetpointStatus& arValue, uint32_t aIndex);
		void Add(const OctetString& arValue, uint32_t aIndex);
		
	private:
		std::vector<IndexedValue<Binary>> mBinaryVec;
		std::vector<IndexedValue<Analog>> mAnalogVec;
		std::vector<IndexedValue<Counter>> mCounterVec;
		std::vector<IndexedValue<ControlStatus>> mControlStatusVec;
		std::vector<IndexedValue<SetpointStatus>> mSetpointStatusVec;		
		std::vector<IndexedValue<OctetString>> mOctetStringVec;

		MeasurementUpdate(const MeasurementUpdate& that);
		MeasurementUpdate& operator=(const MeasurementUpdate& that);
  };

}

#endif
