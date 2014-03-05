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

GroupVariation GroupVariationRecord::GetEnum(uint8_t group, uint8_t variation)
{
	switch(group)
	{
		case(1):
			switch(variation)
			{
				case(0):
					return GroupVariation::Group1Var0;
				case(1):
					return GroupVariation::Group1Var1;	
				case(2):
					return GroupVariation::Group1Var2;
				default:
					return GroupVariation::UNKNOWN;
			}

		case(2):
			switch(variation)
			{
				case(0):
					return GroupVariation::Group2Var0;
				case(1):
					return GroupVariation::Group2Var1;
				case(2):
					return GroupVariation::Group2Var2;
				case(3):
					return GroupVariation::Group2Var3;
				default:
					return GroupVariation::UNKNOWN;
			}
		case(3) :
			switch (variation)
			{
				case(0) :
					return GroupVariation::Group3Var0;
				case(1) :
					return GroupVariation::Group3Var1;
				case(2) :
					return GroupVariation::Group3Var2;
				default:
					return GroupVariation::UNKNOWN;
			}
		case(4) :
			switch (variation)
			{
				case(0) :
					return GroupVariation::Group4Var0;
				case(1) :
					return GroupVariation::Group4Var1;
				case(2) :
					return GroupVariation::Group4Var2;
				case(3) :
					return GroupVariation::Group4Var3;
				default:
					return GroupVariation::UNKNOWN;
			}
		case(10):
			switch(variation)
			{
				case(0):
					return GroupVariation::Group10Var0;
				case(1):
					return GroupVariation::Group10Var1;
				case(2):
					return GroupVariation::Group10Var2;
				default:
					return GroupVariation::UNKNOWN;
			}
		case(12):
			switch(variation)
			{
				case(1):
					return GroupVariation::Group12Var1;			
				default:
					return GroupVariation::UNKNOWN;
			}
		case(20):
			switch(variation)
			{
				case(0):
					return GroupVariation::Group20Var0;
				case(1):
					return GroupVariation::Group20Var1;
				case(2):
					return GroupVariation::Group20Var2;				
				case(5):
					return GroupVariation::Group20Var5;
				case(6):
					return GroupVariation::Group20Var6;				
				default:
					return GroupVariation::UNKNOWN;
			}
		case(21):
			switch(variation)
			{
				case(0):
					return GroupVariation::Group21Var0;
				case(1):
					return GroupVariation::Group21Var1;
				case(2):
					return GroupVariation::Group21Var2;
				case(5):
					return GroupVariation::Group21Var5;
				case(6):
					return GroupVariation::Group21Var6;
				case(9):
					return GroupVariation::Group21Var9;
				case(10):
					return GroupVariation::Group21Var10;
				default:
					return GroupVariation::UNKNOWN;
			}
		case(22):
			switch(variation)
			{
				case(0):
					return GroupVariation::Group22Var0;
				case(1):
					return GroupVariation::Group22Var1;
				case(2):
					return GroupVariation::Group22Var2;
				case(3):
					return GroupVariation::Group22Var3;
				case(4):
					return GroupVariation::Group22Var4;
				case(5):
					return GroupVariation::Group22Var5;
				case(6):
					return GroupVariation::Group22Var6;
				case(7):
					return GroupVariation::Group22Var7;
				case(8):
					return GroupVariation::Group22Var8;
				default:
					return GroupVariation::UNKNOWN;
			}
		case(30):
			switch(variation)
			{
				case(0):
					return GroupVariation::Group30Var0;
				case(1):
					return GroupVariation::Group30Var1;
				case(2):
					return GroupVariation::Group30Var2;
				case(3):
					return GroupVariation::Group30Var3;
				case(4):
					return GroupVariation::Group30Var4;
				case(5):
					return GroupVariation::Group30Var5;
				case(6):
					return GroupVariation::Group30Var6;				
				default:
					return GroupVariation::UNKNOWN;
			}
		case(32):
			switch(variation)
			{
				case(0):
					return GroupVariation::Group32Var0;
				case(1):
					return GroupVariation::Group32Var1;
				case(2):
					return GroupVariation::Group32Var2;
				case(3):
					return GroupVariation::Group32Var3;
				case(4):
					return GroupVariation::Group32Var4;
				case(5):
					return GroupVariation::Group32Var5;
				case(6):
					return GroupVariation::Group32Var6;
				case(7):
					return GroupVariation::Group32Var7;
				case(8):
					return GroupVariation::Group32Var8;			
				default:
					return GroupVariation::UNKNOWN;
			}
		case(40):
			switch(variation)
			{
				case(0):
					return GroupVariation::Group40Var0;
				case(1):
					return GroupVariation::Group40Var1;
				case(2):
					return GroupVariation::Group40Var2;
				case(3):
					return GroupVariation::Group40Var3;
				case(4):
					return GroupVariation::Group40Var4;
				default:
					return GroupVariation::UNKNOWN;
			}
		case(41):
			switch(variation)
			{
				case(0):
					return GroupVariation::Group41Var0;
				case(1):
					return GroupVariation::Group41Var1;
				case(2):
					return GroupVariation::Group41Var2;
				case(3):
					return GroupVariation::Group41Var3;
				case(4):
					return GroupVariation::Group41Var4;
				default:
					return GroupVariation::UNKNOWN;
			}
		case(50):
			switch(variation)
			{
				case(1):
					return GroupVariation::Group50Var1;
				case(2):
					return GroupVariation::Group50Var2;
			}
		case(52):
			switch(variation)
			{
				case(1):
					return GroupVariation::Group52Var1;
				case(2):
					return GroupVariation::Group52Var2;
			}
		case(60):
			switch(variation)
			{
				case(1):
					return GroupVariation::Group60Var1;
				case(2):
					return GroupVariation::Group60Var2;
				case(3):
					return GroupVariation::Group60Var3;
				case(4):
					return GroupVariation::Group60Var4;			
				default:
					return GroupVariation::UNKNOWN;
			}
		case(80):
			switch(variation)
			{
				case(1):
					return GroupVariation::Group80Var1;				
				default:
					return GroupVariation::UNKNOWN;
			}
		case(110):
			return GroupVariation::Group110AnyVar;
		case(111):
			return GroupVariation::Group111AnyVar;
		default:
			return GroupVariation::UNKNOWN;
	}	
}

} //end ns
