
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
#ifndef __OBJECT_HEADER_H_
#define __OBJECT_HEADER_H_

#include <opendnp3/Types.h>
#include <opendnp3/Singleton.h>
#include <opendnp3/Exception.h>
#include <opendnp3/Location.h>
#include <opendnp3/APDUConstants.h>

#include "PackingUnpacking.h"

#include <limits>
#include <sstream>
#include <assert.h>

//irratating windows macro interferes with the <limits> numeric_limits<T>::max()
#undef max

namespace opendnp3
{

class ObjectBase;

enum ObjectHeaderMasks {
	OHM_OBJECT_PREFIX = 0x70,
	OHM_RANGE_SPECIFIER = 0x0F
};

struct ObjectHeaderField {
	ObjectHeaderField() {};
	ObjectHeaderField(uint8_t aGroup, uint8_t aVariation, QualifierCode aQualifier) :
		Group(aGroup),
		Variation(aVariation),
		Qualifier(aQualifier)
	{}

	uint8_t Group;
	uint8_t Variation;
	QualifierCode Qualifier;
};

enum ObjectHeaderTypes {
	OHT_ALL_OBJECTS,
	OHT_RANGED_2_OCTET,
	OHT_RANGED_4_OCTET,
	OHT_RANGED_8_OCTET,
	OHT_COUNT_1_OCTET,
	OHT_COUNT_2_OCTET,
	OHT_COUNT_4_OCTET
};

class IObjectHeader
{
public:
	virtual ~IObjectHeader() {}
	virtual size_t GetSize() const = 0; // depends on the subtype, default size is 3
	virtual ObjectHeaderTypes GetType() const = 0;

	void Get(const uint8_t* apStart, ObjectHeaderField& arData) const;
	void Set(uint8_t* apStart, uint8_t aGrp, uint8_t aVar, QualifierCode aQual) const;

	virtual std::string ToString(const uint8_t* apStart) const = 0;

	static QualifierCode ByteToQualifierCode(uint8_t aCode);
};

struct RangeInfo {
	size_t Start;
	size_t Stop;
};

class IRangeHeader : public IObjectHeader
{
public:
	virtual void GetRange(const uint8_t* apStart, RangeInfo& arInfo) const = 0;
	virtual void SetRange(uint8_t* apStart, const RangeInfo& arInfo) const = 0;
};

class ICountHeader : public IObjectHeader
{
public:
	virtual size_t GetCount(const uint8_t* apStart) const = 0;
	virtual void SetCount(uint8_t* apStart, size_t aCount) const = 0;
};

class AllObjectsHeader : public IObjectHeader
{
	MACRO_SINGLETON_INSTANCE(AllObjectsHeader)

	size_t GetSize() const {
		return 3;
	}
	ObjectHeaderTypes GetType() const {
		return OHT_ALL_OBJECTS;
	}

	virtual std::string ToString(const uint8_t* apStart) const
	{
		return "All Objects";
	}
};

template <class T, ObjectHeaderTypes U>
class RangedHeader : public IRangeHeader
{
	MACRO_SINGLETON_INSTANCE(RangedHeader)

	size_t GetSize() const {
		return Size;
	}
	ObjectHeaderTypes GetType() const {
		return U;
	}

	void GetRange(const uint8_t* apStart, RangeInfo& arInfo) const {
		arInfo.Start = T::Read(apStart + 3);
		arInfo.Stop = T::Read(apStart + 3 + T::Size);
	}

	void SetRange(uint8_t* apStart, const RangeInfo& arInfo) const {
		if(arInfo.Start > arInfo.Stop) throw ArgumentException(LOCATION, "stop > start");
		if(arInfo.Stop > T::Max) throw ArgumentException(LOCATION, "stop > max");

		T::Write(apStart + 3, static_cast<typename T::Type>(arInfo.Start));
		T::Write(apStart + 3 + T::Size, static_cast<typename T::Type>(arInfo.Stop));
	}

	virtual std::string ToString(const uint8_t* apStart) const
	{
		std::ostringstream oss;
		RangeInfo ri;
		this->GetRange(apStart, ri);
		oss << "Start: " << ri.Start << " Stop: " << ri.Stop;
		return oss.str();
	}

	static size_t MaxRange() {
		return T::Max;
	}

	const static size_t Size = 3 + 2 * T::Size;
};

template <class T, ObjectHeaderTypes U>
RangedHeader<T, U> RangedHeader<T, U>::mInstance;

template <class T, ObjectHeaderTypes U>
class CountHeader : public ICountHeader
{
	MACRO_SINGLETON_INSTANCE(CountHeader)

	size_t GetSize() const {
		return Size;
	}
	ObjectHeaderTypes GetType() const {
		return U;
	}

	size_t GetCount(const uint8_t* apStart) const {
		return T::Read(apStart + 3);
	}

	void SetCount(uint8_t* apStart, size_t aCount) const {
		if(aCount > T::Max) throw ArgumentException(LOCATION);
		T::Write(apStart + 3, static_cast<typename T::Type>(aCount));
	}

	static size_t MaxCount() {
		return T::Max;
	}

	virtual std::string ToString(const uint8_t* apStart) const
	{
		std::ostringstream oss;		
		oss << "Count: " << this->GetCount(apStart);
		return oss.str();
	}

	const static size_t Size = 3 + T::Size;
};

template <class T, ObjectHeaderTypes U>
CountHeader<T, U> CountHeader<T, U>::mInstance;

//Typedefs so you don't have to direcly use the templates
typedef RangedHeader<UInt8, OHT_RANGED_2_OCTET>	Ranged2OctetHeader;
typedef RangedHeader<UInt16LE, OHT_RANGED_4_OCTET> Ranged4OctetHeader;
typedef RangedHeader<UInt32LE, OHT_RANGED_8_OCTET> Ranged8OctetHeader;

typedef CountHeader<UInt8, OHT_COUNT_1_OCTET>		Count1OctetHeader;
typedef CountHeader<UInt16LE, OHT_COUNT_2_OCTET>	Count2OctetHeader;
typedef CountHeader<UInt32LE, OHT_COUNT_4_OCTET>	Count4OctetHeader;

}

#endif

