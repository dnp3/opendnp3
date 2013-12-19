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
#include "ResponseContext.h"

#include <opendnp3/Util.h>
#include <opendnp3/DNPConstants.h>

#include <openpal/LoggableMacros.h>


#include "Objects.h"
#include "SlaveResponseTypes.h"

using namespace openpal;

namespace opendnp3
{

ResponseContext::ResponseKey::ResponseKey() : type(RequestType::STATIC), order(0)
{}

ResponseContext::ResponseKey::ResponseKey(ResponseContext::RequestType aType, size_t aOrder) : type(aType), order(aOrder)
{}

// custom less than function used by STL
bool ResponseContext::ResponseKey::operator()(const ResponseContext::ResponseKey& a, const ResponseContext::ResponseKey& b) const
{
	if(a.type < b.type) return true;
	else if(a.type > b.type) return false;
	else {
		return a.order < b.order;
	}
}


ResponseContext::ResponseContext(openpal::Logger& arLogger, Database* apDB, SlaveResponseTypes* apRspTypes) :
	Loggable(arLogger),	
	mMode(Mode::UNDEFINED),
	mpDB(apDB),
	mFIR(true),
	mFIN(false),
	mpRspTypes(apRspTypes),
	mLoadedEventData(false)
{}

/*
IINField ResponseContext::RecordAllObjects(GroupVariation gv)
{
	mMode = Mode::SOLICITED;
	switch(gv)
	{
		case(GroupVariation::Group60Var1):
			return this->RecordIntegrity();		
		default:
			return IINField(IINBit::PARAM_ERROR);
	};	
}


IINField ResponseContext::RecordIntegrity()
{
	return	this->RecordAllStatic(mpRspTypes->mpStaticBinary, mpDB->Binaries()) |
			this->RecordAllStatic(mpRspTypes->mpStaticAnalog, mpDB->Analogs()) |
			this->RecordAllStatic(mpRspTypes->mpStaticCounter, mpDB->Counters()) |
			this->RecordAllStatic(mpRspTypes->mpStaticControlStatus, mpDB->ControlStatii()) |
			this->RecordAllStatic(mpRspTypes->mpStaticSetpointStatus, mpDB->SetpointStatii());		
}
*/

void ResponseContext::Reset()
{
	mFIR = true;
	mLoadedEventData = false;
	mMode = Mode::UNDEFINED;
	
	//this->mStaticWriteMap.clear();
}

void ResponseContext::ClearAndReset()
{
	this->ClearWritten();
	this->Reset();
}

/*
IINField ResponseContext::Configure(const APDU& arRequest)
{
	this->Reset();
	mMode = Mode::SOLICITED;

	for (HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {
		
		// Handle all of the objects that have a Group/Variation tuple
		switch (MACRO_DNP_RADIX(hdr->GetGroup(), hdr->GetVariation())) {

		case(MACRO_DNP_RADIX(1, 0)):
			this->RecordStaticObjects<Binary>(mpRspTypes->mpStaticBinary, hdr, mpDB->BeginBinary());
			break;
		case(MACRO_DNP_RADIX(1, 2)):
			this->RecordStaticObjects<Binary>(Group1Var2::Inst(), hdr, mpDB->BeginBinary());
			break;
		case(MACRO_DNP_RADIX(10, 0)):
			this->RecordStaticObjects<ControlStatus>(mpRspTypes->mpStaticControlStatus, hdr, mpDB->BeginControlStatus());
			break;
		case(MACRO_DNP_RADIX(10, 2)):
			this->RecordStaticObjects<ControlStatus>(Group10Var2::Inst(), hdr, mpDB->BeginControlStatus());
			break;
		case(MACRO_DNP_RADIX(20, 0)):
			this->RecordStaticObjects<Counter>(mpRspTypes->mpStaticCounter, hdr, mpDB->BeginCounter());
			break;
		case(MACRO_DNP_RADIX(20, 1)):
			this->RecordStaticObjects<Counter>(Group20Var1::Inst(), hdr, mpDB->BeginCounter());
			break;
		case(MACRO_DNP_RADIX(20, 2)):
			this->RecordStaticObjects<Counter>(Group20Var2::Inst(), hdr, mpDB->BeginCounter());
			break;
		case(MACRO_DNP_RADIX(20, 3)):
			this->RecordStaticObjects<Counter>(Group20Var3::Inst(), hdr, mpDB->BeginCounter());
			break;
		case(MACRO_DNP_RADIX(20, 4)):
			this->RecordStaticObjects<Counter>(Group20Var4::Inst(), hdr, mpDB->BeginCounter());
			break;
		case(MACRO_DNP_RADIX(20, 5)):
			this->RecordStaticObjects<Counter>(Group20Var5::Inst(), hdr, mpDB->BeginCounter());
			break;
		case(MACRO_DNP_RADIX(20, 6)):
			this->RecordStaticObjects<Counter>(Group20Var6::Inst(), hdr, mpDB->BeginCounter());
			break;
		case(MACRO_DNP_RADIX(20, 7)):
			this->RecordStaticObjects<Counter>(Group20Var7::Inst(), hdr, mpDB->BeginCounter());
			break;
		case(MACRO_DNP_RADIX(20, 8)):
			this->RecordStaticObjects<Counter>(Group20Var8::Inst(), hdr, mpDB->BeginCounter());
			break;
		case(MACRO_DNP_RADIX(30, 0)):
			this->RecordStaticObjects<Analog>(mpRspTypes->mpStaticAnalog, hdr, mpDB->BeginAnalog());
			break;
		case(MACRO_DNP_RADIX(30, 1)):
			this->RecordStaticObjects<Analog>(Group30Var1::Inst(), hdr, mpDB->BeginAnalog());
			break;
		case(MACRO_DNP_RADIX(30, 2)):
			this->RecordStaticObjects<Analog>(Group30Var2::Inst(), hdr, mpDB->BeginAnalog());
			break;
		case(MACRO_DNP_RADIX(30, 3)):
			this->RecordStaticObjects<Analog>(Group30Var3::Inst(), hdr, mpDB->BeginAnalog());
			break;
		case(MACRO_DNP_RADIX(30, 4)):
			this->RecordStaticObjects<Analog>(Group30Var4::Inst(), hdr, mpDB->BeginAnalog());
			break;
		case(MACRO_DNP_RADIX(30, 5)):
			this->RecordStaticObjects<Analog>(Group30Var5::Inst(), hdr, mpDB->BeginAnalog());
			break;
		case(MACRO_DNP_RADIX(30, 6)):
			this->RecordStaticObjects<Analog>(Group30Var6::Inst(), hdr, mpDB->BeginAnalog());
			break;
		case(MACRO_DNP_RADIX(40, 0)):
			this->RecordStaticObjects<SetpointStatus>(mpRspTypes->mpStaticSetpointStatus, hdr, mpDB->BeginSetpointStatus());
			break;
		case(MACRO_DNP_RADIX(40, 1)):
			this->RecordStaticObjects<SetpointStatus>(Group40Var1::Inst(), hdr, mpDB->BeginSetpointStatus());
			break;
		case(MACRO_DNP_RADIX(40, 2)):
			this->RecordStaticObjects<SetpointStatus>(Group40Var2::Inst(), hdr, mpDB->BeginSetpointStatus());
			break;
		case(MACRO_DNP_RADIX(40, 3)):
			this->RecordStaticObjects<SetpointStatus>(Group40Var3::Inst(), hdr, mpDB->BeginSetpointStatus());
			break;
		case(MACRO_DNP_RADIX(40, 4)):
			this->RecordStaticObjects<SetpointStatus>(Group40Var4::Inst(), hdr, mpDB->BeginSetpointStatus());
			break;

			// event objects
		case(MACRO_DNP_RADIX(2, 0)):
			this->SelectEvents(PC_ALL_EVENTS, mpRspTypes->mpEventBinary, mBinaryEvents, GetEventCount(hdr.info()));
			break;
		case(MACRO_DNP_RADIX(22, 0)):
			this->SelectEvents(PC_ALL_EVENTS, mpRspTypes->mpEventCounter, mCounterEvents, GetEventCount(hdr.info()));
			break;
		case(MACRO_DNP_RADIX(32, 0)):
			this->SelectEvents(PC_ALL_EVENTS, mpRspTypes->mpEventAnalog, mAnalogEvents, GetEventCount(hdr.info()));
			break;

			//specific objects
		case(MACRO_DNP_RADIX(2, 1)):
			this->SelectEvents(PC_ALL_EVENTS, Group2Var1::Inst(), mBinaryEvents, GetEventCount(hdr.info()));
			break;
		case(MACRO_DNP_RADIX(2, 2)):
			this->SelectEvents(PC_ALL_EVENTS, Group2Var2::Inst(), mBinaryEvents, GetEventCount(hdr.info()));
			break;
		case(MACRO_DNP_RADIX(2, 3)):
			this->SelectEvents(PC_ALL_EVENTS, Group2Var3::Inst(), mBinaryEvents, GetEventCount(hdr.info()));
			break;

			// Class Objects
		case(MACRO_DNP_RADIX(60, 1)):
			this->RecordStaticObjects<Binary>(mpRspTypes->mpStaticBinary, hdr, mpDB->BeginBinary());
			this->RecordStaticObjects<Analog>(mpRspTypes->mpStaticAnalog, hdr, mpDB->BeginAnalog());
			this->RecordStaticObjects<Counter>(mpRspTypes->mpStaticCounter, hdr, mpDB->BeginCounter());
			this->RecordStaticObjects<ControlStatus>(mpRspTypes->mpStaticControlStatus, hdr, mpDB->BeginControlStatus());
			this->RecordStaticObjects<SetpointStatus>(mpRspTypes->mpStaticSetpointStatus, hdr, mpDB->BeginSetpointStatus());
			break;
		case(MACRO_DNP_RADIX(60, 2)):
			this->SelectEvents(PC_CLASS_1, GetEventCount(hdr.info()));
			break;
		case(MACRO_DNP_RADIX(60, 3)):
			this->SelectEvents(PC_CLASS_2, GetEventCount(hdr.info()));
			break;
		case(MACRO_DNP_RADIX(60, 4)):
			this->SelectEvents(PC_CLASS_3, GetEventCount(hdr.info()));
			break;
		default:
			LOG_BLOCK(LogLevel::Warning, "READ for obj " << hdr->GetGroup() << " var " << hdr->GetVariation() << " not supported.");
			this->mTempIIN.Set(IINBit::FUNC_NOT_SUPPORTED);
			break;
		}
	}

	return mTempIIN;
}

IINField ResponseContext::SelectEvents(PointClass aClass, uint32_t maximum)
{
	auto remain = maximum;

	remain -= this->SelectEvents(aClass, mpRspTypes->mpEventBinary, mBinaryEvents, remain);
	remain -= this->SelectEvents(aClass, mpRspTypes->mpEventAnalog, mAnalogEvents, remain);
	remain -= this->SelectEvents(aClass, mpRspTypes->mpEventCounter, mCounterEvents, remain);

	return IINField::Empty;
}
*/

/*
void ResponseContext::LoadResponse(APDU& apdu)
{
	//delay the setting of FIR/FIN until we know if it will be multifragmented or not
	apdu.Set(FunctionCode::RESPONSE);

	//bool wroteAll = this->LoadEventData(apdu);
	bool wroteAll = true;

	if(wroteAll) wroteAll = LoadStaticData(apdu);

	FinalizeResponse(apdu, wroteAll);
}
*/

/*
bool ResponseContext::SelectUnsol(ClassMask m)
{
	if(m.class1) this->SelectEvents(PC_CLASS_1);
	if(m.class2) this->SelectEvents(PC_CLASS_2);
	if(m.class3) this->SelectEvents(PC_CLASS_3);

	return mBuffer.NumSelected() > 0;
}
*/

bool ResponseContext::HasEvents(ClassMask m)
{
	//if(m.class1 && mBuffer.HasClassData(PC_CLASS_1)) return true;
	//if(m.class2 && mBuffer.HasClassData(PC_CLASS_2)) return true;
	//if(m.class3 && mBuffer.HasClassData(PC_CLASS_3)) return true;

	return false;
}

/*
void ResponseContext::LoadUnsol(APDU& arAPDU, const IINField& arIIN, ClassMask m)
{
	//this->SelectUnsol(m);

	arAPDU.Set(FunctionCode::UNSOLICITED_RESPONSE, true, true, true, true);
	//this->LoadEventData(arAPDU);
}
*/

/*
bool ResponseContext::LoadEventData(APDU& arAPDU)
{
	if (!this->LoadEvents<Binary>(arAPDU, mBuffer.BeginBinary(), mBinaryEvents)) return false;
	if (!this->LoadEvents<Analog>(arAPDU, mBuffer.BeginAnalog(), mAnalogEvents)) return false;
	if (!this->LoadEvents<Counter>(arAPDU, mBuffer.BeginCounter(), mCounterEvents)) return false;	

	return true;
}
*/

bool ResponseContext::IsEmpty()
{
	return this->IsStaticEmpty() && this->IsEventEmpty();
}

bool ResponseContext::IsStaticEmpty()
{
	return false; // TODO - return this->mStaticWriteMap.empty();
}

/*
void ResponseContext::FinalizeResponse(APDU& arAPDU, bool aFIN)
{
	mFIN = aFIN;
	bool confirm = !aFIN || this->mLoadedEventData;
	arAPDU.SetControl(mFIR, mFIN, confirm);
	mFIR = false;
	this->mLoadedEventData = false;
}
*/

/*
bool ResponseContext::LoadStaticData(APDU& arAPDU)
{
	while(!this->mStaticWriteMap.empty()) {

		WriteMap::iterator i = this->mStaticWriteMap.begin();

		if(i->second(arAPDU)) {
			this->mStaticWriteMap.erase(i);
		}
		else return false;
	}

	return true;
}
*/

}

/* vim: set ts=4 sw=4: */
