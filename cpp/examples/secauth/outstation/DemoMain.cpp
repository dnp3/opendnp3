/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */

#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/PrintingSOEHandler.h>
#include <asiodnp3/ConsoleLogger.h>
#include <asiodnp3/MeasUpdate.h>

#include <asiopal/UTCTimeSource.h>

#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/Database.h>
#include <opendnp3/LogLevels.h>

#include <openpal/container/StaticBuffer.h>

#include <osslcrypto/CryptoProvider.h>
#include <secauth/outstation/SimpleOutstationUserDatabase.h>

#include <string>
#include <thread>
#include <iostream>

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;
using namespace secauth;

// Hard-wired configuration of the default user update key for demo purposes
void AddDefaultUser(SimpleOutstationUserDatabase& db)
{
	// add a 128-bit demo key of all 0xFF
	openpal::StaticBuffer<16> data;
	data.GetWriteBuffer().SetAllTo(0xFF);

	UpdateKey key(data.ToReadOnly());

	db.ConfigureUser(
		secauth::User::Default(),
		key,
		Permissions::AllowAll()		
	);
}

int main(int argc, char* argv[])
{
	// Specify what log levels to use. NORMAL is warning and above
	// You can add all the communication logging by uncommenting below.
	const uint32_t FILTERS = levels::NORMAL | levels::ALL_APP_COMMS;
	
	

	// Create a user database
	secauth::SimpleOutstationUserDatabase userDatabase;
	// setup the default user
	AddDefaultUser(userDatabase);

	// crypto provider we will use to create the outstation
	// clean this up last since everything running in the manager depends on it
	osslcrypto::CryptoProvider crypto;

	// This is the main point of interaction with the stack
	// Allocate a single thread to the pool since this is a single outstation
	DNP3Manager manager(1, &crypto);

	// send log messages to the console
	manager.AddLogSubscriber(&ConsoleLogger::Instance());

	// Create a TCP server (listener)	
	auto pChannel= manager.AddTCPServer("server", FILTERS, TimeDuration::Seconds(5), TimeDuration::Seconds(5), "0.0.0.0", 20000);	
	
	// Optionally, you can bind listeners to the channel to get state change notifications
	// This listener just prints the changes to the console
	pChannel->AddStateListener([](ChannelState state)
	{
		std::cout << "channel state: " << ChannelStateToString(state) << std::endl;
	});

	// The main object for a outstation. The defaults are use-able, 
	// but understanding the options are important.
	OutstationAuthStackConfig config;	
	
	// You must specify the shape of your database and the size of the event buffers
	config.dbTemplate = DatabaseTemplate::AllTypes(10);
	config.outstation.eventBufferConfig = EventBufferConfig::AllTypes(10);	
	
	// you can override an default outstation parameters here
	// in this example, we've enabled the outstation to use unsolicited reporting
	// if the master enables it
	config.outstation.params.allowUnsolicited = false;

	// You can override the default link layer settings here
	// in this example we've changed the default link layer addressing
	config.link.LocalAddr = 10;
	config.link.RemoteAddr = 1;	

	// don't challenge confirms, as the master can't authenticate these yet
	config.auth.functions.authConfirm = false;
	
	// Create a new outstation with a log level, command handler, and
	// config info this	returns a thread-safe interface used for
	// updating the outstation's database.	
	auto pOutstation = pChannel->AddOutstation(
		"outstation",
		SuccessCommandHandler::Instance(),
		DefaultOutstationApplication::Instance(),
		config,		
		asiopal::UTCTimeSource::Instance(),
		userDatabase);	

	// Enable the outstation and start communications
	pOutstation->Enable();	

	// variables used in example loop
	string input;
	uint32_t count = 0;
	double value = 0;
	bool binary = false;
	DoubleBit dbit = DoubleBit::DETERMINED_OFF;
	
	while (true)
	{
		std::cout << "Enter one or more measurement changes then press <enter>" << std::endl;
		std::cout << "c = counter, b = binary, d = doublebit, a = analog, x = exit" << std::endl;
		std::cin >> input;
		
		MeasUpdate tx(pOutstation);
		for (char& c : input)
		{
			switch (c)
			{
				case('c') :
				{				
					tx.Update(Counter(count), 0);
					++count;
					break;
				}
				case('a') :
				{				
					tx.Update(Analog(value), 0);
					value += 1;
					break;
				}
				case('b') :
				{				
					tx.Update(Binary(binary), 0);
					binary = !binary;
					break;
				}
				case('d') :
				{				
					tx.Update(DoubleBitBinary(dbit), 0);
					dbit = (dbit == DoubleBit::DETERMINED_OFF) ? DoubleBit::DETERMINED_ON : DoubleBit::DETERMINED_OFF;
					break;
				}
				case('x') :

					// DNP3Manager destructor cleanups up everything automagically
					return 0;

				default:
					std::cout << "No action registered for: " << c << std::endl;
					break;
			}
		}
		
	}

	return 0;
}
