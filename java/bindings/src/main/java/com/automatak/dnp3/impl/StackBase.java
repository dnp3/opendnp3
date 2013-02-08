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
package com.automatak.dnp3.impl;


import com.automatak.dnp3.LogLevel;
import com.automatak.dnp3.Stack;
import com.automatak.dnp3.VTOEndpoint;
import com.automatak.dnp3.VTOEndpointConfig;

abstract class StackBase implements Stack {

    private final long nativeStackPtr;

    public StackBase(long nativeStackPtr)
    {
        this.nativeStackPtr = nativeStackPtr;
    }

    @Override
    public VTOEndpoint addTCPClientVTOEndpoint(String loggerId, LogLevel level, String host, int port, VTOEndpointConfig config)
    {
        long ptr = get_tcpclient_vto_endpoint(nativeStackPtr, loggerId, level, host, port, config.channelId, config.minOpenRetryMs, config.maxOpenRetryMs, config.startLocal, config.disableExtenstion);
        return new VTOEndpointImpl(ptr);
    }

    @Override
    public VTOEndpoint addTCPServerVTOEndpoint(String loggerId, LogLevel level, String adapter, int port, VTOEndpointConfig config)
    {
        long ptr = get_tcpserver_vto_endpoint(nativeStackPtr, loggerId, level, adapter, port, config.channelId, config.minOpenRetryMs, config.maxOpenRetryMs, config.startLocal, config.disableExtenstion);
        return new VTOEndpointImpl(ptr);
    }

    private native long get_tcpclient_vto_endpoint(long nativePtr, String loggerId, LogLevel level, String host, int port, byte channelId, long minOpenRetryMs, long maxOpenRetryMs, boolean startLocal, boolean disableExtenstion);
    private native long get_tcpserver_vto_endpoint(long nativePtr, String loggerId, LogLevel level, String adapter, int port, byte channelId, long minOpenRetryMs, long maxOpenRetryMs, boolean startLocal, boolean disableExtenstion);
}
