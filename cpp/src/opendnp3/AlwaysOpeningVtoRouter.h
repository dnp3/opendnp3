
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
// you under the terms of the License.
//

#ifndef __ALWAYS_OPENING_VTO_ROUTER_H_
#define __ALWAYS_OPENING_VTO_ROUTER_H_

#include "VtoRouter.h"

#include <openpal/Visibility.h>

namespace opendnp3
{

/**
 * the simplest type of vto router, useful for vto ports that should always be
 * online regardless of dnp connection state. The vtorouter on the other side
 * of the dnp connection can be either an AlwaysOpening or a ServerSocket router.
 */
class DLL_LOCAL AlwaysOpeningVtoRouter : public VtoRouter
{
public:

	AlwaysOpeningVtoRouter(const VtoRouterSettings& arSettings, openpal::Logger& arLogger, IVtoWriter* apWriter, IPhysicalLayerAsync* apPhysLayer);

	/// we don't care about any of the callbacks so we have empty implementations
	void DoVtoRemoteConnectedChanged(bool aOpened) {}
	void SetLocalConnected(bool aConnected) {}

	// doesn't care what type of data under any condition
	bool CheckIncomingVtoData(const VtoData& arData) {
		return true;
	}

};

}

/* vim: set ts=4 sw=4: */

#endif

