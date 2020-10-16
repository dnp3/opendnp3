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
package com.automatak.dnp3.impl;

import com.automatak.dnp3.*;
import com.automatak.dnp3.enums.*;
import org.junit.Test;

public class ToStringTest {

    @Test
    public void binaryInput() {
        new BinaryInput(true, new Flags((byte)0x80), new DNPTime(200)).toString();
    }

    @Test
    public void doubleBitBinary() {
        new DoubleBitBinaryInput(DoubleBit.DETERMINED_ON, new Flags((byte)0x80), new DNPTime(200)).toString();
    }

    @Test
    public void analogInput() {
        new AnalogInput(41.0, new Flags((byte)0x80), new DNPTime(200)).toString();
    }

    @Test
    public void counter() {
        new Counter(41, new Flags((byte)0x80), new DNPTime(200)).toString();
    }

    @Test
    public void frozenCounter() {
        new FrozenCounter(41, new Flags((byte)0x80), new DNPTime(200)).toString();
    }

    @Test
    public void binaryOutputStatusQuality() {
        new BinaryOutputStatus(true, new Flags((byte)0x80), new DNPTime(200)).toString();
    }

    @Test
    public void analogOutputStatus() {
        new AnalogOutputStatus(41.0, new Flags((byte)0x80), new DNPTime(200)).toString();
    }

}
