//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __IMEASUREMENT_UPDATE_H_
#define __IMEASUREMENT_UPDATE_H_

#include "DataTypes.h"
#include "OctetString.h"

#include <vector>

#include "IDataObserver.h"

namespace opendnp3 
{
  
  template <class T>
  class IndexedValue
  {
	public:
		IndexedValue(const T& arValue, uint32_t aIndex) : value(arValue), index(aIndex)
		{}

		T value;
		uint32_t index;
  };

  class IMeasurementUpdate
  {
	  public:

	  typedef std::vector<IndexedValue<Binary>> BinaryContainer;
	  typedef std::vector<IndexedValue<Analog>> AnalogContainer;
	  typedef std::vector<IndexedValue<Counter>> CounterContainer;
	  typedef std::vector<IndexedValue<ControlStatus>> ControlStatusContainer;
	  typedef std::vector<IndexedValue<SetpointStatus>> SetpointStatusContainer;
	  typedef std::vector<IndexedValue<OctetString>> OctetStringContainer;
	
	  virtual const BinaryContainer& BinaryUpdates() const = 0;
	  virtual const AnalogContainer& AnalogUpdates() const = 0;
	  virtual const CounterContainer& CounterUpdates() const = 0;
	  virtual const ControlStatusContainer& ControlStatusUpdates() const = 0;
	  virtual const SetpointStatusContainer& SetpointStatusUpdates() const = 0;
	  virtual const OctetStringContainer& OctetStringUpdates() const = 0;

	  // TODO - remove this function, it's only a temporary adapter
	  void Apply(IDataObserver* apObserver) const
	  {
		Transaction t(apObserver);
		for(auto b: this->BinaryUpdates()) apObserver->Update(b.value, b.index);
		for(auto b: this->AnalogUpdates()) apObserver->Update(b.value, b.index);
		for(auto b: this->CounterUpdates()) apObserver->Update(b.value, b.index);
		for(auto b: this->ControlStatusUpdates()) apObserver->Update(b.value, b.index);
		for(auto b: this->SetpointStatusUpdates()) apObserver->Update(b.value, b.index);	
	  }
		
  };

}

#endif
