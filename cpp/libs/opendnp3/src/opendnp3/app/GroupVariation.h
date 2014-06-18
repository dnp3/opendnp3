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

#include <openpal/Uncopyable.h>

#include "opendnp3/gen/QualifierCode.h"

namespace opendnp3
{

enum class GroupVariationType : int
{
    STATIC,
    EVENT,
    OTHER
};

enum class GroupVariation : int
{
    Group1Var0,
    Group1Var1,
    Group1Var2,

    Group2Var0,
    Group2Var1,
    Group2Var2,
    Group2Var3,

    Group3Var0,
    Group3Var1,
    Group3Var2,

    Group4Var0,
    Group4Var1,
    Group4Var2,
    Group4Var3,

    Group10Var0,
    Group10Var1,
    Group10Var2,

    Group11Var0,
    Group11Var1,
    Group11Var2,

    Group12Var1,

    Group20Var0,
    Group20Var1,
    Group20Var2,
    Group20Var5,
    Group20Var6,

    Group21Var0,
    Group21Var1,
    Group21Var2,
    Group21Var5,
    Group21Var6,
    Group21Var9,
    Group21Var10,

    Group22Var0,
    Group22Var1,
    Group22Var2,
    Group22Var5,
    Group22Var6,

    Group23Var0,
    Group23Var1,
    Group23Var2,
    Group23Var5,
    Group23Var6,

    Group30Var0,
    Group30Var1,
    Group30Var2,
    Group30Var3,
    Group30Var4,
    Group30Var5,
    Group30Var6,

    Group32Var0,
    Group32Var1,
    Group32Var2,
    Group32Var3,
    Group32Var4,
    Group32Var5,
    Group32Var6,
    Group32Var7,
    Group32Var8,

    Group40Var0,
    Group40Var1,
    Group40Var2,
    Group40Var3,
    Group40Var4,

    Group41Var0,
    Group41Var1,
    Group41Var2,
    Group41Var3,
    Group41Var4,

    Group42Var0,
    Group42Var1,
    Group42Var2,
    Group42Var3,
    Group42Var4,
    Group42Var5,
    Group42Var6,
    Group42Var7,
    Group42Var8,

    Group50Var1,
    Group50Var2,

    Group51Var1,
    Group51Var2,

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

struct EnumAndType
{
	EnumAndType(GroupVariation enumeration_, GroupVariationType type_) :
		enumeration(enumeration_), type(type_)
	{}

	GroupVariation enumeration;
	GroupVariationType type;
};

class GroupVariationRecord
{

public:

	static EnumAndType GetEnumAndType(uint8_t group, uint8_t variation);

	static GroupVariationRecord GetRecord(uint8_t group, uint8_t variation);

	GroupVariationRecord(uint8_t group_, uint8_t variation_, GroupVariation enumeration_, GroupVariationType type_);

	GroupVariation enumeration;
	GroupVariationType type;
	uint8_t group;
	uint8_t variation;
};

class HeaderRecord : public GroupVariationRecord
{
public:

	HeaderRecord(const GroupVariationRecord& gv, QualifierCode qualifier_);

	QualifierCode qualifier;
};

}

#endif
