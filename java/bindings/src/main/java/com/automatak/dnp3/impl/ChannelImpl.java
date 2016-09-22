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

    private long nativePointer;

    public ChannelImpl(long nativePointer)
    {
        this.nativePointer = nativePointer;
    }

    @Override
    public Master addMaster(String id, SOEHandler handler, MasterApplication application, MasterStackConfig config)
    {
        throw new RuntimeException("unable to add master!");
    }

    @Override
    public void shutdown()
    {
        throw new RuntimeException();
    }

    /*
    private native void shutdown_native(long nativePointer);
    private native long get_native_master(long nativePointer, String id, SOEHandler handler, MasterApplication application, MasterStackConfig config);
    */
}
