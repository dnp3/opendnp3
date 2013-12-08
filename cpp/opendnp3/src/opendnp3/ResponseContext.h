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
#ifndef __RESPONSE_CONTEXT_H_
#define __RESPONSE_CONTEXT_H_

#include <queue>
#include <functional>

#include <openpal/Location.h>

#include <opendnp3/ClassMask.h>

#include "APDU.h"
#include "Database.h"
#include "SlaveEventBuffer.h"
#include "DNPDatabaseTypes.h"

namespace opendnp3
{

class Database;
class SlaveEventBuffer;
class ObjectBase;
class SlaveResponseTypes;


/**
 * Builds and tracks the state of responses. Interprets FunctionCode::READ requests or
 * can be prompted for an unsolicited response fragment.
 *
 * Coordinates the Database and SlaveEventBuffer.
 */
class ResponseContext : public openpal::Loggable
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
	ResponseContext(openpal::Logger& arLogger, Database*, SlaveResponseTypes* apRspTypes, const EventMaxConfig& arEventMaxConfig);

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

	typedef std::map <ResponseKey, WriteFunction, ResponseKey >	WriteMap;

	// the queue that tracks the pending static write operations
	WriteMap mStaticWriteMap;

	typedef std::deque< EventRequest<Binary> >				BinaryEventQueue;
	typedef std::deque< EventRequest<Analog> >				AnalogEventQueue;
	typedef std::deque< EventRequest<Counter> >				CounterEventQueue;	

	//these queues track what events have been requested
	BinaryEventQueue mBinaryEvents;
	AnalogEventQueue mAnalogEvents;
	CounterEventQueue mCounterEvents;	

	template <class T>
	bool LoadEvents(APDU& arAPDU, const typename EventIterator<T>::Type& aBegin, std::deque< EventRequest<T> >& arQueue);	

	//wrappers that select the event buffer and add to the event queues
	void SelectEvents(PointClass aClass, size_t aNum = std::numeric_limits<size_t>::max());

	template <class T>
	size_t SelectEvents(PointClass aClass, const StreamObject<T>* apObj, std::deque< EventRequest<T> >& arQueue, size_t aNum = std::numeric_limits<size_t>::max());

	// T is the event type
	template <class T>
	size_t IterateIndexed(EventRequest<T>& arIters, typename EventIterator<T>::Type& arIter, APDU& arAPDU);

	template <class T>
	size_t IterateCTO(const StreamObject<T>* apObj, size_t aCount, typename EventIterator<T>::Type& arIter, APDU& arAPDU);

	template <class T>
	size_t CalcPossibleCTO(typename EventIterator<T>::Type aIter, size_t aMax);


	// Static write functions

	template <class T>
	void RecordStaticObjects(StreamObject<T>* apObject, const HeaderReadIterator& arIter, const typename StaticIterator<T>::Type& begin);

	template <class T>
	void RecordStaticObjectsByRange(StreamObject<T>* apObject, typename StaticIterator<T>::Type begin, size_t aStart, size_t aStop);

