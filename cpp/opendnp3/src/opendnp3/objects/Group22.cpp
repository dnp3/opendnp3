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

#include "Group22.h"

#include "MeasurementFactory.h"
#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group22Var1 Group22Var1::Read(ReadOnlyBuffer& buffer)
{
  Group22Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group22Var1::Write(const Group22Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
}

Counter Group22Var1::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count);
}


Group22Var2 Group22Var2::Read(ReadOnlyBuffer& buffer)
{
  Group22Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group22Var2::Write(const Group22Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
}

Counter Group22Var2::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count);
}


Group22Var3 Group22Var3::Read(ReadOnlyBuffer& buffer)
{
  Group22Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group22Var3::Write(const Group22Var3& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
}

Counter Group22Var3::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count);
}


Group22Var4 Group22Var4::Read(ReadOnlyBuffer& buffer)
{
  Group22Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group22Var4::Write(const Group22Var4& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
}

Counter Group22Var4::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count);
}


Group22Var5 Group22Var5::Read(ReadOnlyBuffer& buffer)
{
  Group22Var5 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group22Var5::Write(const Group22Var5& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time48);
  buffer.Advance(6);
}

Counter Group22Var5::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count, gv.time48);
}


Group22Var6 Group22Var6::Read(ReadOnlyBuffer& buffer)
{
  Group22Var6 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  obj.time48 = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group22Var6::Write(const Group22Var6& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time48);
  buffer.Advance(6);
}

Counter Group22Var6::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count, gv.time48);
}


Group22Var7 Group22Var7::Read(ReadOnlyBuffer& buffer)
{
  Group22Var7 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  obj.time48 = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group22Var7::Write(const Group22Var7& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
  UInt48::Write(buffer, arg.time48);
  buffer.Advance(6);
}

Counter Group22Var7::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count, gv.time48);
}


Group22Var8 Group22Var8::Read(ReadOnlyBuffer& buffer)
{
  Group22Var8 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  obj.time48 = UInt48::Read(buffer);
  buffer.Advance(6);
  return obj;
}

void Group22Var8::Write(const Group22Var8& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
  UInt48::Write(buffer, arg.time48);
  buffer.Advance(6);
}

Counter Group22Var8::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count, gv.time48);
}



}
