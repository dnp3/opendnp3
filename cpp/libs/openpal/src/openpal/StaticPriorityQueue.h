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
#ifndef __STATIC_PRIORITY_QUEUE_H_
#define __STATIC_PRIORITY_QUEUE_H_

#include "StaticLinkedList.h"
#include "Configure.h"
#include <assert.h>

namespace openpal
{

template <class ValueType>
struct DefaultLessThan
{
	static bool IsLessThan(const ValueType& lhs, const ValueType& rhs)
	{
		return lhs < rhs;
	}
};

template <class ValueType, class IndexType, IndexType N, class LessThan = DefaultLessThan<ValueType>>
class StaticPriorityQueue : private Uncopyable
{

public:		

	IndexType Size() const
	{
		return list.Size();
	}

	IndexType Capacity() const
	{
		return list.Capacity();
	}

	bool IsEmpty() const
	{
		return list.IsEmpty();
	}

	bool IsNotEmpty() const
	{
		return list.IsNotEmpty();
	}

	bool IsFull() const
	{
		return list.IsFull();
	}

	void Clear()
	{
		list.Clear();
	}

	template <class Match>
	bool RemoveFirst(const Match& match)
	{
		auto pNode = list.RemoveFirst(match);
		return pNode ? true : false;				
	}

	template <class Match>
	bool Contains(const Match& match)
	{		
		return list.FindFirst(match) ? true : false;
	}	

	ValueType& Peek()
	{
		auto pNode = list.FindFirst([](const ValueType&)
		{
			return true;
		});
		assert(pNode);
		return (pNode->value);
	}

	ValueType& Pop()
	{
		auto pNode = list.RemoveFirst([](const ValueType&)
		{
			return true;
		});
		assert(pNode);
		return (pNode->value);
	}

	bool Enqueue(const ValueType& value)
	{
		auto lt = [](const ValueType& lhs, const ValueType& rhs){ return LessThan::IsLessThan(lhs, rhs);  };
		return list.Insert(value, lt);
	}

private:
	
	StaticLinkedList<ValueType, IndexType, N> list;


};

}

#endif
