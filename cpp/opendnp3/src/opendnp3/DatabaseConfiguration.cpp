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

#include <opendnp3/DatabaseConfiguration.h>

namespace opendnp3
{

DatabaseConfiguration::DatabaseConfiguration(const DatabaseTemplate& aSizeInformation) :	
	binaryMetadata(aSizeInformation.numBinary),
	analogMetadata(aSizeInformation.numAnalog),
	counterMetadata(aSizeInformation.numCounter),
	frozenCounterMetadata(aSizeInformation.numFrozenCounter),
	numBinaryOutputStatus(aSizeInformation.numBinaryOutputStatus),
	numAnalogOutputStatus(aSizeInformation.numAnalogOutputStatus)
{}

DatabaseConfiguration::DatabaseConfiguration() :
	binaryMetadata(0),
	analogMetadata(0),
	counterMetadata(0),
	frozenCounterMetadata(0),
	numBinaryOutputStatus(0),
	numAnalogOutputStatus(0)
{}

DatabaseTemplate DatabaseConfiguration::GetTemplate() const
{
	DatabaseTemplate tmp(
		static_cast<uint32_t>(binaryMetadata.size()), 
		static_cast<uint32_t>(analogMetadata.size()), 
		static_cast<uint32_t>(counterMetadata.size()), 
		static_cast<uint32_t>(frozenCounterMetadata.size()), 
		numBinaryOutputStatus, 
		numAnalogOutputStatus);
	return tmp;
}

}
