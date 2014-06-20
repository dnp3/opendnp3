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
#ifndef __MASTER_PARAMS_H_
#define __MASTER_PARAMS_H_

#include <openpal/executor/TimeDuration.h>

namespace opendnp3
{

enum class TimeSyncMode
{
	None,
	SerialTimeSync
};

/**
Configuration information for the dnp3 master
*/
struct MasterParams
{
	/// Default constructor
	MasterParams();

	/// Maximum fragment size to use for requests
	uint32_t fragSize;

	/// Application layer response timeout
	openpal::TimeDuration responseTimeout;

	/// If true, the master will do time syncs when it sees the time IIN bit from the outstation
	TimeSyncMode timeSyncMode;

	/// If true, the master will disable unsol on startup for all 3 classes
	bool disableUnsolOnStartup;	

	///	Bitwise mask used determine which classes are enabled for unsol, if 0 unsol is not enabled
	int unsolClassMask;

	/// Period for integrity scans (class 1/2/3/0), negative for non periodic
	openpal::TimeDuration integrityPeriod;

	/// Which classes should be requested in a startup integrity scan, defaults to 3/2/1/0, 0 == no scan
	int startupIntergrityClassMask;

	/// Time delay beforce retrying a failed task
	openpal::TimeDuration taskRetryPeriod;
};

}

#endif

