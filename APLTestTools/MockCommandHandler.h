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
#ifndef __MOCK_COMMAND_HANDLER_H_
#define __MOCK_COMMAND_HANDLER_H_

#include <APL/CommandInterfaces.h>

namespace apl
{

class MockCommandHandler : public ICommandHandler
{
public:

	MockCommandHandler() : num_bo(0), num_sp(0) {}

	CommandStatus HandleControl(BinaryOutput& aControl, size_t aIndex) {
		++num_bo;
		return CS_SUCCESS;
	}
	CommandStatus HandleControl(Setpoint& aControl, size_t aIndex) {
		++num_sp;
		return CS_SUCCESS;
	}

	size_t num_bo;
	size_t num_sp;
};

}

#endif
