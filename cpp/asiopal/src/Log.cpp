#include <sstream>
#include <exception>

#include <asiopal/Log.h>

#include <openpal/Exception.h>

using namespace std;
using namespace openpal;

namespace asiopal
{



void EventLog::Log( const LogEntry& arEntry )
{
for(auto pair: mSubscribers) {
		if(this->SetContains(pair.second, -1) || this->SetContains(pair.second, arEntry.GetErrorCode())) {
			pair.first->Log(arEntry);
		}
	}
}

bool EventLog::SetContains(const std::set<int>& arSet, int aValue)
{
	return arSet.find(aValue) != arSet.end();
}


void EventLog :: AddLogSubscriber(ILogBase* apSubscriber)
{
	this->AddLogSubscriber(apSubscriber, -1);
}

void EventLog :: AddLogSubscriber(ILogBase* apSubscriber, int aErrorCode)
{
	SubscriberMap::iterator i = mSubscribers.find(apSubscriber);
	if(i == mSubscribers.end()) {
		std::set<int> set;
		mSubscribers.insert(SubscriberMap::value_type(apSubscriber, set));
		this->AddLogSubscriber(apSubscriber, aErrorCode);
	}
	else i->second.insert(aErrorCode);
}

void EventLog :: RemoveLogSubscriber(ILogBase* apBase)
{
	SubscriberMap::iterator i = mSubscribers.find(apBase);
	if(i != mSubscribers.end()) mSubscribers.erase(i);
}

}



