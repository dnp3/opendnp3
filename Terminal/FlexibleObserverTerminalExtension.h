
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
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
