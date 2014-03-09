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

#ifndef __BINARY_WRITE_STATE_H_
#define __BINARY_WRITE_STATE_H_

#include "IEventWriterState.h"
#include "CountWriteIterator.h"

namespace opendnp3
{

template <class CountType, class WriteType>
class BinaryWriteState : public IEventWriterState
{
public:

	BinaryWriteState();

	bool Write(const Event<Binary>& evt, APDUEventWriter& ctx) final;
	bool Write(const Event<Analog>& evt, APDUEventWriter& ctx) final;
	bool Write(const Event<Counter>& evt, APDUEventWriter& ctx) final;

private:

	CountWriteIterator<CountType, WriteType> iterator;
};

template <class CountType, class WriteType>
BinaryWriteState<CountType, WriteType>::BinaryWriteState() : iterator(CountWriteIterator<CountType, WriteType>::Null())
{}

template <class CountType, class WriteType>
bool BinaryWriteState<CountType, WriteType>::Write(const Event<Binary>& evt, APDUEventWriter& ctx)
{
	return false;
}

template <class CountType, class WriteType>
bool BinaryWriteState<CountType, WriteType>::Write(const Event<Analog>& evt, APDUEventWriter& ctx)
{
	return false;
}

template <class CountType, class WriteType>
bool BinaryWriteState<CountType, WriteType>::Write(const Event<Counter>& evt, APDUEventWriter& ctx)
{
	return false;
}

}

#endif
