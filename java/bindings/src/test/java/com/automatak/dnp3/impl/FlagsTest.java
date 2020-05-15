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

import com.automatak.dnp3.Flags;
import com.automatak.dnp3.enums.BinaryQuality;
import com.automatak.dnp3.enums.DoubleBitBinaryQuality;
import com.automatak.dnp3.enums.AnalogQuality;
import com.automatak.dnp3.enums.CounterQuality;
import com.automatak.dnp3.enums.FrozenCounterQuality;
import com.automatak.dnp3.enums.BinaryOutputStatusQuality;
import com.automatak.dnp3.enums.AnalogOutputStatusQuality;
import junit.framework.Assert;
import org.junit.Test;

public class FlagsTest {

    @Test
    public void binaryQuality() {
        Flags flags = new Flags();

        flags.set(BinaryQuality.ONLINE);
        Assert.assertTrue(flags.isSet(BinaryQuality.ONLINE));

        flags.set(BinaryQuality.RESTART);
        Assert.assertTrue(flags.isSet(BinaryQuality.RESTART));

        flags.set(BinaryQuality.COMM_LOST);
        Assert.assertTrue(flags.isSet(BinaryQuality.COMM_LOST));

        flags.set(BinaryQuality.REMOTE_FORCED);
        Assert.assertTrue(flags.isSet(BinaryQuality.REMOTE_FORCED));

        flags.set(BinaryQuality.LOCAL_FORCED);
        Assert.assertTrue(flags.isSet(BinaryQuality.LOCAL_FORCED));

        flags.set(BinaryQuality.CHATTER_FILTER);
        Assert.assertTrue(flags.isSet(BinaryQuality.CHATTER_FILTER));

        flags.set(BinaryQuality.RESERVED);
        Assert.assertTrue(flags.isSet(BinaryQuality.RESERVED));

        flags.set(BinaryQuality.STATE);
        Assert.assertTrue(flags.isSet(BinaryQuality.STATE));

        Assert.assertEquals((byte)0xFF, flags.getValue());
    }

    @Test
    public void doubleBitBinaryQuality() {
        Flags flags = new Flags();

        flags.set(DoubleBitBinaryQuality.ONLINE);
        Assert.assertTrue(flags.isSet(DoubleBitBinaryQuality.ONLINE));

        flags.set(DoubleBitBinaryQuality.RESTART);
        Assert.assertTrue(flags.isSet(DoubleBitBinaryQuality.RESTART));

        flags.set(DoubleBitBinaryQuality.COMM_LOST);
        Assert.assertTrue(flags.isSet(DoubleBitBinaryQuality.COMM_LOST));

        flags.set(DoubleBitBinaryQuality.REMOTE_FORCED);
        Assert.assertTrue(flags.isSet(DoubleBitBinaryQuality.REMOTE_FORCED));

        flags.set(DoubleBitBinaryQuality.LOCAL_FORCED);
        Assert.assertTrue(flags.isSet(DoubleBitBinaryQuality.LOCAL_FORCED));

        flags.set(DoubleBitBinaryQuality.CHATTER_FILTER);
        Assert.assertTrue(flags.isSet(DoubleBitBinaryQuality.CHATTER_FILTER));

        flags.set(DoubleBitBinaryQuality.STATE1);
        Assert.assertTrue(flags.isSet(DoubleBitBinaryQuality.STATE1));

        flags.set(DoubleBitBinaryQuality.STATE2);
        Assert.assertTrue(flags.isSet(DoubleBitBinaryQuality.STATE2));

        Assert.assertEquals((byte)0xFF, flags.getValue());
    }

    @Test
    public void analogQuality() {
        Flags flags = new Flags();

        flags.set(AnalogQuality.ONLINE);
        Assert.assertTrue(flags.isSet(AnalogQuality.ONLINE));

        flags.set(AnalogQuality.RESTART);
        Assert.assertTrue(flags.isSet(AnalogQuality.RESTART));

        flags.set(AnalogQuality.COMM_LOST);
        Assert.assertTrue(flags.isSet(AnalogQuality.COMM_LOST));

        flags.set(AnalogQuality.REMOTE_FORCED);
        Assert.assertTrue(flags.isSet(AnalogQuality.REMOTE_FORCED));

        flags.set(AnalogQuality.LOCAL_FORCED);
        Assert.assertTrue(flags.isSet(AnalogQuality.LOCAL_FORCED));

        flags.set(AnalogQuality.OVERRANGE);
        Assert.assertTrue(flags.isSet(AnalogQuality.OVERRANGE));

        flags.set(AnalogQuality.REFERENCE_ERR);
        Assert.assertTrue(flags.isSet(AnalogQuality.REFERENCE_ERR));

        flags.set(AnalogQuality.RESERVED);
        Assert.assertTrue(flags.isSet(AnalogQuality.RESERVED));

        Assert.assertEquals((byte)0xFF, flags.getValue());
    }

