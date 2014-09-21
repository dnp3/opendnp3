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
#ifndef OPENDNP3_INDEXEDVALUE_H
#define OPENDNP3_INDEXEDVALUE_H

#include <cstdint>

namespace opendnp3
{
/**
* A simple tuple for pairing Values with an index
*/
template <class IndexType>
class Indexed
{
public:
	Indexed(IndexType index_) : index(index_)
	{}

	Indexed() : index(0)
	{}

	IndexType index;
};

/**
* A simple tuple for pairing Values with an index
*/
template <class ValueType, class IndexType>
class IndexedValue : public Indexed<IndexType>
{
public:
	IndexedValue(const ValueType& value_, IndexType index_) :
		Indexed<IndexType>(index_),
		value(value_)
	{}

	template <class T>
	IndexedValue<ValueType, T> Widen() const
	{
		T widerIndex = this->index;
		return IndexedValue<ValueType, T>(this->value, widerIndex);
	}

	IndexedValue(): Indexed<IndexType>(), value()
	{}

	ValueType value;
};

}

#endif
