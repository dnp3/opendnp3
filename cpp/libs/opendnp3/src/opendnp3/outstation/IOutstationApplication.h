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

#ifndef OPENDNP3_IOUTSTATIONAPPLICATION_H
#define OPENDNP3_IOUTSTATIONAPPLICATION_H

#include <openpal/executor/UTCTimestamp.h>

#include "opendnp3/gen/PointClass.h"
#include "opendnp3/gen/RestartMode.h"
#include "opendnp3/gen/AssignClassType.h"

#include "opendnp3/link/ILinkListener.h"

#include "opendnp3/app/IndexedValue.h"
#include "opendnp3/app/IterableBuffer.h"
#include "opendnp3/app/TimeAndInterval.h"

#include "opendnp3/outstation/ApplicationIIN.h"


namespace opendnp3
{

///
/// @summary Interface for all outstation application callback info except for control requests
///
class IOutstationApplication : public opendnp3::ILinkListener
{
	public:

	/// Queries whether the the outstation supports absolute time writes
	/// If this function returns false, WriteAbsoluteTime will never be called
	/// and the outstation will return IIN 2.1 (FUNC_NOT_SUPPORTED)
	virtual bool SupportsWriteAbsoluteTime() = 0;

	/// Write the time to outstation, only called if SupportsWriteAbsoluteTime return true
	/// @return boolean value indicating if the time value supplied was accepted. Returning
	/// false will cause the outstation to set IIN 2.3 (PARAM_ERROR) in its response.
	/// The outstation should clear its NEED_TIME field when handling this response
	virtual bool WriteAbsoluteTime(const openpal::UTCTimestamp& timestamp) = 0;

	/// Queries whether the outstation supports the writing of TimeAndInterval
	/// If this function returns false, WriteTimeAndInterval will never be called
	/// and the outstation will return IIN 2.1 (FUNC_NOT_SUPPORTED) when it receives this request
	virtual bool SupportsWriteTimeAndInterval() = 0;

	/// Write an indexed collection of TimeAndInterval values. Only called if SupportsWriteTimeAndInterval returns true.
	/// The outstation application code is reponsible for updating TimeAndInterval values in the database if this behavior
	/// is desired
	/// @return boolean value indicating if the values supplied were accepted. Returning
	/// false will cause the outstation to set IIN 2.3 (PARAM_ERROR) in its response.	
	virtual bool WriteTimeAndInterval(const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas) = 0;

	/// True if the outstation supports the assign class function code
	/// If this function returns false, the assign class callbacks will never be called
	/// and the outstation will return IIN 2.1 (FUNC_NOT_SUPPORTED) when it receives this function code
	virtual bool SupportsAssignClass() = 0;

	/// Called if SupportsAssignClass returns true
	/// The type and range are pre-validated against the outstation's database
	/// and class assignments are automatically applied internally.
	/// This callback allows user code to persist the changes to non-volatile memory	
	virtual void RecordClassAssignment(AssignClassType type, PointClass clazz, uint16_t start, uint16_t stop) = 0;

	/// Returns the application-controlled IIN field
	virtual ApplicationIIN GetApplicationIIN() const = 0;

	/// Query the outstation for the cold restart mode it supports
	virtual RestartMode ColdRestartSupport() const = 0;

	/// Query the outstation for the warm restart mode it supports
	virtual RestartMode WarmRestartSupport() const = 0;

	/// The outstation should perform a complete restart.
	/// See the DNP3 specification for a complete descripton of normal behavior
	/// @return number of seconds or milliseconds until restart is complete. The value
	/// is interpreted based on the Restart Mode returned from ColdRestartSupport()
	virtual uint16_t ColdRestart() = 0;

	/// The outstation should perform a partial restart of only the DNP3 application.
	/// See the DNP3 specification for a complete descripton of normal behavior
	/// @return number of seconds or milliseconds until restart is complete. The value
	/// is interpreted based on the Restart Mode returned from WarmRestartSupport()
	virtual uint16_t WarmRestart() = 0;

	
	virtual ~IOutstationApplication() {}	
};

class DefaultOutstationApplication : public IOutstationApplication
{
	public:

	static IOutstationApplication& DefaultOutstationApplication::Instance();

	virtual bool SupportsWriteAbsoluteTime() override { return false; }

	virtual bool WriteAbsoluteTime(const openpal::UTCTimestamp& timestamp) override { return false; }
	
	virtual bool SupportsWriteTimeAndInterval() override { return false; }
	
	virtual bool WriteTimeAndInterval(const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas) override { return false; }

	virtual bool SupportsAssignClass() override { return false; }
	
	virtual void RecordClassAssignment(AssignClassType type, PointClass clazz, uint16_t start, uint16_t stop) override { }	

	virtual ApplicationIIN GetApplicationIIN() const override { return ApplicationIIN(); };

	virtual RestartMode ColdRestartSupport() const override { return RestartMode::UNSUPPORTED; }
	
	virtual RestartMode WarmRestartSupport() const override { return RestartMode::UNSUPPORTED; }
	
	virtual uint16_t ColdRestart() override { return 65535; }
	
	virtual uint16_t WarmRestart() override { return 65535; }

	virtual void OnStateChange(opendnp3::LinkStatus value) override {}
		
private:

	static DefaultOutstationApplication m_instance;
};

}

#endif
