
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
#ifndef __APP_HEADER_H_
#define __APP_HEADER_H_


#include <APL/Types.h>
#include <APL/Singleton.h>
#include <stddef.h>

#include "APDUConstants.h"

namespace apl
{
namespace dnp
{

FunctionCodes IntToFunction(int aFunction);

/** Represents the first byte in every APDU
*/
struct AppControlField {
	AppControlField() {};
	AppControlField(bool aFIR, bool aFIN, bool aCON, bool aUNS, uint8_t aSEQ = 0) :
		FIR(aFIR),
		FIN(aFIN),
		CON(aCON),
		UNS(aUNS),
		SEQ(aSEQ) {}

	bool FIR;
	bool FIN;
	bool CON;
	bool UNS;
	uint8_t  SEQ;
};

/** DNP3 two-byte IIN field.
*/
class IINField
{
public:

	IINField(uint8_t aLSB = 0, uint8_t aMSB = 0) : 	mLSB(aLSB), mMSB(aMSB) {

	}

#define MACRO_GET_BIT(value, mask) const { return (mask&value) != 0; }

	// LSB
	bool GetAllStations()			MACRO_GET_BIT(mLSB, IIN_LSB_ALL_STATIONS)
	bool GetClass1Events()			MACRO_GET_BIT(mLSB, IIN_LSB_CLASS1_EVENTS)
	bool GetClass2Events()			MACRO_GET_BIT(mLSB, IIN_LSB_CLASS2_EVENTS)
	bool GetClass3Events()			MACRO_GET_BIT(mLSB, IIN_LSB_CLASS3_EVENTS)
	bool GetNeedTime()				MACRO_GET_BIT(mLSB, IIN_LSB_NEED_TIME)
	bool GetLocalControl()			MACRO_GET_BIT(mLSB, IIN_LSB_LOCAL_CONTROL)
	bool GetDeviceTrouble()			MACRO_GET_BIT(mLSB, IIN_LSB_DEVICE_TROUBLE)
	bool GetDeviceRestart()			MACRO_GET_BIT(mLSB, IIN_LSB_DEVICE_RESTART)

	// MSB
	bool GetFuncNotSupported()		MACRO_GET_BIT(mMSB, IIN_MSB_FUNC_NOT_SUPPORTED)
	bool GetObjectUnknown()			MACRO_GET_BIT(mMSB, IIN_MSB_OBJECT_UNKNOWN)
	bool GetParameterError()		MACRO_GET_BIT(mMSB, IIN_MSB_PARAM_ERROR)
	bool GetEventBufferOverflow()	MACRO_GET_BIT(mMSB, IIN_MSB_EVENT_BUFFER_OVERFLOW)
	bool GetAlreadyExecuting()		MACRO_GET_BIT(mMSB, IIN_MSB_ALREADY_EXECUTING)
	bool GetConfigurationCorrupt()	MACRO_GET_BIT(mMSB, IIN_MSB_CONFIG_CORRUPT)
	bool GetReserved1()				MACRO_GET_BIT(mMSB, IIN_MSB_RESERVED1)
	bool GetReserved2()				MACRO_GET_BIT(mMSB, IIN_MSB_RESERVED2)

#define MACRO_SET_BIT(value, mask) { value = (aVal) ? (value|mask) : (value&(~mask)); }

	// LSB
	void SetAllStations(bool aVal)			MACRO_SET_BIT(mLSB, IIN_LSB_ALL_STATIONS)
	void SetClass1Events(bool aVal)			MACRO_SET_BIT(mLSB, IIN_LSB_CLASS1_EVENTS)
	void SetClass2Events(bool aVal)			MACRO_SET_BIT(mLSB, IIN_LSB_CLASS2_EVENTS)
	void SetClass3Events(bool aVal)			MACRO_SET_BIT(mLSB, IIN_LSB_CLASS3_EVENTS)
	void SetNeedTime(bool aVal)				MACRO_SET_BIT(mLSB, IIN_LSB_NEED_TIME)
	void SetLocalControl(bool aVal)			MACRO_SET_BIT(mLSB, IIN_LSB_LOCAL_CONTROL)
	void SetDeviceTrouble(bool aVal)		MACRO_SET_BIT(mLSB, IIN_LSB_DEVICE_TROUBLE)
	void SetDeviceRestart(bool aVal)		MACRO_SET_BIT(mLSB, IIN_LSB_DEVICE_RESTART)

	void SetFuncNotSupported(bool aVal)		MACRO_SET_BIT(mMSB, IIN_MSB_FUNC_NOT_SUPPORTED)
	void SetObjectUnknown(bool aVal)		MACRO_SET_BIT(mMSB, IIN_MSB_OBJECT_UNKNOWN)
	void SetParameterError(bool aVal)		MACRO_SET_BIT(mMSB, IIN_MSB_PARAM_ERROR)
	void SetEventBufferOverflow(bool aVal)	MACRO_SET_BIT(mMSB, IIN_MSB_EVENT_BUFFER_OVERFLOW)
	void SetAlreadyExecuting(bool aVal)		MACRO_SET_BIT(mMSB, IIN_MSB_ALREADY_EXECUTING)
	void SetConfigurationCorrupt(bool aVal) MACRO_SET_BIT(mMSB, IIN_MSB_CONFIG_CORRUPT)
	void SetReserved1(bool aVal)			MACRO_SET_BIT(mMSB, IIN_MSB_RESERVED1)
	void SetReserved2(bool aVal)			MACRO_SET_BIT(mMSB, IIN_MSB_RESERVED2)

	bool operator==(const IINField& arRHS) const;

	uint8_t GetLSB() const {
		return mLSB;
	}
	uint8_t GetMSB() const {
		return mMSB;
	}
	void SetLSB(uint8_t aLSB) {
		mLSB = aLSB;
	}
	void SetMSB(uint8_t aMSB) {
		mMSB = aMSB;
	}

	void Zero() {
		mLSB = mMSB = 0;
	}

	void BitwiseOR(const IINField& arIIN) {
		mLSB |= arIIN.mLSB;
		mMSB |= arIIN.mMSB;
	}

	std::string ToString() const;

private:
	uint8_t mLSB;
	uint8_t mMSB;
};

enum AppHeaderTypes {
	AHT_RESPONSE,
	AHT_REQUEST
};

class IAppHeader
{
public:
	virtual ~IAppHeader() {}
	virtual AppHeaderTypes GetType() const = 0;
	virtual size_t GetSize() const = 0;

	void SetControl(uint8_t* apStart, const AppControlField& arControl) const;
	AppControlField GetControl(const uint8_t* apStart) const;

	void SetFunction(uint8_t* apStart, FunctionCodes aCode) const {
		*(++apStart) = aCode;
	}
	FunctionCodes GetFunction(const uint8_t* apStart) const {
		return IntToFunction(*(++apStart));
	}

};

class RequestHeader : public IAppHeader
{
	MACRO_SINGLETON_INSTANCE(RequestHeader)

	AppHeaderTypes GetType() const {
		return AHT_REQUEST;
	}
	size_t GetSize() const {
		return 2;
	}
};

class ResponseHeader : public IAppHeader
{
	MACRO_SINGLETON_INSTANCE(ResponseHeader)

	AppHeaderTypes GetType() const {
		return AHT_RESPONSE;
	}
	size_t GetSize() const {
		return 4;
	}

	void SetIIN(uint8_t* apStart, const IINField& arIIN) const;
	IINField GetIIN(const uint8_t* apStart) const;
};

}
}

#endif
