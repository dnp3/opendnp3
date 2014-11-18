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
#ifndef ASIODNP3_IMASTER_H
#define ASIODNP3_IMASTER_H

#include "IStack.h"
#include "StackActionHandler.h"

#include <opendnp3/app/ClassField.h>
#include <opendnp3/app/TimeAndInterval.h>
#include <opendnp3/master/MasterScan.h>
#include <opendnp3/master/ICommandProcessor.h>
#include <openpal/executor/TimeDuration.h>

namespace asiodnp3
{

/** 
* @desc Interface that represents a running master.
*/
class IMaster : public IStack
{
public:		

	virtual ~IMaster() {}

	/**
	* @return stack statistics counters
	*/
	virtual opendnp3::StackStatistics GetStackStatistics() = 0;

	/**
	* Add a recurring user-defined scan (via a lambda)
	* @ return A proxy class used to manipulate the scan
	*/
	virtual opendnp3::MasterScan AddScan(openpal::TimeDuration period, const std::function<void(opendnp3::HeaderWriter&)>& builder, opendnp3::ITaskCallback* pCallback = nullptr) = 0;

	/**
	* Add a scan that requests all objects using qualifier code 0x06
	* @ return A proxy class used to manipulate the scan
	*/
	virtual opendnp3::MasterScan AddAllObjectsScan(opendnp3::GroupVariationID gvId, openpal::TimeDuration period, opendnp3::ITaskCallback* pCallback = nullptr) = 0;

	/**
	* Add a class-based scan to the master
	* @return A proxy class used to manipulate the scan
	*/
	virtual opendnp3::MasterScan AddClassScan(const opendnp3::ClassField& field, openpal::TimeDuration period, opendnp3::ITaskCallback* pCallback = nullptr) = 0;

	/**
	* Add a start/stop (range) scan to the master
	* @return A proxy class used to manipulate the scan
	*/
	virtual opendnp3::MasterScan AddRangeScan(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, opendnp3::ITaskCallback* pCallback = nullptr) = 0;

	/**
	* Initiate a single user defined scan via a lambda	
	*/
	virtual void Scan(const std::function<void(opendnp3::HeaderWriter&)>& builder, opendnp3::ITaskCallback* pCallback = nullptr) = 0;

	/**
	* Initiate a single scan that requests all objects (0x06 qualifier code) for a certain group and variation
	*/
	virtual void ScanAllObjects(opendnp3::GroupVariationID gvId, opendnp3::ITaskCallback* pCallback = nullptr) = 0;

	/**
	* Initiate a single class-based scan	
	*/
	virtual void ScanClasses(const opendnp3::ClassField& field, opendnp3::ITaskCallback* pCallback = nullptr) = 0;

	/**
	* Initiate a single start/stop (range) scan
	*/
	virtual void ScanRange(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, opendnp3::ITaskCallback* pCallback = nullptr) = 0;

	/**
	* Write a time and interval object to a specific index
	*/
	virtual void Write(const opendnp3::TimeAndInterval& value, uint16_t index, opendnp3::ITaskCallback* pCallback = nullptr) = 0;

	/**
	* Get a command processor interface to execute controls on the master
	* @return Interface used to invoke commands
	*/
	virtual opendnp3::ICommandProcessor* GetCommandProcessor() = 0;
};

}

#endif

