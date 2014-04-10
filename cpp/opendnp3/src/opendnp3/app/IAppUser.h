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
#ifndef __I_APP_USER_H_
#define __I_APP_USER_H_

#include "APDUHeader.h"
#include "SequenceInfo.h"

#include <openpal/LogRoot.h>

namespace opendnp3
{

// Interface for callbacks from an application layer
class IAppUser
{

public:
	IAppUser(openpal::LogRoot& root);

	virtual void OnLowerLayerUp() = 0;					// The app layer is online
	virtual void OnLowerLayerDown() = 0;				// The app layer is offline

	virtual void OnUnsolSendSuccess() = 0;					// A Send operation has completed
	virtual void OnUnsolFailure() = 0;						// A transaction has failed for some reason

	virtual void OnSolSendSuccess() = 0;					// A Send operation has completed
	virtual void OnSolFailure() = 0;						// A transaction has failed for some reason

	// A non-final response has been received
	virtual void OnPartialResponse(const APDUResponseRecord&);

	// A final response has been received
	virtual void OnFinalResponse(const APDUResponseRecord&);

	// Process unsolicited data
	virtual void OnUnsolResponse(const APDUResponseRecord&);

	// Process request fragment
	virtual void OnRequest(const APDURecord&, SequenceInfo);

protected:

	openpal::Logger logger;

};

} //end ns

#endif
