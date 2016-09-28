package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventBinaryOutputStatusVariation;
import com.automatak.dnp3.enums.StaticBinaryOutputStatusVariation;

public class BinaryOutputStatusConfig extends EventConfig {
    public BinaryOutputStatusConfig(int index) {
        super(index);
        this.eventVariation = EventBinaryOutputStatusVariation.Group11Var1;
        this.staticVariation = StaticBinaryOutputStatusVariation.Group10Var2;
    }

    public EventBinaryOutputStatusVariation eventVariation;
    public StaticBinaryOutputStatusVariation staticVariation;
}
