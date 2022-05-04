// Copyright 2013-2022 Step Function I/O, LLC
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
// LLC (https://stepfunc.io) under one or more contributor license agreements. 
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// An address-port pair
    /// </summary>
    public class IPEndpoint
    {
        public IPEndpoint(string address, UInt16 port)
        {
            this.address = address;
            this.port = port;
        }

        public static IPEndpoint AddAdapters(UInt16 port)
        {
            return new IPEndpoint("0.0.0.0", port);
        }

        public static IPEndpoint Localhost(UInt16 port)
        {
            return new IPEndpoint("127.0.0.1", port);
        }

        public readonly string address;
        public readonly UInt16 port;
    }
}
