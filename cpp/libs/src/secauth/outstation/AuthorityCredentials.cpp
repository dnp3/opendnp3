/*
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
#include "secauth/outstation/AuthorityCredentials.h"

namespace secauth
{

AuthorityCredentials::AuthorityCredentials() : m_statusChangeSeqNum(0)
{}

void AuthorityCredentials::SetSCSN(uint32_t statusChangeSeqNumber)
{
	this->m_statusChangeSeqNum = statusChangeSeqNumber;
}

void AuthorityCredentials::Configure(uint32_t statusChangeSeqNumber, const AuthorityKey& key)
{
	this->m_statusChangeSeqNum = statusChangeSeqNumber;
	this->m_authorityKey = key;
}

openpal::RSlice AuthorityCredentials::GetSymmetricKey() const
{
	return m_authorityKey.IsValid() ? m_authorityKey.GetKeyView() : openpal::RSlice::Empty();
}

bool AuthorityCredentials::GetSymmetricKey(uint32_t& statusChangeSeqNumber, openpal::RSlice& keyView) const
{
	if (!m_authorityKey.IsValid())
	{
		return false;
	}

	statusChangeSeqNumber = m_statusChangeSeqNum;
	keyView = m_authorityKey.GetKeyView();
	return true;
}

}



