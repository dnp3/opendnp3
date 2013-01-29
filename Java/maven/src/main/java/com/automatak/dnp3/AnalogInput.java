package com.automatak.dnp3;


public class AnalogInput extends BaseMeasurement
{
    private final double value;

    public AnalogInput(double value, byte quality, long timestamp)
    {
        super(quality, timestamp);
        this.value = value;
    }

    public double getValue()
    {
        return value;
    }

}
