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
#ifndef __I_SOE_HANDLER_H_
#define	__I_SOE_HANDLER_H_

#include "ITransactable.h"
#include "IterableBuffer.h"
#include "IndexedValue.h"
#include "DataTypes.h"
#include "OctetString.h"

#include <iostream>

namespace opendnp3
{

/**
* An interface for Sequence-Of-Events (SOE) callbacks from a master stack to 
* the application layer.
*
* A call is made to the appropriate member method for every header in a APDU 
*
*/
class ISOEHandler : public ITransactable
{
public:
	
	virtual void Load(const IterableBuffer<IndexedValue<Binary>>& meas) {}
	virtual void Load(const IterableBuffer<IndexedValue<Analog>>& meas) {}
	virtual void Load(const IterableBuffer<IndexedValue<Counter>>& meas) {}
	virtual void Load(const IterableBuffer<IndexedValue<FrozenCounter>>& meas) {}
	virtual void Load(const IterableBuffer<IndexedValue<BinaryOutputStatus>>& meas) {}
	virtual void Load(const IterableBuffer<IndexedValue<AnalogOutputStatus>>& meas) {}
	virtual void Load(const IterableBuffer<IndexedValue<OctetString>>& meas) {}

	virtual ~ISOEHandler() {}	
};


class NullSOEHandler : public ISOEHandler
{

public:

	static ISOEHandler* Inst() { return &msInstance; }

protected:

	void Start() final {}
	void End() final {}

private:
	NullSOEHandler()
	{}
	
	static NullSOEHandler msInstance;
};

class PrintingSOEHandler : public ISOEHandler
{

public:
	static ISOEHandler* Inst() { return &msInstance; }

	void Load(const IterableBuffer<IndexedValue<Binary>>& meas) final;
	void Load(const IterableBuffer<IndexedValue<Analog>>& meas) final;
	void Load(const IterableBuffer<IndexedValue<Counter>>& meas) final;
	void Load(const IterableBuffer<IndexedValue<FrozenCounter>>& meas) final;
	void Load(const IterableBuffer<IndexedValue<BinaryOutputStatus>>& meas) final;
	void Load(const IterableBuffer<IndexedValue<AnalogOutputStatus>>& meas) final;
	void Load(const IterableBuffer<IndexedValue<OctetString>>& meas) final;

protected:

	void Start() final {}
	void End() final {}

private:

	template <class T>
	static void Print(const IterableBuffer<IndexedValue<T>>& buffer, const std::string& name)
	{
		buffer.foreach([&](const IndexedValue<T>& pair) {
			std::cout << name << " [" << pair.index << "] : " << pair.value.GetValue() << " : " << static_cast<int>(pair.value.GetQuality()) << std::endl;
		});
	}

	PrintingSOEHandler()
	{}
	
	static PrintingSOEHandler msInstance;
};


}

#endif
