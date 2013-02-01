package com.automatak.dnp3;


public class AnalogOutputInt16 {

    public final short value;
    public final CommandStatus status;

    public AnalogOutputInt16(short value, CommandStatus status)
    {
        this.value = value;
        this.status = status;
    }

}
