
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
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Adapter;
using DNP3.Interface;

namespace DotNetMasterDemo
{

    class PrintingDataObserver : IDataObserver
    {
        public void End()
        {
            Console.WriteLine("End");
        }

        public void Start()
        {
            Console.WriteLine("Start");
        }

        public void Update(Binary update, System.UInt32 index)
        {
            Console.WriteLine(update.value);
        }

        public void Update(Analog update, System.UInt32 index)
        {
            Console.WriteLine(update.value);
        }

        public void Update(Counter update, System.UInt32 index)
        {
            Console.WriteLine(update.value);
        }

        public void Update(ControlStatus update, System.UInt32 index)
        {
            Console.WriteLine(update.value);
        }

        public void Update(SetpointStatus update, System.UInt32 index)
        {
            Console.WriteLine(update.value);
        }
    }    

    class Program
    {
        static void Main(string[] args)
        {
            var mgr = new DNP3Manager(Environment.ProcessorCount);            
            mgr.AddLogHandler(new PrintingLogAdapter()); //this is optional
            var channel = mgr.AddTCPClient("client", LogLevel.INFO, 5000, "127.0.0.1", 20000);
            var config = new MasterStackConfig();
            config.link.useConfirms = true; //setup your stack configuration here.
            var master = channel.AddMaster("master", LogLevel.INFO, new PrintingDataObserver(), config);

            Console.WriteLine("Enter an index to send a command");

            while (true)
            {
                System.UInt32 index = System.UInt32.Parse(Console.ReadLine());
                DateTime start = DateTime.Now;
                var future = master.GetCommandProcessor().SelectAndOperate(new ControlRelayOutputBlock(ControlCode.CC_PULSE, 1, 100, 100), index);
                CommandStatus result = future.Await();
                DateTime end = DateTime.Now;
                TimeSpan duration = end - start;                
                Console.WriteLine("Result: " + result + " and took " + duration.Ticks + " Ticks");
            }            
        }
    }
}
