#ifndef __FLEXIBLE_OBSERVER_TERMINAL_EXTENSION_H_
#define __FLEXIBLE_OBSERVER_TERMINAL_EXTENSION_H_

#include "TerminalInterfaces.h"

#include <APL/DataInterfaces.h>
#include <DNP3/DeviceTemplate.h>
#include <APL/INotifier.h>

#include <queue>
#include <vector>
#include <string>
#include <map>

namespace apl
{

//class FlexibleDataObserver;
class QueueingFDO;

namespace dnp
{

struct ShowRange {
	enum Type { ST_ALL, ST_BI, ST_AI, ST_C, ST_BOS, ST_SS };
	ShowRange() : type(ST_ALL), start(0), stop(0) {}
	Type type;
	bool allOfType;
	size_t start;
	size_t stop;
};


/** Terminal extension for interacting with a submaster via a flexible data observer
*/
class FlexibleObserverTerminalExtension : public ITerminalExtension, public INotifier
{
public:

	FlexibleObserverTerminalExtension(QueueingFDO* apObserver);

	FlexibleObserverTerminalExtension(QueueingFDO* apObserver, const DeviceTemplate& arTmp);

	virtual ~FlexibleObserverTerminalExtension() {}

	std::string Name() {
		return "FlexibleObserverTerminalExtension";
	}

	typedef std::map<size_t, std::string> NameMap;

	void Notify();
private:

	QueueingFDO* mpObserver;

	ShowRange mRange;

	NameMap mBinaryNames;
	NameMap mAnalogNames;
	NameMap mCounterNames;
	NameMap mControlStatusNames;
	NameMap mSetpointStatusNames;

	size_t mLongestName;

	retcode HandleShow(std::vector<std::string>& arArgs, bool aLogToFile, bool aClearScreenAfter);
	retcode HandleRunUpdates(std::vector<std::string>& arArgs);
	retcode HandleShowUpdates(std::vector<std::string>& arArgs);
	retcode HandleSetShow(std::vector<std::string>& arArgs);
	retcode HandleShowStats(std::vector<std::string>& arArgs);

	void PrintUpdates(bool aWithCount);

	void OnRedirectedLine(const std::string&);

	//implement from ITerminalExtension
	void _BindToTerminal(ITerminal* apTerminal);
};

}
}

#endif
