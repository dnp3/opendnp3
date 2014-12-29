//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "Group13.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group13Var1 Group13Var1::Read(ReadBufferView& buffer)
{
	Group13Var1 obj;
	obj.value = (UInt8::Read(buffer) & 0x80) == 0x80;
	obj.status = CommandStatusFromType(UInt8::Read(buffer) & 0x7F);
	buffer.Advance(1);
	return obj;
}

void Group13Var1::Write(const Group13Var1& arg, openpal::WriteBufferView& buffer)
{
	UInt8::Write(buffer, (arg.value << 7) | CommandStatusToType(arg.status));
	buffer.Advance(1);
}

BinaryCommandEvent Group13Var1::ReadTarget(ReadBufferView& buff)
{
	auto gv = Group13Var1::Read(buff);
	return BinaryCommandEventFactory::From(gv.value, gv.status);
}

void Group13Var1::WriteTarget(const BinaryCommandEvent& value, openpal::WriteBufferView& buff)
{
	Group13Var1::Write(ConvertGroup13Var1::Apply(value), buff);
}

Group13Var2 Group13Var2::Read(ReadBufferView& buffer)
{
	Group13Var2 obj;
	obj.value = (UInt8::Read(buffer) & 0x80) == 0x80;
	obj.status = CommandStatusFromType(UInt8::Read(buffer) & 0x7F);
	buffer.Advance(1);
	obj.time = UInt48::Read(buffer);
	buffer.Advance(6);
	return obj;
}

void Group13Var2::Write(const Group13Var2& arg, openpal::WriteBufferView& buffer)
{
	UInt8::Write(buffer, (arg.value << 7) | CommandStatusToType(arg.status));
	buffer.Advance(1);
	UInt48::Write(buffer, arg.time);
	buffer.Advance(6);
}

BinaryCommandEvent Group13Var2::ReadTarget(ReadBufferView& buff)
{
	auto gv = Group13Var2::Read(buff);
	return BinaryCommandEventFactory::From(gv.value, gv.status, gv.time);
}

void Group13Var2::WriteTarget(const BinaryCommandEvent& value, openpal::WriteBufferView& buff)
{
	Group13Var2::Write(ConvertGroup13Var2::Apply(value), buff);
}

}
