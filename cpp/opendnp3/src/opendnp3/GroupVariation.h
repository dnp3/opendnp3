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
#ifndef __GROUP_VARIATION_H_
#define __GROUP_VARIATION_H_

#include <cstdint>

#include <opendnp3/Uncopyable.h>

namespace opendnp3
{

enum class GroupVariation : int
{	
	Group1Var1,
	Group1Var2,

	Group2Var0,
	Group2Var1,
	Group2Var2,
	Group2Var3,

	Group10Var1,
	Group10Var2,

	Group12Var1,

	Group20Var1,
	Group20Var2,
	Group20Var3,
	Group20Var4,
	Group20Var5,
	Group20Var6,
	Group20Var7,
	Group20Var8,

	Group21Var1,
	Group21Var2,
	Group21Var5,
	Group21Var6,
	Group21Var9,
	Group21Var10,

	Group22Var1,
	Group22Var2,
	Group22Var3,
	Group22Var4,
	Group22Var5,
	Group22Var6,
	Group22Var7,
	Group22Var8,

	Group30Var1,
	Group30Var2,
	Group30Var3,
	Group30Var4,
	Group30Var5,
	Group30Var6,

	Group32Var1,
	Group32Var2,
	Group32Var3,
	Group32Var4,
	Group32Var5,
	Group32Var6,
	Group32Var7,
	Group32Var8,

	Group40Var1,
	Group40Var2,
	Group40Var3,
	Group40Var4,

	Group41Var1,
	Group41Var2,
	Group41Var3,
	Group41Var4,

	Group50Var1,
	Group50Var2,

	Group52Var1,
	Group52Var2,

	Group60Var1,
	Group60Var2,
	Group60Var3,
	Group60Var4,

	Group80Var1,

	Group110AnyVar,
	Group111AnyVar,

	UNKNOWN
};

class GroupVariationRecord
{
	public:

	static GroupVariation GetEnum(uint8_t group, uint8_t variation);

	GroupVariation enumeration;
	uint8_t group;
	uint8_t variation;		
};

}

#endif
