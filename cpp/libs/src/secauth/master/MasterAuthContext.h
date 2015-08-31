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
#ifndef SECAUTH_MASTERAUTHCONTEXT_H
#define SECAUTH_MASTERAUTHCONTEXT_H

#include <opendnp3/master/MasterContext.h>

#include "secauth/master/MasterSecurity.h"
#include "secauth/master/UserStatusChange.h"
#include "secauth/master/BeginUpdateKeyChangeCallbackT.h"
#include "secauth/master/FinishUpdateKeyChangeArgs.h"

namespace secauth
{

class MAuthContext final : public opendnp3::MContext
{

public:

	MAuthContext(
	    openpal::IExecutor& executor,
	    openpal::LogRoot& root,
	    opendnp3::ILowerLayer& lower,
	    opendnp3::ISOEHandler& SOEHandler,
	    IMasterApplicationSA& application,
	    const opendnp3::MasterParams& params,
	    opendnp3::ITaskLock& taskLock,
	    const MasterAuthSettings& authSettings,
	    openpal::ICryptoProvider& crypto
	);


	// ------ override the necessary MContext methods ------

	virtual bool OnLowerLayerUp() override;

	virtual bool OnLowerLayerDown() override;

	virtual void OnParsedHeader(const openpal::RSlice& apdu, const opendnp3::APDUResponseHeader& header, const openpal::RSlice& objects) override final;

	virtual bool CanRun(const opendnp3::IMasterTask& task) override final;

	virtual bool MeetsUserRequirements(const opendnp3::IMasterTask& task) override final;

	virtual void RecordLastRequest(const openpal::RSlice& apdu) override final;

	// --- public helpers ----

	bool AddUser(opendnp3::User user, const UpdateKey& key);

	void ChangeUserStatus(const UserStatusChange& userStatusChange, const opendnp3::TaskConfig& config);

	void BeginUpdateKeyChange(const std::string& userName, const opendnp3::TaskConfig& config, const BeginUpdateKeyChangeCallbackT& callback);

	void FinishUpdateKeyChange(const FinishUpdateKeyChangeArgs& args, const opendnp3::TaskConfig& config);

private:

	void OnReceiveAuthResponse(const openpal::RSlice& apdu, const opendnp3::APDUResponseHeader& header, const openpal::RSlice& objects);

	void OnAuthChallenge(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects);

	void OnAuthError(const openpal::RSlice& apdu, const opendnp3::APDUHeader& header, const openpal::RSlice& objects);

	void InitSessionKeyTaskForUser(const opendnp3::User& user);

	MasterSecurity security;

};


}

#endif
