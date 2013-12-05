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
#ifndef __TLS_BASE_H_
#define __TLS_BASE_H_

#include <opendnp3/Uncopyable.h>
#include <openpal/Visibility.h>
#include <openpal/BufferWrapper.h>

#include <string>
#include <cstdint>

namespace opendnp3
{

class TransportLayer;


/**
Base class for all TransportLayerStates (TLS)
*/
class DLL_LOCAL TLS_Base
{
public:
	virtual void Send(const openpal::ReadOnlyBuffer& arBuffer, TransportLayer*);
	virtual void HandleReceive(const openpal::ReadOnlyBuffer& arBuffer, TransportLayer*);

	// TPDU failure/success handlers
	virtual void HandleSendSuccess(TransportLayer*);
	virtual void HandleSendFailure(TransportLayer*);

	virtual void LowerLayerUp(TransportLayer*);
	virtual void LowerLayerDown(TransportLayer*);
#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	virtual std::string Name() const = 0;
#endif
};

}

#endif

