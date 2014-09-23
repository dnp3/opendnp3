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

#ifndef OPENPAL_FUNCTION0_H
#define OPENPAL_FUNCTION0_H

#include "Erasure.h"

#include "openpal/Configure.h"

#include <assert.h>

namespace openpal
{

template <class T>
class Function0 : private Erasure
{
	typedef T (*Invoke)(const uint8_t* pBuffer);

public:

	Function0() : pInvoke(nullptr)
	{}

	Function0(const Function0& other) : pInvoke(other.pInvoke)
	{
		this->CopyErasure(other);
	}

	Function0& operator=(const Function0& other)
	{
		if (this != &other)
		{
			this->pInvoke = other.pInvoke;
			this->CopyErasure(other);
		}

		return *this;
	}

	template <class Lambda>
	static Function0<T> Bind(Lambda& lambda)
	{
		static_assert(sizeof(Lambda) <= sizes::MAX_ERASURE_SIZE, "Lambda is too big for erasure");		
		Function0<T> function(&RunLambda<Lambda>, sizeof(lambda));
		new(function.bytes) Lambda(lambda); // use placement new
		return  function;
	}

	bool IsSet() const
	{
		return (pInvoke != nullptr);
	}

	T Apply() const
	{
		assert(pInvoke);
		return (*pInvoke)(bytes);
	}

private:

	template <class Lambda>
	static T RunLambda(const uint8_t* pBuffer)
	{
		return (*reinterpret_cast<const Lambda*>(pBuffer))();		
	}

	Function0(Invoke pInvoke_, uint32_t size_) : Erasure(size_), pInvoke(pInvoke_)
	{}

	Invoke pInvoke;	
};

}

#endif
