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
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{   
    public class DefaultListenCallbacks : IListenCallbacks
    {
        readonly Object mutex = new Object();
        UInt64 sessionCount = 0;

        public bool AcceptConnection(string ipaddress)
        {
            return true;
        }

        public TimeSpan GetFirstFrameTimeout()
        {
            return TimeSpan.FromSeconds(30);
        }

        public void OnFirstFrame(LinkHeader header, ISessionAcceptor acceptor)
        {
            var config = new MasterStackConfig();

            config.link.remoteAddr = header.Source;
            config.link.localAddr = header.Destination;
                        
            var master = acceptor.AcceptSession(GetNextSessionId(), PrintingSOEHandler.Instance, DefaultMasterApplication.Instance, config);            
        }

        public void OnSessionClose(IMasterSession session, string sessionid)
        {
            Console.WriteLine(String.Format("Session closed: %s", sessionid));
        }

        private string GetNextSessionId()
        { 
            lock (mutex)
            {
                var ret = String.Format("Session{0}", sessionCount);
                ++sessionCount;
                return ret;
            }   
        }
    }
}
