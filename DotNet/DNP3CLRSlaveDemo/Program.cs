
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

namespace DotNetSlaveDemo
{
    class RejectingCommandHandler : ICommandHandler {


        CommandStatus ICommandHandler.Select(BinaryOutput command, uint index, byte aSequence)
        {
            return CommandStatus.CS_NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Select(Setpoint command, uint index, byte aSequence)
        {
            return CommandStatus.CS_NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Operate(BinaryOutput command, uint index, byte aSequence)
        {
            return CommandStatus.CS_NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.Operate(Setpoint command, uint index, byte aSequence)
        {
            return CommandStatus.CS_NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.DirectOperate(BinaryOutput command, uint index)
        {
            return CommandStatus.CS_NOT_SUPPORTED;
        }

        CommandStatus ICommandHandler.DirectOperate(Setpoint command, uint index)
        {
            return CommandStatus.CS_NOT_SUPPORTED;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            var mgr = new DNP3Manager(1);
            mgr.AddLogHandler(new PrintingLogAdapter()); //this is optional
            var channel = mgr.AddTCPServer("server", FilterLevel.LEV_INFO, 5000, "127.0.0.1", 20000);
            var config = new SlaveStackConfig();
            var outstation = channel.AddOutstation("outstation", FilterLevel.LEV_INFO, new RejectingCommandHandler(), config);
            var publisher = outstation.GetDataObserver();

            Console.WriteLine("Press <Enter> to randomly change a value");

            Random r = new Random();
            while (true)
            {
                Console.ReadLine();
                int value = r.Next(UInt16.MaxValue);
                System.Console.WriteLine("Change Analog 0 to: " + value);
                publisher.Start();
                publisher.Update(new Analog(value, 1, DateTime.Now), 0);
                publisher.End();
            }
        }
    }
}
