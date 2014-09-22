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
#ifndef OPENPAL_RINGBUFFER_H
#define OPENPAL_RINGBUFFER_H

#include <cstdint>

namespace openpal
{

/** 
	A byte-oriented ring buffer.
	Interrupt-safe (w/o disabling interrupts) if single producer
	and single consumer, one reading and one writing.
	
	N must be a power of 2, and is enforced via static assert.
*/	
template <uint8_t N>
class RingBuffer
{		
	static_assert((N > 1) & !(N & (N - 1)), "Should use a power of 2 as template parameter, e.g. 16, 32, 64, 128");

public:

	RingBuffer() : head(0), tail(0) {}

	bool Put(uint8_t byteIn)
	{
		if(Full())
		{
			return false;						
		}
		else
		{
			buffer[(head++) & (N-1)] = byteIn;
			return true;
		}
	}

	bool Get(uint8_t& byteOut)
	{
		 if(Empty())
		 {
			return false;
		 }
		 else
		 {
			byteOut = buffer[(tail++) & (N-1)];
			return true;
		 }
	}
	
	inline bool Full()
	{
		return Count() == N;
	}

	inline bool Empty()
	{
		return Count() == 0;
	}
	
	
private:

	inline uint8_t Count()
	{
		return head - tail;
	}
			
	uint8_t buffer[N];

	uint8_t head;
	uint8_t tail;	
	
	RingBuffer(const RingBuffer&) = delete;
	RingBuffer& operator= (const RingBuffer&) = delete;	
};

}

#endif
