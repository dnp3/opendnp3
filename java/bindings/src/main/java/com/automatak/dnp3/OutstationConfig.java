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

import com.automatak.dnp3.enums.*;

import java.time.Duration;

/**
 * Configuration class that controls how an outstation behaves
 */
public class OutstationConfig {

    /**
     * The maximum number of controls the outstation will attempt to process from a single APDU
     */
    public long maxControlsPerRequest = 4294967295L;

    /**
     * How long the outstation will allow an operate to proceed after a prior select
     */
    public Duration selectTimeout = Duration.ofSeconds(10);

    /**
     * Timeout for solicited confirms
     */
    public Duration solConfirmTimeout = Duration.ofSeconds(5);

    /**
     * Timeout for unsolicited confirms
     */
    public Duration unsolConfirmTimeout = Duration.ofSeconds(5);

    /**
     * Number of unsolicited response retries
     */
    public NumRetries numUnsolRetries = NumRetries.Infinite();

    /**
     * The maximum fragment size the outstation will use for fragments it sends
     */
    public int maxTxFragSize = 2048;

    /**
     * The maximum fragment size the outstation will be able to receive
     */
    public int maxRxFragSize = 2048;

    /**
     * Global enabled / disable for unsolicited messages. If false, the NULL unsolicited message is not even sent
     */
    public boolean allowUnsolicited = true;

    /**
     * A bitmask type that specifies the types allowed in a class 0 reponse
     */
    public StaticTypeBitField typesAllowedInClass0 = StaticTypeBitField.all();

    /**
     * Workaround for test procedure 8.11.2.1. Will respond immediatly to READ requests
     * when waiting for unsolicited NULL responses.
     *
     * This is NOT compliant to IEEE 1815-2012.
     */
    public boolean noDefferedReadDuringUnsolicitedNullResponse = false;
}
