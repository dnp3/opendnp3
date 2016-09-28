package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventBinaryVariation;
import com.automatak.dnp3.enums.StaticBinaryVariation;

public class BinaryConfig extends EventConfig {

    public BinaryConfig(int index) {
        super(index);
        this.eventVariation = EventBinaryVariation.Group2Var1;
        this.staticVariation = StaticBinaryVariation.Group1Var2;
    }

    public EventBinaryVariation eventVariation;
    public StaticBinaryVariation staticVariation;
}
