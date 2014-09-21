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
#ifndef OPENPAL_ACTION0_H
#define OPENPAL_ACTION0_H

#include "Erasure.h"
#include "openpal/Configure.h"

namespace openpal
{

class Action0 : public Erasure
{

public:

	typedef void(*Invoke)(const uint8_t* pBuffer);

	Action0();

	Action0(const Action0& other);

	Action0& operator=(const Action0& other);

	void Apply() const;	
	
	bool IsSet() const;

	template <class Lambda>
	static Action0 Bind(Lambda& lambda)
	{
		static_assert(sizeof(Lambda) <= sizes::MAX_ERASURE_SIZE, "Lambda is too big for erasure");
		Action0 runnable(&Action0::RunLambda<Lambda>, sizeof(lambda));
		new(runnable.bytes) Lambda(lambda); // use placement new
		return runnable;
	}

	template <class T>
	static Action0 BindDelete(T* pPointer)
	{
		auto lambda = [pPointer]() { delete pPointer; };
		return Bind(lambda);
	}

protected:	

	template <class Lambda>
	static void RunLambda(const uint8_t* pBuffer)
	{
		(*reinterpret_cast<const Lambda*>(pBuffer))();
	}

	Action0(Invoke pInvoke_, uint32_t size_);

	Invoke pInvoke;
};

}

#endif
