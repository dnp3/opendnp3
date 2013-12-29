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
#ifndef __OBJECT_INTERFACES_H_
#define __OBJECT_INTERFACES_H_

#include <assert.h>
#include <stddef.h>
#include <cstring>
#include <string>
#include <cstdint>

/*
 * Object hierarchy
 *
 *               +--- PlaceHolderObject
 *               |
 *               |                     +--- FixedObject --- StreamObject<T>
 *               |                     |
 * ObjectBase ---+--- IndexedObject ---+--- BitFieldObject
 *               |                     |
 *               |                     +--- SizeByVariationObject
 *               |
 *               +--- VariableObject
 *
 */
namespace opendnp3
{

class VTOData;
class CopyableBuffer;

/**
 * An enumeration for all of the concrete class types in the ObjectBase
 * hierarchy.
 */
enum ObjectTypes {
	OT_PLACEHOLDER,
	OT_FIXED,
	OT_BITFIELD,
	OT_VARIABLE,
	OT_SIZE_BY_VARIATION
};

/**
 * A class that represents a very generic DNP3 object.  All DNP3 objects
 * must implement some common functionality, such as the group and
 * variation identifiers.  This class provides those common functions.
 */
class ObjectBase
{
public:

	/**
	 * A default destructor for the ObjectBase subclasses.
	 */
	virtual ~ObjectBase() {}

	/**
	 * Returns ObjectTypes code that corresponds with the ObjectBase
	 * subclass.
	 *
	 * @return			the appropriate code from the ObjectTypes
	 *                  enumeration that correspond with the
	 *                  ObjectBase subclass
	 */
	virtual ObjectTypes GetType() const = 0;

	/**
	 * Returns the DNP3 group identifier for the object.
	 *
	 * @return			the DNP3 group identifier for the object
	 */
	virtual int GetGroup() const = 0;

	/**
	 * Returns the DNP3 variation identifier for the object.
	 *
	 * @return			the DNP3 variation identifier for the object.
	 */
	virtual int GetVariation() const = 0;

	virtual bool UseCTO() const {
		return false;
	}

	virtual bool IsEvent() const {
		return false;
	}

	/**
	 * Returns an instance of the appropriate ObjectBase subclass that
	 * corresponds with the given DNP3 group and variation
	 * identifiers.
	 *
	 * @param aGroup		the DNP3 group identifier
	 * @param aVariation	the DNP3 variation identifier
	 *
	 * @return				an instance of the ObjectBase subclass
	 *                      that represents the group and variation
	 */
	static ObjectBase* Get(int aGroup, int aVariation);

	/**
	 * Determines object equality based on the DNP3 group and
	 * variation identifiers.
	 *
	 * @param pObj		the ObjectBase instance with which to compare
	 * 					the current object
	 *
	 * @return			true if the group and variation identifiers
	 * 					are equal, false if not
	 */
	bool Equals(ObjectBase* pObj) const {
		return (pObj->GetGroup() == this->GetGroup() &&
		        pObj->GetVariation() == this->GetVariation());
	}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	/**
	 * Returns the name of the ObjectBase subclass as a string.
	 *
	 * @return			the name of the ObjectBase subclass
	 */
	virtual std::string Name() const = 0;
#endif

};

/**
 * A common base class for all objects that must have an index associated
 * with them.
 */
class IndexedObject : public ObjectBase
{
	/* Nothing specific to define with this object */
};

/**
 * A dummy object that acts like a placeholder for a group/variation
 * identifier.  This is useful to parse messages that have headers defined
 * which are not necessarily supported at this time.
 */
class PlaceHolderObject : public ObjectBase
{
public:

	/**
	 * Implements ObjectBase::GetType() to return OT_PLACEHOLDER.
	 *
	 * @return			OT_PLACEHOLDER
	 */
	virtual ObjectTypes GetType() const {
		return OT_PLACEHOLDER;
	}

	/**
	 * Implements ObjectBase::GetSize() to return 0.
	 *
	 * @return			zero (0)
	 */
	size_t GetSize() {
		return 0;
	}

};

class FixedObject : public IndexedObject
{
public:

	/**
	 * Implements ObjectBase::GetType() to return OT_FIXED.
	 *
	 * @return			OT_FIXED
	 */
	virtual ObjectTypes GetType() const {
		return OT_FIXED;
	}

