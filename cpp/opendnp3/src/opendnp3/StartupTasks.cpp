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


void ClearRestartIIN::ConfigureRequest(APDURequest& request)
{
	request.SetFunction(FunctionCode::WRITE);
	auto writer = request.GetWriter();
	auto iter = writer.IterateOverSingleBitfield<UInt8>(GroupVariationID(80, 1), QualifierCode::UINT8_START_STOP, 7);
	iter.Write(false);
	iter.Complete();	
}

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


void ConfigureUnsol::ConfigureRequest(APDURequest& request)
{	
	request.SetFunction(mIsEnable ? FunctionCode::ENABLE_UNSOLICITED : FunctionCode::DISABLE_UNSOLICITED);
	auto writer = request.GetWriter();
	if (mClassMask & PC_CLASS_1) writer.WriteHeader(GroupVariationID(60, 2), QualifierCode::ALL_OBJECTS);
	if (mClassMask & PC_CLASS_2) writer.WriteHeader(GroupVariationID(60, 3), QualifierCode::ALL_OBJECTS);
	if (mClassMask & PC_CLASS_3) writer.WriteHeader(GroupVariationID(60, 4), QualifierCode::ALL_OBJECTS);

}

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

void TimeSync::ConfigureRequest(APDURequest& request)
{	
	if(mDelay < 0) {
		request.SetFunction(FunctionCode::DELAY_MEASURE);		
		mStart = mpTimeSrc->Now();
	}
	else {
		auto now = mpTimeSrc->Now().msSinceEpoch;
		Group50Var1 time = { now + mDelay };
		request.SetFunction(FunctionCode::WRITE);
		auto writer = request.GetWriter();
		writer.WriteSingleValue<UInt8, Group50Var1>(QualifierCode::UINT8_CNT, time);
	}

}


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


