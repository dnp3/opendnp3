
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
#ifndef __OUTSTATION_RESPONSES_H_
#define __OUTSTATION_RESPONSES_H_

namespace opendnp3
{
enum StaticBinaryResponse {
	SBR_GROUP1_VAR2 = 0
};

enum StaticAnalogResponse {
	SAR_GROUP30_VAR1 = 0,
	SAR_GROUP30_VAR2 = 1,
	SAR_GROUP30_VAR3 = 2,
	SAR_GROUP30_VAR4 = 3,
	SAR_GROUP30_VAR5 = 4,
	SAR_GROUP30_VAR6 = 5
};

enum StaticCounterResponse {
	SCR_GROUP20_VAR1 = 0,
	SCR_GROUP20_VAR2 = 1,
	SCR_GROUP20_VAR5 = 2,
	SCR_GROUP20_VAR6 = 3
};

enum StaticSetpointStatusResponse {
	SSSR_GROUP40_VAR1 = 0,
	SSSR_GROUP40_VAR2 = 1,
	SSSR_GROUP40_VAR3 = 2,
	SSSR_GROUP40_VAR4 = 3
};

enum EventBinaryResponse {
	EBR_GROUP2_VAR1 = 0,
	EBR_GROUP2_VAR2 = 1
};

enum EventAnalogResponse {
	EAR_GROUP32_VAR1 = 0,
	EAR_GROUP32_VAR2 = 1,
	EAR_GROUP32_VAR3 = 2,
	EAR_GROUP32_VAR4 = 3,
	EAR_GROUP32_VAR5 = 4,
	EAR_GROUP32_VAR6 = 5,
	EAR_GROUP32_VAR7 = 6,
	EAR_GROUP32_VAR8 = 7
};

enum EventCounterResponse {
	ECR_GROUP22_VAR1 = 0,
	ECR_GROUP22_VAR2 = 1,
	ECR_GROUP22_VAR5 = 2,
	ECR_GROUP22_VAR6 = 3
};
}

#endif

