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

#include "SlaveDemo.h"

#include <signal.h>
#include <string>
#include <mutex>

#include <APL/Log.h>

using namespace std;
using namespace apl;
using namespace apl::dnp;

std::mutex gMutex;
SlaveDemoBase* gpDemo = NULL;

void SetDemo(SlaveDemoBase* apDemo)
{
	std::lock_guard<std::mutex> lock(gMutex);
	gpDemo = apDemo;
}

void Terminate(int sig)
{
	std::lock_guard<std::mutex> lock(gMutex);
	std::cout << "Signal " << sig << ", shutdown... " << std::endl;
	if(gpDemo) gpDemo->Shutdown();
}

/*
 * Command line syntax:
 *
 *    ./demo-slave-cpp [remote-dnp3] [local-dnp3] [local-ip] [local-port]
 *
 * Defaults:
 *
 *    remote-dnp3    100
 *    local-dnp3     1
 *    local-ip       127.0.0.1
 *    local-port     4999
 */
int main(int argc, char* argv[])
{
	// Default values
	unsigned remote_dnp3 = 100;
	unsigned local_dnp3  = 1;
	string   local_ip    = "127.0.0.1";
	unsigned local_port  = 4999;

	// Parse the command line arguments using a "fall-through"
	// switch statement.
	if (argc > 1 && std::strcmp("help", argv[1]) == 0) {
		cout << argv[0] << " [remote-dnp3] [local-dnp3] [local-ip] [local-port]" << endl;
		return -1;
	}

	switch (argc) {
	case 5:
		{
			istringstream iss(argv[4]);
			iss >> local_port;
		}
	case 4:
		local_ip = argv[3];
	case 3:
		{
			istringstream iss(argv[2]);
			iss >> local_dnp3;
		}
	case 2:
		{
			istringstream iss(argv[1]);
			iss >> remote_dnp3;
		}
	}

	// Create a log object for the stack to use and configure it
	// with a subscriber that print alls messages to the stdout
	EventLog log;
	log.AddLogSubscriber(LogToStdio::Inst());

	// Specify a FilterLevel for the stack/physical layer to use.
	// Log statements with a lower priority will not be logged.
	const FilterLevel LOG_LEVEL = LEV_INFO;

	// create our demo application that handles commands and
	// demonstrates how to publish data give it a loffer with a
	// unique name and log level
	SlaveDemoApp app(log.GetLogger(LOG_LEVEL, "demoapp"));

	// This is the main point of interaction with the stack. The
	// AsyncStackManager object instantiates master/slave DNP
	// stacks, as well as their physical layers
	AsyncStackManager mgr(log.GetLogger(LOG_LEVEL, "dnp"));

	// Add a TCPServer to the manager with the name "tcpserver".
	// The server will wait 3000 ms in between failed bind calls.
	mgr.AddTCPServer(
		"tcpserver",
		PhysLayerSettings(LOG_LEVEL, 3000),
		local_ip,
		local_port
	);

	// The master config object for a slave. The default are
	// useable, but understanding the options are important.
	SlaveStackConfig stackConfig;

	// Override the default link addressing
	stackConfig.link.LocalAddr  = local_dnp3;
	stackConfig.link.RemoteAddr = remote_dnp3;

	// The DeviceTemplate struct specifies the structure of the
	// slave's database, as well as the index range of controls and
	// setpoints it accepts.
	DeviceTemplate device(5, 5, 5, 5, 5, 5, 5);
	stackConfig.device = device;

	// Create a new slave on a previously declared port, with a
	// name, log level, command acceptor, and config info This
	// returns a thread-safe interface used for updating the slave's
	// database.
	IDataObserver* pDataObserver = mgr.AddSlave("tcpserver", "slave", LOG_LEVEL, app.GetCmdAcceptor(), stackConfig);

	// Tell the app where to write opdates
	app.SetDataObserver(pDataObserver);

	// Configure signal handlers so we can exit gracefully
	SetDemo(&app);
	signal(SIGTERM, &Terminate);
	signal(SIGABRT, &Terminate);
	signal(SIGINT,  &Terminate);

	app.Run();

	SetDemo(NULL);

	return 0;
}
