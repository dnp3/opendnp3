package com.automatak.dnp3;

public enum EventAnalogResponse {

    GROUP32_VAR1(0),
    GROUP32_VAR2(1),
    GROUP32_VAR3(2),
    GROUP32_VAR4(3),
    GROUP32_VAR5(4),
    GROUP32_VAR6(5),
    GROUP32_VAR7(6),
    GROUP32_VAR8(7);

    public int getId() {
        return id;
    }

    private final int id;

    private EventAnalogResponse(int id)
    {
        this.id = id;
    }
}
