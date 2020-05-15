/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.automatak.dnp3;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Supplier;

/**
 * Configuration class for the outstation database
 */
public class DatabaseConfig {

    public final Map<Integer, BinaryConfig> binary;
    public final Map<Integer, DoubleBinaryConfig> doubleBinary;
    public final Map<Integer, AnalogConfig> analog;
    public final Map<Integer, CounterConfig> counter;
    public final Map<Integer, FrozenCounterConfig> frozenCounter;
    public final Map<Integer, BinaryOutputStatusConfig> boStatus;
    public final Map<Integer, AnalogOutputStatusConfig> aoStatus;

    public static DatabaseConfig allValues(int num) {
        return new DatabaseConfig(num, num, num, num, num, num, num);
    }

    public DatabaseConfig()
    {
        this.binary = new HashMap<>();
        this.doubleBinary = new HashMap<>();
        this.analog = new HashMap<>();
        this.counter = new HashMap<>();
        this.frozenCounter = new HashMap<>();
        this.boStatus = new HashMap<>();
        this.aoStatus = new HashMap<>();
    }

    public DatabaseConfig(int numBinary, int numDoubleBinary, int numAnalog, int numCounter, int numFrozenCounter, int numBOStatus, int numAOStatus) {
        this.binary = initialize(numBinary, BinaryConfig::new);
        this.doubleBinary = initialize(numDoubleBinary, DoubleBinaryConfig::new);
        this.analog = initialize(numAnalog, AnalogConfig::new);
        this.counter = initialize(numCounter, CounterConfig::new);
        this.frozenCounter = initialize(numFrozenCounter, FrozenCounterConfig::new);
        this.boStatus = initialize(numBOStatus, BinaryOutputStatusConfig::new);
        this.aoStatus = initialize(numAOStatus, AnalogOutputStatusConfig::new);
    }

    private static <T> Map<Integer, T> initialize(int num, Supplier<T> factory) {
        final HashMap<Integer, T> map = new HashMap<>(num);
        for (int i = 0; i < num; ++i) {
            map.put(i, factory.get());
        }
        return map;
    }

}
