package com.automatak.dnp3;

public enum EventBinaryResponse {

    GROUP2_VAR1(0),
    GROUP2_VAR2(1);

    public int getId() {
        return id;
    }

    private final int id;

    private EventBinaryResponse(int id)
    {
        this.id = id;
    }
};
