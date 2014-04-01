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

#include "Group4.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3
{

const GroupVariationID  Group4Var1::ID(4, 1);

Group4Var1 Group4Var1::Read(ReadOnlyBuffer& buffer)
{
	Group4Var1 obj;
	obj.flags = UInt8::Read(buffer);
	buffer.Advance(1);
	return obj;
}

void Group4Var1::Write(const Group4Var1& arg, openpal::WriteBuffer& buffer)
{
	UInt8::Write(buffer, arg.flags);
	buffer.Advance(1);
}

Group4Var1Serializer Group4Var1Serializer::mInstance;

DoubleBitBinary Group4Var1Serializer::Read(ReadOnlyBuffer& buff) const
{
	auto gv = Group4Var1::Read(buff);
	return DoubleBitBinaryFactory::From(gv.flags);
}

void Group4Var1Serializer::Write(const DoubleBitBinary& value, openpal::WriteBuffer& buff) const
{
	Group4Var1::Write(ConvertGroup4Var1::Apply(value), buff);
}


const GroupVariationID  Group4Var2::ID(4, 2);

Group4Var2 Group4Var2::Read(ReadOnlyBuffer& buffer)
{
	Group4Var2 obj;
	obj.flags = UInt8::Read(buffer);
	buffer.Advance(1);
	obj.time = UInt48::Read(buffer);
	buffer.Advance(6);
	return obj;
}

void Group4Var2::Write(const Group4Var2& arg, openpal::WriteBuffer& buffer)
{
	UInt8::Write(buffer, arg.flags);
	buffer.Advance(1);
	UInt48::Write(buffer, arg.time);
	buffer.Advance(6);
}

Group4Var2Serializer Group4Var2Serializer::mInstance;

DoubleBitBinary Group4Var2Serializer::Read(ReadOnlyBuffer& buff) const
{
	auto gv = Group4Var2::Read(buff);
	return DoubleBitBinaryFactory::From(gv.flags, gv.time);
}

void Group4Var2Serializer::Write(const DoubleBitBinary& value, openpal::WriteBuffer& buff) const
{
	Group4Var2::Write(ConvertGroup4Var2::Apply(value), buff);
}


const GroupVariationID  Group4Var3::ID(4, 3);

Group4Var3 Group4Var3::Read(ReadOnlyBuffer& buffer)
{
	Group4Var3 obj;
	obj.flags = UInt8::Read(buffer);
	buffer.Advance(1);
	obj.time = UInt16::Read(buffer);
	buffer.Advance(2);
	return obj;
}

void Group4Var3::Write(const Group4Var3& arg, openpal::WriteBuffer& buffer)
{
	UInt8::Write(buffer, arg.flags);
	buffer.Advance(1);
	UInt16::Write(buffer, arg.time);
	buffer.Advance(2);
}

Group4Var3Serializer Group4Var3Serializer::mInstance;

DoubleBitBinary Group4Var3Serializer::Read(ReadOnlyBuffer& buff) const
{
	auto gv = Group4Var3::Read(buff);
	return DoubleBitBinaryFactory::From(gv.flags, gv.time);
}

void Group4Var3Serializer::Write(const DoubleBitBinary& value, openpal::WriteBuffer& buff) const
{
	Group4Var3::Write(ConvertGroup4Var3::Apply(value), buff);
}



}
