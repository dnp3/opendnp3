package com.automatak.dnp3;

import com.automatak.dnp3.enums.EventAnalogVariation;
import com.automatak.dnp3.enums.StaticAnalogVariation;

public class AnalogConfig extends EventConfig {

    public AnalogConfig(int index) {
        super(index);
    }

    public double deadband = 0;
    public EventAnalogVariation eventVariation = EventAnalogVariation.Group32Var1;
    public StaticAnalogVariation staticVariation = StaticAnalogVariation.Group30Var1;
}
