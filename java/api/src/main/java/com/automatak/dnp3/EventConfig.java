package com.automatak.dnp3;

import com.automatak.dnp3.enums.PointClass;

public class EventConfig extends IndexedConfig {

    public EventConfig(int vIndex) {
        super(vIndex);
        this.clazz = PointClass.Class1;
    }

    PointClass clazz;
}
