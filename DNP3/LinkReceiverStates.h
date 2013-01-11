//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __LINK_RECEIVER_STATES_H_
#define __LINK_RECEIVER_STATES_H_


#include <string>
#include <APL/Uncopyable.h>
#include <APL/Singleton.h>
#include "LinkLayerReceiver.h"

namespace apl
{
namespace dnp
{

/** Base class for the receiver state */
class LRS_Base : private Uncopyable
{
public:

	virtual ~LRS_Base() {}

	/** Perform a bit of work towards reading a frame.
		@return True if the function can be called again to make further progress */
	virtual bool Parse(LinkLayerReceiver*);

	/** @return True if the receiver has a complete frame */
	virtual bool HasFrame() {
		return false;
	}

	/** Implemented by the inherited state
		@return Name of the state */
	virtual std::string Name() const = 0;
};

#define MACRO_LRS_STATE(name, body) \
class name : public LRS_Base \
{ \
	MACRO_STATE_SINGLETON_INSTANCE(name) \
	body \
};

/** @section DESCRIPTION Defines actions for the initial state of the receiver */
MACRO_LRS_STATE(LRS_Sync,
                bool Parse(LinkLayerReceiver*);
               )

MACRO_LRS_STATE(LRS_Header,
                bool Parse(LinkLayerReceiver*);
               )

MACRO_LRS_STATE(LRS_Body,
                bool Parse(LinkLayerReceiver*);
               )




}
}

#endif
