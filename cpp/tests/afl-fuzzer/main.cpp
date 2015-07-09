
#include <cstddef>
#include <cstdio>
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

int main(int argc, char* argv[])
{
	std::cout << "argc: " << argc << std::endl;	
  
	uint8_t buffer[4096];
		
	//try to read from stdin
	size_t num = fread(buffer, 1, 4096, stdin);
	
	std::cout << "read: " << num << std::endl;
	
	ReadBufferView view(buffer, static_cast<uint32_t>(num));
	
	ConsoleLogger handler;
	LogRoot root(&handler, "log", ~0);
	auto logger = root.GetLogger();
	APDUParser::ParseAndLogAll(view, &logger);	
}


