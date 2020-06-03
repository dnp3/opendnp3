/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <opendnp3/ConsoleLogger.h>
#include <opendnp3/DNP3Manager.h>
#include <opendnp3/channel/PrintingChannelListener.h>
#include <opendnp3/logging/LogLevels.h>
#include <opendnp3/master/DefaultMasterApplication.h>
#include <opendnp3/master/PrintingCommandResultCallback.h>
#include <opendnp3/master/PrintingSOEHandler.h>

using namespace std;
using namespace opendnp3;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "usage: master-tls-demo <peer certificate> <local certificate> <private key>" << std::endl;
        return -1;
    }

    std::string peerCertificate(argv[1]);
    std::string localCertificate(argv[2]);
    std::string privateKey(argv[3]);

    std::cout << "Using peer cert: " << peerCertificate << std::endl;
    std::cout << "Using local cert: " << localCertificate << std::endl;
    std::cout << "Using private key file: " << privateKey << std::endl;

    // Specify what log levels to use. NORMAL is warning and above
    // You can add all the comms logging by uncommenting below
    const auto logLevels = levels::NORMAL | levels::ALL_APP_COMMS;

    // This is the main point of interaction with the stack
    // send log messages to the console
    DNP3Manager manager(1, ConsoleLogger::Create());

    // Connect via a TCPClient socket to a outstation
    auto channel = manager.AddTLSClient(
        "tls-client", logLevels, ChannelRetry::Default(), {IPEndpoint("127.0.0.1", 20001)}, "0.0.0.0",
        TLSConfig(peerCertificate, localCertificate, privateKey), PrintingChannelListener::Create());

    // The master config object for a master. The default are
    // useable, but understanding the options are important.
    MasterStackConfig stackConfig;

    // you can override application layer settings for the master here
    // in this example, we've change the application layer timeout to 2 seconds
    stackConfig.master.responseTimeout = TimeDuration::Seconds(2);
    stackConfig.master.disableUnsolOnStartup = true;

    // You can override the default link layer settings here
    // in this example we've changed the default link layer addressing
    stackConfig.link.LocalAddr = 1;
    stackConfig.link.RemoteAddr = 10;

    auto soeHandler = PrintingSOEHandler::Create();

    // Create a new master on a previously declared port, with a
    // name, log level, command acceptor, and config info. This
    // returns a thread-safe interface used for sending commands.
    auto master = channel->AddMaster("master",                           // id for logging
                                     soeHandler,                         // callback for data processing
                                     DefaultMasterApplication::Create(), // master application instance
                                     stackConfig                         // stack configuration
    );

    // do an integrity poll (Class 3/2/1/0) once per minute
    auto integrityScan = master->AddClassScan(ClassField::AllClasses(), TimeDuration::Minutes(1), soeHandler);

    // do a Class 1 exception poll every 5 seconds
    auto exceptionScan = master->AddClassScan(ClassField(ClassField::CLASS_1), TimeDuration::Seconds(2), soeHandler);

    // Enable the master. This will start communications.
    master->Enable();

    do
    {
        std::cout << "Enter a command" << std::endl;
        std::cout << "x - exits program" << std::endl;
        std::cout << "a - performs and ad-hoc range scan" << std::endl;
        std::cout << "i - integrity demand scan" << std::endl;
        std::cout << "e - exception demand scan" << std::endl;
        std::cout << "d - diable unsolcited" << std::endl;
        std::cout << "r - cold restart" << std::endl;
        std::cout << "c - send crob" << std::endl;

        char cmd;
        std::cin >> cmd;
        switch (cmd)
        {
        case ('a'):
            master->ScanRange(GroupVariationID(1, 2), 0, 3, soeHandler);
            break;
        case ('d'):
            master->PerformFunction("disable unsol", FunctionCode::DISABLE_UNSOLICITED,
                                    {Header::AllObjects(60, 2), Header::AllObjects(60, 3), Header::AllObjects(60, 4)});
            break;
        case ('r'):
        {
            auto print = [](const RestartOperationResult& result) {
                if (result.summary == TaskCompletion::SUCCESS)
                {
                    std::cout << "Success, Time: " << result.restartTime.ToString() << std::endl;
                }
                else
                {
                    std::cout << "Failure: " << TaskCompletionSpec::to_string(result.summary) << std::endl;
                }
            };
            master->Restart(RestartType::COLD, print);
            break;
        }
        case ('x'):
            // C++ destructor on DNP3Manager cleans everything up for you
            return 0;
        case ('i'):
            integrityScan->Demand();
            break;
        case ('e'):
            exceptionScan->Demand();
            break;
        case ('c'):
        {
            ControlRelayOutputBlock crob(OperationType::LATCH_ON);
            master->SelectAndOperate(crob, 0, PrintingCommandResultCallback::Get());
            break;
        }
        default:
            std::cout << "Unknown action: " << cmd << std::endl;
            break;
        }
    } while (true);

    return 0;
}
