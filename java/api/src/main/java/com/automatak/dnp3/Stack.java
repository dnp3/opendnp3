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
 * Represents a Outstation or Master. Can be shutdown and can have Virtual Terminal Object (VTO) endpoints bound to it.
 */
public interface Stack {

    /**
     * Permanently shutdown the stack. Calling shutdown() more than once or continuing to use any child objects
     * of the stack can cause a failure.
     */
    void shutdown();

    /**
     * Add a listener for changes to the stack state. All callbacks come from the thread pool.
     * An immediate callback will be made with the current state.
     * @param listener interface to call back with the state enumeration
     */
    void addStateListener(StackStateListener listener);

    /**
     * Associate a TCP Client VTO endpoint with stack
     * @param loggerId Id used for logging
     * @param level Log level for the endpoint
     * @param host host, i.e. 127.0.0.1 or www.google.com
     * @param port port number to connect to
     * @param config Settings that define how the endpoint will behave
     * @return Interface that can be used to shutdown the endpoint
     */
    VTOEndpoint addTCPClientVTOEndpoint(String loggerId, LogLevel level, String host, int port, VTOEndpointConfig config);

    /**
     * Associate a TCP Server VTO endpoint with stack
     * @param loggerId Id used for logging
     * @param level Log level for the endpoint
     * @param adapter adapter, i.e. 127.0.0.1 or 0.0.0.0
     * @param port port number to listen on
     * @param config Settings that define how the endpoint will behave
     * @return Interface that can be used to shutdown the endpoint
     */
    VTOEndpoint addTCPServerVTOEndpoint(String loggerId, LogLevel level, String adapter, int port, VTOEndpointConfig config);

}
