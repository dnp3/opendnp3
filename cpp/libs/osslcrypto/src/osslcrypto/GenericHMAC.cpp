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

#include "GenericHMAC.h"

#include <openpal/util/Finally.h>

using namespace openpal;

namespace osslcrypto
{
	bool CalculateHMAC(
		const EVP_MD* md,
		uint32_t outputSize,
		const openpal::ReadBufferView& key,
		std::initializer_list<openpal::ReadBufferView> data,
		openpal::WriteBufferView& output
		)
	{
		if (output.Size() < outputSize)
		{
			return false;
		}

		HMAC_CTX ctx;
		HMAC_CTX_init(&ctx);
		auto cleanup = Finally([&]() { HMAC_CTX_cleanup(&ctx); });

		if (HMAC_Init_ex(&ctx, key, key.Size(), md, nullptr) == 0)
		{
			return false;
		}
					
		for (auto& bytes : data)
		{
			if (HMAC_Update(&ctx, bytes, bytes.Size()) == 0)
			{
				return false;
			}
		}

		unsigned int length = 0;
		if (HMAC_Final(&ctx, output, &length) == 0)
		{
			return false;
		}

		return true;
	}
}

