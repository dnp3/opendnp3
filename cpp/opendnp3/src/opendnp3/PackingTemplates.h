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
#ifndef __PACKING_TEMPLATES_H_
#define __PACKING_TEMPLATES_H_

#include <openpal/Location.h>

#include <assert.h>

namespace opendnp3
{

template <class T, int U>
class Pack
{
public:
	typename T::Type Get(const uint8_t* apBuffer) const {
		return T::Read(apBuffer + U);
	}

	void Set(uint8_t* apBuffer, typename T::Type aValue) const {
		T::Write(apBuffer + U, aValue);
	}

	typename T::Type Max() const {
		return T::Max;
	}

	typename T::Type Min() const {
		return T::Min;
	}
};

template <class T, int U, uint8_t V>
class PackOverRange : public Pack<T, U>
{
public:
	uint8_t OverRangeMask() const {
		return V;
	}
};

class BitfieldPacker
{
public:
	size_t GetSize(size_t aNumValues) const {
		size_t ret = (aNumValues >> 3); //integer division by eight
		if( (aNumValues & 0x07) != 0) ++ret; //if it's not an even multiple of 8 add an extra byte
		return ret;
	}

	void Zero(uint8_t* apPos, size_t aNumValues) const {
		size_t num_bytes = GetSize(aNumValues);
		for(size_t i = 0; i < num_bytes; i++)
			*(apPos++) = 0;
	}

	bool Read(const uint8_t* apPos, size_t aStartIndex, size_t aIndex) const {
		assert(aIndex >= aStartIndex);
		size_t pos =  aIndex - aStartIndex;
		apPos += (pos >> 3); //figure out which byte you are on and advance the pointer
		return ((*apPos) & (1 << (pos & 0x07))) != 0;
	}

	void Write(uint8_t* apPos, size_t aStartIndex, size_t aIndex, bool aValue) const {
		assert(aIndex >= aStartIndex);
		size_t pos =  aIndex - aStartIndex;
		apPos += (pos >> 3); //figure out which byte you are on and advance the pointer
		size_t bit_mask = 1 << (pos & 0x07);
		if (aValue) *apPos |= bit_mask;
		else *apPos &= ~bit_mask;
	}
};

}

#endif

