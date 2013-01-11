
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
#ifndef __I_TIMER_H_
#define __I_TIMER_H_

#include <chrono>

namespace apl
{

/**
 * This is a wrapper for ASIO timers that are used to post events
 * on a queue. Events can be posted for immediate consumption or
 * some time in the future. Events can be consumbed by the posting
 * thread or another thread.
 *
 * @section Class Goals
 *
 * Decouple APL code form ASIO so ASIO could be replace if need be.
 *
 * There is a problem with ASIO. When cancel is called, an event is
 * posted. We wanted a cancel that does not generate any events.
 *
 * @see TimerASIO
 */
class ITimer
{
public:
	virtual ~ITimer() {}
	virtual void Cancel() = 0;
	virtual std::chrono::high_resolution_clock::time_point ExpiresAt() = 0;
};

}

#endif
