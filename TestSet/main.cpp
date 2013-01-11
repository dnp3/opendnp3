//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//

#include <memory>
#include <csignal>
#include <sys/stat.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include "StackHelpers.h"
#include "AddressScanner.h"
#include <APL/LogToStdio.h>

#include <DNP3XML/XML_DNP3.h>

using namespace std;
using namespace apl;
using namespace apl::dnp;
using namespace APLXML_Base;
using namespace apl::xml;

namespace po = boost::program_options;

void WaitOnEnterKey()
{
	cout << "Press <enter> to quit " << endl;
	string s;
	cin >> s;
}

bool FileExists(const std::string& arFileName)
{
	struct stat stFileInfo;
	int statval = stat(arFileName.c_str(), &stFileInfo);
	return statval == 0;
}

bool GenerateConfig(bool aIsMaster, const std::string& arPath)
{
	try {
		if(aIsMaster) {
			APLXML_MTS::MasterTestSet_t cfg;
			XML_TestSet::Configure(cfg, false);
			WriteXmlToFile(&cfg, arPath);
		}
		else {
			APLXML_STS::SlaveTestSet_t cfg;
			XML_TestSet::Configure(cfg);
			XML_DNP3::Configure(cfg.DeviceTemplate, 10, 10, 10, 2, 2, 2, 2);
			WriteXmlToFile(&cfg, arPath);
		}
		std::cout << "generated " << arPath << std::endl;
		return true;
	}
	catch(const apl::Exception& ex) {
		std::cout << "failure generating " << arPath << ex.GetErrorString() << std::endl;
		return false;
	}
}

template <class T, class U>
void RunStack(const std::string& arConfigFile)
{
	U cfg;
	loadXmlInto(arConfigFile, &cfg);
	FilterLevel lev = Convert(cfg.Log.Filter);
	T stack(&cfg, lev);
	stack.Run();
}

void Scan(const std::string& arConfigFile, boost::uint16_t start, boost::uint16_t stop)
{
	if(stop < start) throw ArgumentException(LOCATION, "Start must be < stop");

	EventLog elog;
	elog.AddLogSubscriber(apl::LogToStdio::Inst());
	APLXML_MTS::MasterTestSet_t cfg;
	loadXmlInto(arConfigFile, &cfg);
	AddressScanner scanner(elog.GetLogger(xml::Convert(cfg.Log.Filter), "Scanner"), cfg, start, stop);
	scanner.Run();
}

int main(int argc, char* argv[])
{
	// uses the simple argument helper to set the config flags approriately
	std::string xmlFilename;

	po::options_description desc("Allowed options");
	desc.add_options()
	("configfile,F", po::value<std::string>(&xmlFilename), "The xml configuration file to use")
	("help,H", "display program options")
	("generate,G", "Generate a new default config file and exit")
	("gen_on_no_exist,E", "Generate the specified config file automatically if it doesn't exist")
	("slave,S", "Use slave test set")
	("scan_start,A", po::value<boost::uint16_t>(), "Start address for a link layer address scan")
	("scan_stop,B", po::value<boost::uint16_t>(), "Stop address for a link layer address scan");

	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch ( boost::program_options::error& ex ) {
		cout << ex.what() << endl;
		cout << desc << endl;
		return -1;
	}

	if(vm.count("configfile") == 0) {
		xmlFilename = vm.count("slave") ? "dnp3_slave_default.xml" : "dnp3_master_default.xml";
	}

	if(vm.count("help")) {
		cout << desc << endl;
		return 0;
	}

	if(vm.count("generate")) {
		return GenerateConfig(vm.count("slave") == 0, xmlFilename) ? 0 : -1;
	}

	if(!FileExists(xmlFilename) && vm.count("gen_on_no_exist")) {
		if(!GenerateConfig(vm.count("slave") == 0, xmlFilename)) return -1;
	}

	try {

		if(vm.count("scan_start") > 0 && vm.count("scan_stop")) {
			boost::uint16_t start = vm["scan_start"].as<boost::uint16_t>();
			boost::uint16_t stop = vm["scan_stop"].as<boost::uint16_t>();

			Scan(xmlFilename, start, stop);
		}
		else {
			if ( vm.count("slave") ) {
				RunStack<SlaveXMLStack, APLXML_STS::SlaveTestSet_t>(xmlFilename);
			}
			else {
				RunStack<MasterXMLStack, APLXML_MTS::MasterTestSet_t>(xmlFilename);
			}
		}
	}
	catch(const Exception& ex) {
		cout << ex.GetErrorString() << endl;
		WaitOnEnterKey();
		return -1;
	}
	return 0;
}

