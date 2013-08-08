
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
#ifndef __LINK_CONFIG_H_
#define __LINK_CONFIG_H_

#include <openpal/Types.h>

namespace opendnp3
{

/**
	Configuration for the dnp3 link layer
*/
struct LinkConfig {
	LinkConfig(
	        bool aIsMaster,
	        bool aUseConfirms,
	        size_t aNumRetry,
	        uint16_t aLocalAddr,
	        uint16_t aRemoteAddr,
	       openpal::millis_t aTimeout) :

		IsMaster(aIsMaster),
		UseConfirms(aUseConfirms),
		NumRetry(aNumRetry),
		LocalAddr(aLocalAddr),
		RemoteAddr(aRemoteAddr),
		Timeout(aTimeout)
	{}

	LinkConfig(
	        bool aIsMaster,
	        bool aUseConfirms) :

		IsMaster(aIsMaster),
		UseConfirms(aUseConfirms),
		NumRetry(0),
		LocalAddr(aIsMaster ? 1 : 1024),
		RemoteAddr(aIsMaster ? 1024 : 1),
		Timeout(1000)
	{}

	/// The master/slave bit set on all messages
	bool IsMaster;

	/// If true, the link layer will send data requesting confirmation
	bool UseConfirms;

	/// The number of retry attempts the link will attempt after the initial try
	size_t NumRetry;

	/// dnp3 address of the local device
	uint16_t LocalAddr;

	/// dnp3 address of the remote device
	uint16_t RemoteAddr;

	/// the response timeout in milliseconds for confirmed requests
	openpal::millis_t Timeout;

private:

	LinkConfig() {}
};

}

#endif

