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

#include "Group20.h"

#include "MeasurementFactory.h"
#include <openpal/Serialization.h>

using namespace openpal;

namespace opendnp3 {

Group20Var1 Group20Var1::Read(ReadOnlyBuffer& buffer)
{
  Group20Var1 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group20Var1::Write(const Group20Var1& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
}

Counter Group20Var1::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count);
}


Group20Var2 Group20Var2::Read(ReadOnlyBuffer& buffer)
{
  Group20Var2 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group20Var2::Write(const Group20Var2& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
}

Counter Group20Var2::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count);
}


Group20Var3 Group20Var3::Read(ReadOnlyBuffer& buffer)
{
  Group20Var3 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group20Var3::Write(const Group20Var3& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
}

Counter Group20Var3::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count);
}


Group20Var4 Group20Var4::Read(ReadOnlyBuffer& buffer)
{
  Group20Var4 obj;
  obj.flags = UInt8::Read(buffer);
  buffer.Advance(1);
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group20Var4::Write(const Group20Var4& arg, openpal::WriteBuffer& buffer)
{
  UInt8::Write(buffer, arg.flags);
  buffer.Advance(1);
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
}

Counter Group20Var4::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.flags, gv.count);
}


Group20Var5 Group20Var5::Read(ReadOnlyBuffer& buffer)
{
  Group20Var5 obj;
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group20Var5::Write(const Group20Var5& arg, openpal::WriteBuffer& buffer)
{
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
}

Counter Group20Var5::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.count);
}


Group20Var6 Group20Var6::Read(ReadOnlyBuffer& buffer)
{
  Group20Var6 obj;
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group20Var6::Write(const Group20Var6& arg, openpal::WriteBuffer& buffer)
{
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
}

Counter Group20Var6::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.count);
}


Group20Var7 Group20Var7::Read(ReadOnlyBuffer& buffer)
{
  Group20Var7 obj;
  obj.count = UInt32::Read(buffer);
  buffer.Advance(4);
  return obj;
}

void Group20Var7::Write(const Group20Var7& arg, openpal::WriteBuffer& buffer)
{
  UInt32::Write(buffer, arg.count);
  buffer.Advance(4);
}

Counter Group20Var7::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.count);
}


Group20Var8 Group20Var8::Read(ReadOnlyBuffer& buffer)
{
  Group20Var8 obj;
  obj.count = UInt16::Read(buffer);
  buffer.Advance(2);
  return obj;
}

void Group20Var8::Write(const Group20Var8& arg, openpal::WriteBuffer& buffer)
{
  UInt16::Write(buffer, arg.count);
  buffer.Advance(2);
}

Counter Group20Var8::Convert(ReadOnlyBuffer& buff)
{
  auto gv = Read(buff);
  return CounterFactory::From(gv.count);
}



}
