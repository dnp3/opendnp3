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
#ifndef __MOCK_KEYWRAP_H_
#define __MOCK_KEYWRAP_H_

#include <openpal/crypto/IKeyWrapAlgo.h>

#include <testlib/BufferHelpers.h>

#include <stdexcept>

namespace opendnp3
{
	class MockKeyWrap : public openpal::IKeyWrapAlgo
	{
	public:				

		virtual openpal::ReadBufferView WrapKey(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output, std::error_code& ec) const
		{
			return WrapOrUnwrap(kek, input, output, ec);
		}
		
		virtual openpal::ReadBufferView UnwrapKey(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output, std::error_code& ec) const
		{
			return WrapOrUnwrap(kek, input, output, ec);
		}
			
		std::string hexOutput;		

	private:

		openpal::ReadBufferView WrapOrUnwrap(const openpal::ReadBufferView& kek, const openpal::ReadBufferView& input, openpal::WriteBufferView& output, std::error_code& ec) const
		{
			if (!(kek.Size() == 16 || kek.Size() == 32))
			{
				throw std::logic_error("MockKeyWrap: bad key size");
			}

			testlib::HexSequence hex(hexOutput);
			auto data = hex.ToReadOnly();
			if (output.Size() < data.Size())
			{
				throw std::logic_error("MockKeyWrap: Output buffer too small");
			}

			return data.CopyTo(output);
		}
	};
}

#endif

