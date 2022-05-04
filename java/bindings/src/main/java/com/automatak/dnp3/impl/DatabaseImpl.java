/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.automatak.dnp3.impl;


import com.automatak.dnp3.*;
import com.automatak.dnp3.enums.EventMode;

public class DatabaseImpl implements Database {

    // return a pointer to a native C++ ChangeSet (implements opendnp3::IDatabase)
    public static native long new_update_builder_native();
    // free the ChangeSet
    public static native void delete_update_builder_native(long nativePointer);

    private final long nativeDatabase;

    public DatabaseImpl(long nativeDatabase)
    {
        this.nativeDatabase = nativeDatabase;
    }

    @Override
    public void update(BinaryInput value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }

    @Override
    public void update(DoubleBitBinaryInput value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }

    @Override
    public void update(AnalogInput value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }

    @Override
    public void update(Counter value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }

    @Override
    public void freezeCounter(int index, boolean clear)
    {
        this.freezeCounter(index, clear, EventMode.Detect);
    }

    @Override
    public void update(BinaryOutputStatus value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }

    @Override
    public void update(AnalogOutputStatus value, int index)
    {
        this.update(value, index, EventMode.Detect);
    }


    @Override
    public void update(BinaryInput value, int index, EventMode mode)
    {
        this.update_binary_native(this.nativeDatabase, value.value, value.quality.getValue(), value.timestamp, index, mode.toType());
    }

    @Override
    public void update(DoubleBitBinaryInput value, int index, EventMode mode)
    {
        this.update_double_binary_native(this.nativeDatabase, value.value.toType(), value.quality.getValue(), value.timestamp, index, mode.toType());
    }

    @Override
    public void update(AnalogInput value, int index, EventMode mode)
    {
        this.update_analog_native(this.nativeDatabase, value.value, value.quality.getValue(), value.timestamp, index, mode.toType());
    }

    @Override
    public void update(Counter value, int index, EventMode mode)
    {
        this.update_counter_native(this.nativeDatabase, value.value, value.quality.getValue(), value.timestamp, index, mode.toType());
    }

    @Override
    public void freezeCounter(int index, boolean clear, EventMode mode)
    {
        this.freeze_counter_native(this.nativeDatabase, index, clear, mode.toType());
    }

    @Override
    public void update(BinaryOutputStatus value, int index, EventMode mode)
    {
        this.update_bo_status_native(this.nativeDatabase, value.value, value.quality.getValue(), value.timestamp, index, mode.toType());
    }

    @Override
    public void update(AnalogOutputStatus value, int index, EventMode mode)
    {
        this.update_ao_status_native(this.nativeDatabase, value.value, value.quality.getValue(), value.timestamp, index, mode.toType());
    }

    private native void update_binary_native(long nativePointer, boolean value, byte flags, DNPTime time, int index, int mode);
    private native void update_double_binary_native(long nativePointer, int value, byte flags, DNPTime time, int index, int mode);
    private native void update_analog_native(long nativePointer, double value, byte flags, DNPTime time, int index, int mode);
    private native void update_counter_native(long nativePointer, long value, byte flags, DNPTime time, int index, int mode);
    private native void freeze_counter_native(long nativePointer, int index, boolean clear, int mode);
    private native void update_bo_status_native(long nativePointer, boolean value, byte flags, DNPTime time, int index, int mode);
    private native void update_ao_status_native(long nativePointer, double value, byte flags, DNPTime time, int index, int mode);
}
