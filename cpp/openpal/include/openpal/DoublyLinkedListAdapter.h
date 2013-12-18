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

template <class T>
class DoubleListNode
{		
	public:
		DoubleListNode(): prev(nullptr), next(nullptr)
		{}

		T value;

	private:
		DoubleListNode* prev;
		DoubleListNode* next;

	template <class T>
	friend class DoublyLinkedListAdapter;

	template <class T>
	friend class DoublyLinkedListIterator;
};


template <class T>
class DoublyLinkedListIterator
{
	public:
		DoublyLinkedListIterator(DoubleListNode<T>* pStart) : pCurrent(pStart)
		{}

		bool HasNext()
		{
			return (pCurrent != nullptr);
		}

		DoubleListNode<T>* Next()
		{
			assert(pCurrent != nullptr);
			auto pRet = pCurrent;
			pCurrent = pCurrent->next;
			return pRet;
		}

	private:
		DoubleListNode<T>* pCurrent;
};


// A container adapter for a doubly-linked list
template <class T>
class DoublyLinkedListAdapter : public HasSize
{
	public:

		typedef DoublyLinkedListIterator<T> Iterator;

		DoublyLinkedListAdapter(Indexable<DoubleListNode<T>> aUnderlying) : 
			HasSize(0),
			pHead(nullptr),
			pTail(nullptr),
			pFree(nullptr),
			underlying(aUnderlying)
		{			
			Initialize();
		}

		Iterator Iterate() { return Iterator(pHead); }
		
		inline DoubleListNode<T>* Add(const T& value);

		inline void Remove(DoubleListNode<T>* apNode);
		
		inline bool IsFull() const;
	
	private:
		DoubleListNode<T>* pHead;
		DoubleListNode<T>* pTail;
		DoubleListNode<T>* pFree;
		Indexable<DoubleListNode<T>> underlying;

		inline static void Link(DoubleListNode<T>* prev, DoubleListNode<T>* next);
		
		void Initialize();
};

template <class T>
DoubleListNode<T>* DoublyLinkedListAdapter<T>::Add(const T& value)
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
		++size;		
		return pTail;
	}
}

template <class T>
void DoublyLinkedListAdapter<T>::Remove(DoubleListNode<T>* apNode)
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
	--size;
}

template <class T>
bool DoublyLinkedListAdapter<T>::IsFull() const
{
	return (pFree == nullptr);
}


template <class T>
void DoublyLinkedListAdapter<T>::Link(DoubleListNode<T>* first, DoubleListNode<T>* second)
{
	if(first) first->next = second;
	if(second) second->prev = first;
}

template <class T>
void DoublyLinkedListAdapter<T>::Initialize()
{
	if(underlying.IsNotEmpty()) 
	{
		pFree = &underlying[0];		
		for(uint32_t i = 1; i < underlying.Size(); ++i)
		{	
			Link(&underlying[i-1], &underlying[i]);
		}
	}
}

}

#endif
