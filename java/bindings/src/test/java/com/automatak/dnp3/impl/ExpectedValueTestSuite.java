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
import com.automatak.dnp3.impl.mocks.ExpectedValue;
import junit.framework.Assert;
import org.junit.Test;

public class ExpectedValueTestSuite {

    @Test
    public void comparisonFailsOnDifferentTypes() {
        BinaryInput m1 = new BinaryInput(true, new Flags((byte)0x00), new DNPTime(0));
        AnalogInput m2 = new AnalogInput(0.00, new Flags((byte)0x00), new DNPTime(0));

        ExpectedValue v1 = new ExpectedValue(m1, 0);
        ExpectedValue v2 = new ExpectedValue(m2, 0);

        Assert.assertFalse(v1.isEqual(v2));
    }

    @Test
    public void comparisonSucceedsOnSameValues() {
        Counter c1 = new Counter(3, new Flags((byte)0x00), new DNPTime(0));
        Counter c2 = new Counter(3, new Flags((byte)0x00), new DNPTime(0));

        ExpectedValue v1 = new ExpectedValue(c1, 0);
        ExpectedValue v2 = new ExpectedValue(c2, 0);

        Assert.assertTrue(v1.isEqual(v2));
    }

    @Test
    public void comparisonFailsOnDifferentValues() {
        Counter c1 = new Counter(4, new Flags((byte)0x00), new DNPTime(0));
        Counter c2 = new Counter(3, new Flags((byte)0x00), new DNPTime(0));

        ExpectedValue v1 = new ExpectedValue(c1, 0);
        ExpectedValue v2 = new ExpectedValue(c2, 0);

        Assert.assertFalse(v1.isEqual(v2));
    }
}
