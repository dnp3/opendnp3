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
#ifndef __LINKED_LIST_ADAPTER_H_
#define __LINKED_LIST_ADAPTER_H_

#include "Indexable.h"

namespace openpal
{

template <class ValueType>
class DoubleListNode
{		
	public:
		DoubleListNode(): prev(nullptr), next(nullptr)
		{}

		ValueType value;

	private:
		DoubleListNode* prev;
		DoubleListNode* next;

	template <class T, class U>
	friend class DoublyLinkedListAdapter;

	template <class T>
	friend class DoublyLinkedListIterator;
};


template <class ValueType>
class DoublyLinkedListIterator
{
	public:
		DoublyLinkedListIterator(DoubleListNode< ValueType>* pStart) : pCurrent(pStart)
		{}

		bool HasNext()
		{
			return (pCurrent != nullptr);
		}

		DoubleListNode< ValueType>* Next()
		{
			assert(pCurrent != nullptr);
			auto pRet = pCurrent;
			pCurrent = pCurrent->next;
			return pRet;
		}

	private:
		DoubleListNode<ValueType>* pCurrent;
};


// A container adapter for a doubly-linked list
template <class ValueType, class IndexType>
class DoublyLinkedListAdapter : public HasSize<IndexType>
{
	public:

		typedef DoublyLinkedListIterator<ValueType> Iterator;

		DoublyLinkedListAdapter(Indexable<DoubleListNode<ValueType>, IndexType> aUnderlying) : 
			HasSize<IndexType>(0),
			pHead(nullptr),
			pTail(nullptr),
			pFree(nullptr),
			underlying(aUnderlying)
		{			
			Initialize();
		}

		Iterator Iterate() { return Iterator(pHead); }
		
		inline DoubleListNode<ValueType>* Add(const ValueType& value);

		inline void Remove(DoubleListNode<ValueType>* apNode);
		
		inline bool IsFull() const;
	
	private:
		DoubleListNode<ValueType>* pHead;
		DoubleListNode<ValueType>* pTail;
		DoubleListNode<ValueType>* pFree;
		Indexable<DoubleListNode<ValueType>, IndexType> underlying;

		inline static void Link(DoubleListNode<ValueType>* prev, DoubleListNode<ValueType>* next);
		
		void Initialize();
};

template <class ValueType, class IndexType>
DoubleListNode<ValueType>* DoublyLinkedListAdapter<ValueType, IndexType>::Add(const ValueType& value)
{
	if(pFree == nullptr) return nullptr;
	else
	{
		auto pNode = pFree;
		pNode->value = value;
		pFree = pFree->next;
		pNode->next = nullptr;
		Link(pTail, pNode);		
		pTail = pNode;
		if(pHead == nullptr) pHead = pTail;
		++(this->size);		
		return pTail;
	}
}

template <class ValueType, class IndexType>
void DoublyLinkedListAdapter<ValueType, IndexType>::Remove(DoubleListNode<ValueType>* apNode)
{
	if(apNode->prev == nullptr) // it's the head
	{
		if(apNode->next == nullptr) pHead = pTail = nullptr; // list is now empty
		else pHead = apNode->next; // head but not tail		
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

template <class ValueType, class IndexType>
bool DoublyLinkedListAdapter<ValueType, IndexType>::IsFull() const
{
	return (pFree == nullptr);
}


template <class ValueType, class IndexType>
void DoublyLinkedListAdapter<ValueType, IndexType>::Link(DoubleListNode<ValueType>* first, DoubleListNode<ValueType>* second)
{
	if(first) first->next = second;
	if(second) second->prev = first;
}

template <class ValueType, class IndexType>
void DoublyLinkedListAdapter<ValueType, IndexType>::Initialize()
{
	if(underlying.IsNotEmpty()) 
	{
		pFree = &underlying[0];		
		for(IndexType i = 1; i < underlying.Size(); ++i)
		{	
			Link(&underlying[i-1], &underlying[i]);
		}
	}
}

}

#endif
