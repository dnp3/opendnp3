package com.automatak.dnp3;


public enum StaticAnalogOutputStatusResponse {

    GROUP40_VAR1(0),
    GROUP40_VAR2(1),
    GROUP40_VAR3(2),
    GROUP40_VAR4(3);

    public int getId() {
        return id;
    }

    private final int id;

    private StaticAnalogOutputStatusResponse(int id)
    {
        this.id = id;
    }
}
