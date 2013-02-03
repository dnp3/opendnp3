
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
#ifndef __RESPONSE_CONTEXT_H_
#define __RESPONSE_CONTEXT_H_

#include <queue>
#include <functional>

#include <boost/bind.hpp>


#include "Loggable.h"
#include "APDU.h"
#include "ClassMask.h"
#include "Database.h"
#include "DNPDatabaseTypes.h"
#include "SlaveEventBuffer.h"


namespace opendnp3
{

class Database;
class SlaveEventBuffer;
class ObjectBase;
class SlaveResponseTypes;


/**
 * Builds and tracks the state of responses. Interprets FC_READ requests or
 * can be prompted for an unsolicited response fragment.
 *
 * Coordinates the Database and SlaveEventBuffer.
 */
class ResponseContext : public Loggable
{
	enum Mode {
		UNDEFINED,
		SOLICITED,
		UNSOLICITED
	};

	enum RequestType {
		RT_STATIC = 0,
		RT_EVENT = 1
	};

	//used as a key that decides in what order response headers are packed into APDUs
	struct ResponseKey {

		ResponseKey();		

		ResponseKey(RequestType aType, size_t aOrder);

		RequestType mType;
		size_t mOrder;

		// custom less than function used by STL
		bool operator()(const ResponseKey& a, const ResponseKey& b) const;
	};
	

	/**
	* This function takes an APDU, writes some data to it, and modifies the current state of the
	* ResponseContext class. It returns true if all of the data was written before the APDU was full, 
	* and false otherwise.
	*/		
	typedef std::function<bool (APDU&)> WriteFunction;

public:
	ResponseContext(Logger*, Database*, SlaveResponseTypes* apRspTypes, const EventMaxConfig& arEventMaxConfig);

	Mode GetMode() {
		return mMode;
	}

	IEventBuffer* GetBuffer() {
		return &mBuffer;
	}

	// Setup the response context with a new read request
	IINField Configure(const APDU& arRequest);

	// Configure the APDU with response data for the next fragment
	void LoadResponse(APDU&);

	bool HasEvents(ClassMask aMask);

	/** Configure the APDU with a FIR/FIN unsol packet based on
		current state of the event buffer
	*/
	void LoadUnsol(APDU&, const IINField& arIIN, ClassMask aMask);

	// @return TRUE is all of the response data has already been written
	bool IsComplete() {
		return IsEmpty();
	}

	// Reset the state of the object to the initial state
	void Reset();

	// Tell the buffer to reset written events
	void ClearWritten();

	// Clear written events and reset the state of the object
	void ClearAndReset();

private:

	// configure the state for unsol, return true of events exist
	bool SelectUnsol(ClassMask aMask);

	SlaveEventBuffer mBuffer;

	Mode mMode;

	// @return TRUE if all of the data has been written
	bool LoadStaticData(APDU&);

	/**
	 * Loads the previously buffered events into the APDU response.
	 *
	 * @param arAPDU			the APDU fragment that should be used to store
	 * 							the events
	 *
	 * @return					'true' if all of the events were written, or
	 * 							'false' if more events remain
	 */
	bool LoadEventData(APDU& arAPDU);

	void FinalizeResponse(APDU&, bool aFIN);
	bool IsEmpty();

	bool IsStaticEmpty();
	bool IsEventEmpty();	

	Database* mpDB;				// Pointer to the database for static data
	bool mFIR;
	bool mFIN;
	SlaveResponseTypes* mpRspTypes;

	IINField mTempIIN;
	bool mLoadedEventData;
	
	template<class T>
	struct EventRequest {
		EventRequest(const StreamObject<T>* apObj, size_t aCount = std::numeric_limits<size_t>::max()) :
			pObj(apObj),
			count(aCount)
		{}

		const StreamObject<T>* pObj;		// Type to use to write
		size_t count;						// Number of events to read
	};

	struct VtoEventRequest {
		VtoEventRequest(const SizeByVariationObject* apObj, size_t aCount = std::numeric_limits<size_t>::max()) :
			pObj(apObj),
			count(aCount)
		{}

		const SizeByVariationObject* pObj;	// Type to use to write
		size_t count;						// Number of events to read
	};	

	typedef std::map <ResponseKey, WriteFunction, ResponseKey >	WriteMap;

	// the queue that tracks the pending static write operations
	WriteMap mStaticWriteMap;

	typedef std::deque< EventRequest<Binary> >				BinaryEventQueue;
	typedef std::deque< EventRequest<Analog> >				AnalogEventQueue;
	typedef std::deque< EventRequest<Counter> >				CounterEventQueue;
	typedef std::deque<VtoEventRequest>						VtoEventQueue;

	//these queues track what events have been requested
	BinaryEventQueue mBinaryEvents;
	AnalogEventQueue mAnalogEvents;
	CounterEventQueue mCounterEvents;
	VtoEventQueue mVtoEvents;	

