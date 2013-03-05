package com.automatak.dnp3;

public enum StaticCounterResponse {

    GROUP20_VAR1(0),
    GROUP20_VAR2(1),
    GROUP20_VAR5(2),
    GROUP20_VAR6(3);

    public int getId() {
        return id;
    }

    private final int id;

    private StaticCounterResponse(int id)
    {
        this.id = id;
    }
}
