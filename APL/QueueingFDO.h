
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
#ifndef __QUEUEING_FDO_H_
#define __QUEUEING_FDO_H_

#include "DataInterfaces.h"
#include "SubjectBase.h"
#include "Util.h"
#include <APL/FlexibleDataObserver.h>

#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <sstream>

namespace apl
{

/** Extension to the FlexibleDataObserver that keeps track of new data updates.
*/
class QueueingFDO : public FlexibleDataObserver
{
public:

	QueueingFDO(size_t aMaxVal = 100) : M_MAX_VAL(aMaxVal)
	{}

	std::deque<std::string> updates;
private:

	virtual void _Update(const Binary& arPoint, size_t aIndex) {
		OnUpdate(arPoint, mBinaryMap, aIndex);
	}
	virtual void _Update(const Analog& arPoint, size_t aIndex) {
		OnUpdate(arPoint, mAnalogMap, aIndex);
	}
	virtual void _Update(const Counter& arPoint, size_t aIndex) {
		OnUpdate(arPoint, mCounterMap,  aIndex);
	}
	virtual void _Update(const ControlStatus& arPoint, size_t aIndex) {
		OnUpdate(arPoint, mControlStatusMap, aIndex);
	}
	virtual void _Update(const SetpointStatus& arPoint, size_t aIndex) {
		OnUpdate(arPoint, mSetpointStatusMap, aIndex);
	}

	const size_t M_MAX_VAL;

	void Push(const std::string& arVal) {
		if(updates.size() == M_MAX_VAL) updates.pop_front();
		updates.push_back(arVal);
	}


	template <class T>
	void OnUpdate(const T& arPoint, typename PointMap<T>::Type& arMap, size_t aIndex) {
		typename PointMap<T>::Type::iterator i = arMap.find(aIndex);
		if(i == arMap.end()) {
			std::ostringstream oss;
			oss << GetString(T::MeasEnum, aIndex) << " --> " << GetString(arPoint);
			this->Push(oss.str());
		}
		else if(!Equal(arPoint, i->second)) {
			std::ostringstream oss;
			oss << GetString(T::MeasEnum, aIndex);
			oss << GetString(i->second) << " --> " << GetString(arPoint);
			this->Push(oss.str());
		}
		Load(arPoint, arMap, aIndex);
	}

	static std::string GetString(DataTypes dt, size_t index) {
		std::ostringstream oss;
		oss << GetDataTypeName(dt) << "[" << index << "]";
		std::string ret = oss.str();
		ret.resize(20, ' ');
		return ret;
	}

	template <class T>
	static std::string GetString(const T& val) {
		std::ostringstream oss;
		oss << val.GetValue() << " " << T::QualConverter::GetSymbolString(val.GetQuality());
		return oss.str();
	}

	template <class T>
	static bool Equal(const T& lhs, const T& rhs) {
		return lhs.GetValue() == rhs.GetValue() && lhs.GetQuality() == rhs.GetQuality();
	}

};
}

#endif
