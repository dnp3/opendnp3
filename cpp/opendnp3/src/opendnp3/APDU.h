
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

#ifndef __APDU_H_
#define __APDU_H_

#include <opendnp3/APDUConstants.h>

#include <openpal/Exception.h>
#include <openpal/Location.h>
#include <openpal/Visibility.h>

#include "CopyableBuffer.h"
#include "AppHeader.h"
#include "ObjectHeader.h"
#include "Objects.h"

#include "HeaderReadIterator.h"
#include "ObjectWriteIterator.h"
#include "IndexedWriteIterator.h"
#include "ErrorCode.h"

#include <vector>
#include <string>
#include <sstream>

namespace opendnp3
{

/**
	A class that represents a DNP3 Application Layer message, providing
	interfaces to read and write the Application Data Unit (APDU).
	Interpret method and read iterators form the read interface, while Set
	method and object write iterators form the write interface.
*/
class DLL_LOCAL APDU
{

public:

	/**
		Create a new APDU instance.

		@param aFragSize		the size of the fragment

		@return					the new APDU instance
	 */
	APDU(size_t aFragSize = DEFAULT_FRAG_SIZE);

	/**
		Parse and validate the entire currently-set buffer.

		@throw Exception		if malformed data is encountered
	 */
	void Interpret();

	/**
		Parse and validate the only the header components of the
		currently-set buffer.

		@throw Exception		if malformed data is encountered
	 */
	void InterpretHeader();

	/**
		Returns the current fragment size.

		@return		the current fragment size
	 */
	size_t Size() const {
		return mFragmentSize;
	}

	/**
		Returns a byte pointer to the current buffer.

		@return		a pointer to the current byte buffer
	 */
	const uint8_t* GetBuffer() const {
		return mBuffer;
	}

	/**
		Returns the maximum possible size of the fragment (in bytes).

		@return		the maximum possible size of the fragment
	 */
	size_t MaxSize() {
		return mBuffer.Size();
	}

	/**
		Reset the buffer to be empty.
	 */
	void Reset();

	/**
		Reset the buffer to be empty and then write the provided byte
		stream into the buffer.  aLength must be less than MaxSize().

		@param apStart				the new byte stream to use
		@param aLength				the length of the byte stream
									provided

		@throw ArgumentException	if aLength exceeds MaxSize()
	 */
	void Write(const uint8_t* apStart, size_t aLength);

	/**
		Returns the Function Code (FC) field from the DNP3 Application
		Layer message.

		@return		the function code of the APDU message
	 */
	FunctionCode GetFunction() const;

	/**
		Sets the Function Code (FC) field of the DNP3 Application Layer
		message.

		@param aCode	the new function code for the APDU message
	 */
	void SetFunction(FunctionCode aCode);

	/**
		Returns the Function Code (FC) field from the DNP3 Application
		Layer message.

		@return		the function code of the APDU message
	 */
	AppControlField GetControl() const;

	/**
		Sets the Application Control (AC) field of the DNP3 Application
		Layer message.

		@param arControl	the new application control value for the
							APDU message
	 */
	void SetControl(const AppControlField& arControl);

	/**
		Sets the Application Control (AC) field of the DNP3 Application
		Layer message, specifying each parameter as a separate function
		argument.

		@param aFIR			a flag to indicate if this APDU is the first
							fragment in a sequence
		@param aFIN			a flag to indicate if this APDU is the final
							fragment in a sequence
		@param aCON			a flag to indicate if this APDU requires the
							receiver to confirm receipt
		@param aUNS			a flag to indicate if this APDU is part of
							an unsolicited response
		@param aSEQ			the sequence number, from 0 to 15 (decimal)

		@throw ArgumentException	if the sequence number is invalid
	 */
	void SetControl(bool aFIR, bool aFIN, bool aCON = false, bool aUNS = false, int aSEQ = 0) {
		if (aSEQ < 0 || aSEQ > 15) {
			MACRO_THROW_EXCEPTION(openpal::ArgumentException, "Invalid sequence number");
		}

		AppControlField f(aFIR, aFIN, aCON, aUNS, aSEQ);
		this->SetControl(f);
	}

	/**
		Sets all header values in the DNP3 Application Layer message,
		specifying each parameter as a separate function argument.

		@param aCode		the new function code for the APDU message
		@param aFIR			a flag to indicate if this APDU is the first
							fragment in a sequence
		@param aFIN			a flag to indicate if this APDU is the final
							fragment in a sequence
		@param aCON			a flag to indicate if this APDU requires the
							receiver to confirm receipt
		@param aUNS			a flag to indicate if this APDU is part of
							an unsolicited response
		@param aSEQ			the sequence number, from 0 to 15 (decimal)

		@throw ArgumentException	if the sequence number is invalid
	 */
	void Set(FunctionCode aCode, bool aFIR = true, bool aFIN = true,
	         bool aCON = false, bool aUNS = false, int aSEQ = 0) {
		this->Reset();
		this->SetFunction(aCode);
		this->SetControl(aFIR, aFIN, aCON, aUNS, aSEQ);
	}

	/**
		Returns the Internal Indications (IIN) field of the message.
		The message must be set as a response type before using this
		function, otherwise an exception will be thrown.

		@returns			the IIN field values for the message

		@throws Exception	if the current message AppHeaderTypes is not
							a AHT_RESPONSE
	 */
	IINField GetIIN() const;

	/**
		Sets the Internal Indications (IIN) field of the message.  The
		message must be set as a response type before using this
		function, otherwise an exception will be thrown.

		@param arIIN		the IIN field values for the message

		@throws Exception	if the current message AppHeaderTypes is not
							a AHT_RESPONSE
	 */
	void SetIIN(const IINField& arIIN);

