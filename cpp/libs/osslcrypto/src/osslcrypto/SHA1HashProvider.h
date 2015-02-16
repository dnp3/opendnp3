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

#ifndef OSSLCRYPTO_SHA1HASHPROVIDER_H
#define OSSLCRYPTO_SHA1HASHPROVIDER_H

#include <openpal/crypto/ICryptoProvider.h>
#include <openpal/util/Uncopyable.h>

#include <openssl/sha.h>


namespace osslcrypto
{

	class SHA1HashProvider : public openpal::IHashProvider, private openpal::Uncopyable
	{
		public:

		virtual uint16_t OutputSizeInBytes() const override final { return 20; }

		// Called to reset the state of the provider
		virtual bool Init() override final;

		// Add the buffer to the running hash calculation
		virtual bool Add(const openpal::ReadBufferView& input) override final;

		// copy the digest into the output buffer and reset the state
		virtual bool Complete(openpal::WriteBufferView& output) override final;

		private:

		SHA_CTX ctx;
	};
}

#endif