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
#ifndef OPENDNP3_AUTHREQUESTADAPTER_H
#define OPENDNP3_AUTHREQUESTADAPTER_H

#include "opendnp3/outstation/authv5/IAuthRequestParserHandler.h"
#include "opendnp3/outstation/authv5/IAuthRequestHandler.h"

#include "opendnp3/outstation/OutstationState.h"

namespace opendnp3
{

class AuthRequestAdapter : public IAuthRequestParserHandler, private openpal::Uncopyable
{
	public:

		AuthRequestAdapter(const APDUHeader& header, OState& ostate, IAuthRequestHandler& handler);

		virtual void OnAuthChallenge(const Group120Var1& challenge) override final;
		virtual void OnAuthReply(const Group120Var2& reply) override final;			
		virtual void OnRequestKeyStatus(const Group120Var4& status) override final;
		virtual void OnChangeSessionKeys(const Group120Var6& keyChange) override final;

	private:
		APDUHeader header;
		OState* pOState;
		IAuthRequestHandler* pHandler;
};


}

#endif

