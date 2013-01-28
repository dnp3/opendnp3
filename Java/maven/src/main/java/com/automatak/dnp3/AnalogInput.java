package com.automatak.dnp3;


public class AnalogInput extends BaseMeasurement<Double>
{

    public AnalogInput(Double value, byte quality, long timestamp)
    {
        super(value, quality, timestamp);
    }

}
