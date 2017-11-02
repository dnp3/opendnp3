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
	friend class ListIterator;
};


template <class T>
class ListIterator
{
public:
	
	static ListIterator<T> From(ListNode<T>* start)
	{
		return ListIterator(start);
	}

	template <class U>
	T* Find(const U& matches)
	{
		while (this->current)
		{
			if (matches(this->current->value))
			{
				return &(this->current->value);
			}

			this->current = this->current->next;
		}

		return nullptr;
	}

	bool HasNext() const
	{
		return this->current;
	}

	ListNode<T>* Next()
	{
		if (!this->current) return nullptr;
		auto ret = this->current;
		this->current = this->current->next;
		return ret;
	}

	inline ListNode<T>* Current()
	{
		return this->current;
	}

	inline T* CurrentValue()
	{
		return (this->current) ? &(this->current->value) : nullptr;
	}

private:

	ListIterator(ListNode<T>* start) : current(start)
	{}

	ListNode<T>* current;
};


// A container adapter for a -linked list
template <class T>
class LinkedList : public HasSize<list_size_type_t>
{
public:

	typedef ListIterator<T> Iterator;

	LinkedList(list_size_type_t maxSize) :
		HasSize<list_size_type_t>(0),		
		underlying(maxSize)
	{
		Initialize();
	}

	list_size_type_t Capacity() const
	{
		return underlying.Size();
	}	

	inline ListNode<T>* Head()
	{
		return this->head;
	}

	Iterator Iterate() const
	{
		return Iterator::From(this->head);
	}	

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
		auto current = iter.Next();
		while (current)
		{
			if (match(current->value))
			{
				auto removed = current;
				current = iter.Next();
				this->Remove(removed);
				++count;
			}
			else
			{
				current = iter.Next();
			}
		}

		return count;
	}

	void Remove(ListNode<T>* node);

	inline bool IsFull() const;

private:

	ListNode<T>* head = nullptr;
	ListNode<T>* tail = nullptr;
	ListNode<T>* free = nullptr;

	Array<ListNode<T>, list_size_type_t> underlying;

	ListNode<T>* Insert(const T& value, ListNode<T>* left, ListNode<T>* right);

	inline static void Link(ListNode<T>* prev, ListNode<T>* next);

	void Initialize();
};

template <class T>
ListNode<T>* LinkedList<T>::Add(const T& value)
{
	return this->Insert(value, this->tail, nullptr);
}

template <class T>
ListNode<T>* LinkedList<T>::Insert(const T& value, ListNode<T>* left, ListNode<T>* right)
{
	if (!this->free) return nullptr;

	// initialize the new node, and increment the size
	auto new_node = this->free;
	this->free = this->free->next;

	new_node->value = value;
	++(this->size);

	this->Link(left, new_node);
	this->Link(new_node, right);

	// change of head
	if (!left)
	{
		this->head = new_node;
	}

	// change of tail
	if (!right)
	{
		this->tail = new_node;
	}

	return new_node;
}

template <class T>
void LinkedList<T>::Remove(ListNode<T>* node)
{
	if(node->prev == nullptr) // it's the head
	{
		if (node->next == nullptr)
		{
			this->head = this->tail = nullptr; // list is now empty
		}
		else
		{
			this->head = node->next; // head but not tail
		}
	}
	else
	{
		if(node->next == nullptr) this->tail = node->prev; // was only the tail
	}

	// attach the adjacent nodes to eachother if they exist
	Link(node->prev, node->next);

	// Now that the data list is complete, attach the freed node to the front of the free list
	node->next = this->free;
	if(this->free != nullptr) this->free->prev = node;
	node->prev = nullptr; // it's the head now
	this->free = node;
	--(this->size);
}

template <class T>
bool LinkedList<T>::IsFull() const
{
	return (this->free == nullptr);
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
	if (underlying.IsEmpty()) return;
	
	this->free = &underlying[0];
	for(list_size_type_t i = 1; i < underlying.Size(); ++i)
	{
		Link(&underlying[i - 1], &underlying[i]);
	}	
}

}

#endif
