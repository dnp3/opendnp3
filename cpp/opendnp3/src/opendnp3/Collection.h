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
#ifndef __COLLECTION_H_
#define __COLLECTION_H_

#include "LazyIterable.h"

#include <opendnp3/Uncopyable.h>

namespace opendnp3
{

template <class T, class U, class MapFunc>
class MappedIterable : public Iterable<U>
{
	public:
		MappedIterable(const Iterable<T>& aProxy, const MapFunc& aMapFunc): 
			Iterable<U>(aProxy.buffer, aProxy.count),
			proxy(aProxy),
			mapFunc(aMapFunc)
		{}

	protected:
		virtual U ValueAt(openpal::ReadOnlyBuffer& buffer, uint32_t i) const final
		{
			return mapFunc(proxy.ValueAt(buffer, i));
		}

	private:
		const Iterable<T>& proxy;
		MapFunc mapFunc;
};

template <class T>
struct Collection : private PureStatic
{
	template <class ReadFunc>
	static LazyIterable<T,ReadFunc> Lazily(const openpal::ReadOnlyBuffer& buffer, uint32_t aSize, const ReadFunc& aReadFunc)
	{
		return LazyIterable<T, ReadFunc>(buffer, aSize, aReadFunc);
	}

	template <class U, class MapFunc>
	static MappedIterable<T, U, MapFunc> Map(const Iterable<T>& proxy, const MapFunc& aMapFunc)
	{
		return MappedIterable<T, U, MapFunc>(proxy, aMapFunc);
	}
};

}

#endif
