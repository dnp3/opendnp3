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

#include <opendnp3/app/APDUParser.h>

namespace opendnp3 {

class ResultSet
{
	public:

		uint64_t numOK;
		uint64_t numNotEnoughDataForHeader;
		uint64_t numNotEnoughDataForRange;
		uint64_t numNotEnoughDataForObjects;
		uint64_t numUnReasonableObjectCount;
		uint64_t numUnknownObject;
		uint64_t numUnknownQualifier;
		uint64_t numIllegalObjectQualifier;
		uint64_t numBadStartStop;

		ResultSet(): 
			numOK(0),
			numNotEnoughDataForHeader(0),
			numNotEnoughDataForRange(0),
			numNotEnoughDataForObjects(0),
			numUnReasonableObjectCount(0),
			numUnknownObject(0),
			numUnknownQualifier(0),
			numIllegalObjectQualifier(0),
			numBadStartStop(0)
		{}

		void Merge(ResultSet rhs)
		{
			numOK += rhs.numOK;
			numNotEnoughDataForHeader += rhs.numNotEnoughDataForHeader;
			numNotEnoughDataForRange += rhs.numNotEnoughDataForRange;
			numNotEnoughDataForObjects += rhs.numNotEnoughDataForObjects;
			numUnReasonableObjectCount += rhs.numUnReasonableObjectCount;
			numUnknownObject += rhs.numUnknownObject;
			numUnknownQualifier += rhs.numUnknownQualifier;
			numIllegalObjectQualifier += rhs.numIllegalObjectQualifier;
			numBadStartStop += rhs.numBadStartStop;
		}

		uint64_t Sum()
		{
			return numOK +
			numNotEnoughDataForHeader +
			numNotEnoughDataForRange +
			numNotEnoughDataForObjects +
			numUnReasonableObjectCount +
			numUnknownObject +
			numUnknownQualifier +
			numIllegalObjectQualifier +
			numBadStartStop;
		}


		void Update(APDUParser::Result result)
		{
			switch(result)
			{
				case(APDUParser::Result::OK):
					++numOK;
					break;
				case(APDUParser::Result::NOT_ENOUGH_DATA_FOR_HEADER):
					++numNotEnoughDataForHeader;
					break;
				case(APDUParser::Result::NOT_ENOUGH_DATA_FOR_RANGE):
					++numNotEnoughDataForRange;
					break;
				case(APDUParser::Result::NOT_ENOUGH_DATA_FOR_OBJECTS):
					++numNotEnoughDataForObjects;
					break;
				case(APDUParser::Result::UNREASONABLE_OBJECT_COUNT):
					++numUnReasonableObjectCount;
					break;
				case(APDUParser::Result::UNKNOWN_OBJECT):
					++numUnknownObject;
					break;
				case(APDUParser::Result::UNKNOWN_QUALIFIER):
					++numUnknownQualifier;
					break;
				case(APDUParser::Result::ILLEGAL_OBJECT_QUALIFIER):
					++numIllegalObjectQualifier;
					break;
				case(APDUParser::Result::BAD_START_STOP):
					++numBadStartStop;
					break;
				default:
					break;
			}
		}
};

}
