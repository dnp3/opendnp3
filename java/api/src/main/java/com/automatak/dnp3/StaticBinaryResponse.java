package com.automatak.dnp3;

public enum StaticBinaryResponse {

    GROUP1_VAR2(0);

    public int getId() {
        return id;
    }

    private final int id;

    private StaticBinaryResponse(int id)
    {
        this.id = id;
    }
}






