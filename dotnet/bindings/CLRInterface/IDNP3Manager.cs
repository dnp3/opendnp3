
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
    /// <summary>
    /// Root interface used for all dnp3 applications
    /// </summary>
    public interface IDNP3Manager
    {
        /// <summary>
        /// Force a blocking shutdown. All channels and stacks are invalidated.
        /// </summary>
        void Shutdown();

		/// <summary>
        /// Add a TCP client channel. The channel does not try to connect until you add a stack.
		/// </summary>
        /// <param name="id">An id used for logging purposes</param>
        /// <param name="filters">The starting level for logging output</param>
        /// <param name="minRetryDelay">Minimum timespan between connection attempts</param>
        /// <param name="maxRetryDelay">Maximum timespan between connection attempts</param>
        /// <param name="address">The address of remote host as a INET address i.e. "127.0.0.1" or name "www.google.com"</param>
        /// <param name="port">The port to make the connection on</param>
        /// <returns>A channel interface</returns>
        IChannel AddTCPClient(String id, UInt32 filters, TimeSpan minRetryDelay, TimeSpan maxRetryDelay, String address, UInt16 port);

        /// <summary>
        /// Add a TCP server channel. The channel does not try to listen until you add a stack.
        /// </summary>
        /// <param name="id">An id used for logging purposes</param>
        /// <param name="filters">The starting level for logging output</param>
        /// <param name="minRetryDelay">Minimum timespan between connection attempts</param>
        /// <param name="maxRetryDelay">Maximum timespan between connection attempts</param>
        /// <param name="endpoint">The address that identifies the network adapter to bind i.e. "127.0.0.1" or "0.0.0.0"</param>
        /// <param name="port">The port to listen on</param>
        /// <returns>A channel interface</returns>
        IChannel AddTCPServer(String id, UInt32 filters, TimeSpan minRetryDelay, TimeSpan maxRetryDelay, String endpoint, UInt16 port);

        /// <summary>
        /// Add a serial channel. The port does not try to open until you add a stack.
        /// </summary>
        /// <param name="id">An id used for logging purposes</param>
        /// <param name="filters">The starting level for logging output</param>
        /// <param name="minRetryDelay">Minimum timespan between connection attempts</param>
        /// <param name="maxRetryDelay">Maximum timespan between connection attempts</param>
        /// <param name="settings">Configuration struct for the serial port</param>        
        /// <returns>A channel interface</returns>
        IChannel AddSerial(String id, UInt32 filters, TimeSpan minRetryDelay, TimeSpan maxRetryDelay, SerialSettings settings);

        /// <summary>
        /// Subscribe to all log messages
        /// </summary>
        /// <param name="logHandler">Callback interface</param>
		void AddLogHandler(ILogHandler logHandler);		
    }
}
