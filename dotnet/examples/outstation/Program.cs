
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

namespace DotNetOutstationDemo
{   
    class Program
    {
        static void Main(string[] args)
        {
            IDNP3Manager mgr = DNP3ManagerFactory.CreateManager();
            mgr.AddLogHandler(PrintingLogAdapter.Instance); //this is optional
            var channel = mgr.AddTCPServer("server", LogLevels.NORMAL, TimeSpan.FromSeconds(5), TimeSpan.FromSeconds(5), "0.0.0.0", 20000);

            // Optional: add a listener for the channel state
            channel.AddStateListener(state => Console.WriteLine("channel state: " + state));     

            var config = new OutstationStackConfig();

            // Optional: overide the default reporting variations
            config.outstation.eventBinary = EventBinaryResponse.Group2Var2;

            // Optional: setup your stack configuration here
            config.link.localAddr = 10;
            config.link.remoteAddr = 1;

            var outstation = channel.AddOutstation("outstation", RejectingCommandHandler.Instance, PrintingTimeWriteHandler.Instance, config);

            outstation.Enable(); // enable communications

            Console.WriteLine("Press <Enter> to randomly change a value");
            var database = outstation.GetDatabase();
            bool value = false;
            while (true)
            {
                Console.ReadLine();
                value = !value;
                System.Console.WriteLine("Change Binary 1 to: " + value);
                database.Start();
                database.Update(new Binary(value, 1, DateTime.Now), 0);
                database.End();
            }
        }
    }
}
