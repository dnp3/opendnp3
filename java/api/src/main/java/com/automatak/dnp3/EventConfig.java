package com.automatak.dnp3;

import com.automatak.dnp3.enums.PointClass;

public class EventConfig {

    public EventConfig(int vIndex) {
        this.vIndex = vIndex;
        this.clazz = PointClass.Class1;
    }

    public int vIndex;
    public PointClass clazz;
}
