package com.automatak.dnp3;


public class BinaryOutputStatus extends BaseMeasurement
{
    private final boolean value;

    public BinaryOutputStatus(boolean value, byte quality, long timestamp)
    {
        super(quality, timestamp);
        this.value = value;
    }

    public boolean getValue()
    {
        return value;
    }
}
