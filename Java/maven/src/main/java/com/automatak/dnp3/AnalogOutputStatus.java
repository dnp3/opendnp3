package com.automatak.dnp3;


public class AnalogOutputStatus extends BaseMeasurement
{
    private final double value;

    public AnalogOutputStatus(double value, byte quality, long timestamp)
    {
        super(quality, timestamp);
        this.value = value;
    }

    public double getValue()
    {
        return value;
    }
}
