package com.automatak.dnp3;

import com.automatak.dnp3.CommandStatus;
import com.automatak.dnp3.ControlCode;

public class AnalogOutput {

    public final double value;
    public final CommandStatus status;

    public AnalogOutput(double value, CommandStatus status)
    {
        this.value = value;
        this.status = status;
    }

}