	/**
		Returns an Iterator to the first DNP3 Object Header.

		@return			an Iterator to the first DNP3 Object Header
	 */
	HeaderReadIterator BeginRead() const;

	/**
		Writes a contiguous block of object data to the message.

		@param apObj		the object that contains the data
		@param aStart		the beginning index of the data
		@param aStop		the ending index of the data
		@param aCode		the Qualifier Code field in the DNP3 Object
							Header, which is automatically determined if
							unspecified

		@return				an Iterator to the object data
	 */
	ObjectWriteIterator WriteContiguous(const FixedObject* apObj, size_t aStart, size_t aStop, QualifierCode aCode = QualifierCode::UNDEFINED);

	/**
		Writes a contiguous block of bitfield data to the message, where
		8 bit objects form one octet/byte.

		@param apObj		the object that contains the data
		@param aStart		the beginning index of the data
		@param aStop		the ending index of the data
		@param aCode		the Qualifier Code field in the DNP3 Object
							Header, which is automatically determined if
							unspecified

		@return				an Iterator to the object data
	 */
	ObjectWriteIterator WriteContiguous(const BitfieldObject* apObj, size_t aStart, size_t aStop, QualifierCode aCode = QualifierCode::UNDEFINED);

	/**
		Writes a block of indexed data.

		@param apObj		the object that contains the data
		@param aCount		the number of objects
		@param aMaxIndex	the maximum index allowed in the data set

		@return				an Iterator to the object data
	 */
	IndexedWriteIterator WriteIndexed(const FixedObject* apObj, size_t aCount, size_t aMaxIndex);

	/**
		Writes a block of indexed data.

		@param apObj		the object that contains the data
		@param aCount		the number of objects
		@param aCode		the Qualifier Code field in the DNP3 Object
							Header, which is automatically determined if
							unspecified

		@return				an Iterator to the object data
	 */
	IndexedWriteIterator WriteIndexed(const FixedObject* apObj, size_t aCount, QualifierCode aCode);

	/**
		Writes a block of indexed data, where the Variation field in the
		DNP3 Object Header corresponds with the length of the data.
		Examples of such data include Virtual Terminal objects (Object
		Groups 112 and 113).

		@param apObj		the object that contains the data
		@param aSize		the size of the data
		@param aIndex		the index value for the data

		@return				an Iterator to the object data
	 */
	IndexedWriteIterator WriteIndexed(const SizeByVariationObject* apObj, size_t aSize, size_t aIndex);

	/**
		Writes a block of indexed data, where the Variation field in the
		DNP3 Object Header corresponds with the length of the data.
		Examples of such data include Virtual Terminal objects (Object
		Groups 112 and 113).

		@param apObj		the object that contains the data
		@param aSize		the size of the data
		@param aCode		the Qualifier Code field in the DNP3 Object
							Header

		@return				an Iterator to the object data
	 */
	IndexedWriteIterator WriteIndexed(const SizeByVariationObject* apObj, size_t aSize, QualifierCode aCode);

	/**
		Performs a write of the provided object data for objects small
		enough to not require an iterator.

		@param apObj		the object data to write

		@return				true if the object was written successfully,
							false if the object could not be written.
	 */
	bool DoPlaceholderWrite(ObjectBase* apObj);

	/**
		A helper function that indicates whether the given Function Code
		(FC) in the DNP3 Application Layer message would indicate that a
		data field should exist in the message.

		@param aCode		the function code to look up

		@return				true if the function code specifies a data
							component, false if not
	 */
	static bool HasData(FunctionCode aCode);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	/**
		Converts the APDU object into a human-readable string
		representation.  The output should match the following syntax:

		@code
		FIR: 0, FIN: 1, CON: 0, UNS: 1, SEQ: 13, Func: Write HdrCount: 5, Header: (Grp: 112, Var: 24, Qual: 6, Count: 1), Size: 37
		@endcode

		@return				a string representation of the APDU object
	 */
	std::string ToString() const;
#endif

	bool operator==(const APDU& rhs);
	bool operator!=(const APDU& rhs) {
		return !(*this == rhs);
	}

private:

	void CheckWriteState(const ObjectBase*);

	IAppHeader* ParseHeader() const;
	size_t Remainder() {
		return mBuffer.Size() - mFragmentSize;
	}

	IndexedWriteIterator WriteCountHeader(size_t aObjectSize, size_t aPrefixSize, uint8_t aGrp, uint8_t aVar, size_t aCount, QualifierCode aQual);
	void WriteContiguousHeader(IObjectHeader* apHdr, uint8_t* apPos, size_t aStart, size_t aStop);

	// Interpreted Information
	bool mIsInterpreted;
	IAppHeader* mpAppHeader;					// uses a singleton so auto copy is safe
	std::vector<HeaderInfo> mObjectHeaders;

	CopyableBuffer mBuffer;		// This makes it dynamically sizable without the need for a special copy constructor.
	size_t mFragmentSize;		// Number of bytes written to the buffer

	QualifierCode GetContiguousQualifier(size_t aStart, size_t aStop);
	QualifierCode GetIndexedQualifier(size_t aMaxIndex, size_t aCount);

	ICountHeader* GetCountHeader(QualifierCode aCode);

	//////////////////////////////////////////
	// Private Functions for Interpreting Frames
	//////////////////////////////////////////

	IObjectHeader* GetObjectHeader(QualifierCode aCode);

	size_t ReadObjectHeader(size_t aOffset, size_t aRemainder);

	ErrorCode<size_t> GetPrefixSizeAndValidate(QualifierCode aCode, ObjectTypes aType);
	size_t GetNumObjects(const IObjectHeader* apHeader, const uint8_t* pStart);

};

}

#endif
