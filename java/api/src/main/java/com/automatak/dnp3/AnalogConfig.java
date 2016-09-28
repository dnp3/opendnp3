package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventAnalogVariation;
import com.automatak.dnp3.enums.StaticAnalogVariation;

public class AnalogConfig extends DeadbandConfig<Double> {
    public AnalogConfig(int index) {
        super(index, 0.0);
        this.eventVariation = EventAnalogVariation.Group32Var1;
        this.staticVariation = StaticAnalogVariation.Group30Var1;
    }

    public EventAnalogVariation eventVariation;
    public StaticAnalogVariation staticVariation;
}
