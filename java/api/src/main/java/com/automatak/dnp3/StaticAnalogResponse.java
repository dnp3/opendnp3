package com.automatak.dnp3;


public enum StaticAnalogResponse {

    GROUP30_VAR1(0),
    GROUP30_VAR2(1),
    GROUP30_VAR3(2),
    GROUP30_VAR4(3),
    GROUP30_VAR5(4),
    GROUP30_VAR6(5);

    public int getId() {
        return id;
    }

    private final int id;

    private StaticAnalogResponse(int id)
    {
        this.id = id;
    }
}
