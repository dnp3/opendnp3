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
#ifndef __IIN_FIELD_H_
#define __IIN_FIELD_H_

#include <opendnp3/APDUConstants.h>
#include <cstdint>

namespace opendnp3
{

/** DNP3 two-byte IIN field.
*/
class IINField
{
public:

	IINField() : LSB(0), MSB(0) 
	{}

	IINField(uint8_t aLSB, uint8_t aMSB) : 	LSB(aLSB), MSB(aMSB) 
	{}

	#define MACRO_GET_BIT(value, mask) { return (mask&value) != 0; }

	// LSB
	bool GetAllStations() const				MACRO_GET_BIT(LSB, IIN_LSB_ALL_STATIONS)
	bool GetClass1Events() const			MACRO_GET_BIT(LSB, IIN_LSB_CLASS1_EVENTS)
	bool GetClass2Events() const			MACRO_GET_BIT(LSB, IIN_LSB_CLASS2_EVENTS)
	bool GetClass3Events() const			MACRO_GET_BIT(LSB, IIN_LSB_CLASS3_EVENTS)
	bool GetNeedTime() const				MACRO_GET_BIT(LSB, IIN_LSB_NEED_TIME)
	bool GetLocalControl() const			MACRO_GET_BIT(LSB, IIN_LSB_LOCAL_CONTROL)
	bool GetDeviceTrouble() const			MACRO_GET_BIT(LSB, IIN_LSB_DEVICE_TROUBLE)
	bool GetDeviceRestart() const			MACRO_GET_BIT(LSB, IIN_LSB_DEVICE_RESTART)

	// MSB
	bool GetFuncNotSupported() const		MACRO_GET_BIT(MSB, IIN_MSB_FUNC_NOT_SUPPORTED)
	bool GetObjectUnknown() const			MACRO_GET_BIT(MSB, IIN_MSB_OBJECT_UNKNOWN)
	bool GetParameterError() const			MACRO_GET_BIT(MSB, IIN_MSB_PARAM_ERROR)
	bool GetEventBufferOverflow() const		MACRO_GET_BIT(MSB, IIN_MSB_EVENT_BUFFER_OVERFLOW)
	bool GetAlreadyExecuting() const		MACRO_GET_BIT(MSB, IIN_MSB_ALREADY_EXECUTING)
	bool GetConfigurationCorrupt() const	MACRO_GET_BIT(MSB, IIN_MSB_CONFIG_CORRUPT)
	bool GetReserved1() const				MACRO_GET_BIT(MSB, IIN_MSB_RESERVED1)
	bool GetReserved2() const				MACRO_GET_BIT(MSB, IIN_MSB_RESERVED2)

	#define MACRO_SET_BIT(value, mask) { value = (aVal) ? (value|mask) : (value&(~mask)); }

	// LSB
	void SetAllStations(bool aVal)			MACRO_SET_BIT(LSB, IIN_LSB_ALL_STATIONS)
	void SetClass1Events(bool aVal)			MACRO_SET_BIT(LSB, IIN_LSB_CLASS1_EVENTS)
	void SetClass2Events(bool aVal)			MACRO_SET_BIT(LSB, IIN_LSB_CLASS2_EVENTS)
	void SetClass3Events(bool aVal)			MACRO_SET_BIT(LSB, IIN_LSB_CLASS3_EVENTS)
	void SetNeedTime(bool aVal)				MACRO_SET_BIT(LSB, IIN_LSB_NEED_TIME)
	void SetLocalControl(bool aVal)			MACRO_SET_BIT(LSB, IIN_LSB_LOCAL_CONTROL)
	void SetDeviceTrouble(bool aVal)		MACRO_SET_BIT(LSB, IIN_LSB_DEVICE_TROUBLE)
	void SetDeviceRestart(bool aVal)		MACRO_SET_BIT(LSB, IIN_LSB_DEVICE_RESTART)

	void SetFuncNotSupported(bool aVal)		MACRO_SET_BIT(MSB, IIN_MSB_FUNC_NOT_SUPPORTED)
	void SetObjectUnknown(bool aVal)		MACRO_SET_BIT(MSB, IIN_MSB_OBJECT_UNKNOWN)
	void SetParameterError(bool aVal)		MACRO_SET_BIT(MSB, IIN_MSB_PARAM_ERROR)
	void SetEventBufferOverflow(bool aVal)	MACRO_SET_BIT(MSB, IIN_MSB_EVENT_BUFFER_OVERFLOW)
	void SetAlreadyExecuting(bool aVal)		MACRO_SET_BIT(MSB, IIN_MSB_ALREADY_EXECUTING)
	void SetConfigurationCorrupt(bool aVal) MACRO_SET_BIT(MSB, IIN_MSB_CONFIG_CORRUPT)
	void SetReserved1(bool aVal)			MACRO_SET_BIT(MSB, IIN_MSB_RESERVED1)
	void SetReserved2(bool aVal)			MACRO_SET_BIT(MSB, IIN_MSB_RESERVED2)

	bool operator==(const IINField& arRHS) const;
	
	void Zero() 
	{
		LSB = MSB = 0;
	}

	void BitwiseOR(const IINField& aIIN) {
		LSB |= aIIN.LSB;
		MSB |= aIIN.MSB;
	}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string ToString() const;
#endif

	uint8_t LSB;
	uint8_t MSB;
};

}

#endif
