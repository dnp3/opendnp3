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
#ifndef SECAUTH_OUTSTATIONAUTHCONTEXT_H
#define SECAUTH_OUTSTATIONAUTHCONTEXT_H

#include <opendnp3/outstation/OutstationContext.h>

#include "secauth/AuthorityKey.h"
#include "secauth/outstation/OutstationSecurity.h"
#include "secauth/outstation/OutstationAuthSettings.h"
#include "secauth/outstation/IOutstationApplicationSA.h"
#include "secauth/outstation/OutstationUserInfo.h"

#include <openpal/util/Uncopyable.h>
#include <openpal/executor/IUTCTimeSource.h>

namespace secauth
{

/**
	SAv5 outstation authentication provider
*/
class OAuthContext final : public opendnp3::OContext
{	
	public:

	OAuthContext(
		const opendnp3::OutstationConfig& config,
		const opendnp3::DatabaseTemplate& dbTemplate,
		openpal::Logger logger,
		openpal::IExecutor& executor,
		opendnp3::ILowerLayer& lower,
		opendnp3::ICommandHandler& commandHandler,
		IOutstationApplicationSA& application,
		const OutstationAuthSettings& settings,			
		openpal::ICryptoProvider& crypto
	);

	//// --- external methods ----

	void AddUser(const OutstationUserInfo& userInfo);

	void ConfigureAuthority(uint32_t statusChangeSeqNumber, const AuthorityKey& key);

	//// --- methods overriden in base class -----

	virtual bool OnLowerLayerDown() override final;

	virtual void CheckForTaskStart() override final;
		
	virtual void ReceiveParsedHeader(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects) override final;

	virtual void Increment(opendnp3::SecurityStatIndex index) override final;

	//// ---- public state for testing purposes only ----

	OutstationSecurity security;

	private:		

	enum APDUResult
	{
		PROCESSED,
		DISCARDED
	};


	APDUResult ProcessAuthAPDU(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects);

	APDUResult ProcessAuthRequest(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects);

	APDUResult ProcessChallengeReply(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects);

	APDUResult ProcessNormalFunction(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects);

	APDUResult ProcessRequestKeyStatus(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects);

	APDUResult ProcessChangeSessionKeys(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects);

	APDUResult ProcessBeginUpdateKeyChange(const openpal::RSlice& fragment, const opendnp3::APDUHeader& header, const openpal::RSlice& objects);

	APDUResult ProcessFinishUpdateKeyChange(const openpal::RSlice& fragment, const opendnp3::APDUHeader& header, const openpal::RSlice& objects);

	APDUResult ProcessUserStatusChange(const openpal::RSlice& fragment, const opendnp3::APDUHeader& header, const openpal::RSlice& objects);

	APDUResult ProcessUserStatusChange_Add(const opendnp3::APDUHeader& header, const std::string& username, const opendnp3::Group120Var10& change);

	APDUResult ProcessUserStatusChange_Change(const opendnp3::APDUHeader& header, const std::string& username, const opendnp3::Group120Var10& change);

	APDUResult ProcessUserStatusChange_Delete(const opendnp3::APDUHeader& header, const std::string& username, const opendnp3::Group120Var10& change);

	APDUResult QueueUserStatusChange(const opendnp3::APDUHeader& header, const std::string& username, const opendnp3::Group120Var10& change);

	APDUResult TryRespondWithAuthError(opendnp3::AppSeqNum seq, uint32_t authSeqNum, const opendnp3::User& user, opendnp3::AuthErrorCode code);
	
	void OnChallengeTimeout();	

	/// --- Helper methods ----
			
	void IncrementSessionAuthCount(const opendnp3::User& user);

	void ConfigureSecStats(const StatThresholds& thresholds);

	opendnp3::APDUResponse StartAuthResponse(uint8_t seq);		

	bool AuthenticateUserStatusChange(const opendnp3::APDUHeader& header, const opendnp3::Group120Var10& change);	

	bool TransmitChallenge(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header);

	static openpal::IKeyWrapAlgo& GetKeyWrapAlgo(openpal::ICryptoProvider& crypto, opendnp3::UpdateKeyMode type);	

	static opendnp3::DatabaseTemplate EnableSecStats(const opendnp3::DatabaseTemplate& dbTemplate);


};

}

#endif

