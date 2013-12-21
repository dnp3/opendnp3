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
#ifndef __APDU_WRITER_H_
#define __APDU_WRITER_H_

#include "APDUOut.h"
#include "IINField.h"
#include "GroupVariation.h"
#include "gen/QualifierCode.h"
#include "Settable.h"

#include "RangeWriteIterator.h"
#include "CountWriteIterator.h"

#include "objects/GroupVariationID.h"

#include <opendnp3/Uncopyable.h>

namespace opendnp3
{

// A facade for writing APDUs to an external buffer
class APDUWriter : private Uncopyable
{
	public:
	
	void SetFunction(FunctionCode code);		
	void SetControl(const AppControlField& control);

	openpal::WriteBuffer GetWritten() const;

	openpal::ReadOnlyBuffer ToReadOnly() const;

	bool WriteHeader(GroupVariationID id, QualifierCode qc);

	template <class IndexType, class WriteType>
	RangeWriteIterator<IndexType, WriteType> IterateOverRange(QualifierCode qc, typename IndexType::Type start);

	template <class CountType, class WriteType>
	CountWriteIterator<CountType, WriteType> IterateOverCount(QualifierCode qc);

	// record the current position in case we need to rollback	
	void Mark();

	// roll back to the last mark
	void Rollback();

	private:
	APDUWriter();
	
	Settable<openpal::WriteBuffer> mark;
	openpal::WriteBuffer position;

	protected:

	APDUWriter(openpal::WriteBuffer aBuffer, uint32_t aStartSize);	
	openpal::WriteBuffer buffer;	
};

template <class IndexType, class WriteType>
RangeWriteIterator<IndexType, WriteType> APDUWriter::IterateOverRange(QualifierCode qc, typename IndexType::Type start)
{
	if(this->WriteHeader(WriteType::ID, qc))
	{
		return RangeWriteIterator<IndexType, WriteType>(start, position);
	}
	else return RangeWriteIterator<IndexType, WriteType>::Null();
}

template <class CountType, class WriteType>
CountWriteIterator<CountType, WriteType> APDUWriter::IterateOverCount(QualifierCode qc)
{
	if(this->WriteHeader(WriteType::ID, qc))
	{
		return CountWriteIterator<CountType, WriteType>(position);
	}
	else return CountWriteIterator<CountType, WriteType>::Null();
}

class APDURequestWriter : public APDUWriter
{
	public:
	APDURequestWriter(openpal::WriteBuffer aBuffer);	
};

class APDUResponseWriter : public APDUWriter
{
	public:
	APDUResponseWriter(openpal::WriteBuffer aBuffer);

	void SetIIN(const IINField& indications);
};

}

#endif
