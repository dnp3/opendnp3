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
#ifndef OPENPAL_MANAGEDPTR_H
#define OPENPAL_MANAGEDPTR_H

#include <assert.h>

namespace openpal
{

/**
	A smart pointer that may or may not delete the pointer on destruction depending on how it is configured.
	The type cannot be copied, only moved, so that only one instance at a time owns the pointer (no reference counting)
*/
template <class T>
class ManagedPtr
{
public:

	static ManagedPtr<T> Deleted(T* pValue)
	{
		return ManagedPtr<T>(pValue, true);
	}

	static ManagedPtr<T> WrapperOnly(T* pValue)
	{
		return ManagedPtr<T>(pValue, false);
	}

	ManagedPtr() : pValue(nullptr), managed(false)
	{}

	~ManagedPtr()
	{
		if (managed)
		{
			delete pValue;
		}
	}

	void Release()
	{
		if (managed)
		{
			managed = false;
			delete pValue;
		}

		pValue = nullptr;
	}

	T* operator->()
	{
		return pValue;
	}

	const T* operator->() const
	{
		return pValue;
	}

	T& operator*() const
	{
		assert(pValue != nullptr);
		return *pValue;
	}

	// implicit cast to bool
	bool IsDefined() const
	{
		return pValue != nullptr;
	}

	// move constructor
	ManagedPtr(ManagedPtr&& other) : pValue(other.pValue), managed(other.managed)
	{
		other.managed = false;
		other.pValue = nullptr;
	}

	// move assign operator
	ManagedPtr& operator=(ManagedPtr && other)
	{
		if (this != &other)
		{
			if (managed && pValue)
			{
				delete pValue;
			}

			this->managed = other.managed;
			this->pValue = other.pValue;

			other.managed = false;
			other.pValue = nullptr;
		}

		return *this;
	}

private:

	ManagedPtr& operator=(const ManagedPtr&) = delete;

	ManagedPtr(const ManagedPtr&) = delete;

	ManagedPtr(T* pValue_, bool managed_) : pValue(pValue_), managed(managed_)
	{}

	T* pValue;
	bool managed;

};

}


#endif

