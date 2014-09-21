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

#ifndef OPENPAL_FUNCTION1_H
#define OPENPAL_FUNCTION1_H

#include "Erasure.h"

namespace openpal
{

template <class A, class R>
class Function1 : private Erasure
{
	typedef R (*Invoke)(const uint8_t* pBuffer, const A& arg);

public:

	Function1() : pInvoke(nullptr)
	{}

	Function1(const Function1& other) : pInvoke(other.pInvoke)
	{
		this->CopyErasure(other);
	}

	Function1& operator=(const Function1& other)
	{
		if (this != &other)
		{
			this->pInvoke = other.pInvoke;
			this->CopyErasure(other);
		}

		return *this;
	}

	template <class Lambda>
	static Function1<A,R> Bind(Lambda& lambda)
	{
		static_assert(sizeof(Lambda) <= sizes::MAX_ERASURE_SIZE, "Lambda is too big for erasure");		
		Function1<A, R> function(&RunLambda<Lambda>, sizeof(lambda));
		new(function.bytes) Lambda(lambda); // use placement new
		return function;
	}	

	R Apply(const A& arg) const
	{
		assert(pInvoke);
		return (*pInvoke)(bytes, arg);		
	}

private:

	template <class Lambda>
	static R RunLambda(const uint8_t* pBuffer, const A& arg)
	{
		return (*reinterpret_cast<const Lambda*>(pBuffer))(arg);		
	}

	Function1(Invoke pInvoke_, uint32_t size_) : Erasure(size_), pInvoke(pInvoke_)
	{}

	Invoke pInvoke;	
};

}

#endif
