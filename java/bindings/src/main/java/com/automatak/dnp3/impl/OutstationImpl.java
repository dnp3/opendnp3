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

import com.automatak.dnp3.DataObserver;
import com.automatak.dnp3.Outstation;

class OutstationImpl extends StackBase implements Outstation {

    private long nativePointer;
    private final DataObserver obs;

    public OutstationImpl(long nativePointer)
    {
        super(nativePointer);
        this.nativePointer = nativePointer;
        this.obs = new DataObserverImpl(get_native_data_observer(nativePointer));
    }

    @Override
    public DataObserver getDataObserver()
    {
        return obs;
    }

    @Override
    public void shutdown()
    {
        shutdown_native(nativePointer);
    }

    private native void shutdown_native(long nativePointer);
    private native long get_native_data_observer(long nativePointer);
}
