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

#ifndef __ERASED_TYPE_H_
#define __ERASED_TYPE_H_

#include <cstdint>
#include <cstring>

namespace openpal
{

// a generic cell that can be used to store events of any type using an erased buffer
template <size_t MaxSize>
class ErasedType
{
	public:
	
	ErasedType() : size(0)
	{}

	ErasedType(const ErasedType& copy) : size(copy.size)
	{
		memcpy(buffer, copy.buffer, copy.size);
	}

	ErasedType& operator= (const ErasedType& other)
	{
		if (this != &other)
		{
			this->size = other.size;
			memcpy(buffer, other.buffer, other.size);
		}
		
		return *this;
	}	

	template <class T>
	void Set(const T& type)
	{
		static_assert(sizeof(T) <= MaxSize, "Type is too big for erasure");
		(*reinterpret_cast<T*>(buffer)) = type;
		size = sizeof(T);
	}

	template <class T>
	bool Read(T& type) const
	{
		static_assert(sizeof(T) <= MaxSize, "Type is too big for erasure");
		if (size < sizeof(T))
		{
			return false;
		}
		else
		{
			const T& casted = *(reinterpret_cast<const T*>(buffer));
			type = casted;
			return true;
		}
		
	}
	
	private:
	size_t size;
	uint8_t buffer[MaxSize];
};

}

#endif
