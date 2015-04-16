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

#ifndef OPENDNP3_PREFIXFIELDS_H
#define OPENDNP3_PREFIXFIELDS_H

#include <openpal/container/WriteBufferView.h>
#include <openpal/util/Uncopyable.h>

namespace opendnp3
{

	class PrefixFields : private openpal::PureStatic
	{
		public:

		template <typename... Args>
		static bool Read(openpal::ReadBufferView& input, Args&... fields)
		{						
			if (input.Size() < (sizeof...(Args)*UInt16::SIZE))
			{
				// not enough in the buffer to even read the length prefixes
				return false;
			}

			uint16_t lengths[sizeof...(Args)];

			for (int i = 0; i < sizeof...(Args); ++i)
			{
				lengths[i] = openpal::UInt16::ReadBuffer(input);
			}

			uint32_t sum = 0;

			for (int i = 0; i < sizeof...(Args); ++i)
			{
				sum += lengths[i];
			}

			if (input.Size() < sum)
			{
				// not enough for the defined fields
				return false;
			}

			ReadFields(input, lengths, fields...);

			return true;
		}

		private:
		
		template <typename... Args>
		static void ReadFields(openpal::ReadBufferView& input, uint16_t* pLength, openpal::ReadBufferView& output, Args&... fields)
		{			
			output = input.Take(*pLength);
			input.Advance(*pLength);
			ReadFields(input, ++pLength, fields...);
		}
		
		static void ReadFields(openpal::ReadBufferView& input, uint16_t* pLength) {}
		
		
	};


}

#endif
