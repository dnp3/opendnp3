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
#ifndef __PHYSICAL_LAYER_STATES_H_
#define __PHYSICAL_LAYER_STATES_H_

#include <string>

#include <sstream>

namespace apl
{

enum PhysicalLayerState {
	PLS_CLOSED,		// layer is offline and idle
	PLS_OPENING,	// layer is trying to open
	PLS_WAITING,	// layer is waiting to open
	PLS_OPEN,		// layer is open
	PLS_SHUTDOWN	// stopped and will never do anything again
};

std::string ConvertPhysicalLayerStateToString(PhysicalLayerState aState);

std::ostream& operator<<(std::ostream& output, PhysicalLayerState aState);

}

#endif
