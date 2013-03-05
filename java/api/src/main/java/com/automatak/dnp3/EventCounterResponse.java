package com.automatak.dnp3;

public enum EventCounterResponse {

    GROUP22_VAR1(0),
    GROUP22_VAR2(1),
    GROUP22_VAR5(2),
    GROUP22_VAR6(3);

    public int getId() {
        return id;
    }

    private final int id;

    private EventCounterResponse(int id)
    {
        this.id = id;
    }
}
