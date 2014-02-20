/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include "StartupTasks.h"

#include "TimeSyncHandler.h"
#include "APDUParser.h"

#include <opendnp3/PointClass.h>
#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{

/* ------ Clear Restart ------- */

ClearRestartIIN::ClearRestartIIN(openpal::Logger& arLogger) :
	SimpleRspBase(arLogger)
{}

/*
void ClearRestartIIN::ConfigureRequest(APDU& arAPDU)
{
	arAPDU.Set(FunctionCode::WRITE);
	Group80Var1* pObj = Group80Var1::Inst(); // Internal indications object
	ObjectWriteIterator i = arAPDU.WriteContiguous(pObj, 7, 7); // index 7 == device restart
	pObj->Write(*i, 7, 7, false);
}
*/

/* ------ Configure Unsol ------- */

ConfigureUnsol::ConfigureUnsol(openpal::Logger& arLogger) :
	SimpleRspBase(arLogger),
	mIsEnable(false),
	mClassMask(0)
{}

void ConfigureUnsol::Set(bool aIsEnable, int aClassMask)
{
	mIsEnable = aIsEnable;
	mClassMask = aClassMask;
}

/*
void ConfigureUnsol::ConfigureRequest(APDU& arAPDU)
{
	arAPDU.Set(mIsEnable ? FunctionCode::ENABLE_UNSOLICITED : FunctionCode::DISABLE_UNSOLICITED);
	if(mClassMask & PC_CLASS_1) arAPDU.DoPlaceholderWrite(Group60Var2::Inst());
	if(mClassMask & PC_CLASS_2) arAPDU.DoPlaceholderWrite(Group60Var3::Inst());
	if(mClassMask & PC_CLASS_3) arAPDU.DoPlaceholderWrite(Group60Var4::Inst());
}
*/


/* ------ Time Sync ------- */

TimeSync::TimeSync(openpal::Logger& arLogger, IUTCTimeSource* apTimeSrc) :
	SingleRspBase(arLogger),
	mpTimeSrc(apTimeSrc),
	mDelay(-1)
{}

void TimeSync::Init()
{
	mDelay = -1;
}

/*
void TimeSync::ConfigureRequest(APDU& arAPDU)
{
	if(mDelay < 0) {
		arAPDU.Set(FunctionCode::DELAY_MEASURE);
		mStart = mpTimeSrc->Now();
	}
	else {
		arAPDU.Set(FunctionCode::WRITE);
		ObjectWriteIterator owi = arAPDU.WriteContiguous(Group50Var1Temp::Inst(), 0, 0, QualifierCode::UINT8_CNT);
		Group50Var1Temp::Inst()->mTime.Set(*owi, mpTimeSrc->Now().msSinceEpoch + mDelay);
	}
}
*/

TaskResult TimeSync::_OnFinalResponse(const APDUResponseRecord& record)
{
	if(mDelay < 0) {

		TimeSyncHandler handler(mLogger);
		auto result = APDUParser::ParseTwoPass(record.objects, &handler);
		if(result == APDUParser::Result::OK) {
			uint16_t rtuTurnAroundTime;
			if(handler.GetTimeDelay(rtuTurnAroundTime)) 
			{
				auto now = mpTimeSrc->Now();
				auto sendReceieveTime = now.msSinceEpoch - mStart.msSinceEpoch;				

				// The later shouldn't happen, but could cause a negative delay which would
				// result in a weird time setting
				mDelay = (sendReceieveTime >= rtuTurnAroundTime) ? (sendReceieveTime - rtuTurnAroundTime) / 2 : 0;

				return TR_CONTINUE;
			}
			else return TR_FAIL;
		} else {
			LOG_BLOCK(LogLevel::Warning, "Error parsing response headers: " << static_cast<int>(result)); // TODO - turn these into strings
			return TR_FAIL;
		}

		
		/*  TODO - move this logic to the TimeSyncHandler
		HeaderReadIterator hri = arAPDU.BeginRead();
		if(hri.Count() != 1) {
			LOG_BLOCK(LogLevel::Warning, "DelayMeas response w/ unexcpected header count");
			return TR_FAIL;
		}

		if(!hri->GetBaseObject()->Equals(Group52Var2::Inst())) {
			LOG_BLOCK(LogLevel::Warning, "DelayMeas response w/ unexpected object: " << hri->GetBaseObject()->Name());
			return TR_FAIL;
		}

		ObjectReadIterator ori = hri.BeginRead();
		if(ori.Count() != 1) {
			LOG_BLOCK(LogLevel::Warning, "DelayMeas got more than 1 object in response");
			return TR_FAIL;
		}
		*/

		
	}
	else {
		return TR_SUCCESS;
	}
}

} //ens ns


