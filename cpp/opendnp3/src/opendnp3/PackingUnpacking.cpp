
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
// you under the terms of the License.
//

#include "PackingUnpacking.h"


#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include <opendnp3/Util.h>
#include <memory>

namespace opendnp3
{
const uint8_t UInt8::Max = std::numeric_limits<uint8_t>::max();
const uint8_t UInt8::Min = std::numeric_limits<uint8_t>::min();


/* ARM has a word alignment issue when using reinterpret cast. The float/double read/write routines
now uses an intermediate buffer that the compiler word aligns. */
float SingleFloat::Read(const uint8_t* apStart)
{
	return Float<float>::SafeRead(apStart);
}

void SingleFloat::Write(uint8_t* apStart, float aValue)
{
	Float<float>::SafeWrite(apStart, aValue);
}

double DoubleFloat::Read(const uint8_t* apStart)
{
	return Float<double>::SafeRead(apStart);
}

void DoubleFloat::Write(uint8_t* apStart, double aValue)
{
	Float<double>::SafeWrite(apStart, aValue);
}

#if OPENDNP3_ARM_FLOAT_WORKAROUND
double DoubleFloat::FlipWord32(double aValue)
{
	volatile double x = aValue;
	volatile uint8_t* p = reinterpret_cast<volatile uint8_t*>(&x);
	uint32_t tmp = p[0];
	p[0] = p[1];
	p[1] = tmp;
	return x;
}
#endif

}
