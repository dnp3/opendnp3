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

import com.automatak.dnp3.enums.CommandStatus;
import com.automatak.dnp3.enums.OperationType;
import com.automatak.dnp3.enums.TripCloseCode;

/**
 * Command request for control a relay output
 */
public class ControlRelayOutputBlock {

    public final OperationType opType;
    public final TripCloseCode tcc;
    public final boolean clear;
    public final short count;
    public final long onTimeMs;
    public final long offTimeMs;
    public final CommandStatus status;

    /**
     * Primary constructor
     * @param opType Enumeration that controls the behavior of the command
     * @param tcc Enumeration that controls the behavior of the command
     * @param clear Boolean that controls the behavior of the command
     * @param count How many times to repeat the operation
     * @param onTimeMs How long to set the output active
     * @param offTimeMs How long to set the output inactive
     * @param status Status code received from an outstation
     */
    public ControlRelayOutputBlock(OperationType opType, TripCloseCode tcc, boolean clear, short count, long onTimeMs, long offTimeMs, CommandStatus status)
    {
        this.opType = opType;
        this.tcc = tcc;
        this.clear = clear;
        this.count = count;
        this.onTimeMs = onTimeMs;
        this.offTimeMs = offTimeMs;
        this.status = status;
    }

}
