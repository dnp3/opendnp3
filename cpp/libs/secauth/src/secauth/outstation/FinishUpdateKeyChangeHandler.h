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
#ifndef SECAUTH_FINISH_UPDATE_KEY_CHANGE_HANDLER_H
#define SECAUTH_FINISH_UPDATE_KEY_CHANGE_HANDLER_H

#include <opendnp3/app/parsing/IAPDUHandler.h>

#include <opendnp3/objects/Group120.h>

namespace secauth
{

class FinishUpdateKeyChangeHandler final : public opendnp3::IAPDUHandler, private openpal::Uncopyable
{
	public:

		enum Result : uint8_t
		{
			NONE,					// if the required objects were not present
			HMAC,					// if the 2nd object was a g120v15
			DIGITAL_SIGNATURE		// if the 2nd object was a g120v14
		};

		FinishUpdateKeyChangeHandler() : m_result(Result::NONE)
		{}

		Result GetResult() const
		{
			return m_result;
		}
		
		virtual bool IsAllowed(uint32_t headerCount, opendnp3::GroupVariation gv, opendnp3::QualifierCode qc) override;

		virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var13& value, const openpal::RSlice& object) override;
		virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var14& value, const openpal::RSlice& object) override;
		virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var15& value, const openpal::RSlice& object) override;

		// the actual data that will be set correct if the result != NONE
		opendnp3::Group120Var13 keyChange;
		openpal::RSlice authData;

	private:
		
		Result m_result;
			
};


}

#endif

