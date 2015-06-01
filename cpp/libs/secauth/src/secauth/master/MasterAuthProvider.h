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
#ifndef SECAUTH_MASTERAUTHPROVIDER_H
#define SECAUTH_MASTERAUTHPROVIDER_H

#include <opendnp3/master/IMasterAuthProvider.h>

#include <openpal/util/Uncopyable.h>

#include "MasterSecurityState.h"
#include "SessionKeyTask.h"
#include "IAuthResponseReceiver.h"

namespace secauth
{

class MasterAuthProvider final : public opendnp3::IMasterAuthProvider, private IAuthResponseReceiver, private openpal::Uncopyable
{	

public:	

	MasterAuthProvider(
		opendnp3::IMasterApplication& application,
		openpal::IExecutor& executor,
		openpal::Logger logger,		
		openpal::ICryptoProvider& crypto,
		IMasterUserDatabase& userDB
	);


	// ------ Implement IMasterAuthProvider ------		

	virtual void GoOnline() override final;	

	virtual void GoOffline() override final;

	virtual void OnReceive(const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects) override final;

	virtual void RecordLastRequest(const openpal::ReadBufferView& apdu) override final;

private:

	void OnReceiveAuthResponse(const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects);

	// ------ Implement IAuthResponseReceiver ------	

	virtual void OnAuthChallenge(const opendnp3::APDUHeader& header, const opendnp3::Group120Var1& challenge) override final;

	virtual void OnAuthError(const opendnp3::APDUHeader& header, const opendnp3::Group120Var7& error) override final;

	
	MSState msstate;
	SessionKeyTask sessionKeyTask;
	openpal::ReadBufferView lastRequest;

};


}

#endif
