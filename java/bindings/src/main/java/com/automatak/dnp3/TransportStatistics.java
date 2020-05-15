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

public class TransportStatistics
{
    public TransportStatistics(
        long numTransportRx,
        long numTransportTx,
        long numTransportErrorRx,
        long numTransportBufferOverflow,
        long numTransportDiscard,
        long numTransportIgnore
    )
    {
        this.numTransportRx = numTransportRx;
        this.numTransportTx = numTransportTx;
        this.numTransportErrorRx = numTransportErrorRx;
        this.numTransportBufferOverflow = numTransportBufferOverflow;
        this.numTransportDiscard = numTransportDiscard;
        this.numTransportIgnore = numTransportIgnore;

    }
    /**
     * Number of valid TPDU's received
     */
    public final long numTransportRx;

    /**
     * Number of valid TPDU's transmitted
     */
    public final long numTransportTx;

    /**
     * Number of TPDUs dropped due to malformed contents
     */
    public final long numTransportErrorRx;

    /**
     * Number of times received data was too big for reassembly buffer
     */
    public final long numTransportBufferOverflow;

    /**
     * number of times transport buffer is discard due to new FIR
     */
    public final long numTransportDiscard;

    /**
     * number of segments ignored due to bad FIR/FIN or SEQ
     */
    public final long numTransportIgnore;
}
