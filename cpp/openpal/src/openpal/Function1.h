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
#ifndef __FUNCTION1_H_
#define __FUNCTION1_H_

namespace openpal
{

const uint32_t MAX_FUNCTION1_SIZE = 64;

template <class T>
class Function1
{
	

public:

	Function1() : pInvoke(nullptr), size(0)
	{}

	Function1(const Function1& other) : pInvoke(other.pInvoke), size(other.size)
	{
		memcpy(bytes, other.bytes, size);
	}

	bool IsSet() const
	{
		return (pInvoke != nullptr);
	}

	void Run(const T& arg)
	{
		if (pInvoke)
		{
			(*pInvoke)(bytes, arg);
		}
	}

protected:

	typedef void(*Invoke)(const uint8_t* pBuffer, const T& arg);

	void Apply(const T& arg) const
	{
		if (pInvoke)
		{
			(*pInvoke)(bytes, arg);
		}
	}	

	Function1(Invoke pInvoke_, uint32_t size_) : pInvoke(pInvoke_), size(size_)
	{
		
	}

	Invoke pInvoke;
	uint32_t size;
	uint8_t bytes[MAX_FUNCTION1_SIZE];
};

template <class T, class Lambda>
class LambdaFunction1 : public Function1<T>
{
	static_assert(sizeof(Lambda) <= MAX_FUNCTION1_SIZE, "Lambda is too big for static buffer");

public:

	LambdaFunction1(Lambda& lambda) : Function1(&RunLambda, sizeof(Lambda))
	{
		auto pLambda = reinterpret_cast<Lambda*>(bytes);
		*pLambda = lambda;
	}

private:

	static void RunLambda(const uint8_t* pBuffer, const T& arg)
	{
		auto pLambda = reinterpret_cast<const Lambda*>(pBuffer);
		(*pLambda)(arg);
	}
};

template <class T, class Lambda>
Function1<T> Bind(Lambda& lambda)
{
	return LambdaFunction1<T, Lambda>(lambda);
}


}

#endif
