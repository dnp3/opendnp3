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

#ifndef OPENDNP3_EVENTWRITER_H
#define OPENDNP3_EVENTWRITER_H

#include <openpal/util/Uncopyable.h>
#include <openpal/container/LinkedList.h>

#include "opendnp3/app/HeaderWriter.h"
#include "opendnp3/outstation/SOERecord.h"
#include "opendnp3/outstation/IEventRecorder.h"


namespace opendnp3
{

class EventWriter : openpal::PureStatic
{
	public:	

	static bool Write(HeaderWriter& writer, IEventRecorder& recorder, openpal::LinkedListIterator<SOERecord> iterator);
	
	private:	

	class Result
	{
		public:

			Result(bool isFragmentFull_, openpal::LinkedListIterator<SOERecord> location_) : isFragmentFull(isFragmentFull_), location(location_)
		{}

		bool isFragmentFull;
		openpal::LinkedListIterator<SOERecord> location;
		

		private:
		
		Result() = delete;
	};

	typedef Result(*EventLoadFun)(HeaderWriter& writer, IEventRecorder& recorder, openpal::LinkedListIterator<SOERecord> iterator);

	static EventLoadFun GetLoadFunction(const SOERecord& record);

	inline static bool IsWritable(const SOERecord& record) { return record.selected && !record.written; }
};

}

#endif
