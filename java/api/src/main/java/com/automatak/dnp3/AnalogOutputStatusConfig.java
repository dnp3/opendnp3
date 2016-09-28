package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventAnalogOutputStatusVariation;
import com.automatak.dnp3.enums.StaticAnalogOutputStatusVariation;

public class AnalogOutputStatusConfig extends DeadbandConfig<Double> {
    public AnalogOutputStatusConfig(int index) {
        super(index, 0.0);
        this.eventVariation = EventAnalogOutputStatusVariation.Group42Var1;
        this.staticVariation = StaticAnalogOutputStatusVariation.Group40Var1;
    }

    public EventAnalogOutputStatusVariation eventVariation;
    public StaticAnalogOutputStatusVariation staticVariation;
}
