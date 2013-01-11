
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
#include "TimeBase.h"


namespace apl
{
TimeBase::TimeBase()
{

}

void TimeBase::SetTo(millis_t aTimeMS)
{
	mSec = static_cast<size_t>(aTimeMS / 1000);
	mUsec = static_cast<size_t>((aTimeMS % 1000) * 1000);
}

void TimeBase::AddMS(millis_t aAddMS)
{
	millis_t time = this->GetValueMS();

	time += aAddMS;

	this->SetTo(time);
}

int64_t TimeBase::GetValueMS() const
{
	return static_cast<int64_t>(this->mSec) * 1000 + this->mUsec / 1000;
}

millis_t TimeBase::CalcDeltaMS(const TimeBase& aTime1, const TimeBase& aTime2)
{
	millis_t seconds1 = aTime1.GetSecSinceEpoch();
	millis_t seconds2 = aTime2.GetSecSinceEpoch();
	millis_t uSeconds1 = aTime1.GetUsecSinceEpoch();
	millis_t uSeconds2 = aTime2.GetUsecSinceEpoch();
	millis_t totalSeconds = seconds1 - seconds2;
	millis_t totalUSecs = uSeconds1 - uSeconds2;

	//on g++ this ended up with odd values
	//millis_t totalSeconds = aTime1.GetSecSinceEpoch() - aTime2.GetSecSinceEpoch();
	//millis_t totalUSecs = aTime1.GetUsecSinceEpoch() - aTime2.GetUsecSinceEpoch();
	return totalSeconds * 1000 + totalUSecs / 1000;
}

}
