
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
#include "StartupTasks.h"

#include "APDU.h"
#include "ObjectReadIterator.h"
#include "ITimeSource.h"
#include "LoggableMacros.h"

#include <opendnp3/PointClass.h>
#include <opendnp3/Logger.h>


namespace opendnp3
{

/* ------ Clear Restart ------- */

ClearRestartIIN::ClearRestartIIN(Logger* apLogger) :
	SimpleRspBase(apLogger)
{}

void ClearRestartIIN::ConfigureRequest(APDU& arAPDU)
{
	arAPDU.Set(FC_WRITE);
	Group80Var1* pObj = Group80Var1::Inst(); // Internal indications object
	ObjectWriteIterator i = arAPDU.WriteContiguous(pObj, 7, 7); // index 7 == device restart
	pObj->Write(*i, 7, 7, false);
}

/* ------ Configure Unsol ------- */

ConfigureUnsol::ConfigureUnsol(Logger* apLogger) :
	SimpleRspBase(apLogger),
	mIsEnable(false),
	mClassMask(0)
{}

void ConfigureUnsol::Set(bool aIsEnable, int aClassMask)
{
	mIsEnable = aIsEnable;
	mClassMask = aClassMask;
}

void ConfigureUnsol::ConfigureRequest(APDU& arAPDU)
{
	arAPDU.Set(mIsEnable ? FC_ENABLE_UNSOLICITED : FC_DISABLE_UNSOLICITED);
	if(mClassMask & PC_CLASS_1) arAPDU.DoPlaceholderWrite(Group60Var2::Inst());
	if(mClassMask & PC_CLASS_2) arAPDU.DoPlaceholderWrite(Group60Var3::Inst());
	if(mClassMask & PC_CLASS_3) arAPDU.DoPlaceholderWrite(Group60Var4::Inst());
}


/* ------ Time Sync ------- */

TimeSync::TimeSync(Logger* apLogger, ITimeSource* apTimeSrc) :
	SingleRspBase(apLogger),
	mpTimeSrc(apTimeSrc),
	mDelay(-1)
{}

void TimeSync::Init()
{
	mDelay = -1;
}

void TimeSync::ConfigureRequest(APDU& arAPDU)
{
	if(mDelay < 0) {
		arAPDU.Set(FC_DELAY_MEASURE);
		mStart = mpTimeSrc->GetUTC();
	}
	else {
		arAPDU.Set(FC_WRITE);
		ObjectWriteIterator owi = arAPDU.WriteContiguous(Group50Var1::Inst(), 0, 0, QC_1B_CNT);
		Group50Var1::Inst()->mTime.Set(*owi, mpTimeSrc->GetMillisecondsSinceEpoch() + mDelay);
	}
}

TaskResult TimeSync::_OnFinalResponse(const APDU& arAPDU)
{
	if(mDelay < 0) {
		timer_clock::time_point now = mpTimeSrc->GetUTC();

		HeaderReadIterator hri = arAPDU.BeginRead();
		if(hri.Count() != 1) {
			LOG_BLOCK(LEV_WARNING, "DelayMeas response w/ unexcpected header count");
			return TR_FAIL;
		}

		if(!hri->GetBaseObject()->Equals(Group52Var2::Inst())) {
			LOG_BLOCK(LEV_WARNING, "DelayMeas response w/ unexpected object: " << hri->GetBaseObject()->Name());
			return TR_FAIL;
		}

		ObjectReadIterator ori = hri.BeginRead();
		if(ori.Count() != 1) {
			LOG_BLOCK(LEV_WARNING, "DelayMeas got more than 1 object in response");
			return TR_FAIL;
		}

		millis_t send_rcv_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - mStart).count();
		millis_t rtu_turn_around = Group52Var2::Inst()->mTime.Get(*ori);

		// The later shouldn't happen, but could cause a negative delay which would
		// result in a weird time setting
		mDelay = (send_rcv_time >= rtu_turn_around) ? (send_rcv_time - rtu_turn_around) / 2 : 0;

		return TR_CONTINUE;
	}
	else {
		return TR_SUCCESS;
	}
}

} //ens ns


