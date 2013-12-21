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
#ifndef __APDU_HEADER_WRITER_H_
#define __APDU_HEADER_WRITER_H_

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
class APDUHeaderWriter : private Uncopyable
{
	public:

	APDUHeaderWriter(openpal::WriteBuffer aHeaderBuffer);	
	
	uint32_t Size() const; // The number of bytes written to the buffer

	openpal::ReadOnlyBuffer ToReadOnly() const;

	bool WriteHeader(GroupVariationID id, QualifierCode qc);

	template <class IndexType, class WriteType>
	RangeWriteIterator<IndexType, WriteType> IterateOverRange(QualifierCode qc, typename IndexType::Type start);

	template <class CountType, class WriteType>
	CountWriteIterator<CountType, WriteType> IterateOverCount(QualifierCode qc);

	template <class PrefixType, class WriteType>
	PrefixedWriteIterator<PrefixType, WriteType> IterateOverCountWithPrefix(QualifierCode qc);

	// record the current position in case we need to rollback	
	void Mark();

	// roll back to the last mark
	void Rollback();	
	
	private:
	
	openpal::WriteBuffer buffer;	
	openpal::WriteBuffer position;

	Settable<openpal::WriteBuffer> mark;	
};

template <class IndexType, class WriteType>
RangeWriteIterator<IndexType, WriteType> APDUHeaderWriter::IterateOverRange(QualifierCode qc, typename IndexType::Type start)
{
	if(this->WriteHeader(WriteType::ID, qc))
	{
		return RangeWriteIterator<IndexType, WriteType>(start, position);
	}
	else return RangeWriteIterator<IndexType, WriteType>::Null();
}

template <class CountType, class WriteType>
CountWriteIterator<CountType, WriteType> APDUHeaderWriter::IterateOverCount(QualifierCode qc)
{
	if(this->WriteHeader(WriteType::ID, qc))
	{
		return CountWriteIterator<CountType, WriteType>(position);
	}
	else return CountWriteIterator<CountType, WriteType>::Null();
}

template <class PrefixType, class WriteType>
PrefixedWriteIterator<PrefixType, WriteType> APDUHeaderWriter::IterateOverCountWithPrefix(QualifierCode qc)
{
	if(this->WriteHeader(WriteType::ID, qc))
	{
		return PrefixedWriteIterator<PrefixType, WriteType>(position);
	}
	else return PrefixedWriteIterator<PrefixType, WriteType>::Null();
}

}

#endif
