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
#ifndef OPENPAL_ICRYPTOPROVIDER_H
#define OPENPAL_ICRYPTOPROVIDER_H

#include "IHashProvider.h"

#include <memory>


namespace openpal
{
	/**
	* A provider of cryptographic services. All function are assumed to be thread-safe
	* such that multiple threads can safely share a single instance of this class.
	*/
	class ICryptoProvider
	{
	public:
		
		virtual ~ICryptoProvider() {}

		/** 
		 * Fill the specified buffer with secure random bytes
		 * @return true if the specified buffer can be completely filled with secure random numbers	     		 
		*/
		virtual bool GetSecureRandom(WriteBufferView& buffer) = 0;
		
		/// --- Functions related to AES 128 Key Wrap Algorithm ----

		virtual bool WrapKeyAES128(const ReadBufferView& kek, const ReadBufferView& input, WriteBufferView& output) = 0;
		virtual bool UnwrapKeyAES128(const ReadBufferView& kek, const ReadBufferView& input, WriteBufferView& output) = 0;

		/// --- Functions related to AES 256 Key Wrap Algorithm ----
		virtual bool WrapKeyAES256(const ReadBufferView& kek, const ReadBufferView& input, WriteBufferView& output) = 0;
		virtual bool UnwrapKeyAES256(const ReadBufferView& kek, const ReadBufferView& input, WriteBufferView& output) = 0;

		/// --- Functions related to SHA1 ----

		// Dynamically creates a provider that can incrementally calculate a SHA1 hash
		virtual std::unique_ptr<IHashProvider> CreateSHA1Provider() = 0;

		// Calculates a SHA1 hash out in single input buffer
		virtual bool CalcSHA1(const ReadBufferView& input, WriteBufferView& output) = 0;

		/// --- Functions related to SHA256 ----

		// Dynamically creates a provider that can incrementally calculate a SHA256 hash
		virtual std::unique_ptr<IHashProvider> CreateSHA256Provider() = 0;

		// Calculates a SHA1 hash out in single input buffer
		virtual bool CalcSHA256(const ReadBufferView& input, WriteBufferView& output) = 0;
	};

}

#endif

