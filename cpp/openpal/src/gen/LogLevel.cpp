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
#include <openpal/gen/LogLevel.h>

namespace openpal {

std::string LogLevelToString(LogLevel arg)
{
  switch(arg)
  {
    case(LogLevel::Event):
      return "Event";
    case(LogLevel::Error):
      return "Error";
    case(LogLevel::Warning):
      return "Warning";
    case(LogLevel::Info):
      return "Info";
    case(LogLevel::Interpret):
      return "Interpret";
    case(LogLevel::Comm):
      return "Comm";
    case(LogLevel::Debug):
      return "Debug";
  }
  return "Debug";
}
int LogLevelToType(LogLevel arg)
{
  return static_cast<int>(arg);
}
LogLevel LogLevelFromType(int arg)
{
  switch(arg)
  {
    case(0x1):
      return LogLevel::Event;
    case(0x2):
      return LogLevel::Error;
    case(0x4):
      return LogLevel::Warning;
    case(0x8):
      return LogLevel::Info;
    case(0x10):
      return LogLevel::Interpret;
    case(0x20):
      return LogLevel::Comm;
    case(0x40):
      return LogLevel::Debug;
  }
  return LogLevel::Debug;
}

}
