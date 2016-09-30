package com.automatak.dnp3.impl;

import com.automatak.dnp3.AnalogInput;
import com.automatak.dnp3.BinaryInput;
import com.automatak.dnp3.Counter;
import com.automatak.dnp3.impl.mocks.ExpectedValue;
import junit.framework.Assert;
import org.junit.Test;

public class ExpectedValueTestSuite {

    @Test
    public void comparisonFailsOnDifferentTypes() {
        BinaryInput m1 = new BinaryInput(true, (byte) 0x00, 0);
        AnalogInput m2 = new AnalogInput(0.00, (byte) 0x00, 0);

        ExpectedValue v1 = new ExpectedValue(m1, 0);
        ExpectedValue v2 = new ExpectedValue(m2, 0);

        Assert.assertFalse(v1.isEqual(v2));
    }

    @Test
    public void comparisonSucceedsOnSameValues() {
        Counter c1 = new Counter(3, (byte) 0x00, 0);
        Counter c2 = new Counter(3, (byte) 0x00, 0);

        ExpectedValue v1 = new ExpectedValue(c1, 0);
        ExpectedValue v2 = new ExpectedValue(c2, 0);

        Assert.assertTrue(v1.isEqual(v2));
    }

    @Test
    public void comparisonFailsOnDifferentValues() {
        Counter c1 = new Counter(4, (byte) 0x00, 0);
        Counter c2 = new Counter(3, (byte) 0x00, 0);

        ExpectedValue v1 = new ExpectedValue(c1, 0);
        ExpectedValue v2 = new ExpectedValue(c2, 0);

        Assert.assertFalse(v1.isEqual(v2));
    }
}
