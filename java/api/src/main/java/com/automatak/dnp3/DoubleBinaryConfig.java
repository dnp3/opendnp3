package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventDoubleBinaryVariation;
import com.automatak.dnp3.enums.StaticDoubleBinaryVariation;

public class DoubleBinaryConfig extends EventConfig {
    public DoubleBinaryConfig(int index) {
        super(index);
        this.eventVariation = EventDoubleBinaryVariation.Group4Var1;
        this.staticVariation = StaticDoubleBinaryVariation.Group3Var2;
    }

    public EventDoubleBinaryVariation eventVariation;
    public StaticDoubleBinaryVariation staticVariation;
}
