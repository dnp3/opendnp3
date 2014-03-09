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

#include "Group3.h"

#include "opendnp3/app/MeasurementFactory.h"
#include "opendnp3/app/WriteConversions.h"
#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3
{

const GroupVariationID  Group3Var0::ID(3, 0);

const GroupVariationID  Group3Var1::ID(3, 1);

const GroupVariationID  Group3Var2::ID(3, 2);

Group3Var2 Group3Var2::Read(ReadOnlyBuffer& buffer)
{
	Group3Var2 obj;
	obj.flags = UInt8::Read(buffer);
	buffer.Advance(1);
	return obj;
}

void Group3Var2::Write(const Group3Var2& arg, openpal::WriteBuffer& buffer)
{
	UInt8::Write(buffer, arg.flags);
	buffer.Advance(1);
}

Group3Var2Serializer Group3Var2Serializer::mInstance;

DoubleBitBinary Group3Var2Serializer::Read(ReadOnlyBuffer& buff) const
{
	auto gv = Group3Var2::Read(buff);
	return DoubleBitBinaryFactory::From(gv.flags);
}

void Group3Var2Serializer::Write(const DoubleBitBinary& value, openpal::WriteBuffer& buff) const
{
	Group3Var2::Write(ConvertGroup3Var2::Apply(value), buff);
}



}
