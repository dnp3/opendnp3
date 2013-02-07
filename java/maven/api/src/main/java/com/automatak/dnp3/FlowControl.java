package com.automatak.dnp3;

public enum FlowControl {
    FLOW_NONE(0),
    FLOW_HARDWARE(1),
    FLOW_XONXOFF(2);

    private int id;

    FlowControl(int id)
    {
        this.id = id;
    }

    public int toInt()
    {
        return id;
    }
}

