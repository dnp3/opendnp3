package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventCounterVariation;
import com.automatak.dnp3.enums.StaticCounterVariation;

public class CounterConfig extends EventConfig {

    public CounterConfig(int index) {
        super(index);
    }

    public int deadband = 0;
    public EventCounterVariation eventVariation = EventCounterVariation.Group22Var1;
    public StaticCounterVariation staticVariation = StaticCounterVariation.Group20Var1;
}
