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

class ChannelImpl implements Channel {

    private long nativeChannel;

    public ChannelImpl(long nativeChannel)
    {
        this.nativeChannel = nativeChannel;
    }

    @Override
    public void addStateListener(ChannelStateListener listener)
    {
       ChannelStateProxy proxy = new ChannelStateProxy(listener);
       add_native_state_change_listener(nativeChannel, proxy);
    }

    @Override
    public Master addMaster(String loggerId, LogLevel level, DataObserver publisher, MasterStackConfig config)
    {
        DataObserverAdapter adapter = new DataObserverAdapter(publisher);
        long ptr = get_native_master(nativeChannel, loggerId, level.toType(), adapter, config);
        return new MasterImpl(ptr);
    }

    @Override
    public Outstation addOutstation(String loggerId, LogLevel level, CommandHandler cmdHandler, OutstationStackConfig config)
    {
        CommandHandlerAdapter adapter = new CommandHandlerAdapter(cmdHandler);
        long ptr = get_native_slave(nativeChannel, loggerId, level.toType(), adapter, config);
        return new OutstationImpl(ptr);
    }

    @Override
    public void shutdown()
    {
        shutdown_native(nativeChannel);
    }

    private native void shutdown_native(long ptrChannel);
    private native void add_native_state_change_listener(long ptrChannel, ChannelStateProxy proxy);
    private native long get_native_master(long ptrChannel, String loggerId, int level, DataObserverAdapter publisher, MasterStackConfig config);
    private native long get_native_slave(long ptrChannel, String loggerId, int level, CommandHandlerAdapter handler, OutstationStackConfig config);


}
