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
#include "DatabaseBuffers.h"

#include <openpal/logging/LogMacros.h>

#include "opendnp3/ErrorCodes.h"

#include <assert.h>
#include <array>

using namespace openpal;

namespace opendnp3
{

DatabaseBuffers::DatabaseBuffers(const DatabaseTemplate& dbTemplate) :
	current(dbTemplate),
	selected(dbTemplate)
{

}

void DatabaseBuffers::Unselect()
{
	this->Deselect<Binary>();
	this->Deselect<DoubleBitBinary>();
	this->Deselect<Counter>();
	this->Deselect<FrozenCounter>();
	this->Deselect<Analog>();
	this->Deselect<BinaryOutputStatus>();
	this->Deselect<AnalogOutputStatus>();
	this->Deselect<TimeAndInterval>();
}

IINField DatabaseBuffers::SelectAll(GroupVariation gv)
{
	if (gv == GroupVariation::Group60Var1)
	{		
		this->SelectAll<Binary>();
		this->SelectAll<DoubleBitBinary>();
		this->SelectAll<Counter>();
		this->SelectAll<FrozenCounter>();
		this->SelectAll<Analog>();
		this->SelectAll<BinaryOutputStatus>();
		this->SelectAll<AnalogOutputStatus>();
		this->SelectAll<TimeAndInterval>();

		return IINField::Empty();
	}
	else
	{	
		switch (gv)
		{
			case(GroupVariation::Group1Var0):
				return this->SelectAll<Binary>();				
			case(GroupVariation::Group1Var2) :
				return this->SelectAllUsing<Binary>(StaticBinaryResponse::Group1Var2);				

			case(GroupVariation::Group3Var0) :
				return this->SelectAll<DoubleBitBinary>();				
			case(GroupVariation::Group3Var2) :
				return this->SelectAllUsing<DoubleBitBinary>(StaticDoubleBinaryResponse::Group3Var2);				

			case(GroupVariation::Group10Var0) :
				return this->SelectAll<BinaryOutputStatus>();
			case(GroupVariation::Group10Var2) :
				return this->SelectAllUsing<BinaryOutputStatus>(StaticBinaryOutputStatusResponse::Group10Var2);		

			case(GroupVariation::Group20Var0):
				return this->SelectAll<Counter>();
			case(GroupVariation::Group20Var1):
				return this->SelectAllUsing<Counter>(StaticCounterResponse::Group20Var1);
			case(GroupVariation::Group20Var2) :
				return this->SelectAllUsing<Counter>(StaticCounterResponse::Group20Var2);
			case(GroupVariation::Group20Var5) :
				return this->SelectAllUsing<Counter>(StaticCounterResponse::Group20Var5);
			case(GroupVariation::Group20Var6) :
				return this->SelectAllUsing<Counter>(StaticCounterResponse::Group20Var6);

			case(GroupVariation::Group21Var0) :
				return this->SelectAll<FrozenCounter>();
			case(GroupVariation::Group21Var1) :
				return this->SelectAllUsing<FrozenCounter>(StaticFrozenCounterResponse::Group21Var1);
			case(GroupVariation::Group21Var2) :
				return this->SelectAllUsing<FrozenCounter>(StaticFrozenCounterResponse::Group21Var2);
			case(GroupVariation::Group21Var5) :
				return this->SelectAllUsing<FrozenCounter>(StaticFrozenCounterResponse::Group21Var5);
			case(GroupVariation::Group21Var6) :
				return this->SelectAllUsing<FrozenCounter>(StaticFrozenCounterResponse::Group21Var6);			
			case(GroupVariation::Group21Var9) :
				return this->SelectAllUsing<FrozenCounter>(StaticFrozenCounterResponse::Group21Var9);
			case(GroupVariation::Group21Var10) :
				return this->SelectAllUsing<FrozenCounter>(StaticFrozenCounterResponse::Group21Var10);

			case(GroupVariation::Group30Var0) :
				return this->SelectAll<Analog>();
			case(GroupVariation::Group30Var1) :
				return this->SelectAllUsing<Analog>(StaticAnalogResponse::Group30Var1);
			case(GroupVariation::Group30Var2) :
				return this->SelectAllUsing<Analog>(StaticAnalogResponse::Group30Var2);
			case(GroupVariation::Group30Var3) :
				return this->SelectAllUsing<Analog>(StaticAnalogResponse::Group30Var3);
			case(GroupVariation::Group30Var4) :
				return this->SelectAllUsing<Analog>(StaticAnalogResponse::Group30Var4);
			case(GroupVariation::Group30Var5) :
				return this->SelectAllUsing<Analog>(StaticAnalogResponse::Group30Var5);
			case(GroupVariation::Group30Var6) :
				return this->SelectAllUsing<Analog>(StaticAnalogResponse::Group30Var6);

			case(GroupVariation::Group40Var0) :
				return this->SelectAll<AnalogOutputStatus>();
			case(GroupVariation::Group40Var1) :
				return this->SelectAllUsing<AnalogOutputStatus>(StaticAnalogOutputStatusResponse::Group40Var1);
			case(GroupVariation::Group40Var2) :
				return this->SelectAllUsing<AnalogOutputStatus>(StaticAnalogOutputStatusResponse::Group40Var2);
			case(GroupVariation::Group40Var3) :
				return this->SelectAllUsing<AnalogOutputStatus>(StaticAnalogOutputStatusResponse::Group40Var3);
			case(GroupVariation::Group40Var4) :
				return this->SelectAllUsing<AnalogOutputStatus>(StaticAnalogOutputStatusResponse::Group40Var4);

			case(GroupVariation::Group50Var4) :
				return this->SelectAllUsing<TimeAndInterval>(StaticTimeAndIntervalResponse::Group50Var4);

			default:
				return IINField(IINBit::FUNC_NOT_SUPPORTED);
		}				
	}
}

IINField DatabaseBuffers::SelectRange(GroupVariation gv, const Range& range)
{
	switch (gv)
	{
	case(GroupVariation::Group1Var0) :
		return this->SelectRange<Binary>(range);
	case(GroupVariation::Group1Var2) :
		return this->SelectRangeUsing<Binary>(range, StaticBinaryResponse::Group1Var2);

	case(GroupVariation::Group3Var0) :
		return this->SelectRange<DoubleBitBinary>(range);
	case(GroupVariation::Group3Var2) :
		return this->SelectRangeUsing<DoubleBitBinary>(range, StaticDoubleBinaryResponse::Group3Var2);

	case(GroupVariation::Group10Var0) :
		return this->SelectRange<BinaryOutputStatus>(range);
	case(GroupVariation::Group10Var2) :
		return this->SelectRangeUsing<BinaryOutputStatus>(range, StaticBinaryOutputStatusResponse::Group10Var2);

	case(GroupVariation::Group20Var0) :
		return this->SelectRange<Counter>(range);
	case(GroupVariation::Group20Var1) :
		return this->SelectRangeUsing<Counter>(range, StaticCounterResponse::Group20Var1);
	case(GroupVariation::Group20Var2) :
		return this->SelectRangeUsing<Counter>(range, StaticCounterResponse::Group20Var2);
	case(GroupVariation::Group20Var5) :
		return this->SelectRangeUsing<Counter>(range, StaticCounterResponse::Group20Var5);
	case(GroupVariation::Group20Var6) :
		return this->SelectRangeUsing<Counter>(range, StaticCounterResponse::Group20Var6);

	case(GroupVariation::Group21Var0) :
		return this->SelectRange<FrozenCounter>(range);
	case(GroupVariation::Group21Var1) :
		return this->SelectRangeUsing<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var1);
	case(GroupVariation::Group21Var2) :
		return this->SelectRangeUsing<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var2);
	case(GroupVariation::Group21Var5) :
		return this->SelectRangeUsing<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var5);
	case(GroupVariation::Group21Var6) :
		return this->SelectRangeUsing<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var6);
	case(GroupVariation::Group21Var9) :
		return this->SelectRangeUsing<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var9);
	case(GroupVariation::Group21Var10) :
		return this->SelectRangeUsing<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var10);

	case(GroupVariation::Group30Var0) :
		return this->SelectRange<Analog>(range);
	case(GroupVariation::Group30Var1) :
		return this->SelectRangeUsing<Analog>(range, StaticAnalogResponse::Group30Var1);
	case(GroupVariation::Group30Var2) :
		return this->SelectRangeUsing<Analog>(range, StaticAnalogResponse::Group30Var2);
	case(GroupVariation::Group30Var3) :
		return this->SelectRangeUsing<Analog>(range, StaticAnalogResponse::Group30Var3);
	case(GroupVariation::Group30Var4) :
		return this->SelectRangeUsing<Analog>(range, StaticAnalogResponse::Group30Var4);
	case(GroupVariation::Group30Var5) :
		return this->SelectRangeUsing<Analog>(range, StaticAnalogResponse::Group30Var5);
	case(GroupVariation::Group30Var6) :
		return this->SelectRangeUsing<Analog>(range, StaticAnalogResponse::Group30Var6);

	case(GroupVariation::Group40Var0) :
		return this->SelectRange<AnalogOutputStatus>(range);
	case(GroupVariation::Group40Var1) :
		return this->SelectRangeUsing<AnalogOutputStatus>(range, StaticAnalogOutputStatusResponse::Group40Var1);
	case(GroupVariation::Group40Var2) :
		return this->SelectRangeUsing<AnalogOutputStatus>(range, StaticAnalogOutputStatusResponse::Group40Var2);
	case(GroupVariation::Group40Var3) :
		return this->SelectRangeUsing<AnalogOutputStatus>(range, StaticAnalogOutputStatusResponse::Group40Var3);
	case(GroupVariation::Group40Var4) :
		return this->SelectRangeUsing<AnalogOutputStatus>(range, StaticAnalogOutputStatusResponse::Group40Var4);

	case(GroupVariation::Group50Var4) :
		return this->SelectRangeUsing<TimeAndInterval>(range, StaticTimeAndIntervalResponse::Group50Var4);

	default:
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

bool DatabaseBuffers::Load(HeaderWriter& writer)
{
	typedef bool (DatabaseBuffers::*LoadFun)(HeaderWriter& writer);	

	std::array<LoadFun, 8> loaders{{
			&DatabaseBuffers::LoadType<Binary>,
			&DatabaseBuffers::LoadType<DoubleBitBinary>,
			&DatabaseBuffers::LoadType<Counter>,
			&DatabaseBuffers::LoadType<FrozenCounter>,
			&DatabaseBuffers::LoadType<Analog>,
			&DatabaseBuffers::LoadType<BinaryOutputStatus>,
			&DatabaseBuffers::LoadType<AnalogOutputStatus>,
			&DatabaseBuffers::LoadType < TimeAndInterval >
	}};

	for (LoadFun fun : loaders)
	{
		if (!(this->*fun)(writer))
		{
			// return early because the APDU is full
			return false;
		}		
	}	

	return true;
}

Range DatabaseBuffers::RangeOf(const HasSize<uint16_t>& sized)
{
	return sized.IsEmpty() ? Range::Invalid() : Range::From(0, sized.Size() - 1);
}


}

