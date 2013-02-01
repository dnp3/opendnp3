package com.automatak.dnp3;


public class AnalogOutputDouble64 {

    public final double value;
    public final CommandStatus status;

    public AnalogOutputDouble64(double value, CommandStatus status)
    {
        this.value = value;
        this.status = status;
    }

}
