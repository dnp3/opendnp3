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

#ifndef __OPENDNP3_GENERATED_GROUP41_H_
#define __OPENDNP3_GENERATED_GROUP41_H_

#include <openpal/BufferWrapper.h>
#include "GroupVariationID.h"
#include <opendnp3/gen/CommandStatus.h>
#include <opendnp3/AnalogOutput.h>

namespace opendnp3 {

struct Group41Var1
{
  static const GroupVariationID ID;
  static const size_t SIZE = 5;

  typedef AnalogOutputInt32 Target;
  static AnalogOutputInt32 ReadAndConvert(openpal::ReadOnlyBuffer&);
  static void ConvertAndWrite(const AnalogOutputInt32&, openpal::WriteBuffer&);

  static Group41Var1 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var1&, openpal::WriteBuffer&);

  typedef int32_t ValueType;
  int32_t value;
  CommandStatus status;
};

struct Group41Var2
{
  static const GroupVariationID ID;
  static const size_t SIZE = 3;

  typedef AnalogOutputInt16 Target;
  static AnalogOutputInt16 ReadAndConvert(openpal::ReadOnlyBuffer&);
  static void ConvertAndWrite(const AnalogOutputInt16&, openpal::WriteBuffer&);

  static Group41Var2 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var2&, openpal::WriteBuffer&);

  typedef int16_t ValueType;
  int16_t value;
  CommandStatus status;
};

struct Group41Var3
{
  static const GroupVariationID ID;
  static const size_t SIZE = 5;

  typedef AnalogOutputFloat32 Target;
  static AnalogOutputFloat32 ReadAndConvert(openpal::ReadOnlyBuffer&);
  static void ConvertAndWrite(const AnalogOutputFloat32&, openpal::WriteBuffer&);

  static Group41Var3 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var3&, openpal::WriteBuffer&);

  typedef float ValueType;
  float value;
  CommandStatus status;
};

struct Group41Var4
{
  static const GroupVariationID ID;
  static const size_t SIZE = 9;

  typedef AnalogOutputDouble64 Target;
  static AnalogOutputDouble64 ReadAndConvert(openpal::ReadOnlyBuffer&);
  static void ConvertAndWrite(const AnalogOutputDouble64&, openpal::WriteBuffer&);

  static Group41Var4 Read(openpal::ReadOnlyBuffer&);
  static void Write(const Group41Var4&, openpal::WriteBuffer&);

  typedef double ValueType;
  double value;
  CommandStatus status;
};


}

#endif
