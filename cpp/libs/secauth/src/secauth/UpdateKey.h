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
#ifndef SECAUTH_UPDATEKEY_H
#define SECAUTH_UPDATEKEY_H

#include <openpal/crypto/SecureStaticBuffer.h>

#include <opendnp3/gen/KeyWrapAlgorithm.h>

namespace secauth
{

/**
	Stores and provides read/write access to an update key
*/
class UpdateKey
{
	public:

		// a view of the underlying key
		class View
		{
			
		public:

			View() : algorithm(opendnp3::KeyWrapAlgorithm::UNDEFINED)
			{}
			
			View(
				opendnp3::KeyWrapAlgorithm algorithm_,
				openpal::RSlice key_
				) : 
				algorithm(algorithm_), data(key_)
			{}

			opendnp3::KeyWrapAlgorithm algorithm;
			openpal::RSlice data;
		};		
	
		static const uint8_t UPDATE_KEY_SIZE_128 = 16;
		static const uint8_t UPDATE_KEY_SIZE_256 = 32;		

		
		/**
		* Construct an invalid default key
		*/
		UpdateKey();

		/**
		* Test constructor that initializes a key with the same value for every byte
		*/
		UpdateKey(uint8_t repeat, opendnp3::KeyWrapAlgorithm algorithm);

		/**
		* Initialize the key base on a view
		*/
		UpdateKey(const openpal::RSlice& key);

		/**
		* Retrieve update key view
		*/
		View GetView() const;	

		/**
		* Checks the length for validity
		*/
		bool IsValid() const { return m_algorithm != opendnp3::KeyWrapAlgorithm::UNDEFINED; }

		/**
		* Only accepts 128 or 256 bit update keys
		*
		* returns true if the key was of valid size, false otherwise
		*/
		bool Initialize(const openpal::RSlice& key);

	private:

		static uint32_t GetSize(opendnp3::KeyWrapAlgorithm);
		static opendnp3::KeyWrapAlgorithm UpdateKey::GetKeyWrapAlgorithm(uint32_t size);
		
	  
		void Copy(const openpal::RSlice& key);

			
		opendnp3::KeyWrapAlgorithm m_algorithm;
		openpal::SecureStaticBuffer<UPDATE_KEY_SIZE_256> m_buffer;		
};

}

#endif

