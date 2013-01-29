package com.automatak.dnp3;


public class BinaryInput extends BaseMeasurement
{
    private final boolean value;

    public BinaryInput(boolean value, byte quality, long timestamp)
    {
        super(quality, timestamp);
        this.value = value;
    }

    public boolean getValue()
    {
        return value;
    }

}
