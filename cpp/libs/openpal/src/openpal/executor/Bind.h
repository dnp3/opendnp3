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
#ifndef __OPENPAL_BIND_H_
#define __OPENPAL_BIND_H_

#include "Runnable.h"

#include "openpal/Configure.h"
#include "openpal/StaticSizeConfiguration.h"

namespace openpal
{

template <class Lambda>
class LambdaRunnable : public Runnable
{
	static_assert(sizeof(Lambda) <= sizes::MAX_FUNCTION_ZERO_SIZE, "Lambda is too big for static buffer");

	public:

	LambdaRunnable(Lambda& lambda) : Runnable(&RunLambda, sizeof(Lambda))
	{
		new(bytes) Lambda(lambda); // use placement new
	}

	private:

	static void RunLambda(const uint8_t* pBuffer)
	{
		(*reinterpret_cast<const Lambda*>(pBuffer))();		
	}
};

template <class T>
Runnable BindDelete(T* pType)
{
	auto lambda = [pType]() { delete pType; };
	return Bind(lambda);
}



template <class Lambda>
Runnable Bind(Lambda& lambda)
{
	return LambdaRunnable<Lambda>(lambda);
}

}

#endif
