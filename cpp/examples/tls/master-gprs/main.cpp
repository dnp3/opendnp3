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
#include <opendnp3/logging/LogLevels.h>
#include <opendnp3/master/DefaultListenCallbacks.h>

#include <iostream>
#include <thread>

using namespace std;
using namespace opendnp3;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "usage: master-gprs-tls-demo <ca certificate> <certificate chain> <private key>" << std::endl;
        return -1;
    }

    std::string caCertificate(argv[1]);
    std::string certificateChain(argv[2]);
    std::string privateKey(argv[3]);

    std::cout << "Using CA certificate: " << caCertificate << std::endl;
    std::cout << "Using certificate chain: " << certificateChain << std::endl;
    std::cout << "Using private key file: " << privateKey << std::endl;

    // Specify what log levels to use. NORMAL is warning and above
    // You can add all the comms logging by uncommenting below
    const auto logLevels = levels::NORMAL | levels::ALL_COMMS;

    const auto numThread = std::thread::hardware_concurrency();

    auto callbacks = std::make_shared<DefaultListenCallbacks>();

    // This is the main point of interaction with the stack
    DNP3Manager manager(numThread, ConsoleLogger::Create());

    auto server1 = manager.CreateListener("server-20001", logLevels, IPEndpoint::AllAdapters(20001),
                                          TLSConfig(caCertificate, certificateChain, privateKey), callbacks);

    do
    {
        std::cout << "Enter a command" << std::endl;
        std::cout << "x - exits program" << std::endl;

        char cmd;
        std::cin >> cmd;
        switch (cmd)
        {
        case ('x'):
            return 0;
        default:
            std::cout << "Unknown action: " << cmd << std::endl;
            break;
        }
    } while (true);

    return 0;
}
