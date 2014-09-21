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
#ifndef OPENPAL_IMUTEX_H
#define OPENPAL_IMUTEX_H

#include <assert.h>

#include "openpal/util/Uncopyable.h"

namespace openpal
{

/**
    A non-reentrant Mutex. In C++11 this maps to std::mutex
*/
class IMutex : private Uncopyable
{
public:

	friend class CriticalSection;
	virtual ~IMutex() {}

protected:

	virtual void Lock() = 0;
	virtual void Unlock() = 0;
};

/**
  Uses RAII to safely perform a transaction
*/
class CriticalSection : private openpal::Uncopyable
{
public:

	/// Can be constructed with nullptr or an implementation
	CriticalSection(IMutex* pMutex_) : pMutex(pMutex_)
	{
		Lock(pMutex);
	}

	~CriticalSection()
	{
		Unlock(pMutex);
	}

	static void Lock(IMutex* pMutex)
	{
		if (pMutex)
		{
			pMutex->Lock();
		}
	}

	static void Unlock(IMutex* pMutex)
	{
		if (pMutex)
		{
			pMutex->Unlock();
		}
	}

private:
	IMutex* pMutex;

};

}

#endif
