package com.automatak.dnp3;


public class Counter extends BaseMeasurement
{
    private final long value;

    public Counter(long value, byte quality, long timestamp)
    {
        super(quality, timestamp);
        this.value = value;
    }

    public long getValue()
    {
        return value;
    }
}
