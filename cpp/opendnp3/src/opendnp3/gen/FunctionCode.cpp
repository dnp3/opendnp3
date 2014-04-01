//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// Copyright 2013 Automatak LLC
//
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
//
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "FunctionCode.h"

namespace opendnp3
{

uint8_t FunctionCodeToType(FunctionCode arg)
{
	return static_cast<uint8_t>(arg);
}
FunctionCode FunctionCodeFromType(uint8_t arg)
{
	switch(arg)
	{
	case(0x0):
		return FunctionCode::CONFIRM;
	case(0x1):
		return FunctionCode::READ;
	case(0x2):
		return FunctionCode::WRITE;
	case(0x3):
		return FunctionCode::SELECT;
	case(0x4):
		return FunctionCode::OPERATE;
	case(0x5):
		return FunctionCode::DIRECT_OPERATE;
	case(0x6):
		return FunctionCode::DIRECT_OPERATE_NO_ACK;
	case(0x7):
		return FunctionCode::FREEZE;
	case(0x8):
		return FunctionCode::FREEZE_NO_ACK;
	case(0x9):
		return FunctionCode::FREEZE_CLEAR;
	case(0xA):
		return FunctionCode::FREEZE_CLEAR_NO_ACK;
	case(0xB):
		return FunctionCode::FREEZE_AT_TIME;
	case(0xC):
		return FunctionCode::FREEZE_AT_TIME_NO_ACK;
	case(0xD):
		return FunctionCode::COLD_RESTART;
	case(0xE):
		return FunctionCode::WARM_RESTART;
	case(0xF):
		return FunctionCode::INITIALIZE_DATA;
	case(0x10):
		return FunctionCode::INITIALIZE_APPLICATION;
	case(0x11):
		return FunctionCode::START_APPLICATION;
	case(0x12):
		return FunctionCode::STOP_APPLICATION;
	case(0x13):
		return FunctionCode::SAVE_CONFIGURATION;
	case(0x14):
		return FunctionCode::ENABLE_UNSOLICITED;
	case(0x15):
		return FunctionCode::DISABLE_UNSOLICITED;
	case(0x16):
		return FunctionCode::ASSIGN_CLASS;
	case(0x17):
		return FunctionCode::DELAY_MEASURE;
	case(0x18):
		return FunctionCode::RECORD_TIME;
	case(0x19):
		return FunctionCode::FILE_OPEN;
	case(0x1A):
		return FunctionCode::FILE_CLOSE;
	case(0x1B):
		return FunctionCode::FILE_DELETE;
	case(0x1C):
		return FunctionCode::FILE_INFO;
	case(0x1D):
		return FunctionCode::FILE_AUTHENTICATE;
	case(0x1E):
		return FunctionCode::FILE_ABORT;
	case(0x81):
		return FunctionCode::RESPONSE;
	case(0x82):
		return FunctionCode::UNSOLICITED_RESPONSE;
	case(0xFF):
		return FunctionCode::UNKNOWN;
	}
	return FunctionCode::UNKNOWN;
}
std::string FunctionCodeToString(FunctionCode arg)
{
	switch(arg)
	{
	case(FunctionCode::CONFIRM):
		return "CONFIRM";
	case(FunctionCode::READ):
		return "READ";
	case(FunctionCode::WRITE):
		return "WRITE";
	case(FunctionCode::SELECT):
		return "SELECT";
	case(FunctionCode::OPERATE):
		return "OPERATE";
	case(FunctionCode::DIRECT_OPERATE):
		return "DIRECT_OPERATE";
	case(FunctionCode::DIRECT_OPERATE_NO_ACK):
		return "DIRECT_OPERATE_NO_ACK";
	case(FunctionCode::FREEZE):
		return "FREEZE";
	case(FunctionCode::FREEZE_NO_ACK):
		return "FREEZE_NO_ACK";
	case(FunctionCode::FREEZE_CLEAR):
		return "FREEZE_CLEAR";
	case(FunctionCode::FREEZE_CLEAR_NO_ACK):
		return "FREEZE_CLEAR_NO_ACK";
	case(FunctionCode::FREEZE_AT_TIME):
		return "FREEZE_AT_TIME";
	case(FunctionCode::FREEZE_AT_TIME_NO_ACK):
		return "FREEZE_AT_TIME_NO_ACK";
	case(FunctionCode::COLD_RESTART):
		return "COLD_RESTART";
	case(FunctionCode::WARM_RESTART):
		return "WARM_RESTART";
	case(FunctionCode::INITIALIZE_DATA):
		return "INITIALIZE_DATA";
	case(FunctionCode::INITIALIZE_APPLICATION):
		return "INITIALIZE_APPLICATION";
	case(FunctionCode::START_APPLICATION):
		return "START_APPLICATION";
	case(FunctionCode::STOP_APPLICATION):
		return "STOP_APPLICATION";
	case(FunctionCode::SAVE_CONFIGURATION):
		return "SAVE_CONFIGURATION";
	case(FunctionCode::ENABLE_UNSOLICITED):
		return "ENABLE_UNSOLICITED";
	case(FunctionCode::DISABLE_UNSOLICITED):
		return "DISABLE_UNSOLICITED";
	case(FunctionCode::ASSIGN_CLASS):
		return "ASSIGN_CLASS";
	case(FunctionCode::DELAY_MEASURE):
		return "DELAY_MEASURE";
	case(FunctionCode::RECORD_TIME):
		return "RECORD_TIME";
	case(FunctionCode::FILE_OPEN):
		return "FILE_OPEN";
	case(FunctionCode::FILE_CLOSE):
		return "FILE_CLOSE";
	case(FunctionCode::FILE_DELETE):
		return "FILE_DELETE";
	case(FunctionCode::FILE_INFO):
		return "FILE_INFO";
	case(FunctionCode::FILE_AUTHENTICATE):
		return "FILE_AUTHENTICATE";
	case(FunctionCode::FILE_ABORT):
		return "FILE_ABORT";
	case(FunctionCode::RESPONSE):
		return "RESPONSE";
	case(FunctionCode::UNSOLICITED_RESPONSE):
		return "UNSOLICITED_RESPONSE";
	case(FunctionCode::UNKNOWN):
		return "UNKNOWN";
	}
	return "UNKNOWN";
}

}
