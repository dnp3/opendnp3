/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the GNU Affero General Public License
 * Version 3.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/agpl.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3;

/**
 * The main entry point for all dnp3 applications. Use this object to create communication channels to which
 * you can then add masters and outstations.
 *
 * To obtain a concrete implementation of this interface, use the DNP3ManagerFactory class in the impl package.
 *
 */
public interface DNP3Manager {

    /**
     * Add a TCP client channel. The channel does not try to connect until you add a stack.
     * @param id An id used for logging purposes
     * @param level The starting level for logging output
     * @param retryMs The number of milliseconds in between connection attempts
     * @param address The address of remote host as a INET address i.e. "127.0.0.1" or name "www.google.com"
     * @param port The port to make the connection on. Note that only the range 0 to 65535 is valid
     * @return A channel interface
     */
    Channel addTCPClient(String id, LogLevel level, long retryMs, String address, int port);

    /**
     * Add a TCP server channel. The channel does not try to listen for a connection until you add a stack.
     * @param id An id used for logging purposes
     * @param level The starting level for logging output
     * @param retryMs The number of milliseconds in between connection attempts (in the case of a bind failure)
     * @param endpoint The address that identifies the network adapter to bind i.e. "127.0.0.1" or "0.0.0.0"
     * @param port The port to listen on. Note that only the range 0 to 65535 is valid
     * @return A channel interface
     */
    Channel addTCPServer(String id, LogLevel level, long retryMs, String endpoint, int port);

    /**
     * Subscribe to all log messages
     * @param sub An interface with which to listen
     */
    void addLogSubscriber(LogSubscriber sub);

    /**
     * Permanently shutdown all channels and any sub-objects (stacks, etc)
     */
    void shutdown();
}
