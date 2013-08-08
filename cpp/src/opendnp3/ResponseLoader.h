
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
#ifndef __RESPONSE_LOADER_H_
#define __RESPONSE_LOADER_H_

#include <openpal/Types.h>
#include <opendnp3/IDataObserver.h>
#include <opendnp3/ObjectInterfaces.h>
#include <opendnp3/Logger.h>
#include <opendnp3/Visibility.h>

#include "Loggable.h"
#include "LoggableMacros.h"
#include "CTOHistory.h"
#include "ObjectReadIterator.h"
#include "VtoReader.h"

namespace opendnp3
{

class HeaderReadIterator;

/**
 * Dedicated class for processing response data in the master.
 */
class DLL_LOCAL ResponseLoader : Loggable
{
public:
	/**
	 * Creates a new ResponseLoader instance.
	 *
	 * @param log			the Logger that the loader should use for
	 * 						message reporting
	 * @param apPublisher	the IDataObserver for any responses that match
	 * @param apVtoReader	the VtoReader for any responses that match
	 *
	 * @return				a new ResponseLoader instance
	 */
	ResponseLoader(Logger* log,
	               IDataObserver* apPublisher,
	               VtoReader* apVtoReader);

	/**
	 * Processes a DNP3 object received by the Master.  The real heavy
	 * lifting is done in ResponseLoader::ProcessData().
	 *
	 * @param itr			the header iterator that provides access to
	 * 						the group, variation, data, etc.
	 */
	void Process(HeaderReadIterator& itr);

private:

	/**
	 * Processes the data field of a DNP3 object received by the Master.
	 * This function multiplexes objects to the Read(), ReadBitfield(), or
	 * ReadCTO() as is appropriate for the group/variation tuple.
	 *
	 * @param itr		the header iterator that provides access to the
	 * 					group, variation, data, etc.
	 * @param aGrp		the DNP3 group object id
	 * @param aVar		the DNP3 variation object id
	 */
	void ProcessData(HeaderReadIterator& itr, int aGrp, int aVar);

	/**
	* Processes size by variation objects received by the master.  The
	* variation id ('aVar') is the size of the data object.
	*
	* @param itr		the header iterator that provides access to the
	* 					group, variation, data, etc.
	* @param aGrp		the DNP3 group object id
	* @param aVar		the DNP3 variation id
	*/
	void ProcessSizeByVariation(HeaderReadIterator& itr, int aGrp, int aVar);

	template <class T>
	void Read(HeaderReadIterator& arIter, StreamObject<T>* apObj);

	template <class T>
	void ReadBitfield(HeaderReadIterator& arHeader);

	template <class T>
	void ReadCTO(HeaderReadIterator& arIter);

	/**
	 * Convert an incoming data stream for DNP3 Object Groups 112 or 113
	 * into a VtoData object and pass the object to the user application
	 * via the VtoReader instance.
	 *
	 * @param arIter	the header iterator that provides access to the
	 * 					group, variation, data, etc.
	 * @param apObj		the pointer to the DNP3 object instance that can
	 * 					read/write the byte stream
	 */
	void ReadVto(HeaderReadIterator& arIter, SizeByVariationObject* apObj);

	IDataObserver* mpPublisher;

	/**
	 * A pointer to the VtoReader instance that will accept the VtoData
	 * processed by this ResponseLoader.
	 */
	VtoReader* mpVtoReader;

	Transaction mTransaction;

	CTOHistory mCTO;
};

template <class T>
void ResponseLoader::ReadCTO(HeaderReadIterator& arIter)
{
	ObjectReadIterator i = arIter.BeginRead();

	if (i.Count() != 1) {
		LOG_BLOCK(LEV_WARNING, "Invalid number of CTO objects");
		return;
	}

	openpal::millis_t t = T::Inst()->mTime.Get(*i);
	mCTO.SetCTO(t);
}

template <class T>
void ResponseLoader::Read(HeaderReadIterator& arIter, StreamObject<T>* apObj)
{
	openpal::millis_t t = 0; //base time

	if (apObj->UseCTO() && !mCTO.GetCTO(t)) {
		LOG_BLOCK(LEV_ERROR,
		          "No CTO for relative time type " << apObj->Name());
		return;
	}

	ObjectReadIterator obj = arIter.BeginRead();
	LOG_BLOCK(LEV_INTERPRET,
	          "Converting " << obj.Count() << " " << apObj->Name() << " "
	          "To " << typeid(T).name());

	for ( ; !obj.IsEnd(); ++obj) {
		size_t index = obj->Index();
		T value = apObj->Read(*obj);

		/* Make sure the value has time information */
		if (apObj->UseCTO()) {
			value.SetTime(t + value.GetTime());
		}

		/* Make sure the value has quality information */
		if (!apObj->HasQuality()) {
			value.SetQuality(T::ONLINE);
		}

		mpPublisher->Update(value, index);
	}
}

template <class T>
void ResponseLoader::ReadBitfield(HeaderReadIterator& arIter)
{
	Binary b; b.SetQuality(Binary::ONLINE);

	ObjectReadIterator obj = arIter.BeginRead();
	LOG_BLOCK(LEV_INTERPRET,
	          "Converting " << obj.Count() << " " << T::Inst()->Name() << " "
	          "To " << typeid(b).name());

	for (; !obj.IsEnd(); ++obj) {
		bool val = BitfieldObject::StaticRead(*obj, obj->Start(), obj->Index());
		b.SetValue(val);
		mpPublisher->Update(b, obj->Index());
	}
}

}

/* vim: set ts=4 sw=4: */

#endif

