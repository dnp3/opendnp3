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

#include "SHA256HashProvider.h"

#include <assert.h>

namespace osslcrypto
{
	bool SHA256HashProvider::CalcHash(const openpal::ReadBufferView& input, openpal::WriteBufferView& output)
	{
		if (output.Size() < OUTPUT_SIZE)
		{
			return false;
		}
		else
		{
			SHA256(input, input.Size(), output);
			output.Advance(OUTPUT_SIZE);
			return true;
		}
	}

	// Called to reset the state of the provider
	bool SHA256HashProvider::Init()
	{
		return SHA256_Init(&ctx) > 0;
	}

	// Add the buffer to the running hash calculation
	bool SHA256HashProvider::Add(const openpal::ReadBufferView& input)
	{
		return SHA256_Update(&ctx, input, input.Size()) > 0;
	}

	// copy the digest into the output buffer and reset the state
	bool SHA256HashProvider::Complete(openpal::WriteBufferView& output)
	{
		if (output.Size() < OUTPUT_SIZE)
		{
			return false;
		}
		else
		{

			bool success = SHA256_Final(output, &ctx) > 0;
			if (success)
			{
				output.Advance(OUTPUT_SIZE);
			}
			return success;
		}		
	}

}

