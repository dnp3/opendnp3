
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

#include "ResponseContext.h"

#include <opendnp3/Util.h>
#include <opendnp3/DNPConstants.h>

#include <openpal/LoggableMacros.h>


#include "Objects.h"
#include "SlaveResponseTypes.h"

using namespace openpal;

namespace opendnp3
{

ResponseContext::ResponseKey::ResponseKey() : mType(RT_STATIC), mOrder(0)
{}

ResponseContext::ResponseKey::ResponseKey(ResponseContext::RequestType aType, size_t aOrder) : mType(aType), mOrder(aOrder)
{}

// custom less than function used by STL
bool ResponseContext::ResponseKey::operator()(const ResponseContext::ResponseKey& a, const ResponseContext::ResponseKey& b) const
{
	if(a.mType < b.mType) return true;
	else if(a.mType > b.mType) return false;
	else {
		return a.mOrder < b.mOrder;
	}
}


ResponseContext::ResponseContext(openpal::Logger& arLogger, Database* apDB, SlaveResponseTypes* apRspTypes, const EventMaxConfig& arEventMaxConfig) :
	Loggable(arLogger),
	mBuffer(arEventMaxConfig),
	mMode(UNDEFINED),
	mpDB(apDB),
	mFIR(true),
	mFIN(false),
	mpRspTypes(apRspTypes),
	mLoadedEventData(false)
{}

void ResponseContext::Reset()
{
	mFIR = true;
	mLoadedEventData = false;
	mMode = UNDEFINED;
	mTempIIN.Zero();

	this->mStaticWriteMap.clear();

	this->mBinaryEvents.clear();
	this->mAnalogEvents.clear();
	this->mCounterEvents.clear();
	this->mVtoEvents.clear();

	mBuffer.Deselect();
}

void ResponseContext::ClearWritten()
{
	size_t written = mBuffer.ClearWritten();

	size_t deselected = mBuffer.Deselect();

	LOG_BLOCK(LEV_DEBUG, "Clearing written events: " << written << " deselected: " << deselected);
}

void ResponseContext::ClearAndReset()
{
	this->ClearWritten();
	this->Reset();
}

inline size_t GetEventCount(const HeaderInfo& arHeader)
{
	switch(arHeader.GetQualifier()) {
	case QC_1B_CNT:
	case QC_2B_CNT:
		return arHeader.GetCount();
	default:
		return std::numeric_limits<size_t>::max();
	}
}

IINField ResponseContext::Configure(const APDU& arRequest)
{
	this->Reset();
	mMode = SOLICITED;

	for (HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {
		/*
		 * Handle all of the objects that only use a Group identifier.  The
		 * switch statement is responsible for selecting all of the events
		 * that are in the various queues that could be used to respond to the
		 * arRequest message.  Then a separate handler will loop through and
		 * cherry pick the events that will make it into the response.
		 *
		 * For this first switch statement set, use "continue" rather than
		 * "break" so that control loops back around to the for loop.
		 */
		switch (hdr->GetGroup()) {
			/* Virtual Terminal Objects */
		case 113:
			this->SelectVtoEvents(PC_ALL_EVENTS, Group113Var0::Inst(), GetEventCount(hdr.info()));
			continue;
		default:
			/*
			 * Note: the next switch statement's default statement will
			 * catch unknown object types.
			 */
			break;
		}

		/* Handle all of the objects that have a Group/Variation tuple */
		switch (MACRO_DNP_RADIX(hdr->GetGroup(), hdr->GetVariation())) {

		case(MACRO_DNP_RADIX(1, 0)):
			this->RecordStaticObjects<BinaryInfo>(mpRspTypes->mpStaticBinary, hdr);
			break;
		case(MACRO_DNP_RADIX(1, 2)):
			this->RecordStaticObjects<BinaryInfo>(Group1Var2::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(10, 0)):
			this->RecordStaticObjects<ControlStatusInfo>(mpRspTypes->mpStaticControlStatus, hdr);
			break;
		case(MACRO_DNP_RADIX(10, 2)):
			this->RecordStaticObjects<ControlStatusInfo>(Group10Var2::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(20, 0)):
			this->RecordStaticObjects<CounterInfo>(mpRspTypes->mpStaticCounter, hdr);
			break;
		case(MACRO_DNP_RADIX(20, 1)):
			this->RecordStaticObjects<CounterInfo>(Group20Var1::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(20, 2)):
			this->RecordStaticObjects<CounterInfo>(Group20Var2::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(20, 3)):
			this->RecordStaticObjects<CounterInfo>(Group20Var3::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(20, 4)):
			this->RecordStaticObjects<CounterInfo>(Group20Var4::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(20, 5)):
			this->RecordStaticObjects<CounterInfo>(Group20Var5::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(20, 6)):
			this->RecordStaticObjects<CounterInfo>(Group20Var6::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(20, 7)):
			this->RecordStaticObjects<CounterInfo>(Group20Var7::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(20, 8)):
			this->RecordStaticObjects<CounterInfo>(Group20Var8::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(30, 0)):
			this->RecordStaticObjects<AnalogInfo>(mpRspTypes->mpStaticAnalog, hdr);
			break;
		case(MACRO_DNP_RADIX(30, 1)):
			this->RecordStaticObjects<AnalogInfo>(Group30Var1::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(30, 2)):
			this->RecordStaticObjects<AnalogInfo>(Group30Var2::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(30, 3)):
			this->RecordStaticObjects<AnalogInfo>(Group30Var3::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(30, 4)):
			this->RecordStaticObjects<AnalogInfo>(Group30Var4::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(30, 5)):
			this->RecordStaticObjects<AnalogInfo>(Group30Var5::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(30, 6)):
			this->RecordStaticObjects<AnalogInfo>(Group30Var6::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(40, 0)):
			this->RecordStaticObjects<SetpointStatusInfo>(mpRspTypes->mpStaticSetpointStatus, hdr);
			break;
		case(MACRO_DNP_RADIX(40, 1)):
			this->RecordStaticObjects<SetpointStatusInfo>(Group40Var1::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(40, 2)):
			this->RecordStaticObjects<SetpointStatusInfo>(Group40Var2::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(40, 3)):
			this->RecordStaticObjects<SetpointStatusInfo>(Group40Var3::Inst(), hdr);
			break;
		case(MACRO_DNP_RADIX(40, 4)):
			this->RecordStaticObjects<SetpointStatusInfo>(Group40Var4::Inst(), hdr);
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
			this->RecordStaticObjects<BinaryInfo>(mpRspTypes->mpStaticBinary, hdr);
			this->RecordStaticObjects<AnalogInfo>(mpRspTypes->mpStaticAnalog, hdr);
			this->RecordStaticObjects<CounterInfo>(mpRspTypes->mpStaticCounter, hdr);
			this->RecordStaticObjects<ControlStatusInfo>(mpRspTypes->mpStaticControlStatus, hdr);
			this->RecordStaticObjects<SetpointStatusInfo>(mpRspTypes->mpStaticSetpointStatus, hdr);
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
			LOG_BLOCK(LEV_WARNING, "READ for obj " << hdr->GetGroup() << " var " << hdr->GetVariation() << " not supported.");
			this->mTempIIN.SetFuncNotSupported(true);
			break;
		}
	}

	return mTempIIN;
}

void ResponseContext::SelectEvents(PointClass aClass, size_t aNum)
{
	size_t remain = aNum;

	if (mBuffer.IsOverflow()) {
		mTempIIN.SetEventBufferOverflow(true);
	}

	remain -= this->SelectEvents(aClass, mpRspTypes->mpEventBinary, mBinaryEvents, remain);
	remain -= this->SelectEvents(aClass, mpRspTypes->mpEventAnalog, mAnalogEvents, remain);
	remain -= this->SelectEvents(aClass, mpRspTypes->mpEventCounter, mCounterEvents, remain);
	remain -= this->SelectVtoEvents(aClass, mpRspTypes->mpEventVto, remain);
}

size_t ResponseContext::SelectVtoEvents(PointClass aClass, const SizeByVariationObject* apObj, size_t aNum)
{
	// only select as many messages we are likley to be able to send
	// TODO: remove MAX_VTO_EVENTS once eventbuffer is fixed so selecting/adding/deselecting events keeps same size
	const size_t MAX_VTO_EVENTS = 7;

	size_t selectable = Min<size_t>(aNum, MAX_VTO_EVENTS);
	size_t num = mBuffer.Select(BT_VTO, aClass, selectable);

	LOG_BLOCK(LEV_INTERPRET, "Selected: " << num << " vto events");

	if (num > 0) {
		VtoEventRequest r(apObj, aNum);
		this->mVtoEvents.push_back(r);
	}

	return num;
}

void ResponseContext::LoadResponse(APDU& arAPDU)
{
	//delay the setting of FIR/FIN until we know if it will be multifragmented or not
	arAPDU.Set(FC_RESPONSE);

	bool wrote_all = this->LoadEventData(arAPDU);

	if(wrote_all) wrote_all = LoadStaticData(arAPDU);

	FinalizeResponse(arAPDU, wrote_all);
}

bool ResponseContext::SelectUnsol(ClassMask m)
{
	if(m.class1) this->SelectEvents(PC_CLASS_1);
	if(m.class2) this->SelectEvents(PC_CLASS_2);
	if(m.class3) this->SelectEvents(PC_CLASS_3);

	return mBuffer.NumSelected() > 0;
}

bool ResponseContext::HasEvents(ClassMask m)
{
	if(m.class1 && mBuffer.HasClassData(PC_CLASS_1)) return true;
	if(m.class2 && mBuffer.HasClassData(PC_CLASS_2)) return true;
	if(m.class3 && mBuffer.HasClassData(PC_CLASS_3)) return true;

	return false;
}

void ResponseContext::LoadUnsol(APDU& arAPDU, const IINField& arIIN, ClassMask m)
{
	this->SelectUnsol(m);

	arAPDU.Set(FC_UNSOLICITED_RESPONSE, true, true, true, true);
	this->LoadEventData(arAPDU);
}

bool ResponseContext::LoadEventData(APDU& arAPDU)
{
	if (!this->LoadEvents<Binary>(arAPDU, mBinaryEvents)) return false;
	if (!this->LoadEvents<Analog>(arAPDU, mAnalogEvents)) return false;
	if (!this->LoadEvents<Counter>(arAPDU, mCounterEvents)) return false;
	if (!this->LoadVtoEvents(arAPDU)) return false;

	return true;
}

bool ResponseContext::LoadVtoEvents(APDU& arAPDU)
{
	VtoDataEventIter itr;
	mBuffer.Begin(itr);
	size_t remain = mBuffer.NumSelected(BT_VTO);

	while (this->mVtoEvents.size() > 0) {
		/* Get the number of events requested */
		VtoEventRequest& r = this->mVtoEvents.front();

		if (r.count > remain) {
			r.count = remain;
		}

		size_t written = this->IterateIndexed(r, itr, arAPDU);
		remain -= written;

		if (written > 0) {
			/* At least one event was loaded */
			this->mLoadedEventData = true;
		}

		if (written == r.count) {
			/* all events were written, finished with request */
			this->mVtoEvents.pop_front();
		}
		else {
			/* more event data remains in the queue */
			r.count -= written;
			return false;
		}
	}

	return true;	// the queue has been exhausted on this iteration
}

size_t ResponseContext::IterateIndexed(VtoEventRequest& arRequest, VtoDataEventIter& arIter, APDU& arAPDU)
{
	for (size_t i = 0; i < arRequest.count; ++i) {
		IndexedWriteIterator itr = arAPDU.WriteIndexed(
		                                   arRequest.pObj,
		                                   arIter->mValue.GetSize(),
		                                   arIter->mIndex
		                           );

		/*
		 * Check to see if the APDU fragment has enough room for the
		 * data segment.  If the fragment is full, return out of this
		 * function and let the fragment send.
		 */
		if (itr.IsEnd()) {
			return i;
		}

		/* Set the object index */
		itr.SetIndex(arIter->mIndex);

		/* Write the data to the APDU message */
		arRequest.pObj->Write(
		        *itr,
		        arIter->mValue.GetSize(),
				arIter->mValue.Data()
		);

		/* Mark the data segment as being written */
		arIter->mWritten = true;

		/* Move to the next data segment in the reader buffer */
		++arIter;
	}

	return arRequest.count; // all requested events were written
}

bool ResponseContext::IsEmpty()
{
	return this->IsStaticEmpty() && this->IsEventEmpty();
}

bool ResponseContext::IsStaticEmpty()
{
	return this->mStaticWriteMap.empty();
}

bool ResponseContext::IsEventEmpty()
{
	// are there unwritten events in the selection buffer?
	return mBuffer.NumSelected() == 0;
}

void ResponseContext::FinalizeResponse(APDU& arAPDU, bool aFIN)
{
	mFIN = aFIN;
	bool confirm = !aFIN || this->mLoadedEventData;
	arAPDU.SetControl(mFIR, mFIN, confirm);
	mFIR = false;
	this->mLoadedEventData = false;
}

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

}

/* vim: set ts=4 sw=4: */
