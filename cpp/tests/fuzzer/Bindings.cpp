
#include <cstddef>
#include <iostream>

#include <opendnp3/app/parsing/APDUParser.h>
#include <openpal/logging/LogRoot.h>

using namespace openpal;
using namespace opendnp3;

class ConsoleLogger : public openpal::ILogHandler
{

public:	
	virtual void Log(const openpal::LogEntry& entry) override final
	{
		std::cout << entry.GetMessage() << std::endl;
	}
};

static ConsoleLogger handler;

static LogRoot root(&handler, "logger", LogFilters(~0));

extern "C" void TestOneInput(const unsigned char *data, size_t size) 
{

  ReadBufferView input(data, size);
  auto logger = root.GetLogger();
  //APDUParser::ParseAndLogAll(input, &logger);
  APDUParser::ParseAndLogAll(input, nullptr);
}
