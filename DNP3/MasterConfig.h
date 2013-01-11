//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __MASTER_CONFIG_H_
#define __MASTER_CONFIG_H_

#include "DNPConstants.h"
#include "PointClass.h"
#include "MasterConfigTypes.h"
#include "IStackObserver.h"

#include <vector>

namespace apl
{
namespace dnp
{


/**
Configuration information for the dnp3 master
*/
struct MasterConfig {
	// Default constructor
	MasterConfig() :
		FragSize(DEFAULT_FRAG_SIZE),
		VtoWriterQueueSize(DEFAULT_VTO_WRITER_QUEUE_SIZE),
		UseNonStandardVtoFunction(false),
		AllowTimeSync(true),
		DoUnsolOnStartup(false),
		EnableUnsol(true),
		UnsolClassMask(PC_ALL_EVENTS),
		IntegrityRate(5000),
		TaskRetryRate(5000),
		mpObserver(NULL)
	{}

	/** Adds a periodic exception scan to the configuration

		@param aClassMask	Bitwise mask representing the classes to scan
		@param aPeriod		Period of the scan in milliseconds
	*/
	void AddExceptionScan(int aClassMask, millis_t aPeriod) {
		ExceptionScan ex = { aClassMask, aPeriod };
		mScans.push_back(ex);
	}

	// Maximum fragment size to use for requests
	size_t FragSize;

	// The number of objects to store in the VtoWriter queue.
	size_t VtoWriterQueueSize;

	// Using FC_WRITE is a problem with vto because the spec won't allow it to retry
	bool UseNonStandardVtoFunction;

	// If true, the master will do time syncs when it sees the time IIN bit from the slave
	bool AllowTimeSync;

	// If true, the master will enable/disable unsol on startup
	bool DoUnsolOnStartup;

	// If DoUnsolOnStartup == true, the master will use this bit to decide wether to enable (true) or disable (false)
	bool EnableUnsol;

	//	Bitwise mask used determine which classes are enabled/disabled for unsol
	int UnsolClassMask;

	// Period for integrity scans (class 0), -1 for non periodic
	millis_t IntegrityRate;

	// Time delay between task retries
	millis_t TaskRetryRate;

	// vector that holds exception scans
	std::vector<ExceptionScan> mScans;

	// Observer class for notifying outside world what the master is doing
	IStackObserver* mpObserver;
};

}
}

#endif
