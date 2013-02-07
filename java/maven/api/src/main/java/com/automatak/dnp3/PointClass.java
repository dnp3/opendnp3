package com.automatak.dnp3;


public enum PointClass {
    CLASS_0(0x01),
    CLASS_1(0x02),
    CLASS_2(0x04),
    CLASS_3(0x08),
    ALL_EVENTS(CLASS_1.toInt() | CLASS_2.toInt() | CLASS_3.toInt());

    private final int id;

    PointClass(int id)
    {
        this.id = id;
    }

    int toInt()
    {
        return id;
    }

    public static int getMask(PointClass... args)
    {
        int i = 0;
        for(PointClass pc : args) { i |= pc.toInt(); }
        return i;
    }
}