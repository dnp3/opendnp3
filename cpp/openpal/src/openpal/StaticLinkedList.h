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
#ifndef __STATIC_LINKED_LIST_H_
#define __STATIC_LINKED_LIST_H_

#include "Uncopyable.h"
#include "LinkedListAdapter.h"
#include "StaticArray.h"

namespace openpal
{

template <class ValueType, class IndexType, IndexType N>
class StaticLinkedList : private Uncopyable
{
public:
	StaticLinkedList() : underlying(), adapter(underlying.ToIndexable())
	{}

	IndexType Size() const
	{
		return adapter.size();
	}

	bool Remove(const ValueType& value)
	{
		auto iter = adapter.Iterate();
		while (iter.HasNext())
		{
			auto pNode = iter.Next();
			if (pNode->value == value)
			{
				adapter.Remove(pNode);
				return true;
			}
		}
		return false;
	}

	bool Add(const ValueType& value)
	{
		return (adapter.Add(value) != nullptr);
	}

	template <class Selector>
	ListNode<ValueType>* FindFirst(Selector select)
	{
		auto iter = adapter.Iterate();
		while (iter.HasNext())
		{
			auto pNode = iter.Next();
			if (select(pNode->value))
			{				
				return pNode;
			}			
		}
		return nullptr;
	}

	template <class Selector>
	void Foreach(Selector select)
	{
		auto iter = adapter.Iterate();
		while (iter.HasNext())
		{			
			select(iter.Next()->value);
		}		
	}

	template <class Selector>
	ListNode<ValueType>* RemoveFirst(Selector select)
	{
		auto pNode = this->FindFirst(select);
		if (pNode)
		{
			adapter.Remove(pNode);
		}
		return pNode;
	}

private:

	StaticArray<ListNode<ValueType>, IndexType, N> underlying;
	LinkedListAdapter<ValueType, IndexType> adapter;
};

}

#endif
