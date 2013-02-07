
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

#include "SerialTypes.h"

namespace opendnp3
{


ParityType GetParityFromInt(int parity)
{
	switch(parity)
	{
		case (1): return PAR_EVEN;
		case (2): return PAR_ODD;
		default: return PAR_NONE;
	}
}

FlowType GetFlowTypeFromInt(int flowControl)
{
	switch(flowControl)
	{
		case (1): return FLOW_HARDWARE;
		case (2): return FLOW_XONXOFF;
		default: return FLOW_NONE;
	}	
}

}



