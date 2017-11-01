/*
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
#ifndef OPENPAL_LINKEDLIST_H
#define OPENPAL_LINKEDLIST_H

#include "ArrayView.h"

#include "openpal/container/Array.h"

#include <cstdio>

namespace openpal
{

using list_size_type_t = uint32_t;

template <class T>
class ListNode
{
public:
	ListNode() = default;

	T value;

private:
	ListNode* prev = nullptr;
	ListNode* next = nullptr;

	template <class T>
	friend class LinkedList;

	template <class T>
	friend class LinkedListIterator;
};


template <class T>
class LinkedListIterator
{
public:
	static LinkedListIterator<T> Undefined()
	{
		return LinkedListIterator(nullptr);
	}

	static LinkedListIterator<T> From(ListNode<T>* pStart)
	{
		return LinkedListIterator(pStart);
	}

	template <class U>
	T* Find(const U& matches)
	{
		while (pCurrent)
		{
			if (matches(pCurrent->value))
			{
				return &(pCurrent->value);
			}
			pCurrent = pCurrent->next;
		}

		return nullptr;
	}

	bool HasNext() const
	{
		return (pCurrent != nullptr);
	}

	ListNode<T>* Next()
	{
		if (pCurrent == nullptr)
		{
			return nullptr;
		}
		else
		{
			auto pRet = pCurrent;
			pCurrent = pCurrent->next;
			return pRet;
		}
	}

	ListNode<T>* Current()
	{
		return pCurrent;
	}

	T* CurrentValue()
	{
		if (pCurrent)
		{
			return &pCurrent->value;
		}
		else
		{
			return nullptr;
		}
	}

private:

	LinkedListIterator(ListNode<T>* pStart) : pCurrent(pStart)
	{}

	ListNode<T>* pCurrent;
};


// A container adapter for a -linked list
template <class T>
class LinkedList : public HasSize<list_size_type_t>
{
public:

	typedef LinkedListIterator<T> Iterator;

	LinkedList(list_size_type_t maxSize) :
		HasSize<list_size_type_t>(0),
		pHead(nullptr),
		pTail(nullptr),
		pFree(nullptr),
		underlying(maxSize)
	{
		Initialize();
	}

	list_size_type_t Capacity() const
	{
		return underlying.Size();
	}

	void Clear()
	{
		if (this->IsNotEmpty())
		{
			// link the remaining free list to the end of active list
			this->Link(pTail, pFree);
			// set the free pointer to the head
			pFree = pHead;
			pHead = pTail = nullptr;
			this->size = 0;
		}
	}

	inline ListNode<T>* Head()
	{
		return pHead;
	}

	Iterator Iterate() const
	{
		return Iterator::From(pHead);
	}

	template <class Selector>
	ListNode<T>* FindFirst(Selector select);

	ListNode<T>* Add(const T& value);

	template <class U>
	void While(const U& select)
	{
		auto iter = this->Iterate();
		bool result = true;
		while (result && iter.HasNext())
		{
			result = select(iter.Next()->value);
		}
	}

	template <class U>
	void Foreach(const U& action)
	{
		auto iter = this->Iterate();
		while (iter.HasNext())
		{
			action(iter.Next()->value);
		}
	}
	
	template <class U>
	list_size_type_t RemoveAll(const U& match)
	{
		list_size_type_t count = 0;

		auto iter = this->Iterate();
		auto pCurrent = iter.Next();
		while (pCurrent)
		{
			if (match(pCurrent->value))
			{
				auto pRemoved = pCurrent;
				pCurrent = iter.Next();
				this->Remove(pRemoved);
				++count;
			}
			else
			{
				pCurrent = iter.Next();
			}
		}

		return count;
	}

	void Remove(ListNode<T>* apNode);

	inline bool IsFull() const;

private:

	ListNode<T>* pHead;
	ListNode<T>* pTail;
	ListNode<T>* pFree;

	Array<ListNode<T>, list_size_type_t> underlying;

	ListNode<T>* Insert(const T& value, ListNode<T>* left, ListNode<T>* right);

	inline static void Link(ListNode<T>* prev, ListNode<T>* next);

	void Initialize();
};

template <class T>
ListNode<T>* LinkedList<T>::Add(const T& value)
{
	return this->Insert(value, pTail, nullptr);
}

template <class T>
ListNode<T>* LinkedList<T>::Insert(const T& value, ListNode<T>* pLeft, ListNode<T>* pRight)
{
	if (pFree == nullptr)
	{
		return nullptr;
	}
	else
	{
		// initialize the new node, and increment the size
		auto pNode = pFree;
		pFree = pFree->next;
		pNode->value = value;
		++(this->size);

		this->Link(pLeft, pNode);
		this->Link(pNode, pRight);

		// change of head
		if (pLeft == nullptr)
		{
			pHead = pNode;
		}

		// change of tail
		if (pRight == nullptr)
		{
			pTail = pNode;
		}

		return pNode;
	}
}

template <class T>
template <class Selector>
ListNode<T>* LinkedList<T>::FindFirst(Selector select)
{
	auto iter = this->Iterate();
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

template <class T>
void LinkedList<T>::Remove(ListNode<T>* apNode)
{
	if(apNode->prev == nullptr) // it's the head
	{
		if (apNode->next == nullptr)
		{
			pHead = pTail = nullptr; // list is now empty
		}
		else
		{
			pHead = apNode->next; // head but not tail
		}
	}
	else
	{
		if(apNode->next == nullptr) pTail = apNode->prev; // was only the tail
	}

	// attach the adjacent nodes to eachother if they exist
	Link(apNode->prev, apNode->next);

	// Now that the data list is complete, attach the freed node to the front of the free list
	apNode->next = pFree;
	if(pFree != nullptr) pFree->prev = apNode;
	apNode->prev = nullptr; // it's the head now
	pFree = apNode;
	--(this->size);
}

template <class T>
bool LinkedList<T>::IsFull() const
{
	return (pFree == nullptr);
}



template <class T>
void LinkedList<T>::Link(ListNode<T>* first, ListNode<T>* second)
{
	if(first) first->next = second;
	if(second) second->prev = first;
}

template <class T>
void LinkedList<T>::Initialize()
{
	if(underlying.IsNotEmpty())
	{
		pFree = &underlying[0];
		for(list_size_type_t i = 1; i < underlying.Size(); ++i)
		{
			Link(&underlying[i - 1], &underlying[i]);
		}
	}
}

}

#endif
