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

	Group12Var1,

	Group22Var1,
	Group22Var2,
	Group22Var3,
	Group22Var4,
	Group22Var5,
	Group22Var6,
	Group22Var7,
	Group22Var8,

	UNKNOWN
};

class GroupVariationEnum : private PureStatic
{
	public:

	static GroupVariation Get(uint8_t group, uint8_t variation);	
		
};

}

#endif
