
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __LINK_RECEIVER_STATES_H_
#define __LINK_RECEIVER_STATES_H_


#include <string>

#include <opendnp3/Uncopyable.h>
#include <opendnp3/Singleton.h>
#include <opendnp3/Visibility.h>

#include "LinkLayerReceiver.h"

namespace opendnp3
{

/** Base class for the receiver state */
class DLL_LOCAL LRS_Base : private Uncopyable
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
class DLL_LOCAL name : public LRS_Base \
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

#endif
