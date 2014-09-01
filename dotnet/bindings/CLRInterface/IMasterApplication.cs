
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
// you under the terms of the License.
//

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Master application code implements this interface to interface with stack
    /// </summary>
    public interface IMasterApplication
    {
        UInt64 GetMillisecondsSinceEpoch();
        void OnReceiveIIN(IINField iin);
    }

    /// <summary>
    /// A default master application instance for demo purposes
    /// </summary>
    public class DefaultMasterApplication : IMasterApplication
    {
        private static IMasterApplication instance = new DefaultMasterApplication();

        private static DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, 0);

        public static IMasterApplication Instance
        {
            get
            {
                return instance;
            }
        }

        private DefaultMasterApplication() { }

        UInt64 IMasterApplication.GetMillisecondsSinceEpoch()
        {
            var ticks = DateTime.Now.ToUniversalTime().Subtract(epoch).Ticks;
            return (UInt64) (ticks / TimeSpan.TicksPerMillisecond);
        }

        void IMasterApplication.OnReceiveIIN(IINField iin)
        {
            if (iin.IsSet(IINBit.ALL_STATIONS))
                Console.WriteLine("All Stations");
            if (iin.IsSet(IINBit.CLASS1_EVENTS))
                Console.WriteLine("Class 1 Events");
            if (iin.IsSet(IINBit.CLASS2_EVENTS))
                Console.WriteLine("Class 2 Events");
            if (iin.IsSet(IINBit.CLASS3_EVENTS))
                Console.WriteLine("Class 3 Events");
            if (iin.IsSet(IINBit.NEED_TIME))
                Console.WriteLine("Need Time");
            if (iin.IsSet(IINBit.LOCAL_CONTROL))
                Console.WriteLine("Local Control");
            if (iin.IsSet(IINBit.DEVICE_TROUBLE))
                Console.WriteLine("Device Trouble");
            if (iin.IsSet(IINBit.DEVICE_RESTART))
                Console.WriteLine("Device Restart");
            if (iin.IsSet(IINBit.FUNC_NOT_SUPPORTED))
                Console.WriteLine("Function Not Supported");
            if (iin.IsSet(IINBit.OBJECT_UNKNOWN))
                Console.WriteLine("Object Unknown");
            if (iin.IsSet(IINBit.PARAM_ERROR))
                Console.WriteLine("Param Error");
            if (iin.IsSet(IINBit.EVENT_BUFFER_OVERFLOW))
                Console.WriteLine("Buffer Overflow");
            if (iin.IsSet(IINBit.ALREADY_EXECUTING))
                Console.WriteLine("Already Executing");
            if (iin.IsSet(IINBit.CONFIG_CORRUPT))
                Console.WriteLine("Config Corrupt");
            if (iin.IsSet(IINBit.RESERVED1))
                Console.WriteLine("Reserved 1");
            if (iin.IsSet(IINBit.RESERVED2))
                Console.WriteLine("Reserved 2");
        }
    }
}
