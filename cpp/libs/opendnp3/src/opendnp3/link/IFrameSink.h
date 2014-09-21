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
#ifndef OPENDNP3_IFRAMESINK_H
#define OPENDNP3_IFRAMESINK_H

#include <openpal/container/ReadOnlyBuffer.h>

#include <stddef.h>
#include <cstdint>

namespace opendnp3
{

// @section DESCRIPTION Interface that receives parsed FT3 frame information
class IFrameSink
{
public:

	virtual ~IFrameSink() {}

	//	Sec to Pri

	virtual void Ack(bool isMaster, bool isRcvBuffFull, uint16_t dest, uint16_t source) = 0;
	virtual void Nack(bool isMaster, bool isRcvBuffFull, uint16_t dest, uint16_t source) = 0;
	virtual void LinkStatus(bool isMaster, bool isRcvBuffFull, uint16_t dest, uint16_t source) = 0;
	virtual void NotSupported(bool isMaster, bool isRcvBuffFull, uint16_t dest, uint16_t source) = 0;


	//	Pri to Sec

	virtual void TestLinkStatus(bool isMaster, bool aFcb, uint16_t dest, uint16_t source) = 0;
	virtual void ResetLinkStates(bool isMaster, uint16_t dest, uint16_t source) = 0;
	virtual void RequestLinkStatus(bool isMaster, uint16_t dest, uint16_t source) = 0;
	virtual void ConfirmedUserData(bool isMaster, bool aFcb, uint16_t dest, uint16_t source, const openpal::ReadOnlyBuffer& buffer) = 0;
	virtual void UnconfirmedUserData(bool isMaster, uint16_t dest, uint16_t source, const openpal::ReadOnlyBuffer& buffer) = 0;
};

}

#endif

