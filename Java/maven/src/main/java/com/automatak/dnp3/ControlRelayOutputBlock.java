package com.automatak.dnp3;

public class ControlRelayOutputBlock {

    public final ControlCode function;
    public final short count;
    public final long onTimeMs;
    public final long offTimeMs;
    public final CommandStatus status;

    public ControlRelayOutputBlock(ControlCode function, short count, long onTimeMs, long offTimeMs, CommandStatus status)
    {
        this.function = function;
        this.count = count;
        this.onTimeMs = onTimeMs;
        this.offTimeMs = offTimeMs;
        this.status = status;
    }

}
