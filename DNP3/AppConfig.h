
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
#ifndef __APP_CONFIG_H_
#define __APP_CONFIG_H_


#include "DNPConstants.h"

namespace apl
{
namespace dnp
{

/** Configuration structure for the dnp3 application layer
*/
struct AppConfig {
	// Default constructor
	AppConfig() : RspTimeout(5000), NumRetry(0), FragSize(DEFAULT_FRAG_SIZE) {}

	AppConfig(millis_t aRspTimeout, size_t aNumRetry = 0, size_t aFragSize = DEFAULT_FRAG_SIZE) :
		RspTimeout(aRspTimeout),
		NumRetry(aNumRetry),
		FragSize(aFragSize)
	{}

	// The response/confirm timeout in millisec
	millis_t RspTimeout;

	// Number of retries performed for applicable frames
	size_t NumRetry;

	// The maximum size of received application layer fragments
	size_t FragSize;

};

}
}

#endif
