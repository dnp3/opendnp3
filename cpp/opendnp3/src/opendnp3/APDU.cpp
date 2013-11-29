
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

#include "APDU.h"

#include <openpal/Exception.h>
#include <opendnp3/Util.h>
#include <opendnp3/DNPConstants.h>
#include <opendnp3/APDUConstants.h>

#include "ObjectWriteIterator.h"

#include <assert.h>
#include <sstream>
#include <limits>

using namespace std;

namespace opendnp3
{

APDU::APDU(size_t aFragSize) :
	mIsInterpreted(false),
	mpAppHeader(NULL),
	mObjectHeaders(0),
	mBuffer(aFragSize),
	mFragmentSize(0)
{

}


bool APDU::operator==(const APDU& rhs)
{
	if ( this->Size() != rhs.Size() )
		return false;
	for ( size_t i = 0; i < this->Size(); i++ )
		if ( this->GetBuffer()[i] != rhs.GetBuffer()[i] ) return false;
	return true;
}


AppControlField APDU::GetControl() const
{
	assert(mpAppHeader != NULL);

	return mpAppHeader->GetControl(mBuffer);
}

IINField APDU::GetIIN() const
{
	assert(mpAppHeader != NULL);

	if(mpAppHeader->GetType() != AHT_RESPONSE) {
		MACRO_THROW_EXCEPTION(openpal::Exception, "Only response packets have IIN fields");
	}

	return static_cast<ResponseHeader*>(mpAppHeader)->GetIIN(mBuffer);
}

FunctionCode APDU::GetFunction() const
{
	assert(mpAppHeader != NULL);

	return mpAppHeader->GetFunction(mBuffer);
}

void APDU::SetFunction(FunctionCode aCode)
{
	assert(mpAppHeader == NULL);

	if(aCode == FunctionCode::RESPONSE || aCode == FunctionCode::UNSOLICITED_RESPONSE) {
		mpAppHeader = ResponseHeader::Inst();
	}
	else {
		mpAppHeader = RequestHeader::Inst();
	}

	mFragmentSize = mpAppHeader->GetSize();

	mpAppHeader->SetFunction(mBuffer, aCode);
}

void APDU::SetControl(const AppControlField& arControl)
{
	assert(mpAppHeader != NULL);

	mpAppHeader->SetControl(mBuffer, arControl);
}

void APDU::SetIIN(const IINField& arIIN)
{
	assert(mpAppHeader != NULL);
	assert(mpAppHeader->GetType() == AHT_RESPONSE);

	static_cast<ResponseHeader*>(mpAppHeader)->SetIIN(mBuffer, arIIN);
}

void APDU::Reset()
{
	mFragmentSize = 0;
	mIsInterpreted = false;
	mpAppHeader = NULL;
	mObjectHeaders.clear();
}

void APDU::Write(const uint8_t* apData, size_t aLength)
{
	if(aLength > mBuffer.Size()) {
		MACRO_THROW_EXCEPTION_COMPLEX(openpal::ArgumentException, "Size " << aLength << " exceeds max fragment size of " << mBuffer.Size());
	}

	this->Reset();
	memcpy(mBuffer, apData, aLength);
	mFragmentSize = aLength;
}

void APDU::Interpret()
{
	if(mIsInterpreted) return;

	this->InterpretHeader();

	size_t consumed = mpAppHeader->GetSize();
	size_t remainder = mFragmentSize - consumed;

	while(remainder > 0) {
		size_t header_size = this->ReadObjectHeader(consumed, remainder);
		remainder -= header_size;
		consumed += header_size;
	}

	mIsInterpreted = true;
}

// Parse the header only. Throws exception if header is malformed
void APDU::InterpretHeader()
{
	if(mpAppHeader != NULL) return;
	mpAppHeader = this->ParseHeader();
}

IAppHeader* APDU::ParseHeader() const
{
	if(mFragmentSize < 2) {
		MACRO_THROW_EXCEPTION_WITH_CODE(openpal::Exception, "Insufficent size", ALERR_INSUFFICIENT_DATA_FOR_FRAG);
	}

	// start by assuming that it's a request header since they have same starting structure
	IAppHeader* pHeader = RequestHeader::Inst();
	FunctionCode function = pHeader->GetFunction(mBuffer);
	AppControlField control = pHeader->GetControl(mBuffer);

	if( IsResponse(function) ) {
		if(mFragmentSize < 4) {
			MACRO_THROW_EXCEPTION_WITH_CODE(openpal::Exception, "Insufficent size", ALERR_INSUFFICIENT_DATA_FOR_RESPONSE);
		}

		pHeader = ResponseHeader::Inst();
	}

	return pHeader;
}

size_t APDU::ReadObjectHeader(size_t aOffset, size_t aRemainder)
{

	const uint8_t* pStart = mBuffer.Buffer() + aOffset;
	IObjectHeader* pHdr = AllObjectsHeader::Inst(); //Start by interpreting using the smallest possible header
	ObjectHeaderField hdrData;

	if(aRemainder < pHdr->GetSize()) {
		MACRO_THROW_EXCEPTION_WITH_CODE(openpal::Exception, "Insufficent size", ALERR_INSUFFICIENT_DATA_FOR_HEADER);
	}

	//Read the header data and select the correct object header based on this information
	pHdr->Get(pStart, hdrData);

	if(hdrData.Qualifier == QualifierCode::UNDEFINED) {
		MACRO_THROW_EXCEPTION_WITH_CODE(openpal::Exception, "Unknown qualifier", ALERR_UNKNOWN_QUALIFIER);
	}

	pHdr = this->GetObjectHeader(hdrData.Qualifier);

	//lookup the object type
	ObjectBase* pObj = ObjectBase::Get(hdrData.Group, hdrData.Variation);

	if(pObj == NULL) {
		MACRO_THROW_EXCEPTION_COMPLEX(openpal::ObjectException, "Undefined object, " << "Group: " << static_cast<int>(hdrData.Group) << " Var: " << static_cast<int>(hdrData.Variation));
	}

	if(aRemainder < pHdr->GetSize()) {
		MACRO_THROW_EXCEPTION_WITH_CODE(openpal::Exception, "Insufficent size", ALERR_INSUFFICIENT_DATA_FOR_HEADER);
	}

	aRemainder -= pHdr->GetSize();

	//figure out what the size of the prefixes are in bytes and how many objects there are.	 
	auto result = this->GetPrefixSizeAndValidate(hdrData.Qualifier, pObj->GetType());
	if(result.IsError()) throw openpal::Exception(LOCATION, "Unknown prefix code", result.Code());
	size_t prefixSize = result.Result();
	size_t objCount = this->GetNumObjects(pHdr, pStart);

	//pStart += pHdr->GetSize(); //move the reading position to the first object

	size_t data_size = 0;

	//Some function codes, aka read implicitly do not carry data, only indices
	bool has_data = APDU::HasData(this->GetFunction());

	switch(pObj->GetType()) {
	case(OT_PLACEHOLDER):
		break;
	case(OT_FIXED):
		data_size = prefixSize;
		if(has_data) data_size += static_cast<FixedObject*>(pObj)->GetSize();
		data_size *= objCount;
		break;
	case(OT_BITFIELD):
		data_size = has_data ? static_cast<BitfieldObject*>(pObj)->GetSize(objCount) : 0;
		break;
	case(OT_SIZE_BY_VARIATION):
		data_size = prefixSize;
		data_size += has_data ? hdrData.Variation : 0;
		break;
	default:
		MACRO_THROW_EXCEPTION(openpal::Exception, "Unknown object type");
	}

	if(data_size > aRemainder) {
		MACRO_THROW_EXCEPTION_WITH_CODE(openpal::Exception, "", ALERR_INSUFFICIENT_DATA_FOR_OBJECTS);
	}

	mObjectHeaders.push_back(HeaderInfo(hdrData, objCount, prefixSize, pHdr, pObj, aOffset));

	return pHdr->GetSize() + data_size;
}

IObjectHeader* APDU::GetObjectHeader(QualifierCode aCode)
{
	switch(aCode) {
	case(QualifierCode::UINT8_START_STOP):
		return Ranged2OctetHeader::Inst();
	case(QualifierCode::UINT16_START_STOP):
		return Ranged4OctetHeader::Inst();
	case(QualifierCode::UINT32_START_STOP):
		return Ranged8OctetHeader::Inst();
	case(QualifierCode::ALL_OBJECTS):
		return AllObjectsHeader::Inst();
	case(QualifierCode::UINT8_CNT):
	case(QualifierCode::UINT8_CNT_UINT8_INDEX):
		return Count1OctetHeader::Inst();
	case(QualifierCode::UINT16_CNT):
	case(QualifierCode::UINT16_CNT_UINT16_INDEX):
		return Count2OctetHeader::Inst();
	case(QualifierCode::UINT32_CNT):
	case(QualifierCode::UINT32_CNT_UINT32_INDEX):
		return Count4OctetHeader::Inst();
	default:
		MACRO_THROW_EXCEPTION(openpal::Exception, "Unknown range specifier");
	}
}

size_t APDU::GetNumObjects(const IObjectHeader* apHeader, const uint8_t* apStart)
{
	switch(apHeader->GetType()) {
	case(OHT_ALL_OBJECTS):
		return 0;
	case(OHT_RANGED_2_OCTET):
	case(OHT_RANGED_4_OCTET):
	case(OHT_RANGED_8_OCTET):
		RangeInfo info;
		static_cast<const IRangeHeader*>(apHeader)->GetRange(apStart, info);
		if(info.Start > info.Stop) {
			MACRO_THROW_EXCEPTION_WITH_CODE(openpal::Exception, "", ALERR_START_STOP_MISMATCH);
		}
		return (info.Stop - info.Start + 1); //indices are inclusive
	case(OHT_COUNT_1_OCTET):
	case(OHT_COUNT_2_OCTET):
	case(OHT_COUNT_4_OCTET):
		return static_cast<const ICountHeader*>(apHeader)->GetCount(apStart);
	default:
		assert(false);
		return 0;
	}
}

ErrorCode<size_t> APDU::GetPrefixSizeAndValidate(QualifierCode aCode, ObjectTypes aType)
{
	switch(aCode) 
	{
		case(QualifierCode::ALL_OBJECTS):
			switch(aType)
			{
				case(OT_PLACEHOLDER):
					return ErrorCode<size_t>::Success(0);
				case(OT_FIXED):
					return ErrorCode<size_t>::Success(0);
				case(OT_BITFIELD):
					return ErrorCode<size_t>::Success(0);
				default:
					return ErrorCode<size_t>::Failure(ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
			}			
		case(QualifierCode::UINT8_START_STOP):
			switch(aType)
			{
				case(OT_PLACEHOLDER):
					return ErrorCode<size_t>::Success(0);
				case(OT_FIXED):
					return ErrorCode<size_t>::Success(0);
				case(OT_BITFIELD):
					return ErrorCode<size_t>::Success(0);
				default:
					return ErrorCode<size_t>::Failure(ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
			}
		case(QualifierCode::UINT16_START_STOP):
			switch(aType)
			{
				case(OT_PLACEHOLDER):
					return ErrorCode<size_t>::Success(0);
				case(OT_FIXED):
					return ErrorCode<size_t>::Success(0);
				case(OT_BITFIELD):
					return ErrorCode<size_t>::Success(0);
				default:
					return ErrorCode<size_t>::Failure(ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
			}
		case(QualifierCode::UINT32_START_STOP):
			switch(aType)
			{
				case(OT_PLACEHOLDER):
					return ErrorCode<size_t>::Success(0);
				case(OT_FIXED):
					return ErrorCode<size_t>::Success(0);
				case(OT_BITFIELD):
					return ErrorCode<size_t>::Success(0);
				default:
					return ErrorCode<size_t>::Failure(ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
			}
		case(QualifierCode::UINT8_CNT):
			switch(aType)
			{
				case(OT_PLACEHOLDER):
					return ErrorCode<size_t>::Success(0);
				case(OT_FIXED):
					return ErrorCode<size_t>::Success(0);
				case(OT_BITFIELD):
					return ErrorCode<size_t>::Success(0);
				default:
					return ErrorCode<size_t>::Failure(ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
			}
		case(QualifierCode::UINT16_CNT):
			switch(aType)
			{
				case(OT_PLACEHOLDER):
					return ErrorCode<size_t>::Success(0);
				case(OT_FIXED):
					return ErrorCode<size_t>::Success(0);
				case(OT_BITFIELD):
					return ErrorCode<size_t>::Success(0);
				default:
					return ErrorCode<size_t>::Failure(ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
			}
		case(QualifierCode::UINT32_CNT):
			switch(aType)
			{
				case(OT_PLACEHOLDER):
					return ErrorCode<size_t>::Success(0);
				case(OT_FIXED):
					return ErrorCode<size_t>::Success(0);
				case(OT_BITFIELD):
					return ErrorCode<size_t>::Success(0);
				default:
					return ErrorCode<size_t>::Failure(ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
			}
		case(QualifierCode::UINT8_CNT_UINT8_INDEX):
			switch(aType)
			{
				case(OT_FIXED):
					return ErrorCode<size_t>::Success(1);
				case(OT_SIZE_BY_VARIATION):
					return ErrorCode<size_t>::Success(1);
				default:
					return ErrorCode<size_t>::Failure(ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
			}
		case(QualifierCode::UINT16_CNT_UINT16_INDEX):
			switch(aType)
			{
				case(OT_FIXED):
					return ErrorCode<size_t>::Success(2);
				case(OT_SIZE_BY_VARIATION):
					return ErrorCode<size_t>::Success(2);
				default:
					return ErrorCode<size_t>::Failure(ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
			}
		case(QualifierCode::UINT32_CNT_UINT32_INDEX):
			switch(aType)
			{
				case(OT_FIXED):
					return ErrorCode<size_t>::Success(4);
				case(OT_SIZE_BY_VARIATION):
					return ErrorCode<size_t>::Success(4);
				default:
					return ErrorCode<size_t>::Failure(ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
			}
		default:
			return ErrorCode<size_t>::Failure(ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
	}

}

HeaderReadIterator APDU::BeginRead() const
{
	return HeaderReadIterator(&mObjectHeaders, mBuffer, HasData(this->GetFunction()));
}

bool APDU::HasData(FunctionCode aCode)
{
	switch(aCode) {
	case(FunctionCode::READ):
	case(FunctionCode::ASSIGN_CLASS):
		return false;
	default:
		return true;
	}
}

ObjectWriteIterator APDU::WriteContiguous(const FixedObject* apObj, size_t aStart, size_t aStop, QualifierCode aCode)
{
	this->CheckWriteState(apObj);
	if(aCode == QualifierCode::UNDEFINED)  aCode = this->GetContiguousQualifier(aStart, aStop);

	IObjectHeader* pHdr = this->GetObjectHeader(aCode);
	if(pHdr->GetSize() > this->Remainder()) return ObjectWriteIterator();

	size_t maxObjects = (this->Remainder() - pHdr->GetSize()) / apObj->GetSize();

	//No point in writing a header if it can't hold an object
	if(maxObjects == 0) return ObjectWriteIterator();

	//do we write all the objects or the max in the space
	size_t requested = aStop - aStart + 1;
	size_t count = (requested < maxObjects) ? requested : maxObjects;
	size_t stop = aStart + count - 1;

	//we're commited to writing some data, so proceed
	uint8_t* pHeaderPos = mBuffer + mFragmentSize;
	pHdr->Set(pHeaderPos, apObj->GetGroup(), apObj->GetVariation(), aCode);
	uint8_t* pPos = pHeaderPos + pHdr->GetSize();
	this->WriteContiguousHeader(pHdr, pHeaderPos, aStart, stop);

	mFragmentSize += pHdr->GetSize();
	bool has_data = APDU::HasData(this->GetFunction());
	size_t obj_size = has_data ? apObj->GetSize() : 0;
	mFragmentSize += count * obj_size;

	return ObjectWriteIterator(pPos, aStart, stop, obj_size);
}

ObjectWriteIterator APDU::WriteContiguous(const BitfieldObject* apObj, size_t aStart, size_t aStop, QualifierCode aCode)
{
	this->CheckWriteState(apObj);
	if(aCode == QualifierCode::UNDEFINED)  aCode = this->GetContiguousQualifier(aStart, aStop);

	IObjectHeader* pHdr = this->GetObjectHeader(aCode);
	if(pHdr->GetSize() > this->Remainder()) return ObjectWriteIterator();

	size_t maxObjects = (this->Remainder() - pHdr->GetSize()) * 8; //bitfield, 8 objects per byte

	//No point in writing a header if it can't hold an object
	if(maxObjects == 0) return ObjectWriteIterator();

	//do we write all the objects or the max in the space
	size_t requested = aStop - aStart + 1;
	size_t count = (requested < maxObjects) ? requested : maxObjects;
	size_t stop = aStart + count - 1;

	//we're commited to writing some data, so proceed
	uint8_t* pHeaderPos = mBuffer + mFragmentSize;
	pHdr->Set(pHeaderPos, apObj->GetGroup(), apObj->GetVariation(), aCode);
	uint8_t* pPos = pHeaderPos + pHdr->GetSize();
	apObj->Zero(pPos, count);

	this->WriteContiguousHeader(pHdr, pHeaderPos, aStart, stop);

	mFragmentSize += pHdr->GetSize();
	bool has_data = APDU::HasData(this->GetFunction());
	size_t data_size = has_data ? apObj->GetSize(count) : 0;
	mFragmentSize += data_size;

	return ObjectWriteIterator(pPos, aStart, stop, 0);
}

void APDU::WriteContiguousHeader(IObjectHeader* apHdr, uint8_t* apPos, size_t aStart, size_t aStop)
{
	switch(apHdr->GetType()) {
	case(OHT_COUNT_1_OCTET):
	case(OHT_COUNT_2_OCTET):
	case(OHT_COUNT_4_OCTET):
		static_cast<ICountHeader*>(apHdr)->SetCount(apPos, aStop - aStart + 1);
		break;
	case(OHT_RANGED_2_OCTET):
	case(OHT_RANGED_4_OCTET):
	case(OHT_RANGED_8_OCTET):
		RangeInfo r;
		r.Start = aStart;
		r.Stop = aStop;
		static_cast<IRangeHeader*>(apHdr)->SetRange(apPos, r);
		break;
	default:
		MACRO_THROW_EXCEPTION(openpal::Exception, "Invalid header type");
	}
}

IndexedWriteIterator APDU::WriteIndexed(const SizeByVariationObject* apObj, size_t aSize, size_t aIndex)
{
	return WriteIndexed(apObj, aSize, GetIndexedQualifier(aIndex, 1));
}

IndexedWriteIterator APDU::WriteIndexed(const FixedObject* apObj, size_t aCount, size_t aMaxIndex)
{
	return WriteIndexed(apObj, aCount, GetIndexedQualifier(aMaxIndex, aCount));
}

IndexedWriteIterator APDU::WriteIndexed(const SizeByVariationObject* apObj, size_t aSize, QualifierCode aCode)
{
	this->CheckWriteState(apObj);

	// This object type encodes the size in the variation field, the prefix is used to encode something else

	if(aSize > 255) throw openpal::ArgumentException(LOCATION, "size out of range");
	uint8_t variation = static_cast<uint8_t>(aSize);

	size_t obj_size = APDU::HasData(this->GetFunction()) ? aSize : 0;
	auto result = this->GetPrefixSizeAndValidate(aCode, apObj->GetType());
	if(result.IsError()) throw openpal::Exception(LOCATION, "Unable to get prefix size", result.Code());
	size_t prefix_size = result.Result();

	return WriteCountHeader(obj_size, prefix_size, apObj->GetGroup(), variation, 1, aCode);
}

IndexedWriteIterator APDU::WriteIndexed(const FixedObject* apObj, size_t aCount, QualifierCode aCode)
{
	this->CheckWriteState(apObj);

	size_t obj_size = APDU::HasData(this->GetFunction()) ? apObj->GetSize() : 0;

	auto result = this->GetPrefixSizeAndValidate(aCode, apObj->GetType());
	if(result.IsError()) throw openpal::Exception(LOCATION, "Unable to get prefix size", result.Code());
	size_t prefix_size = result.Result();

	return WriteCountHeader(obj_size, prefix_size, apObj->GetGroup(), apObj->GetVariation(), aCount, aCode);
}

IndexedWriteIterator APDU::WriteCountHeader(size_t aObjectSize, size_t aPrefixSize, uint8_t aGrp, uint8_t aVar, size_t aCount, QualifierCode aQual)
{
	ICountHeader* pHdr = this->GetCountHeader(aQual); //Get the count header
	if(pHdr->GetSize() > this->Remainder()) return IndexedWriteIterator();

	size_t obj_plus_prefix_size = aObjectSize + aPrefixSize;

	//how many objects can we write?
	size_t max_num = (this->Remainder() - pHdr->GetSize()) / obj_plus_prefix_size;

	// No point in writing a header if it can't hold an object/index
	if(max_num == 0) return IndexedWriteIterator();

	//how many will we write?
	size_t count = (aCount < max_num) ? aCount : max_num;
	size_t data_size = count * obj_plus_prefix_size;
	uint8_t* pHeaderPos = mBuffer + mFragmentSize;

	//commited to writing at this point, so set the header
	pHdr->Set(pHeaderPos, aGrp, aVar, aQual);
	pHdr->SetCount(pHeaderPos, count);
	mFragmentSize += pHdr->GetSize();
	uint8_t* pObjectPos = mBuffer + mFragmentSize;
	mFragmentSize += data_size;

	return IndexedWriteIterator(pObjectPos, count, aQual, aObjectSize);
}



bool APDU::DoPlaceholderWrite(ObjectBase* apObj)
{
	this->CheckWriteState(apObj);

	//determine what the prefix and range specifier should be
	IObjectHeader* pHdr = AllObjectsHeader::Inst();

	size_t remainder = mBuffer.Size() - mFragmentSize;
	if(pHdr->GetSize() > remainder) return false;

	pHdr->Set(mBuffer + mFragmentSize, apObj->GetGroup(), apObj->GetVariation(), QualifierCode::ALL_OBJECTS);
	mFragmentSize += pHdr->GetSize();

	return true;
}



void APDU::CheckWriteState(const ObjectBase* apObj)
{
	if(mpAppHeader == NULL) MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "Header has not be configured");
	if(mIsInterpreted) MACRO_THROW_EXCEPTION(openpal::InvalidStateException, "APDU is interpreted");
	if(apObj == NULL) MACRO_THROW_EXCEPTION(openpal::ArgumentException, "Object cannot be NULL");
}

ICountHeader* APDU::GetCountHeader(QualifierCode aCode)
{
	switch(aCode) {
	case(QualifierCode::UINT8_CNT):
	case(QualifierCode::UINT8_CNT_UINT8_INDEX):	
		return Count1OctetHeader::Inst();
	case(QualifierCode::UINT16_CNT):
	case(QualifierCode::UINT16_CNT_UINT16_INDEX):
		return Count2OctetHeader::Inst();
	case(QualifierCode::UINT32_CNT):
	case(QualifierCode::UINT32_CNT_UINT32_INDEX):
		return Count4OctetHeader::Inst();
	default:
		MACRO_THROW_EXCEPTION(openpal::ArgumentException, "Invalid qualifier for count header");
	}
}


// Found a bug here... The Max index is not enough information because the count can exceed
// the max index when binary events are reported because of SOE
QualifierCode APDU::GetIndexedQualifier(size_t aMaxIndex, size_t aCount)
{
	if(aMaxIndex <= Count1OctetHeader::MaxCount() && aCount <= Count1OctetHeader::MaxCount()) {
		return QualifierCode::UINT8_CNT_UINT8_INDEX;
	}
	else if(aMaxIndex <= Count2OctetHeader::MaxCount() && aCount <= Count2OctetHeader::MaxCount()) {
		return QualifierCode::UINT16_CNT_UINT16_INDEX;
	}
	else {
		return QualifierCode::UINT32_CNT_UINT32_INDEX;
	}

}

QualifierCode APDU::GetContiguousQualifier(size_t aStart, size_t aStop)
{
	if(aStop <= Ranged2OctetHeader::MaxRange()) {
		return QualifierCode::UINT8_START_STOP;
	}
	else if(aStop <= Ranged4OctetHeader::MaxRange()) {
		return QualifierCode::UINT16_START_STOP;
	}
	else {
		return QualifierCode::UINT32_START_STOP;
	}
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string APDU::ToString() const
{
	ostringstream oss;

	APDU copy(*this);

	FunctionCode func = copy.GetFunction();
	AppControlField acf = copy.GetControl();

	oss << "FIR: " << acf.FIR;
	oss << ", FIN: " << acf.FIN;
	oss << ", CON: " << acf.CON;
	oss << ", UNS: " << acf.UNS;
	oss << ", SEQ: " << static_cast<int>(acf.SEQ);
	oss << ", Func: " << FunctionCodeToString(func);
	if ( IsResponse(func) ) oss << copy.GetIIN().ToString();

	try {
		copy.Interpret();
		HeaderReadIterator itr = copy.BeginRead();
		oss << " HdrCount: " << itr.Count();
		if ( itr.Count() != 0 )
			oss << ",";
		for ( ; !itr.IsEnd(); ++itr) {
			oss << " Header: (Grp: " << itr->GetGroup();
			oss << ", Var: " << itr->GetVariation();
			oss << ", Qual: " << QualifierCodeToString(itr->GetQualifier()) << ", ";
			oss << itr->GetHeader()->ToString(*itr);
			oss << ")";
		}
	}
	catch(openpal::Exception) {
		oss << " Malformed header data preceeds";
	}

	oss << ", Size: " << this->Size();

	return oss.str();
}
#endif

}