    @Test
    public void counterQuality() {
        Flags flags = new Flags();

        flags.set(CounterQuality.ONLINE);
        Assert.assertTrue(flags.isSet(CounterQuality.ONLINE));

        flags.set(CounterQuality.RESTART);
        Assert.assertTrue(flags.isSet(CounterQuality.RESTART));

        flags.set(CounterQuality.COMM_LOST);
        Assert.assertTrue(flags.isSet(CounterQuality.COMM_LOST));

        flags.set(CounterQuality.REMOTE_FORCED);
        Assert.assertTrue(flags.isSet(CounterQuality.REMOTE_FORCED));

        flags.set(CounterQuality.LOCAL_FORCED);
        Assert.assertTrue(flags.isSet(CounterQuality.LOCAL_FORCED));

        flags.set(CounterQuality.ROLLOVER);
        Assert.assertTrue(flags.isSet(CounterQuality.ROLLOVER));

        flags.set(CounterQuality.DISCONTINUITY);
        Assert.assertTrue(flags.isSet(CounterQuality.DISCONTINUITY));

        flags.set(CounterQuality.RESERVED);
        Assert.assertTrue(flags.isSet(CounterQuality.RESERVED));

        Assert.assertEquals((byte)0xFF, flags.getValue());
    }

    @Test
    public void frozenCounterQuality() {
        Flags flags = new Flags();

        flags.set(FrozenCounterQuality.ONLINE);
        Assert.assertTrue(flags.isSet(FrozenCounterQuality.ONLINE));

        flags.set(FrozenCounterQuality.RESTART);
        Assert.assertTrue(flags.isSet(FrozenCounterQuality.RESTART));

        flags.set(FrozenCounterQuality.COMM_LOST);
        Assert.assertTrue(flags.isSet(FrozenCounterQuality.COMM_LOST));

        flags.set(FrozenCounterQuality.REMOTE_FORCED);
        Assert.assertTrue(flags.isSet(FrozenCounterQuality.REMOTE_FORCED));

        flags.set(FrozenCounterQuality.LOCAL_FORCED);
        Assert.assertTrue(flags.isSet(FrozenCounterQuality.LOCAL_FORCED));

        flags.set(FrozenCounterQuality.ROLLOVER);
        Assert.assertTrue(flags.isSet(FrozenCounterQuality.ROLLOVER));

        flags.set(FrozenCounterQuality.DISCONTINUITY);
        Assert.assertTrue(flags.isSet(FrozenCounterQuality.DISCONTINUITY));

        flags.set(FrozenCounterQuality.RESERVED);
        Assert.assertTrue(flags.isSet(FrozenCounterQuality.RESERVED));

        Assert.assertEquals((byte)0xFF, flags.getValue());
    }

    @Test
    public void binaryOutputStatusQuality() {
        Flags flags = new Flags();

        flags.set(BinaryOutputStatusQuality.ONLINE);
        Assert.assertTrue(flags.isSet(BinaryOutputStatusQuality.ONLINE));

        flags.set(BinaryOutputStatusQuality.RESTART);
        Assert.assertTrue(flags.isSet(BinaryOutputStatusQuality.RESTART));

        flags.set(BinaryOutputStatusQuality.COMM_LOST);
        Assert.assertTrue(flags.isSet(BinaryOutputStatusQuality.COMM_LOST));

        flags.set(BinaryOutputStatusQuality.REMOTE_FORCED);
        Assert.assertTrue(flags.isSet(BinaryOutputStatusQuality.REMOTE_FORCED));

        flags.set(BinaryOutputStatusQuality.LOCAL_FORCED);
        Assert.assertTrue(flags.isSet(BinaryOutputStatusQuality.LOCAL_FORCED));

        flags.set(BinaryOutputStatusQuality.RESERVED1);
        Assert.assertTrue(flags.isSet(BinaryOutputStatusQuality.RESERVED1));

        flags.set(BinaryOutputStatusQuality.RESERVED2);
        Assert.assertTrue(flags.isSet(BinaryOutputStatusQuality.RESERVED2));

        flags.set(BinaryOutputStatusQuality.STATE);
        Assert.assertTrue(flags.isSet(BinaryOutputStatusQuality.STATE));

        Assert.assertEquals((byte)0xFF, flags.getValue());
    }

    @Test
    public void analogOutputStatusQuality() {
        Flags flags = new Flags();

        flags.set(AnalogOutputStatusQuality.ONLINE);
        Assert.assertTrue(flags.isSet(AnalogOutputStatusQuality.ONLINE));

        flags.set(AnalogOutputStatusQuality.RESTART);
        Assert.assertTrue(flags.isSet(AnalogOutputStatusQuality.RESTART));

        flags.set(AnalogOutputStatusQuality.COMM_LOST);
        Assert.assertTrue(flags.isSet(AnalogOutputStatusQuality.COMM_LOST));

        flags.set(AnalogOutputStatusQuality.REMOTE_FORCED);
        Assert.assertTrue(flags.isSet(AnalogOutputStatusQuality.REMOTE_FORCED));

        flags.set(AnalogOutputStatusQuality.LOCAL_FORCED);
        Assert.assertTrue(flags.isSet(AnalogOutputStatusQuality.LOCAL_FORCED));

        flags.set(AnalogOutputStatusQuality.OVERRANGE);
        Assert.assertTrue(flags.isSet(AnalogOutputStatusQuality.OVERRANGE));

        flags.set(AnalogOutputStatusQuality.REFERENCE_ERR);
        Assert.assertTrue(flags.isSet(AnalogOutputStatusQuality.REFERENCE_ERR));

        flags.set(AnalogOutputStatusQuality.RESERVED);
        Assert.assertTrue(flags.isSet(AnalogOutputStatusQuality.RESERVED));

        Assert.assertEquals((byte)0xFF, flags.getValue());
    }
}
