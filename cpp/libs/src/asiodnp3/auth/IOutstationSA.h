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
#ifndef ASIODNP3_IOUTSTATIONSA_H
#define ASIODNP3_IOUTSTATIONSA_H

#include "asiodnp3/IOutstation.h"

#include <secauth/AuthorityKey.h>
#include <secauth/outstation/OutstationUserInfo.h>


namespace asiodnp3
{

/**
Extend the normal IOutstation interface w/ SA features
*/
class IOutstationSA : public IOutstation
{
	public:	
	
	/**
	* Add a user to the outstation. This is normally only done during initialization.
	*/
	virtual void ConfigureUser(const secauth::OutstationUserInfo& info) = 0;

	/**
	* Set persisted value of the status change seq number, and the authority symmetric key used to validate UserStatusChange requests.
	*/
	virtual void ConfigureAuthority(uint32_t statusChangeSeqNumber, const secauth::AuthorityKey& key) = 0;

};

}

#endif
