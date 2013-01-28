package com.automatak.dnp3;


public class Counter extends BaseMeasurement<Long>
{
    public Counter(Long value, byte quality, long timestamp)
    {
        super(value, quality, timestamp);
    }
}
