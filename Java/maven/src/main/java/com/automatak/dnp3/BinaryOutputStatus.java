package com.automatak.dnp3;


public class BinaryOutputStatus extends BaseMeasurement<Boolean>
{
    public BinaryOutputStatus(Boolean value, byte quality, long timestamp)
    {
        super(value, quality, timestamp);
    }
}
