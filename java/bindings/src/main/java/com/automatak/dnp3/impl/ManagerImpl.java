/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3.impl;

import com.automatak.dnp3.*;
import sun.reflect.generics.reflectiveObjects.NotImplementedException;

class ManagerImpl implements DNP3Manager {

    private long nativePointer;

    public ManagerImpl(int concurrency, LogHandler handler)
    {
        throw new NotImplementedException();
        //this.nativePointer = create_native_manager(concurrency);
    }

    @Override
    public Channel addTCPClient(String id, int levels, ChannelRetry retry, String address, int port)
    {
        throw new NotImplementedException();

        /*
        long ptr = get_native_channel_tcp_client(nativePointer, name, level.toInt(), retryMs, address, port);
        return new ChannelImpl(ptr);
        */
    }

    @Override
    public void shutdown()
    {
        throw new NotImplementedException();
    }

    /*
    private native long create_native_manager(int concurrency);
    private native void destroy_native_manager(long ptr);

    private native long get_native_channel_tcp_client(long ptrManager, String name, int level, long retryMs, String address, int port);
    private native long get_native_channel_tcp_server(long ptrManager, String name, int level, long retryMs, String endpoint, int port);
    private native long get_native_channel_serial(long ptrManager, String name, int level, long retryMs, String port, int baudRate, int dataBits, int parity, int stopBits, int flowControl);

    private native void native_add_log_subscriber(long ptrManager, LogSubscriber sub);
    */
}
