/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
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
package com.automatak.dnp3;

import com.automatak.dnp3.enums.BinaryQuality;
import com.automatak.dnp3.enums.DoubleBitBinaryQuality;
import com.automatak.dnp3.enums.AnalogQuality;
import com.automatak.dnp3.enums.CounterQuality;
import com.automatak.dnp3.enums.FrozenCounterQuality;
import com.automatak.dnp3.enums.BinaryOutputStatusQuality;
import com.automatak.dnp3.enums.AnalogOutputStatusQuality;

public class Flags {
    private byte value;

    public Flags() {
        this((byte)0);
    }

    public Flags(byte value) {
        this.value = value;
    }

    public byte getValue() {
        return value;
    }

    public boolean isSet(BinaryQuality flag) {
        return (this.value & (byte)flag.toType()) != 0;
    }

    public boolean isSet(DoubleBitBinaryQuality flag) {
        return (this.value & (byte)flag.toType()) != 0;
    }

    public boolean isSet(AnalogQuality flag) {
        return (this.value & (byte)flag.toType()) != 0;
    }

    public boolean isSet(CounterQuality flag) {
        return (this.value & (byte)flag.toType()) != 0;
    }

    public boolean isSet(FrozenCounterQuality flag) {
        return (this.value & (byte)flag.toType()) != 0;
    }

    public boolean isSet(BinaryOutputStatusQuality flag) {
        return (this.value & (byte)flag.toType()) != 0;
    }

    public boolean isSet(AnalogOutputStatusQuality flag) {
        return (this.value & (byte)flag.toType()) != 0;
    }

    public void set(BinaryQuality flag) {
        this.value |= (byte)flag.toType();
    }

    public void set(DoubleBitBinaryQuality flag) {
        this.value |= (byte)flag.toType();
    }

    public void set(AnalogQuality flag) {
        this.value |= (byte)flag.toType();
    }

    public void set(CounterQuality flag) {
        this.value |= (byte)flag.toType();
    }

    public void set(FrozenCounterQuality flag) {
        this.value |= (byte)flag.toType();
    }

    public void set(BinaryOutputStatusQuality flag) {
        this.value |= (byte)flag.toType();
    }

    public void set(AnalogOutputStatusQuality flag) {
        this.value |= (byte)flag.toType();
    }
}
