
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
#ifndef __XML_APL_CONFIGURE_H_
#define __XML_APL_CONFIGURE_H_

#include <XMLBindings/APLXML_Base.h>

namespace apl
{
namespace xml
{

class XML_APL
{
public:

	static APLXML_Base::TCPServer_t* AddTCPServer(APLXML_Base::PhysicalLayerList_t& arList, const std::string& arDevice, const std::string& arEndpoint, int aPort);
	static APLXML_Base::TCPClient_t* AddTCPClient(APLXML_Base::PhysicalLayerList_t& arList, const std::string& arDevice, const std::string& arAdderss, int aPort);
	static APLXML_Base::Serial_t* AddSerial(APLXML_Base::PhysicalLayerList_t& arList, const std::string& arName, const std::string& arDevice);

};

}
}

#endif

