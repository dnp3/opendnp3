package com.automatak.dnp3;


public class AnalogOutputInt32 {

    public final int value;
    public final CommandStatus status;

    public AnalogOutputInt32(int value, CommandStatus status)
    {
        this.value = value;
        this.status = status;
    }

}
