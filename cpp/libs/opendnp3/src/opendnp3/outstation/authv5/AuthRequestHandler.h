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
#ifndef OPENDNP3_AUTHREQUESTHANDLER_H
#define OPENDNP3_AUTHREQUESTHANDLER_H

#include "opendnp3/app/parsing/APDUHandlerBase.h"
#include "opendnp3/outstation/authv5/IAuthRequestHandler.h"

#include "opendnp3/outstation/OutstationState.h"

namespace opendnp3
{

class AuthRequestHandler : public APDUHandlerBase, private openpal::Uncopyable
{
	public:

		AuthRequestHandler(const APDUHeader& header, OState& ostate, IAuthRequestHandler& handler);

		virtual IINField ProcessCountOf(const HeaderRecord& record, const IterableBuffer<Group120Var4>& values) override final;
		virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var1& value) override final;
		virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var2& value) override final;
		virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var6& value) override final;

		static bool WhiteList(uint32_t count, GroupVariation gv, QualifierCode qc);

	private:
		APDUHeader header;
		OState* pOState;
		IAuthRequestHandler* pHandler;
};


}

#endif

