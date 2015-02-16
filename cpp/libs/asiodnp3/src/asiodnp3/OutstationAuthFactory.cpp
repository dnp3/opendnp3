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


#include "OutstationAuthFactory.h"

#include <opendnp3/outstation/NullOutstationAuthProvider.h>
#include <secauthv5/outstation/SAv5OutstationAuthProvider.h>

#include <memory>
#include <stdexcept>

using namespace opendnp3;
using namespace secauthv5;

namespace asiodnp3
{
	std::unique_ptr<IOutstationAuthProvider> OutstationAuthFactory::Create(const OutstationStackConfig& config, openpal::ICryptoProvider* pCrypto)
	{
		switch (config.authentication.mode)
		{
			case(ConfigAuthMode::SAV5) :
				return CreateSAv5Provider(config, pCrypto);
			default:
				return std::unique_ptr<IOutstationAuthProvider>(new NullOutstationAuthProvider());
		}
	}	

	std::unique_ptr<IOutstationAuthProvider> OutstationAuthFactory::CreateSAv5Provider(const OutstationStackConfig& config, openpal::ICryptoProvider* pCrypto)
	{
		if (pCrypto)
		{
			return std::unique_ptr<IOutstationAuthProvider>(
				new SAv5OutstationAuthProvider(config.outstation.params.maxRxFragSize, *pCrypto)
			);
		}
		else
		{
			throw std::runtime_error("SAv5 requires a crypto provider");
		}
	}
}