	template <class T>
	bool WriteStaticObjects(StreamObject<T>* apObject, typename StaticIterator<T>::Type aStart, typename StaticIterator<T>::Type aStop, ResponseKey aKey, APDU& arAPDU);
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
void ResponseContext::RecordStaticObjects(StreamObject<T>* apObject, const HeaderReadIterator& arIter, const typename StaticIterator<T>::Type& begin)
{
	size_t num = mpDB->NumType(T::MeasEnum);

	//figure out what type of read request this is
	switch(arIter->GetHeaderType()) {
	case(OHT_ALL_OBJECTS): {
			if(num > 0) this->RecordStaticObjectsByRange<T>(apObject, begin, 0, num - 1);
		}
		break;

	case(OHT_RANGED_2_OCTET):
	case(OHT_RANGED_4_OCTET):
	case(OHT_RANGED_8_OCTET): {
			if(num > 0) {
				size_t max = num - 1;
				RangeInfo ri;
				const IRangeHeader* pHeader = reinterpret_cast<const IRangeHeader*>(arIter->GetHeader());
				pHeader->GetRange(*arIter, ri);

				if(ri.Start > max || ri.Stop > max || ri.Start > ri.Stop) this->mTempIIN.SetParameterError(true);
				else this->RecordStaticObjectsByRange<T>(apObject, begin, ri.Start, ri.Stop);
			}
			else this->mTempIIN.SetParameterError(true);
		}
		break;

	case(OHT_COUNT_1_OCTET):
	case(OHT_COUNT_2_OCTET):
	case(OHT_COUNT_4_OCTET): {
			if(num > 0) {
				size_t max = num - 1;
				size_t count = reinterpret_cast<const ICountHeader*>(arIter->GetHeader())->GetCount(*arIter);
				if(count > 0) {
					size_t start = 0;
					size_t stop = count - 1;

					if(start > max || stop > max || start > stop) this->mTempIIN.SetParameterError(true);
					else this->RecordStaticObjectsByRange<T>(apObject, begin, start, stop);
				}
				else this->mTempIIN.SetParameterError(true);
			}
			else this->mTempIIN.SetParameterError(true);
		}
		break;
	}
}

template <class T>
void ResponseContext::RecordStaticObjectsByRange(StreamObject<T>* apObject, typename StaticIterator<T>::Type start, size_t aStart, size_t aStop)
{	
	auto first = start + aStart;
	auto last = start + aStop;	
	
	ResponseKey key(RT_STATIC, this->mStaticWriteMap.size());
	WriteFunction func = [ = ](APDU & arAPDU) {
		return this->WriteStaticObjects<T>(apObject, first, last, key, arAPDU);
	};	
	this->mStaticWriteMap[key] = func;
}

template <class T>
bool ResponseContext::WriteStaticObjects(StreamObject<T>* apObject, typename StaticIterator<T>::Type aStart, typename StaticIterator<T>::Type aStop, ResponseKey aKey, APDU& arAPDU)
{
	size_t start = aStart->index;
	size_t stop = aStop->index;
	ObjectWriteIterator owi = arAPDU.WriteContiguous(apObject, start, stop);

	for(size_t i = start; i <= stop; ++i) {
		if(owi.IsEnd()) { // out of space in the fragment
			this->mStaticWriteMap[aKey] = [ = ](APDU & arAPDU) {
				return this->WriteStaticObjects<T>(apObject, aStart, aStop, aKey, arAPDU);
			};
			return false;
		}
		apObject->Write(*owi, aStart->value);
		++aStart; //increment the iterators
		++owi;
	}

	return true;
}

template <class T>
bool ResponseContext::LoadEvents(APDU& arAPDU, const typename EventIterator<T>::Type& aBegin, std::deque< EventRequest<T> >& arQueue)
{
	auto itr = aBegin;	
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
size_t ResponseContext::IterateIndexed(EventRequest<T>& arRequest, typename EventIterator<T>::Type& arIter, APDU& arAPDU)
{
	size_t numType = mpDB->NumType(T::MeasEnum);
	IndexedWriteIterator write = arAPDU.WriteIndexed(arRequest.pObj, arRequest.count, numType);

	for(size_t i = 0; i < arRequest.count; ++i) {
		if(write.IsEnd()) return i;										//that's all we can get into this fragment

		write.SetIndex(arIter->index);
		arRequest.pObj->Write(*write, arIter->value);					// do the write
		arIter->mWritten = true;										// flag it as written
		++arIter;														// advance the read iterator
		++write;														// advance the write iterator
	}

	return arRequest.count; // all requested events were written
}

template <class T>
size_t ResponseContext::CalcPossibleCTO(typename EventIterator<T>::Type aIter, size_t aMax)
{
	auto start = aIter->value.GetTime();

	size_t num = 0;
	while(num < aMax) {
		if((aIter->value.GetTime() - start) > openpal::UInt16::Max) break;
		++num;
		++aIter;
	}

	return num;
}

// T is the point info type
template <class T>
size_t ResponseContext::IterateCTO(const StreamObject<T>* apObj, size_t aCount, typename EventIterator<T>::Type& arIter, APDU& arAPDU)
{
	size_t numType = mpDB->NumType(T::MeasEnum);	

	auto start = arIter->value.GetTime();

	// first try to write a CTO object for the first value that we're pushing
	ObjectWriteIterator itr = arAPDU.WriteContiguous(Group51Var1::Inst(), 0, 0, QualifierCode::UINT8_CNT);
	if(itr.IsEnd()) return 0;
	else Group51Var1::Inst()->mTime.Set(*itr, start);

	// predetermine how many results you're going to be able to fit given the time differences
	size_t num = this->CalcPossibleCTO<T>(arIter, aCount);
	IndexedWriteIterator write = arAPDU.WriteIndexed(apObj, num, numType); //start the object write

	for(size_t i = 0; i < num; ++i) {
		if(write.IsEnd()) return i;										// that's all we can get into this fragment

		T tmp = arIter->value;											// make a copy and adjust the time
		tmp.SetTime(tmp.GetTime() - start);

		write.SetIndex(arIter->index);
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
