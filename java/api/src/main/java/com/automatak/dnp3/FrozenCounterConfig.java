package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventFrozenCounterVariation;
import com.automatak.dnp3.enums.StaticFrozenCounterVariation;

public class FrozenCounterConfig extends EventConfig {

    public FrozenCounterConfig(int index) {
        super(index);
    }

    public int deadband = 0;
    public EventFrozenCounterVariation eventVariation = EventFrozenCounterVariation.Group23Var1;
    public StaticFrozenCounterVariation staticVariation = StaticFrozenCounterVariation.Group21Var1;
}