	virtual size_t GetSize() const = 0;

};

template <class T>
class StreamObject : public FixedObject
{
public:

	virtual void Write(uint8_t* apPos, const T&) const = 0;

	virtual T Read(const uint8_t*) const = 0;

	virtual bool HasQuality() const {
		return false;
	}

	typedef T DataType;
};

template <class T>
class CommandObject : public StreamObject<T>
{
public:

	virtual CopyableBuffer GetValueBytes(const uint8_t*) const = 0;

};

class BitfieldObject : public IndexedObject
{
public:

	virtual ObjectTypes GetType() const {
		return OT_BITFIELD;
	}

	size_t GetSize(size_t aNumValues) const {
		/* Integer division by 8 */
		size_t ret = (aNumValues >> 3);

		/*
		 * If it's not an even multiple of 8, add an extra byte
		 */
		if ((aNumValues & 0x07) != 0) {
			++ret;
		}

		return ret;
	}

	void Zero(uint8_t* apPos, size_t aNumValues) const {
		size_t num_bytes = GetSize(aNumValues);

		for (size_t i = 0; i < num_bytes; i++) {
			*(apPos++) = 0;
		}
	}

	bool Read(const uint8_t* apPos, size_t aStartIndex, size_t aIndex) const {
		return StaticRead(apPos, aStartIndex, aIndex);
	}

	void Write(uint8_t* apPos, size_t aStartIndex, size_t aIndex, bool aValue) const {
		StaticWrite(apPos, aStartIndex, aIndex, aValue);
	}

	static bool StaticRead(const uint8_t* apPos, size_t aStartIndex, size_t aIndex) {
		assert(aIndex >= aStartIndex);

		size_t pos =  aIndex - aStartIndex;

		apPos += (pos >> 3);	/* figure out which byte you are */
		/* on and advance the pointer    */

		return ((*apPos) & (1 << (pos & 0x07))) != 0;
	}

	static void StaticWrite(uint8_t* apPos, size_t aStartIndex, size_t aIndex, bool aValue) {
		assert(aIndex >= aStartIndex);

		size_t pos =  aIndex - aStartIndex;

		apPos += (pos >> 3);	/* figure out which byte you are */
		/* on and advance the pointer    */

		size_t bit_mask = 1 << (pos & 0x07);

		if (aValue) {
			*apPos |= bit_mask;
		}
		else {
			*apPos &= ~bit_mask;
		}
	}
};

/**
 * Represents a byte stream object whose size is encoded by its
 * variation.  As per IEEE 1815 "Standard for Electric Power Systems
 * Communications - Distributed Network Protocol (DNP3)", Annex A "DNP3
 * Object Library", page 429, the only DNP3 object types that support the
 * "size-by-variation" feature are 100 (floating-point), 110 (octet
 * string), 111 (octet string event), 112 (virtual terminal output block),
 * and 113 (virtual terminal event data).
 *
 */
class SizeByVariationObject : public IndexedObject
{
public:

	/**
	 * Implements ObjectBase::GetType() to return
	 * OT_SIZE_BY_VARIATION.
	 *
	 * @return			OT_SIZE_BY_VARIATION
	 */
	virtual ObjectTypes GetType() const {
		return OT_SIZE_BY_VARIATION;
	}

	/**
	 * Copies the specified number of bytes (defined by aVariation)
	 * from apPos to apOut.
	 *
	 * @param apPos			the source byte stream
	 * @param aVariation	the number of bytes to copy
	 * @param apOut			the destination byte stream
	 *
	 * @return				true if the copy succeeds, false if not
	 */
	bool Read(const uint8_t* apPos, size_t aVariation, uint8_t* apOut) const {
		assert(aVariation <= 255);
		memcpy(apOut, apPos, aVariation);
		return true;
	}

	/**
	 * Copies the specified number of bytes (defined by aVariation)
	 * from apIn to apPos.
	 *
	 * @param apPos			the destination byte stream
	 * @param aVariation	the number of bytes to copy
	 * @param apIn			the source byte stream
	 *
	 * @return				true if the copy succeeds, false if not
	 */
	void Write(uint8_t* apPos, size_t aVariation, const uint8_t* apIn) const {
		assert(aVariation <= 255);
		memcpy(apPos, apIn, aVariation);
	}

};

}



#endif

