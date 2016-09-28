package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventFrozenCounterVariation;
import com.automatak.dnp3.enums.StaticFrozenCounterVariation;

public class FrozenCounterConfig extends DeadbandConfig<Integer> {
    public FrozenCounterConfig(int index) {
        super(index, 0);
        this.eventVariation = EventFrozenCounterVariation.Group23Var1;
        this.staticVariation = StaticFrozenCounterVariation.Group21Var1;
    }

    public EventFrozenCounterVariation eventVariation;
    public StaticFrozenCounterVariation staticVariation;
}