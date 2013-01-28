package com.automatak.dnp3;


public class AnalogOutputStatus extends BaseMeasurement<Double>
{
    public AnalogOutputStatus(Double value, byte quality, long timestamp)
    {
        super(value, quality, timestamp);
    }
}
