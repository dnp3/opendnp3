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

import com.automatak.dnp3.*;

class ManagerImpl implements DNP3Manager {

    private long nativePointer;

    public ManagerImpl(int concurrency)
    {
        this.nativePointer = create_native_manager(concurrency);
    }

    public void addLogSubscriber(LogSubscriber sub)
    {
         native_add_log_subscriber(nativePointer, sub);
    }

    public Channel addTCPClient(String name, LogLevel level, long retryMs, String address, int port)
    {
        long ptr = get_native_channel_tcp_client(nativePointer, name, level, retryMs, address, port);
        return new ChannelImpl(ptr);
    }

    public Channel addTCPServer(String name, LogLevel level, long retryMs, String endpoint, int port)
    {
        long ptr = get_native_channel_tcp_server(nativePointer, name, level, retryMs, endpoint, port);
        return new ChannelImpl(ptr);
    }

    public Channel addSerial(String id, LogLevel level, long retryMs, SerialSettings settings)
    {
         long ptr = get_native_channel_serial(
                 nativePointer,
                 id,
                 level,
                 retryMs,
                 settings.port,
                 settings.baudRate,
                 settings.dataBits,
                 settings.parity.toInt(),
                 settings.stopBits,
                 settings.flowControl.toInt()
         );
        return new ChannelImpl(ptr);
    }

    public void shutdown()
    {
        if(nativePointer != 0) {
            destroy_native_manager(nativePointer);
            nativePointer = 0;
        }
    }

    private native long create_native_manager(int concurrency);
    private native void destroy_native_manager(long ptr);

    private native long get_native_channel_tcp_client(long ptrManager, String name, LogLevel level, long retryMs, String address, int port);
    private native long get_native_channel_tcp_server(long ptrManager, String name, LogLevel level, long retryMs, String endpoint, int port);
    private native long get_native_channel_serial(long ptrManager, String name, LogLevel level, long retryMs, String port, int baudRate, int dataBits, int parity, int stopBits, int flowControl);

    private native void native_add_log_subscriber(long ptrManager, LogSubscriber sub);

}
