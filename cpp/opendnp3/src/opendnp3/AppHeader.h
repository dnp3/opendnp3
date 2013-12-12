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
#ifndef __APP_HEADER_H_
#define __APP_HEADER_H_

#include <opendnp3/Singleton.h>
#include <opendnp3/APDUConstants.h>

#include "AppControlField.h"
#include "IINField.h"
#include "gen/FunctionCode.h"

#include <cstdint>
#include <stddef.h>

namespace opendnp3
{

enum AppHeaderTypes {
	AHT_RESPONSE,
	AHT_REQUEST
};

class IAppHeader
{
public:
	virtual ~IAppHeader() {}
	virtual AppHeaderTypes GetType() const = 0;
	virtual size_t GetSize() const = 0;	
};

class RequestHeader : public IAppHeader
{
	MACRO_SINGLETON_INSTANCE(RequestHeader)

	AppHeaderTypes GetType() const {
		return AHT_REQUEST;
	}
	size_t GetSize() const {
		return 2;
	}
};

class ResponseHeader : public IAppHeader
{
	MACRO_SINGLETON_INSTANCE(ResponseHeader)

	AppHeaderTypes GetType() const {
		return AHT_RESPONSE;
	}
	size_t GetSize() const {
		return 4;
	}

	void SetIIN(uint8_t* apStart, const IINField& arIIN) const;
	IINField GetIIN(const uint8_t* apStart) const;
};

}

#endif
