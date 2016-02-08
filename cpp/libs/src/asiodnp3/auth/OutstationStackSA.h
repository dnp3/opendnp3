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
#ifndef ASIODNP3_OUTSTATIONSTACKSA_H
#define ASIODNP3_OUTSTATIONSTACKSA_H

#include <secauth/outstation/OutstationAuthStackConfig.h>
#include <secauth/outstation/IOutstationApplicationSA.h>
#include <secauth/outstation/OutstationAuthContext.h>

#include <openpal/crypto/ICryptoProvider.h>

#include "asiodnp3/auth/IOutstationSA.h"
#include "asiodnp3/OutstationStackBase.h"

namespace asiodnp3
{

/** @section desc A stack object for an SA-enabled outstation */
class OutstationStackSA : public OutstationStackBase<IOutstationSA>
{
public:

	/// standard DNP3 constructor
	OutstationStackSA(	    
	    openpal::LogRoot root,
	    openpal::IExecutor& executor,
	    opendnp3::ICommandHandler& commandHandler,
	    secauth::IOutstationApplicationSA& application,
	    const secauth::OutstationAuthStackConfig& config,
	    IStackLifecycle& lifecycle,
	    openpal::ICryptoProvider& crypto);


	// ------- implement IOutstationSA -------

	virtual void ConfigureUser(const secauth::OutstationUserInfo& info) override final;

	virtual void ConfigureAuthority(uint32_t statusChangeSeqNumber, const secauth::AuthorityKey& key) override final;

private:

	secauth::OAuthContext ocontext;
};

}

#endif

