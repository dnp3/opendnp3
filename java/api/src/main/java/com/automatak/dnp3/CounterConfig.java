package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventCounterVariation;
import com.automatak.dnp3.enums.StaticCounterVariation;

public class CounterConfig extends DeadbandConfig<Integer> {
    public CounterConfig(int index) {
        super(index, 0);
        this.eventVariation = EventCounterVariation.Group22Var1;
        this.staticVariation = StaticCounterVariation.Group20Var1;
    }

    public EventCounterVariation eventVariation;
    public StaticCounterVariation staticVariation;
}
