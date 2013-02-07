package com.automatak.dnp3;


public enum Parity {

    PAR_NONE(0),
    PAR_EVEN(1),
    PAR_ODD(2);

    private final int id;

    Parity(int id)
    {
        this.id = id;
    }

    public int toInt()
    {
        return id;
    }
}
