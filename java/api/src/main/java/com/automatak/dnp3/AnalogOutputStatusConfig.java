package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventAnalogOutputStatusVariation;
import com.automatak.dnp3.enums.StaticAnalogOutputStatusVariation;

public class AnalogOutputStatusConfig extends EventConfig {

    public AnalogOutputStatusConfig(int index) {
        super(index);
    }

    public double deadband = 0;
    public EventAnalogOutputStatusVariation eventVariation = EventAnalogOutputStatusVariation.Group42Var1;
    public StaticAnalogOutputStatusVariation staticVariation = StaticAnalogOutputStatusVariation.Group40Var1;
}
