package com.automatak.dnp3;


public class AnalogOutputFloat32 {

    public final float value;
    public final CommandStatus status;

    public AnalogOutputFloat32(float value, CommandStatus status)
    {
        this.value = value;
        this.status = status;
    }

}
