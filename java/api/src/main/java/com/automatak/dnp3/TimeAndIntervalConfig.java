package com.automatak.dnp3;

import com.automatak.dnp3.enums.StaticTimeAndIntervalVariation;

public class TimeAndIntervalConfig extends IndexedConfig {
    public TimeAndIntervalConfig(int index) {
        super(index);
        this.staticVariation = StaticTimeAndIntervalVariation.Group50Var4;
    }

    public StaticTimeAndIntervalVariation staticVariation;
}
