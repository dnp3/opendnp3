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
#ifndef __APDU_CONSTANTS_H_
#define __APDU_CONSTANTS_H_


#include <string>

namespace apl
{
namespace dnp
{

enum AppControlMasks {
	ACM_FIR = 0x80,
	ACM_FIN = 0x40,
	ACM_CON = 0x20,
	ACM_UNS = 0x10,
	ACM_SEQ = 0x0F
};

enum IINIndices {
	IINI_ALL_STATIONS = 0,
	IINI_CLASS1_EVENTS,
	IINI_CLASS2_EVENTS,
	IINI_CLASS3_EVENTS,
	IINI_NEED_TIME,
	IINI_LOCAL_CONTROL,
	IINI_DEVICE_TROUBLE,
	IINI_DEVICE_RESTART,
	IINI_FUNC_NOT_SUPPORTED,
	IINI_OBJECT_UNKNOWN,
	IINI_PARAM_ERROR,
	IINI_EVENT_BUFFER_OVERFLOW,
	IINI_ALREADY_EXECUTING,
	IINI_CONFIG_CORRUPT,
	IINI_RESERVED1,
	IINI_RESERVED2 = 15
};

enum IINMasksLSB {
	IIN_LSB_ALL_STATIONS = 0x01,
	IIN_LSB_CLASS1_EVENTS = 0x02,
	IIN_LSB_CLASS2_EVENTS = 0x04,
	IIN_LSB_CLASS3_EVENTS = 0x08,
	IIN_LSB_NEED_TIME = 0x10,
	IIN_LSB_LOCAL_CONTROL = 0x20,
	IIN_LSB_DEVICE_TROUBLE = 0x40,
	IIN_LSB_DEVICE_RESTART = 0x80
};

enum IINMasksMSB {
	IIN_MSB_FUNC_NOT_SUPPORTED = 0x01,
	IIN_MSB_OBJECT_UNKNOWN = 0x02,
	IIN_MSB_PARAM_ERROR = 0x04,
	IIN_MSB_EVENT_BUFFER_OVERFLOW = 0x08,
	IIN_MSB_ALREADY_EXECUTING = 0x10,
	IIN_MSB_CONFIG_CORRUPT = 0x20,
	IIN_MSB_RESERVED1 = 0x40,
	IIN_MSB_RESERVED2 = 0x80
};
enum FunctionCodes {
	FC_CONFIRM = 0,
	FC_READ = 1,
	FC_WRITE = 2,
	FC_SELECT = 3,
	FC_OPERATE = 4,
	FC_DIRECT_OPERATE = 5,
	FC_DIRECT_OPERATE_NO_ACK = 6,

	FC_FREEZE = 7,
	FC_FREEZE_NO_ACK = 8,
	FC_FREEZE_CLEAR = 9,
	FC_FREEZE_CLEAR_NO_ACK = 10,
	FC_FREEZE_AT_TIME = 11,
	FC_FREEZE_AT_TIME_NO_ACK = 12,

	FC_COLD_RESTART = 13,
	FC_WARM_RESTART = 14,
	FC_INITIALIZE_DATA = 15,
	FC_INITIALIZE_APPLICATION = 16,
	FC_START_APPLICATION = 17,
	FC_STOP_APPLICATION = 18,
	FC_SAVE_CONFIGURATION = 19,
	FC_ENABLE_UNSOLICITED = 20,
	FC_DISABLE_UNSOLICITED = 21,
	FC_ASSIGN_CLASS = 22,
	FC_DELAY_MEASURE = 23,
	FC_RECORD_TIME = 24,

	FC_FILE_OPEN = 25,
	FC_FILE_CLOSE = 26,
	FC_FILE_DELETE = 27,
	FC_FILE_INFO = 28,
	FC_FILE_AUTHENTICATE = 29,
	FC_FILE_ABORT = 30,

	FC_PROPRIETARY_VTO_TRANSFER = 100,

	FC_RESPONSE = 129,
	FC_UNSOLICITED_RESPONSE = 130,

	FC_UNKNOWN = 255
};

/*
	Spec page 538.

	DNP permits combinations of Qualifier Code and Index Size Code that are not shown in
	Table 3-1. These combinations are explained in Figures 3-16 and 3-17. Note that
	combinations that do not appear in Table 3-1 are rarely used in DNP implementations,
	and only where special conditions warrant their inclusion. It is recommended that only
	combinations appearing in Table 3-1 should be used.
*/
enum QualifierCode {
	QC_1B_START_STOP = 0x00,
	QC_2B_START_STOP = 0x01,
	QC_4B_START_STOP = 0x02,

	QC_ALL_OBJ = 0x06,

	QC_1B_CNT = 0x07,
	QC_2B_CNT = 0x08,
	QC_4B_CNT = 0x09,

	QC_1B_CNT_1B_INDEX = 0x17,
	QC_2B_CNT_2B_INDEX = 0x28,
	QC_4B_CNT_4B_INDEX = 0x39,

	QC_1B_VCNT_1B_SIZE = 0x4B,
	QC_1B_VCNT_2B_SIZE = 0x5B,
	QC_1B_VCNT_4B_SIZE = 0x6B,

	QC_UNDEFINED = 0xFF

};

bool IsResponse(FunctionCodes aCode);
bool IsRequest(FunctionCodes aCode);

std::string ToString(FunctionCodes aCode);


}
}

#endif
