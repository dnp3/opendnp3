
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
#include "FlexibleObserverTerminalExtension.h"

#include <APL/FlexibleDataObserver.h>
#include <APL/QueueingFDO.h>
#include <APL/CommandInterfaces.h>
#include <APL/CommandTypes.h>
#include <APL/CommandResponseQueue.h>
#include <APL/Util.h>

#include <functional>
#include <sstream>


using namespace std;
using namespace std::placeholders;

namespace apl
{
namespace dnp
{

typedef std::function<std::string (uint8_t)> QualityFunctor;

FlexibleObserverTerminalExtension::FlexibleObserverTerminalExtension(QueueingFDO* apObserver)
	: mpObserver(apObserver), mLongestName(0)
{

}


void FlexibleObserverTerminalExtension::Notify()
{
	this->Post(std::bind(&FlexibleObserverTerminalExtension::PrintUpdates, this, false));
}


template <class T>
void FillOutNames( const vector<T>& arSrcVec, FlexibleObserverTerminalExtension::NameMap& arNameMap, size_t& arLongestName )
{
	for(size_t i = 0; i < arSrcVec.size(); ++i) {
		arLongestName = max(arLongestName, arSrcVec[i].Name.size());
		arNameMap[i] = arSrcVec[i].Name;
	}
}

FlexibleObserverTerminalExtension::FlexibleObserverTerminalExtension(QueueingFDO* apObserver, const DeviceTemplate& arTmp)
	: mpObserver(apObserver), mLongestName(0)
{
	size_t aLongestName = 0;

	for(size_t i = 0; i < arTmp.mBinary.size(); ++i) {
		aLongestName = max(aLongestName, arTmp.mBinary[i].Name.size());
		mBinaryNames[i] = arTmp.mBinary[i].Name;
	}

	for(size_t i = 0; i < arTmp.mAnalog.size(); ++i) {
		aLongestName = max(aLongestName, arTmp.mAnalog[i].Name.size());
		mAnalogNames[i] = arTmp.mAnalog[i].Name;
	}

	for(size_t i = 0; i < arTmp.mCounter.size(); ++i) {
		aLongestName = max(aLongestName, arTmp.mCounter[i].Name.size());
		mCounterNames[i] = arTmp.mCounter[i].Name;
	}

	FillOutNames( arTmp.mControls, mControlStatusNames, aLongestName );
	FillOutNames( arTmp.mSetpoints, mSetpointStatusNames, aLongestName );

	mLongestName = aLongestName;
}

void FlexibleObserverTerminalExtension::_BindToTerminal(ITerminal* apTerminal)
{
	CommandNode cmd;

	cmd.mName = "show";
	cmd.mUsage = "show";
	cmd.mDesc = "Displays measurement data";
	cmd.mHandler = std::bind(&FlexibleObserverTerminalExtension::HandleShow, this, _1, true, false);
	apTerminal->BindCommand(cmd, "show");

	cmd.mName = "show set";
	cmd.mUsage = "show set <all|bi|ai|c|cs|ss> <index|start> <stop>";
	cmd.mDesc = "Sets a range to displayed with the show command";
	cmd.mHandler = std::bind(&FlexibleObserverTerminalExtension::HandleSetShow, this, _1);
	apTerminal->BindCommand(cmd, "show set");

	cmd.mName = "show stats";
	cmd.mUsage = "show stats";
	cmd.mDesc = "Displays number and types of measurement in data observer";
	cmd.mHandler = std::bind(&FlexibleObserverTerminalExtension::HandleShowStats, this, _1);
	apTerminal->BindCommand(cmd, "show stats");

	cmd.mName = "update";
	cmd.mUsage = "update";
	cmd.mDesc = "Displays cached data updates";
	cmd.mHandler = std::bind(&FlexibleObserverTerminalExtension::HandleShowUpdates, this, _1);
	apTerminal->BindCommand(cmd, "update");

	cmd.mName = "update run";
	cmd.mUsage = "update run";
	cmd.mDesc = "Continuously displays data updates until <enter> is pressed";
	cmd.mHandler = std::bind(&FlexibleObserverTerminalExtension::HandleRunUpdates, this, _1);
	apTerminal->BindCommand(cmd, "update run");
}

template <class T>
void DisplayPoints(ostringstream& arOss, typename PointMap<T>::Type& arMap, FlexibleObserverTerminalExtension::NameMap aNameMap, const char* arTitle, const ShowRange& aRange, size_t aLongestName)
{
	arOss << arTitle << ITerminal::EOL;

	for(auto pair: arMap) {
		if ( aRange.type != ShowRange::ST_ALL && !aRange.allOfType && (pair.first < aRange.start || pair.first > aRange.stop) )
			continue;

		if(aNameMap.find(pair.first) != aNameMap.end()) {
			string name = aNameMap[pair.first];
			for(size_t i = name.size(); i < aLongestName; i++) arOss << " ";
			arOss << name << " ";
		}
		else {
			arOss << "Index: " << pair.first << "\t";
		}

		arOss << "Value: " << pair.second.GetValue() << "\t" << " Quality: " << T::QualConverter::GetSymbolString(pair.second.GetQuality()) << ITerminal::EOL;
	}
}

retcode FlexibleObserverTerminalExtension::HandleShow(std::vector<std::string>& arArgs, bool aLogToFile, bool aClearScreenAfter)
{
	Transaction t(mpObserver);

	ostringstream oss;

	oss << "SHOWING: ";
	if ( mRange.type == ShowRange::ST_ALL ) {
		oss << "all" << ITerminal::EOL;
	}
	else {
		if ( mRange.allOfType )
			oss << "all of type ";
		if ( mRange.type == ShowRange::ST_BI )
			oss << "binary input";
		if ( mRange.type == ShowRange::ST_AI )
			oss << "analog input";
		if ( mRange.type == ShowRange::ST_C )
			oss << "counter";
		if ( mRange.type == ShowRange::ST_BOS )
			oss << "control status";
		if ( mRange.type == ShowRange::ST_SS )
			oss << "setpoint status";
		if ( !mRange.allOfType ) {
			oss << ", ";
			if ( mRange.start == mRange.stop )
				oss << "index: " << mRange.start;
			else
				oss << "start: " << mRange.start << ", stop: " << mRange.stop;
		}
		oss << ITerminal::EOL;
	}

	if ( mRange.type == ShowRange::ST_ALL || mRange.type == ShowRange::ST_BI )
		DisplayPoints<Binary>(oss, mpObserver->mBinaryMap, mBinaryNames, "--- Binary Input ---", mRange, mLongestName);
	if ( mRange.type == ShowRange::ST_ALL || mRange.type == ShowRange::ST_AI )
		DisplayPoints<Analog>(oss, mpObserver->mAnalogMap, mAnalogNames, "--- Analog Input ---", mRange, mLongestName);
	if ( mRange.type == ShowRange::ST_ALL || mRange.type == ShowRange::ST_C )
		DisplayPoints<Counter>(oss, mpObserver->mCounterMap, mCounterNames, "--- Counter Input ---", mRange, mLongestName);
	if ( mRange.type == ShowRange::ST_ALL || mRange.type == ShowRange::ST_BOS )
		DisplayPoints<ControlStatus>(oss, mpObserver->mControlStatusMap, mControlStatusNames, "--- Control Status ---", mRange, mLongestName);
	if ( mRange.type == ShowRange::ST_ALL || mRange.type == ShowRange::ST_SS )
		DisplayPoints<SetpointStatus>(oss, mpObserver->mSetpointStatusMap, mSetpointStatusNames, "--- Setpoint Status ---", mRange, mLongestName);

	this->Send(oss.str(), aClearScreenAfter);

	return SUCCESS;
}


retcode FlexibleObserverTerminalExtension::HandleShowUpdates(std::vector<std::string>& arArgs)
{
	PrintUpdates(true);
	return SUCCESS;
}

retcode FlexibleObserverTerminalExtension::HandleRunUpdates(std::vector<std::string>& arArgs)
{
	mpObserver->AddObserver(this);
	SetRedirect(std::bind(&FlexibleObserverTerminalExtension::OnRedirectedLine, this, _1));
	PrintUpdates(false);

	return SUCCESS;
}


void FlexibleObserverTerminalExtension::OnRedirectedLine(const std::string&)
{
	mpObserver->RemoveObserver(this);
	ClearRedirect();
}


void FlexibleObserverTerminalExtension::PrintUpdates(bool aWithCount)
{
	Transaction tr(mpObserver);

	if ( aWithCount && !mpObserver->updates.empty() )
		std::cout << "Displaying " << mpObserver->updates.size() << " updates." << ITerminal::EOL;

	while (!mpObserver->updates.empty()) {
		std::cout << mpObserver->updates.front() << ITerminal::EOL;
		mpObserver->updates.pop_front();
	}
}

retcode FlexibleObserverTerminalExtension::HandleShowStats(std::vector<std::string>& /*arArgs*/)
{
	Transaction t(mpObserver);
	std::cout << "Binary Input: " << mpObserver->mBinaryMap.size() << std::endl;
	std::cout << "Analog Input: " << mpObserver->mAnalogMap.size() << std::endl;
	std::cout << "Counter Input: " << mpObserver->mCounterMap.size() << std::endl;
	std::cout << "ControlStatus Input: " << mpObserver->mControlStatusMap.size() << std::endl;
	std::cout << "SetpointStatus Input: " << mpObserver->mSetpointStatusMap.size() << std::endl;
	return SUCCESS;
}

ShowRange::Type ParseShowType( const std::string& arString )
{
	std::string lower(arString);
	toLowerCase(lower);

	if ( lower.compare("bi") == 0 ) {
		return ShowRange::ST_BI;
	}
	else if ( lower.compare("ai") == 0 ) {
		return ShowRange::ST_AI;
	}
	else if ( lower.compare("c") == 0 ) {
		return ShowRange::ST_C;
	}
	else if ( lower.compare("cs") == 0 ) {
		return ShowRange::ST_BOS;
	}
	else if ( lower.compare("ss") == 0 ) {
		return ShowRange::ST_SS;
	}
	else {
		return ShowRange::ST_ALL;
	}
}

retcode FlexibleObserverTerminalExtension::HandleSetShow(std::vector<std::string>& arArgs)
{
	if(arArgs.size() < 1) return BAD_ARGUMENTS;

	ShowRange::Type type = ParseShowType(arArgs[0]);

	if ( type == ShowRange::ST_ALL ) {
		mRange.allOfType = true;
		mRange.start = 0;
		mRange.stop = 0;
	}
	else if ( arArgs.size() == 1 ) {
		mRange.allOfType = true;
		mRange.start = 0;
		mRange.stop = 0;
	}
	else if ( arArgs.size() == 2 ) {
		mRange.allOfType = false;
		int index = 0;
		stringstream ss;
		ss << arArgs[1];
		ss >> index;
		mRange.start = index;
		mRange.stop = index;
	}
	else if ( arArgs.size() == 3 ) {
		mRange.allOfType = false;
		size_t start = 0;
		size_t stop = 0;

		stringstream ss;
		ss << arArgs[1];
		ss >> start;
		ss.clear();
		ss << arArgs[2];
		ss >> stop;

		mRange.start = start;
		mRange.stop = stop;
	}
	else {
		return BAD_ARGUMENTS;
	}

	mRange.type = type;
	return SUCCESS;
}
}
}

