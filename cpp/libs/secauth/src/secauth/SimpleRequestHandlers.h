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
#ifndef SECAUTH_SIMPLE_REQUEST_HANDLERS_H
#define SECAUTH_SIMPLE_REQUEST_HANDLERS_H

#include "secauth/SingleValueHandler.h"

#include "opendnp3/objects/Group120.h"

namespace secauth
{

class ChallengeHandler : public SingleValueHandler<opendnp3::Group120Var1, opendnp3::GroupVariation::Group120Var1, opendnp3::QualifierCode::UINT16_FREE_FORMAT>
{
public:

	virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var1& object) override final
	{
		this->value = object;
		this->m_valid = true;
		return opendnp3::IINField();
	}
};

class ChallengeReplyHandler : public SingleValueHandler<opendnp3::Group120Var2, opendnp3::GroupVariation::Group120Var2, opendnp3::QualifierCode::UINT16_FREE_FORMAT>
{
	public:		

		virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var2& object) override final
		{
			this->value = object;
			this->m_valid = true;
			return opendnp3::IINField();
		}				
};

class RequestKeyStatusHandler : public SingleValueHandler<opendnp3::Group120Var4, opendnp3::GroupVariation::Group120Var4, opendnp3::QualifierCode::UINT8_CNT>
{
public:

	virtual opendnp3::IINField ProcessHeader(const opendnp3::CountHeader& header, const opendnp3::ICollection<opendnp3::Group120Var4>& objects) override final
	{
		this->m_valid = objects.ReadOnlyValue(this->value);
		return m_valid ? opendnp3::IINField() : opendnp3::IINBit::PARAM_ERROR;
	}
};

class ChangeSessionKeysHandler : public SingleValueHandler<opendnp3::Group120Var6, opendnp3::GroupVariation::Group120Var6, opendnp3::QualifierCode::UINT16_FREE_FORMAT>
{
public:

	virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var6& object) override final
	{
		this->value = object;
		this->m_valid = true;
		return opendnp3::IINField();
	}
};

class UserStatusChangeHandler : public SingleValueHandler<opendnp3::Group120Var10, opendnp3::GroupVariation::Group120Var10, opendnp3::QualifierCode::UINT16_FREE_FORMAT>
{
public:

	virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var10& object) override final
	{
		this->value = object;
		this->m_valid = true;
		return opendnp3::IINField();
	}
};

class ErrorHandler : public SingleValueHandler<opendnp3::Group120Var7, opendnp3::GroupVariation::Group120Var7, opendnp3::QualifierCode::UINT16_FREE_FORMAT>
{
public:

	virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var7& object) override final
	{
		this->value = object;
		this->m_valid = true;
		return opendnp3::IINField();
	}
};


}

#endif

