#ifndef __MOCK_LOG_SUBSCRIBER_H_
#define __MOCK_LOG_SUBSCRIBER_H_

#include <openpal/LogBase.h>
#include <queue>

namespace opendnp3
{

class MockLogSubscriber : public openpal::ILogBase
{
public:

	void Log( const openpal::LogEntry& arEntry ) {
		mEntries.push(arEntry);
	}

	// updating a variable/metric in the system
	void SetVar(const std::string& aSource, const std::string& aVarName, int aValue) {}

	std::queue<openpal::LogEntry> mEntries;
};


}

#endif
