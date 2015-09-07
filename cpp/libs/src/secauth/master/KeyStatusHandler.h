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
#ifndef SECAUTH_KEYSTATUSHANDLER_H
#define SECAUTH_KEYSTATUSHANDLER_H

#include <opendnp3/app/parsing/IAPDUHandler.h>

#include <opendnp3/LogLevels.h>

#include <openpal/logging/LogMacros.h>

namespace secauth
{

/**
 * Handles key-status responses
 */
class KeyStatusHandler : public opendnp3::IAPDUHandler
{

public:

	explicit KeyStatusHandler(openpal::Logger logger);

	virtual bool IsAllowed(uint32_t headerCount, opendnp3::GroupVariation gv, opendnp3::QualifierCode) override final
	{
		return (headerCount == 0) && (gv == opendnp3::GroupVariation::Group120Var5);
	}

	bool GetStatus(opendnp3::Group120Var5& status, openpal::RSlice& rawObject) const;

private:

	openpal::Logger logger;
	bool valid;
	opendnp3::Group120Var5 status;
	openpal::RSlice rawObject;

	virtual opendnp3::IINField ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var5& status, const openpal::RSlice& rawObject) override final;

};

}



#endif

