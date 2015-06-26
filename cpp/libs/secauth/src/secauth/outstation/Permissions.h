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
#ifndef SECAUTH_PERMISSIONS_H
#define SECAUTH_PERMISSIONS_H

#include <opendnp3/gen/FunctionCode.h>

namespace secauth
{

/**
*	Describes what function codes a user is authorized to perform
*/
class Permissions
{	
	class Mask
	{
	public:
		static Mask None() { return Mask(false, 0); }
		static Mask Bit(uint8_t bit) {  return Mask(true, static_cast<uint64_t>(1) << bit); }		
			
		bool valid;
		uint64_t value;
		
		inline uint64_t And(uint64_t other) const 
		{ 
		  return valid ? (other & value) : 0;
		}
		

	private:
		Mask(bool valid, uint64_t value) : valid(valid), value(value) {}
		Mask() = delete;
	};


public:

	Permissions() : permissions(0) {}
	
	template <typename... Args>
	static Permissions Allowed(Args... args)
	{
		return Permissions(GetBitfield(args...));
	}
	
	Permissions operator|(const Permissions& other)
	{
	  return Permissions(this->permissions | other.permissions); 
	}

	static Permissions AllowNothing();
	static Permissions AllowAll();
	
	void Allow(opendnp3::FunctionCode code);

	bool IsAllowed(opendnp3::FunctionCode) const;
	
private:
  
	template <typename... Args>
	static uint64_t GetBitfield(opendnp3::FunctionCode fc, Args... args)
	{
		return GetMask(fc).value | GetBitfield(args...);
	}

	// base case for variadic method
	static uint64_t GetBitfield() { return 0; }
	
	
	static Mask GetMask(opendnp3::FunctionCode code);

	uint64_t permissions;
		
	Permissions(uint64_t mask);

};

}

#endif

