
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
using System.Runtime.CompilerServices;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Root interface used for master-server style applications
    /// </summary>
    public interface IDNP3SessionManager
    {
        /// <summary>
        /// Blocking shutdown. Call this before exiting.
        /// </summary>
        void Shutdown();

        /// <summary>
        /// Create a new TCP listener an bind it to the specified callback interface
        /// </summary>
        /// <param name="loggerid"></param>
        /// <param name="filters"></param>
        /// <param name="endpoint"></param>
        /// <param name="callbacks"></param>
        /// <returns>A listener which can be canceled</returns>        
        IListener CreateListener(string loggerid, UInt32 filters, IPEndpoint endpoint, IListenCallbacks callbacks);

        /// <summary>
        /// Create a new TLS listener an bind it to the specified callback interface
        /// </summary>
        /// <param name="loggerid"></param>
        /// <param name="filters"></param>
        /// <param name="endpoint"></param>
        /// <param name="config"></param>
        /// <param name="callbacks"></param>
        /// <returns>A listener which can be canceled</returns>        
        IListener CreateListener(string loggerid, UInt32 filters, IPEndpoint endpoint, TLSConfig config, IListenCallbacks callbacks);
    }
}
