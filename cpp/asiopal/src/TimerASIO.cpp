
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

#include "TimerASIO.h"

using namespace openpal;

namespace asiopal
{

TimerASIO::TimerASIO(boost::asio::strand* apStrand) :
	mCanceled(false),
	mTimer(apStrand->get_io_service())
{

}

/**
 * Return the timer's expiry time as an absolute time.
 */
openpal::MonotonicTimestamp TimerASIO::ExpiresAt()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(mTimer.expires_at().time_since_epoch()).count();
}

void TimerASIO::Cancel()
{
	assert(!mCanceled);
	mTimer.cancel();
	mCanceled = true;
}


}
