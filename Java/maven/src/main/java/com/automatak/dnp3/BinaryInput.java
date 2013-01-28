package com.automatak.dnp3;


public class BinaryInput extends BaseMeasurement<Boolean>
{

    public BinaryInput(boolean value, byte quality, long timestamp)
    {
        super(value, quality, timestamp);
    }

}
