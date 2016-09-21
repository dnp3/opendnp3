package com.automatak.dnp3;

import com.automatak.dnp3.enums.PointClass;

public class ClassAssignment {

    public ClassAssignment(byte group, byte variation, PointClass clazz, Range range) {
        this.group = group;
        this.variation = variation;
        this.clazz = clazz;
        this.range = range;
    }

    public final byte group;
    public final byte variation;
    public final PointClass clazz;

    // if range is full, this implies 0x06
    public final Range range;

}
