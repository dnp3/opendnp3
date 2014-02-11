/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3;

/**
 * Configuration class that controls how an outstation behaves
 */
public class OutstationConfig {

    /**
     * Outstation constructor with reasonable defaults
     */
    public OutstationConfig()
    {

    }

    /**
     * The maximum number of controls the slave will attempt to process from a single APDU, defaults to 1
     */
    public int maxControls = 1;

    /**
     * if true, fully disables unsolicited mode as if the slave didn't support it, defaults to false
     */
    public boolean disableUnsol = false;

    /**
     * controls what unsol classes are enabled by default. defaults to Class 1, 2, & 3
     *
     * Use PointClass.getMask to set this value
     */
    public int unsolMask = PointClass.ALL_EVENTS.toInt();

    /**
     * if true, the outstation will request time synchronization on an interval. Defaults to false.
     */
    public boolean allowTimeSync = false;


    /**
     * The period of time sync interval in milliseconds. Defaults to 10 minutes (10 * 60 * 1000)
     */
    public long timeSyncPeriodMs = 10 * 60 * 1000;

    /**
     * The amount of time in milliseconds the slave will wait before sending new unsolicited data ( <= 0 == immediately, no delay). Defaults to 200ms.
     */
    public  long unsolPackDelayMs = 200;

    /**
     * How long the slave will wait in milliseconds before retrying an unsuccessful unsolicited response  Defaults to 2000ms.
     */
    public long unsolRetryDelayMs = 2000;

    /**
     * Maximum delay between a Select and an Operate
     */
    public long selectTimeoutMs = 5000;

    /**
     * The maximum fragment size the slave will use for data it sends. Defaults to 2048.
     */
    public int maxFragSize = 2048;

    /**
     * The number of objects to store in the VtoWriter queue. Defaults to 1024.
     */
    public int vtoWriterQueueSize = 1024;

    /**
     * The number of binary events the slave will buffer before overflowing (defaults to 1000)
     */
    public int maxBinaryEvents = 1000;

    /**
     * The number of analog events the slave will buffer before overflowing (defaults to 1000)
     */
    public int maxAnalogEvents = 1000;

    /**
     * The number of counter events the slave will buffer before overflowing (defaults to 1000)
     */
    public int maxCounterEvents = 1000;

    /**
     * The number of vto events the slave will buffer before overflowing (defaults to 100)
     */
    public int maxVtoEvents = 100;

    /**
     * The default group/variation to use for static binary responses (default 1:2)
     */
    public StaticBinaryResponse staticBinaryInput  = StaticBinaryResponse.Group1Var2;

    /**
     * The default group/variation to use for static analog responses (default 30:1)
     */
    public StaticAnalogResponse staticAnalogInput = StaticAnalogResponse.Group30Var1;

    /**
     * The default group/variation to use for static counter responses (default 20:1)
     */
    public StaticCounterResponse staticCounter = StaticCounterResponse.Group20Var1;

    /**
     * The default group/variation to use for frozen counter responses (default 21:1)
     */
    public StaticFrozenCounterResponse staticFrozenCounter = StaticFrozenCounterResponse.Group21Var1;

    /**
     * The default group/variation to use for static AnalogOutput status responses (default 40:1)
     */
    public StaticSetpointStatusResponse staticSetpointStatus =  StaticSetpointStatusResponse.Group40Var1;

    /**
     * The default group/variation to use for binary event responses (default 2:1)
     */
    public EventBinaryResponse eventBinaryInput = EventBinaryResponse.Group2Var1;

    /**
     * The default group/variation to use for analog event responses (default 32:1)
     */
    public EventAnalogResponse eventAnalogInput = EventAnalogResponse.Group32Var1;

    /**
     * The default group/variation to use for counter event responses (default 22:1)
     */
    public EventCounterResponse eventCounter = EventCounterResponse.Group22Var1;

}
