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

class DataObserverImpl implements DataObserver {

    private final long nativeptr;

    public DataObserverImpl(long nativeptr)
    {
        this.nativeptr = nativeptr;
    }

    public void start()
    {
        this.native_start(nativeptr);
    }

    public void update(BinaryInput meas, long index)
    {
        this.native_update_bi(nativeptr, meas.getValue(), meas.getQuality(), meas.getMsSinceEpoch(), index);
    }

    public void update(AnalogInput meas, long index)
    {
        this.native_update_ai(nativeptr, meas.getValue(), meas.getQuality(), meas.getMsSinceEpoch(), index);
    }

    public void update(Counter meas, long index)
    {
        this.native_update_c(nativeptr, meas.getValue(), meas.getQuality(), meas.getMsSinceEpoch(), index);
    }

    public void update(BinaryOutputStatus meas, long index)
    {
        this.native_update_bos(nativeptr, meas.getValue(), meas.getQuality(), meas.getMsSinceEpoch(), index);
    }

    public void update(AnalogOutputStatus meas, long index)
    {
        this.native_update_aos(nativeptr, meas.getValue(), meas.getQuality(), meas.getMsSinceEpoch(), index);
    }

    public void end()
    {
        this.native_end(nativeptr);
    }

    private native void native_start(long nativeptr);
    private native void native_update_bi(long nativeptr, boolean value, byte quality, long time, long index);
    private native void native_update_ai(long nativeptr, double value, byte quality, long time, long index);
    private native void native_update_c(long nativeptr, long value, byte quality, long time, long index);
    private native void native_update_bos(long nativeptr, boolean value, byte quality, long time, long index);
    private native void native_update_aos(long nativeptr, double value, byte quality, long time, long index);
    private native void native_end(long nativeptr);
}
