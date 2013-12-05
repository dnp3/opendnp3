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
