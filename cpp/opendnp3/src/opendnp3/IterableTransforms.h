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
#ifndef __ITERABLE_TRANSFORMS_H_
#define __ITERABLE_TRANSFORMS_H_

#include "LazyIterable.h"

#include "Uncopyable.h"

namespace opendnp3
{

template <class T>
class IterableTransforms : private PureStatic
{
	public:

	template <class ReadFunc>
	static LazyIterable<T,ReadFunc> From(const openpal::ReadOnlyBuffer& buffer, uint32_t aSize, const ReadFunc& aReadFunc)
	{
		return LazyIterable<T, ReadFunc>(buffer, aSize, aReadFunc);
	}	

	template <class U, class MapToU>
	class MappedIterableBuffer : public IterableBuffer<U>
	{
		public:
			MappedIterableBuffer(const IterableBuffer<T>& aProxy, const MapToU& aMapFun) : 
				IterableBuffer<U>(aProxy.buffer, aProxy.count),
				proxy(aProxy),
				mapFun(aMapFun)
			{}

		protected:
			virtual U ValueAt(openpal::ReadOnlyBuffer& buff, uint32_t pos) const final
			{
				return mapFun(proxy.ValueAt(buff, pos));
			}

		private:
			const IterableBuffer<T>& proxy;
			MapToU mapFun;
	};

	template<class U, class MapToU>
	static MappedIterableBuffer<U, MapToU> Map(const IterableBuffer<T> &iter, const MapToU& fun)
	{
		return MappedIterableBuffer<U, MapToU>(iter, fun);
	}

};

}

#endif
