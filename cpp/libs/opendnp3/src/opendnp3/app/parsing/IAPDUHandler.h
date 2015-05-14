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
#ifndef OPENDNP3_IAPDUHANDLER_H
#define OPENDNP3_IAPDUHANDLER_H

#include "opendnp3/app/GroupVariationRecord.h"

#include "opendnp3/app/parsing/ICollection.h"

#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/app/IINValue.h"
#include "opendnp3/app/ControlRelayOutputBlock.h"
#include "opendnp3/app/AnalogOutput.h"
#include "opendnp3/app/BinaryCommandEvent.h"
#include "opendnp3/app/AnalogCommandEvent.h"
#include "opendnp3/app/Indexed.h"
#include "opendnp3/app/OctetString.h"
#include "opendnp3/app/Range.h"

#include "opendnp3/gen/QualifierCode.h"

#include "opendnp3/objects/Group50.h"
#include "opendnp3/objects/Group51.h"
#include "opendnp3/objects/Group52.h"

#include "opendnp3/objects/Group120.h"
#include "opendnp3/objects/Group121.h"
#include "opendnp3/objects/Group122.h"

namespace opendnp3
{

/**
* Second stage of dealing with
*/
class IAPDUHandler
{
public:

	virtual void AllObjects(const HeaderRecord& record) = 0;
	virtual void OnRangeRequest(const HeaderRecord& record, const Range& range) = 0;
	virtual void OnCountRequest(const HeaderRecord& record, uint16_t count) = 0;

	// ------	Count callbacks for qualifiers 0x07 and 0x08 ------
	
	virtual void OnCount(const HeaderRecord& record, const ICollection<Group50Var1>& values) = 0;
	virtual void OnCount(const HeaderRecord& record, const ICollection<Group51Var1>& values) = 0;
	virtual void OnCount(const HeaderRecord& record, const ICollection<Group51Var2>& values) = 0;
	virtual void OnCount(const HeaderRecord& record, const ICollection<Group52Var2>& values) = 0;
	virtual void OnCount(const HeaderRecord& record, const ICollection<Group120Var4>& values) = 0;

	// ------ Variable-length free-format types  ------

	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var1& value) = 0;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var2& value) = 0;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var5& value) = 0;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var6& value) = 0;

	// ------ Special ranged values like IIN (group 80) ------

	virtual void OnRange(const HeaderRecord& record, const ICollection<Indexed<IINValue>> &values) = 0;

	// ------ range callbacks for qualifiers 0x00 and 0x01 ------

	virtual void OnRange(const HeaderRecord& record, const ICollection<Indexed<Binary>>& values) = 0;
	virtual void OnRange(const HeaderRecord& record, const ICollection<Indexed<DoubleBitBinary>>& values) = 0;
	virtual void OnRange(const HeaderRecord& record, const ICollection<Indexed<BinaryOutputStatus>>& values) = 0;
	virtual void OnRange(const HeaderRecord& record, const ICollection<Indexed<Counter>>& values) = 0;
	virtual void OnRange(const HeaderRecord& record, const ICollection<Indexed<FrozenCounter>>& values) = 0;
	virtual void OnRange(const HeaderRecord& record, const ICollection<Indexed<Analog>>& values) = 0;
	virtual void OnRange(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputStatus>>& values) = 0;
	virtual void OnRange(const HeaderRecord& record, const ICollection<Indexed<OctetString>>& values) = 0;
	virtual void OnRange(const HeaderRecord& record, const ICollection<Indexed<TimeAndInterval>>& values) = 0;
	virtual void OnRange(const HeaderRecord& record, const ICollection<Indexed<Group121Var1>>& values) = 0;

	// ------ index-prefix callbacks for qualifiers 0x17 and 0x28 ------

	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Binary>>& values) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<BinaryOutputStatus>>& values) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<DoubleBitBinary>>& values) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Counter>>& values) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<FrozenCounter>>& values) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Analog>>& values) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputStatus>>& values) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<OctetString>>& values) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<TimeAndInterval>>& values) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<BinaryCommandEvent>>& values) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogCommandEvent>>& values) = 0;

	// --- security stat events ---

	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Group122Var1>> &values) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Group122Var2>> &values) = 0;

	// --- commmands ---

	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<ControlRelayOutputBlock>>& meas) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt16>>& meas) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt32>>& meas) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputFloat32>>& meas) = 0;
	virtual void OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputDouble64>>& meas) = 0;
	
};

}

#endif
