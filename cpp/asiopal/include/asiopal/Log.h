#ifndef __EVENT_LOG_H_
#define __EVENT_LOG_H_


#include <assert.h>
#include <map>
#include <vector>
#include <mutex>
#include <set>

#include <openpal/LogBase.h>
#include <openpal/Visibility.h>

namespace asiopal
{

class DLL_LOCAL EventLog : public openpal::ILogBase
{
public:

	/**
	* Binds a listener to ALL log messages
	*/
	void AddLogSubscriber(ILogBase* apSubscriber);

	/**
	* Binds a listener to only certain error messages
	*/
	void AddLogSubscriber(ILogBase* apSubscriber, int aErrorCode);

	/**
	* Cancels a previous binding
	*/
	void RemoveLogSubscriber(ILogBase* apBase);

	//implement the log function from ILogBase
	void Log( const openpal::LogEntry& arEntry );

private:

	bool SetContains(const std::set<int>& arSet, int aValue);

	std::mutex mMutex;
	typedef std::map<ILogBase*, std::set<int> > SubscriberMap;
	SubscriberMap mSubscribers;

};


}

#endif