	template <class T>
	bool LoadEvents(APDU& arAPDU, std::deque< EventRequest<T> >& arQueue);

	bool LoadVtoEvents(APDU& arAPDU);	

	//wrappers that select the event buffer and add to the event queues
	void SelectEvents(PointClass aClass, size_t aNum = std::numeric_limits<size_t>::max());

	template <class T>
	size_t SelectEvents(PointClass aClass, const StreamObject<T>* apObj, std::deque< EventRequest<T> >& arQueue, size_t aNum = std::numeric_limits<size_t>::max());

	size_t SelectVtoEvents(PointClass aClass, const SizeByVariationObject* apObj, size_t aNum);


	// T is the event type
	template <class T>
	size_t IterateIndexed(EventRequest<T>& arIters, typename EvtItr< EventInfo<T> >::Type& arIter, APDU& arAPDU);

	template <class T>
	size_t IterateCTO(const StreamObject<T>* apObj, size_t aCount, typename EvtItr< EventInfo<T> >::Type& arIter, APDU& arAPDU);

	template <class T>
	size_t CalcPossibleCTO(typename EvtItr< EventInfo<T> >::Type aIter, size_t aMax);

	size_t IterateIndexed(VtoEventRequest& arRequest, VtoDataEventIter& arIter, APDU& arAPDU);


	// Static write functions

	template <class T>
	void RecordStaticObjects(StreamObject<typename T::MeasType>* apObject, const HeaderReadIterator& arIter);

	template <class T>
	void RecordStaticObjectsByRange(StreamObject<typename T::MeasType>* apObject, size_t aStart, size_t aStop);

