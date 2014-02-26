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
#ifndef __OBJECT_WRITER_H_
#define __OBJECT_WRITER_H_

#include "Settable.h"
#include "GroupVariation.h"
#include "gen/QualifierCode.h"

#include "RangeWriteIterator.h"
#include "CountWriteIterator.h"
#include "PrefixedWriteIterator.h"
#include "objects/IDNP3Serializer.h"

#include "objects/GroupVariationID.h"

namespace opendnp3
{

// A facade for writing APDUs to an external buffer
class ObjectWriter
{
	friend class APDUWrapper;
	
	public:

	bool WriteHeader(GroupVariationID id, QualifierCode qc);

	template <class IndexType, class WriteType>
	RangeWriteIterator<IndexType, WriteType> IterateOverRange(QualifierCode qc, IDNP3Serializer<WriteType>* pSerializer, typename IndexType::Type start);

	template <class CountType, class WriteType>
	CountWriteIterator<CountType, WriteType> IterateOverCount(QualifierCode qc, IDNP3Serializer<WriteType>* pSerializer);

	template <class CountType, class ValueType>
	bool WriteSingleValue(QualifierCode qc, IDNP3Serializer<ValueType>* pSerializer, const ValueType&);

	template <class CountType, class WriteType>
	bool WriteSingleValue(QualifierCode qc, const WriteType&);

	template <class CountType, class ValueType>
	bool WriteSingleIndexedValue(QualifierCode qc, IDNP3Serializer<ValueType>* pSerializer, const ValueType&, typename CountType::Type index);

	template <class PrefixType, class WriteType>
	PrefixedWriteIterator<PrefixType, WriteType> IterateOverCountWithPrefix(QualifierCode qc, IDNP3Serializer<WriteType>* pSerializer);

	// record the current position in case we need to rollback	
	void Mark();

	// roll back to the last mark
	void Rollback();	
	
	private:

	ObjectWriter(openpal::WriteBuffer* position_);			

	bool WriteHeaderWithReserve(GroupVariationID id, QualifierCode qc, uint32_t reserve);
		
	openpal::WriteBuffer* position;

	Settable<openpal::WriteBuffer> mark;	
};

template <class CountType, class ValueType>
bool ObjectWriter::WriteSingleValue(QualifierCode qc, IDNP3Serializer<ValueType>* pSerializer, const ValueType& value)
{
	auto reserveSize = CountType::Size + pSerializer->Size();
	if(this->WriteHeaderWithReserve(ValueType::ID, qc, reserveSize))
	{
		CountType::WriteBuffer(*position, 1); //write the count
		pSerializer->Write(value, *position);
		return true;
	}
	else return false;
}

template <class CountType, class WriteType>
bool ObjectWriter::WriteSingleValue(QualifierCode qc, const WriteType& value)
{
	uint32_t reserveSize = CountType::Size + WriteType::SIZE;
	if(this->WriteHeaderWithReserve(WriteType::ID, qc, reserveSize))
	{
		CountType::WriteBuffer(*position, 1); //write the count
		WriteType::Write(value, *position);
		return true;
	}
	else return false;
}

template <class CountType, class ValueType>
bool ObjectWriter::WriteSingleIndexedValue(QualifierCode qc, IDNP3Serializer<ValueType>* pSerializer, const ValueType& value, typename CountType::Type index)
{
	uint32_t reserveSize = 2 * CountType::Size + pSerializer->Size();
	if(this->WriteHeaderWithReserve(pSerializer->ID(), qc, reserveSize))
	{
		CountType::WriteBuffer(*position, 1); //write the count
		CountType::WriteBuffer(*position, index); // write the index
		pSerializer->Write(value, *position);
		return true;
	}
	else return false;
}

template <class IndexType, class WriteType>
RangeWriteIterator<IndexType, WriteType> ObjectWriter::IterateOverRange(QualifierCode qc, IDNP3Serializer<WriteType>* pSerializer, typename IndexType::Type start)
{
	uint32_t reserveSize = 2*IndexType::Size + pSerializer->Size();
	if(this->WriteHeaderWithReserve(pSerializer->ID(), qc, reserveSize))
	{
		return RangeWriteIterator<IndexType, WriteType>(start, pSerializer, *position);
	}
	else return RangeWriteIterator<IndexType, WriteType>::Null();
}

template <class CountType, class WriteType>
CountWriteIterator<CountType, WriteType> ObjectWriter::IterateOverCount(QualifierCode qc, IDNP3Serializer<WriteType>* pSerializer)
{
	uint32_t reserveSize = CountType::Size + pSerializer->Size();
	if(this->WriteHeaderWithReserve(pSerializer->ID(), qc, reserveSize))
	{
		return CountWriteIterator<CountType, WriteType>(pSerializer, *position);
	}
	else return CountWriteIterator<CountType, WriteType>::Null();
}

template <class PrefixType, class WriteType>
PrefixedWriteIterator<PrefixType, WriteType> ObjectWriter::IterateOverCountWithPrefix(QualifierCode qc, IDNP3Serializer<WriteType>* pSerializer)
{
	uint32_t reserveSize = 2*PrefixType::Size + pSerializer->Size();  //enough space for the count, 1 prefix + object
	if(this->WriteHeaderWithReserve(pSerializer->ID(), qc, reserveSize))
	{
		return PrefixedWriteIterator<PrefixType, WriteType>(pSerializer, *position);
	}
	else return PrefixedWriteIterator<PrefixType, WriteType>::Null();
}

}

#endif
