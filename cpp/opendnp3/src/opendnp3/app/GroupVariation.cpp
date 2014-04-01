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

#include "GroupVariation.h"

#include <sstream>

namespace opendnp3
{

std::string GroupVariationRecord::ToString() const
{
	std::ostringstream oss;
	oss << "Group" << static_cast<int>(group) << "Variation" << static_cast<int>(variation);
	return oss.str();
}

GroupVariationRecord::GroupVariationRecord(uint8_t group_, uint8_t variation_, GroupVariation enumeration_, GroupVariationType type_) :
	enumeration(enumeration_),
	type(type_),
	group(group_),
	variation(variation_)
{

}

HeaderRecord::HeaderRecord(const GroupVariationRecord& gv, QualifierCode qualifier_) : GroupVariationRecord(gv), qualifier(qualifier_)
{
}

GroupVariationRecord GroupVariationRecord::GetRecord(uint8_t group, uint8_t variation)
{
	auto pair = GetEnumAndType(group, variation);
	return GroupVariationRecord(group, variation, pair.enumeration, pair.type);
}

EnumAndType GroupVariationRecord::GetEnumAndType(uint8_t group, uint8_t variation)
{
	switch(group)
	{
	case(1):
		switch(variation)
		{
		case(0) :
			return EnumAndType(GroupVariation::Group1Var0, GroupVariationType::STATIC);
		case(1):
			return EnumAndType(GroupVariation::Group1Var1, GroupVariationType::STATIC);
		case(2):
			return EnumAndType(GroupVariation::Group1Var2, GroupVariationType::STATIC);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(2):
		switch(variation)
		{
		case(0):
			return EnumAndType(GroupVariation::Group2Var0, GroupVariationType::EVENT);
		case(1):
			return EnumAndType(GroupVariation::Group2Var1, GroupVariationType::EVENT);
		case(2):
			return EnumAndType(GroupVariation::Group2Var2, GroupVariationType::EVENT);
		case(3):
			return EnumAndType(GroupVariation::Group2Var3, GroupVariationType::EVENT);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(3) :
		switch (variation)
		{
		case(0) :
			return EnumAndType(GroupVariation::Group3Var0, GroupVariationType::STATIC);
		case(1) :
			return EnumAndType(GroupVariation::Group3Var1, GroupVariationType::STATIC);
		case(2) :
			return EnumAndType(GroupVariation::Group3Var2, GroupVariationType::STATIC);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(4) :
		switch (variation)
		{
		case(0) :
			return EnumAndType(GroupVariation::Group4Var0, GroupVariationType::EVENT);
		case(1) :
			return EnumAndType(GroupVariation::Group4Var1, GroupVariationType::EVENT);
		case(2) :
			return EnumAndType(GroupVariation::Group4Var2, GroupVariationType::EVENT);
		case(3) :
			return EnumAndType(GroupVariation::Group4Var3, GroupVariationType::EVENT);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(10):
		switch(variation)
		{
		case(0):
			return EnumAndType(GroupVariation::Group10Var0, GroupVariationType::STATIC);
		case(1):
			return EnumAndType(GroupVariation::Group10Var1, GroupVariationType::STATIC);
		case(2):
			return EnumAndType(GroupVariation::Group10Var2, GroupVariationType::STATIC);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(11) :
		switch (variation)
		{
			case(0) :
				return EnumAndType(GroupVariation::Group11Var0, GroupVariationType::EVENT);
			case(1) :
				return EnumAndType(GroupVariation::Group11Var1, GroupVariationType::EVENT);
			case(2) :
				return EnumAndType(GroupVariation::Group11Var2, GroupVariationType::EVENT);
			default:
				return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(12):
		switch(variation)
		{
		case(1):
			return EnumAndType(GroupVariation::Group12Var1, GroupVariationType::OTHER);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(20):
		switch(variation)
		{
		case(0):
			return EnumAndType(GroupVariation::Group20Var0, GroupVariationType::STATIC);
		case(1):
			return EnumAndType(GroupVariation::Group20Var1, GroupVariationType::STATIC);
		case(2):
			return EnumAndType(GroupVariation::Group20Var2, GroupVariationType::STATIC);
		case(5):
			return EnumAndType(GroupVariation::Group20Var5, GroupVariationType::STATIC);
		case(6):
			return EnumAndType(GroupVariation::Group20Var6, GroupVariationType::STATIC);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(21):
		switch(variation)
		{
		case(0):
			return EnumAndType(GroupVariation::Group21Var0, GroupVariationType::STATIC);
		case(1):
			return EnumAndType(GroupVariation::Group21Var1, GroupVariationType::STATIC);
		case(2):
			return EnumAndType(GroupVariation::Group21Var2, GroupVariationType::STATIC);
		case(5):
			return EnumAndType(GroupVariation::Group21Var5, GroupVariationType::STATIC);
		case(6):
			return EnumAndType(GroupVariation::Group21Var6, GroupVariationType::STATIC);
		case(9):
			return EnumAndType(GroupVariation::Group21Var9, GroupVariationType::STATIC);
		case(10):
			return EnumAndType(GroupVariation::Group21Var10, GroupVariationType::STATIC);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(22):
		switch(variation)
		{
		case(0):
			return EnumAndType(GroupVariation::Group22Var0, GroupVariationType::EVENT);
		case(1):
			return EnumAndType(GroupVariation::Group22Var1, GroupVariationType::EVENT);
		case(2):
			return EnumAndType(GroupVariation::Group22Var2, GroupVariationType::EVENT);
		case(5):
			return EnumAndType(GroupVariation::Group22Var5, GroupVariationType::EVENT);
		case(6):
			return EnumAndType(GroupVariation::Group22Var6, GroupVariationType::EVENT);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(23) :
		switch (variation)
		{
		case(0) :
			return EnumAndType(GroupVariation::Group23Var0, GroupVariationType::EVENT);
		case(1) :
			return EnumAndType(GroupVariation::Group23Var1, GroupVariationType::EVENT);
		case(2) :
			return EnumAndType(GroupVariation::Group23Var2, GroupVariationType::EVENT);
		case(5) :
			return EnumAndType(GroupVariation::Group23Var5, GroupVariationType::EVENT);
		case(6) :
			return EnumAndType(GroupVariation::Group23Var6, GroupVariationType::EVENT);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(30):
		switch(variation)
		{
		case(0):
			return EnumAndType(GroupVariation::Group30Var0, GroupVariationType::STATIC);
		case(1):
			return EnumAndType(GroupVariation::Group30Var1, GroupVariationType::STATIC);
		case(2):
			return EnumAndType(GroupVariation::Group30Var2, GroupVariationType::STATIC);
		case(3):
			return EnumAndType(GroupVariation::Group30Var3, GroupVariationType::STATIC);
		case(4):
			return EnumAndType(GroupVariation::Group30Var4, GroupVariationType::STATIC);
		case(5):
			return EnumAndType(GroupVariation::Group30Var5, GroupVariationType::STATIC);
		case(6):
			return EnumAndType(GroupVariation::Group30Var6, GroupVariationType::STATIC);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(32):
		switch(variation)
		{
		case(0) :
			return EnumAndType(GroupVariation::Group32Var0, GroupVariationType::EVENT);
		case(1):
			return EnumAndType(GroupVariation::Group32Var1, GroupVariationType::EVENT);
		case(2):
			return EnumAndType(GroupVariation::Group32Var2, GroupVariationType::EVENT);
		case(3):
			return EnumAndType(GroupVariation::Group32Var3, GroupVariationType::EVENT);
		case(4):
			return EnumAndType(GroupVariation::Group32Var4, GroupVariationType::EVENT);
		case(5):
			return EnumAndType(GroupVariation::Group32Var5, GroupVariationType::EVENT);
		case(6):
			return EnumAndType(GroupVariation::Group32Var6, GroupVariationType::EVENT);
		case(7):
			return EnumAndType(GroupVariation::Group32Var7, GroupVariationType::EVENT);
		case(8):
			return EnumAndType(GroupVariation::Group32Var8, GroupVariationType::EVENT);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(40):
		switch(variation)
		{
		case(0) :
			return EnumAndType(GroupVariation::Group40Var0, GroupVariationType::STATIC);
		case(1):
			return EnumAndType(GroupVariation::Group40Var1, GroupVariationType::STATIC);
		case(2):
			return EnumAndType(GroupVariation::Group40Var2, GroupVariationType::STATIC);
		case(3):
			return EnumAndType(GroupVariation::Group40Var3, GroupVariationType::STATIC);
		case(4):
			return EnumAndType(GroupVariation::Group40Var4, GroupVariationType::STATIC);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(41):
		switch(variation)
		{
		case(0):
			return EnumAndType(GroupVariation::Group41Var0, GroupVariationType::EVENT);
		case(1):
			return EnumAndType(GroupVariation::Group41Var1, GroupVariationType::EVENT);
		case(2):
			return EnumAndType(GroupVariation::Group41Var2, GroupVariationType::EVENT);
		case(3):
			return EnumAndType(GroupVariation::Group41Var3, GroupVariationType::EVENT);
		case(4):
			return EnumAndType(GroupVariation::Group41Var4, GroupVariationType::EVENT);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(42) :
		switch (variation)
		{
			case(0) :
				return EnumAndType(GroupVariation::Group42Var0, GroupVariationType::EVENT);
			case(1) :
				return EnumAndType(GroupVariation::Group42Var1, GroupVariationType::EVENT);
			case(2) :
				return EnumAndType(GroupVariation::Group42Var2, GroupVariationType::EVENT);
			case(3) :
				return EnumAndType(GroupVariation::Group42Var3, GroupVariationType::EVENT);
			case(4) :
				return EnumAndType(GroupVariation::Group42Var4, GroupVariationType::EVENT);
			case(5) :
				return EnumAndType(GroupVariation::Group42Var5, GroupVariationType::EVENT);
			case(6) :
				return EnumAndType(GroupVariation::Group42Var6, GroupVariationType::EVENT);
			case(7) :
				return EnumAndType(GroupVariation::Group42Var7, GroupVariationType::EVENT);
			case(8) :
				return EnumAndType(GroupVariation::Group42Var8, GroupVariationType::EVENT);
			default:
				return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(50):
		switch(variation)
		{
		case(1):
			return EnumAndType(GroupVariation::Group50Var1, GroupVariationType::OTHER);
		case(2):
			return EnumAndType(GroupVariation::Group50Var2, GroupVariationType::OTHER);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(51) :
		switch (variation)
		{
		case(1) :
			return EnumAndType(GroupVariation::Group51Var1, GroupVariationType::OTHER);
		case(2) :
			return EnumAndType(GroupVariation::Group51Var2, GroupVariationType::OTHER);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);

		}
	case(52):
		switch(variation)
		{
		case(1):
			return EnumAndType(GroupVariation::Group52Var1, GroupVariationType::OTHER);
		case(2):
			return EnumAndType(GroupVariation::Group52Var2, GroupVariationType::OTHER);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);

		}
	case(60):
		switch(variation)
		{
		case(1):
			return EnumAndType(GroupVariation::Group60Var1, GroupVariationType::STATIC);
		case(2):
			return EnumAndType(GroupVariation::Group60Var2, GroupVariationType::EVENT);
		case(3):
			return EnumAndType(GroupVariation::Group60Var3, GroupVariationType::EVENT);
		case(4):
			return EnumAndType(GroupVariation::Group60Var4, GroupVariationType::EVENT);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(80):
		switch(variation)
		{
		case(1):
			return EnumAndType(GroupVariation::Group80Var1, GroupVariationType::OTHER);
		default:
			return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
		}
	case(110):
		return EnumAndType(GroupVariation::Group110AnyVar, GroupVariationType::STATIC);
	case(111):
		return EnumAndType(GroupVariation::Group111AnyVar, GroupVariationType::EVENT);
	default:
		return EnumAndType(GroupVariation::UNKNOWN, GroupVariationType::OTHER);
	}
}

} //end ns