	template <class T>
	bool WriteStaticObjects(StreamObject<typename T::MeasType>* apObject, typename StaticIter<T>::Type& arStart, typename StaticIter<T>::Type& arStop, const ResponseKey& arKey, APDU& arAPDU);
};

template <class T>
size_t ResponseContext::SelectEvents(PointClass aClass, const StreamObject<T>* apObj, std::deque< EventRequest<T> >& arQueue, size_t aNum)
{
	size_t num = mBuffer.Select(Convert(T::MeasEnum), aClass, aNum);

	if (num > 0) {
		EventRequest<T> r(apObj, aNum);
		arQueue.push_back(r);
	}

	return num;
}

template <class T>
void ResponseContext::RecordStaticObjects(StreamObject<typename T::MeasType>* apObject, const HeaderReadIterator& arIter)
{	
	size_t num = mpDB->NumType(T::MeasType::MeasEnum);	
	
	//figure out what type of read request this is
	switch(arIter->GetHeaderType())
	{
		case(OHT_ALL_OBJECTS):
			{				
				if(num > 0) this->RecordStaticObjectsByRange<T>(apObject, 0, num - 1);
			}
			break;
		
		case(OHT_RANGED_2_OCTET):			
		case(OHT_RANGED_4_OCTET):			
		case(OHT_RANGED_8_OCTET):
			{
				if(num > 0) {
					size_t max = num - 1;
					RangeInfo ri;
					const IRangeHeader* pHeader = reinterpret_cast<const IRangeHeader*>(arIter->GetHeader());
					pHeader->GetRange(*arIter, ri);			

					if(ri.Start > max || ri.Stop > max || ri.Start > ri.Stop) this->mTempIIN.SetParameterError(true);
					else this->RecordStaticObjectsByRange<T>(apObject, ri.Start, ri.Stop);
				}
				else this->mTempIIN.SetParameterError(true);
			}
			break;

		case(OHT_COUNT_1_OCTET):
		case(OHT_COUNT_2_OCTET):
		case(OHT_COUNT_4_OCTET):
			{
				if(num > 0) {
					size_t max = num - 1;
					size_t count = reinterpret_cast<const ICountHeader*>(arIter->GetHeader())->GetCount(*arIter);
					if(count > 0) {
						size_t start = 0;
						size_t stop = count - 1;
						
						if(start > max || stop > max || start > stop) this->mTempIIN.SetParameterError(true);
						else this->RecordStaticObjectsByRange<T>(apObject, start, stop);
					}
					else this->mTempIIN.SetParameterError(true);
				}
				else this->mTempIIN.SetParameterError(true);
			}
			break;
	}		
}

template <class T>
void ResponseContext::RecordStaticObjectsByRange(StreamObject<typename T::MeasType>* apObject, size_t aStart, size_t aStop)
{
	typename StaticIter<T>::Type first;
	typename StaticIter<T>::Type last;
	mpDB->Begin(first);
	last = first + aStop;
	first = first + aStart;	
	ResponseKey key(RT_STATIC, this->mStaticWriteMap.size());
	WriteFunction func = boost::bind(&ResponseContext::WriteStaticObjects<T>, this, apObject, first, last, key, _1);		
	this->mStaticWriteMap[key] = func;
}

template <class T>
bool ResponseContext::WriteStaticObjects(StreamObject<typename T::MeasType>* apObject, typename StaticIter<T>::Type& arStart, typename StaticIter<T>::Type& arStop, const ResponseKey& arKey, APDU& arAPDU)
{
	size_t start = arStart->mIndex;
	size_t stop = arStop->mIndex;	
	ObjectWriteIterator owi = arAPDU.WriteContiguous(apObject, start, stop);

	for(size_t i = start; i <= stop; ++i) {
		if(owi.IsEnd()) { // out of space in the fragment
			this->mStaticWriteMap[arKey] = boost::bind(&ResponseContext::WriteStaticObjects<T>, this, apObject, arStart, arStop, arKey, _1);
			return false; 
		}
		apObject->Write(*owi, arStart->mValue);
		++arStart; //increment the iterators
		++owi;
	}

	return true;	
}

template <class T>
bool ResponseContext::LoadEvents(APDU& arAPDU, std::deque< EventRequest<T> >& arQueue)
{
	typename EvtItr< EventInfo<T> >::Type itr;
	mBuffer.Begin(itr);
	size_t remain = mBuffer.NumSelected(Convert(T::MeasEnum));

	while (arQueue.size() > 0) {
		/* Get the number of events requested */
		EventRequest<T>& r = arQueue.front();

		if (r.count > remain) {
			r.count = remain;
		}

		size_t written = r.pObj->UseCTO() ? this->IterateCTO<T>(r.pObj, r.count, itr, arAPDU) : this->IterateIndexed<T>(r, itr, arAPDU);
		remain -= written;

		if (written > 0) {
			/* At least one event was loaded */
			this->mLoadedEventData = true;
		}

		if (written == r.count) {
			/* all events were written, finished with request */
			arQueue.pop_front();
		}
		else {
			/* more event data remains in the queue */
			r.count -= written;
			return false;
		}
	}

	return true;	// the queue has been exhausted on this iteration
}


// T is the point info type
template <class T>
size_t ResponseContext::IterateIndexed(EventRequest<T>& arRequest, typename EvtItr< EventInfo<T> >::Type& arIter, APDU& arAPDU)
{
	size_t max_index = mpDB->MaxIndex(T::MeasEnum);
	IndexedWriteIterator write = arAPDU.WriteIndexed(arRequest.pObj, arRequest.count, max_index);

	for(size_t i = 0; i < arRequest.count; ++i) {
		if(write.IsEnd()) return i;										//that's all we can get into this fragment

		write.SetIndex(arIter->mIndex);
		arRequest.pObj->Write(*write, arIter->mValue);					// do the write
		arIter->mWritten = true;										// flag it as written
		++arIter;														// advance the read iterator
		++write;														// advance the write iterator
	}

	return arRequest.count; // all requested events were written
}

template <class T>
size_t ResponseContext::CalcPossibleCTO(typename EvtItr< EventInfo<T> >::Type aIter, size_t aMax)
{
	millis_t start = aIter->mValue.GetTime();

	size_t num = 0;
	while(num < aMax) {
		if((aIter->mValue.GetTime() - start) > UInt16LE::Max) break;
		++num;
		++aIter;
	}

	return num;
}

// T is the point info type
template <class T>
size_t ResponseContext::IterateCTO(const StreamObject<T>* apObj, size_t aCount, typename EvtItr< EventInfo<T> >::Type& arIter, APDU& arAPDU)
{
	size_t max_index = mpDB->MaxIndex(T::MeasEnum);

	millis_t start = arIter->mValue.GetTime();

	// first try to write a CTO object for the first value that we're pushing
	ObjectWriteIterator itr = arAPDU.WriteContiguous(Group51Var1::Inst(), 0, 0, QC_1B_CNT);
	if(itr.IsEnd()) return 0;
	else Group51Var1::Inst()->mTime.Set(*itr, start);

	// predetermine how many results you're going to be able to fit given the time differences
	size_t num = this->CalcPossibleCTO<T>(arIter, aCount);
	IndexedWriteIterator write = arAPDU.WriteIndexed(apObj, num, max_index); //start the object write

	for(size_t i = 0; i < num; ++i) {
		if(write.IsEnd()) return i;										// that's all we can get into this fragment

		T tmp = arIter->mValue;											// make a copy and adjust the time
		tmp.SetTime(tmp.GetTime() - start);

		write.SetIndex(arIter->mIndex);
		apObj->Write(*write, tmp);										// do the write, with the tmp
		arIter->mWritten = true;										// flag it as written
		++arIter;														// advance the read iterator
		++write;														// advance the write iterator
	}

	if(num == aCount) return num;
	else return num + this->IterateCTO(apObj, aCount - num, arIter, arAPDU); //recurse, and do another CTO header
}

}

/* vim: set ts=4 sw=4: */

#endif
