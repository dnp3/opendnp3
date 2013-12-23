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

#include "objects/GroupVariationID.h"

#include <opendnp3/Uncopyable.h>

namespace opendnp3
{

// A facade for writing APDUs to an external buffer
class ObjectWriter : private Uncopyable
{
	public:

	ObjectWriter(openpal::WriteBuffer aHeaderBuffer);	
	
	uint32_t Size() const; // The number of bytes written to the buffer

	openpal::ReadOnlyBuffer ToReadOnly() const;

	bool WriteHeader(GroupVariationID id, QualifierCode qc);

	template <class IndexType, class WriteType>
	RangeWriteIterator<IndexType, WriteType> IterateOverRange(QualifierCode qc, typename IndexType::Type start);

	template <class CountType, class WriteType>
	CountWriteIterator<CountType, WriteType> IterateOverCount(QualifierCode qc);

	template <class CountType, class ValueType>
	bool WriteSingleValue(QualifierCode qc, const ValueType&);

	template <class PrefixType, class WriteType>
	PrefixedWriteIterator<PrefixType, WriteType> IterateOverCountWithPrefix(QualifierCode qc);

	// record the current position in case we need to rollback	
	void Mark();

	// roll back to the last mark
	void Rollback();	
	
	private:

	bool WriteHeaderWithReserve(GroupVariationID id, QualifierCode qc, uint32_t reserve);
	
	openpal::WriteBuffer buffer;	
	openpal::WriteBuffer position;

	Settable<openpal::WriteBuffer> mark;	
};

template <class CountType, class ValueType>
bool ObjectWriter::WriteSingleValue(QualifierCode qc, const ValueType& value)
{
	auto reserveSize = CountType::Size + ValueType::SIZE;
	if(this->WriteHeaderWithReserve(ValueType::ID, qc, reserveSize))
	{
		CountType::WriteBuffer(position, 1); //write the count
		ValueType::Write(value, position); // write the value
		return true;
	}
	else return false;
}

template <class IndexType, class WriteType>
RangeWriteIterator<IndexType, WriteType> ObjectWriter::IterateOverRange(QualifierCode qc, typename IndexType::Type start)
{
	auto reserveSize = 2*IndexType::Size + WriteType::SIZE;
	if(this->WriteHeaderWithReserve(WriteType::ID, qc, reserveSize))
	{
		return RangeWriteIterator<IndexType, WriteType>(start, position);
	}
	else return RangeWriteIterator<IndexType, WriteType>::Null();
}

template <class CountType, class WriteType>
CountWriteIterator<CountType, WriteType> ObjectWriter::IterateOverCount(QualifierCode qc)
{
	auto reserveSize = CountType::Size + WriteType::SIZE;
	if(this->WriteHeaderWithReserve(WriteType::ID, qc, reserveSize))
	{
		return CountWriteIterator<CountType, WriteType>(position);
	}
	else return CountWriteIterator<CountType, WriteType>::Null();
}

template <class PrefixType, class WriteType>
PrefixedWriteIterator<PrefixType, WriteType> ObjectWriter::IterateOverCountWithPrefix(QualifierCode qc)
{
	auto reserveSize = 2*PrefixType::Size + WriteType::SIZE;  //enough space for the count, 1 prefix + object
	if(this->WriteHeaderWithReserve(WriteType::ID, qc, reserveSize))
	{
		return PrefixedWriteIterator<PrefixType, WriteType>(position);
	}
	else return PrefixedWriteIterator<PrefixType, WriteType>::Null();
}

}

#endif
