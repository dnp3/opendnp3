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

/**
 * Statistics for the link layer parser
 */
public class ParserStatistics
{
    public ParserStatistics(
            long numHeaderCrcError,
            long numBodyCrcError,
            long numLinkFrameRx,
            long numBadLength,
            long numBadFunctionCode,
            long numBadFCV,
            long numBadFCB
    )
    {
        this.numHeaderCrcError = numHeaderCrcError;
        this.numBodyCrcError = numBodyCrcError;
        this.numLinkFrameRx = numLinkFrameRx;
        this.numBadLength = numBadLength;
        this.numBadFunctionCode = numBadFunctionCode;
        this.numBadFCV = numBadFCV;
        this.numBadFCB = numBadFCB;
    }

    /**
     * Number of frames discarded due to header CRC errors
     */
    public final long numHeaderCrcError;

    /**
     * Number of frames discarded due to body CRC errors
     */
    public final long numBodyCrcError;

    /**
     * Number of frames received
     */
    public final long numLinkFrameRx;

    /**
     * number of bad LEN fields received (malformed frame)
     */
    public final long numBadLength;

    /**
     * number of bad function codes (malformed frame)
     */
    public final long numBadFunctionCode;

    /**
     * number of FCV / function code mismatches (malformed frame)
     */
    public final long numBadFCV;

    /**
     * number of frames w/ unexpected FCB bit set (malformed frame)
     */
    public final long numBadFCB;
}
