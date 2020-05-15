// Copyright 2013-2020 Automatak, LLC
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements. 
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
using System;

using Automatak.DNP3.Interface;
using Automatak.DNP3.Adapter;

namespace DotNetMasterGPRSDemo
{
    class Program
    {
        static int Main(string[] args)
        {
            var manager = DNP3ManagerFactory.CreateManager(4, new PrintingLogAdapter());

            var listener = manager.CreateListener("listener", LogLevels.ALL, IPEndpoint.Localhost(20000), new DefaultListenCallbacks());

            Console.WriteLine("Enter a command");

            while (true)
            {
                switch (Console.ReadLine())
                {                    
                    case "x":                        
                        // The manager does not automatically reclaim resources for stop executing threads in a finalizer
                        // Be sure to call this before exiting to cleanup all resources.
                        manager.Shutdown();
                        return 0;
                    default:
                        break;
                }
            }
        }
    }
}
